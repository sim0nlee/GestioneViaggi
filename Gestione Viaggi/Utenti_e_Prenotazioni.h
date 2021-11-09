//In questa libreria sono contenute le definizioni e le funzioni finalizzate alle operazioni su prenotazioni e utenti.

#ifndef UEP_H
#define UEP_H

#include "Grafo.h"
#include "Ausiliari.h"

typedef struct User {
//  Un utente consiste in username, password, un campo che tiene traccia dei suoi punti e una lista di sue prenotazioni attive,
//  oltre ai campi necessari per l'inserimento in albero AVL
    String username;
    String password;
    int punti;
    struct Prenotazione* prenotazioni_attive;
    int h;
    struct User* sx;
    struct User* dx;
} User;

typedef struct Prenotazione {
//  Una prenotazione e' formata da una tratta, ovvero una lista di nodi contenenti vertici, e un costo, una durata e un codice univoco.
//   Il campo next e' necessario per l'inserimento in lista
    struct Path* tratta;
    float costo;
    int durata;
    int codice;
    struct Prenotazione* next;
} Prenotazione;


Prenotazione* newPrenotazione (Path* tratta, float costo, int durata, int codice) {

    //Crea una nuova prenotazione
    Prenotazione* new = (Prenotazione*)malloc(sizeof(Prenotazione));
    if (new) {
        new->tratta = tratta;
        new->costo = costo;
        new->durata = durata;
        new->codice = codice;
        new->next = NULL;
    }
    return new;

}
Prenotazione* addPrenotazione (Prenotazione* list, Path* tratta, float costo, int durata, int codice) {

    //Aggiunge una nuova prenotazione in coda alla lista
    if (!list)
        return newPrenotazione(tratta, costo, durata, codice);

    else {
        list->next = addPrenotazione(list->next, tratta, costo, durata, codice);
        return list;
    }

}
Prenotazione* removePrenotazione (Prenotazione* list, int codice) {

    //Rimuove una prenotazione dalla lista in base al codice
    if (list) {
        if (list->codice == codice) {
            Prenotazione* tmp = list;
            list = list->next;
            free(tmp);
        }
        else
            list->next = removePrenotazione(list->next, codice);
    }

    return list;

}

void print_list_Prenotazione (Prenotazione* list) {

    //Stampa una lista di prenotazioni
    if (list) {
        printf("Tratta: ");
        printPath(list->tratta);
        printf("\n");
        printf("Costo: %.2f   Durata complessiva: %d min.\n", list->costo, list->durata);
        printf("Codice prenotazione: %d\n", list->codice);
        printf("\n\n");
        print_list_Prenotazione(list->next);
    }

}
void free_list_Prenotazione (Prenotazione* list) {

    //Libera la lista di prenotazioni
    if (list) {
        free_list_Prenotazione(list->next);
        freePath(list->tratta);
        free(list);
    }

}

void calcolaSconti (User* U, float* costo) {

//  Avvia un procedimento di scelta di sconto nel caso l'utente ne disponga, effettuando le opportune modifiche
//  ai punti dell'utente che ne usufruisce e al costo del volo in questione.

    char scelta;

    if (U->punti >= 100) {

        printf("\nDisponi dei seguenti sconti:\n\n");
        printf("     1) 10%% (100 punti)\n");
        if (U->punti >= 250)
            printf("     2) 25%% (250 punti)\n");
        if (U->punti >= 400)
            printf("     3) 40%% (400 punti)");

        printf("\n");

        if (U->punti < 250) {
            do {
                printf("\nScegli quale sconto usare ('n' per non usarne nessuno).\n\nATTENZIONE: una volta usato uno sconto, i punti non sono rimborsabili.\n\n");
                scanf("%c", &scelta);
                fflush(stdin);
            } while (scelta != '1' && scelta != 'n');
        }
        else if (U->punti < 400) {
            do {
                printf("\nScegli quale sconto usare ('n' per non usarne nessuno).\n\nATTENZIONE: una volta usato uno sconto, i punti non sono rimborsabili.\n\n");
                scanf("%c", &scelta);
                fflush(stdin);
            } while (scelta != '1' && scelta != '2' && scelta != 'n');
        }
        else {
            do {
                printf("\nScegli quale sconto usare ('n' per non usarne nessuno).\n\nATTENZIONE: una volta usato uno sconto, i punti non sono rimborsabili.\n\n");
                scanf("%c", &scelta);
                fflush(stdin);
            } while (scelta != '1' && scelta != '2' && scelta != '3' && scelta != 'n');
        }

        switch (scelta) {
            case '1':
                U->punti -= 100;
                *costo = *costo - (0.1 * *costo);
                printf("\n\nSconto del 10%% usato, nuovo costo: %.2f\n\n", *costo);
                break;
            case '2':
                U->punti -= 250;
                *costo = *costo - (0.25 * *costo);
                printf("\n\nSconto del 25%% usato, nuovo costo: %.2f\n\n", *costo);
                break;
            case '3':
                U->punti -= 400;
                *costo = *costo - (0.4 * *costo);
                printf("\n\nSconto del 40%% usato, nuovo costo: %.2f\n\n", *costo);
                break;
            default:
                printf("\n\n");
                break;
        }

        printf("\n");

    }

}

char effettuaPrenotazione (User* U, Graph* G, Vertex* start, Vertex* dest, variante variante) {

//  Questa funzione calcola il percorso piu' breve o il percorso piu' economico, a seconda del parametro passato in
//  ingresso, invocando l'algoritmo di Dijkstra; dopodiche' lo costruisce, e genera una prenotazione per l'utente con
//  i dovuti campi, procedendo con le operazioni di aggiornamento dei dati nel caso l'utente la confermi.
//  Nel caso di una prenotazione di tipo 'fast', Dijkstra non viene invocato, in quanto la meta piu' economica sara'
//  sempre il primo adiacente della citta' di partenza.

    Path* path = NULL;

    float costo; int durata; static int codice = 100;

    if (variante == FAST) {

        costo = start->adjacents->costo;
        durata = start->adjacents->durata;

        path = newNode(start->citta);
        path->next = newNode(dest->citta);

    }

    else {

        Predecessore** p = Dijkstra(G, start, dest, variante);

        costo = 0; durata = 0;
        path = buildPath(p, start, dest, &costo, &durata);

        free_Predecessori(G, p);

    }

    printf("Il tuo percorso:  ");
    printPath(path);
    printf("\n");
    printf("Costo: %.2f   Durata complessiva: %d min.\n", costo, durata);
    printf("Codice prenotazione: %d", codice);
    printf("\n\n");

    calcolaSconti(U, &costo);

    char scelta;

    do {
        printf("Prenotare? (s/n): ");
        scanf("%c", &scelta);
        fflush(stdin);
    } while (scelta != 's' && scelta != 'n');

    if (scelta == 's') {
        U->prenotazioni_attive = addPrenotazione(U->prenotazioni_attive, path, costo, durata, codice);
        U->punti += floor(costo/10);
        dest->visite++;
        codice++;
        printf("\n\nPrenotazione registrata con successo. Grazie!");
        waitKey();
    }

    return scelta;

}



//Di seguito le funzioni necessarie alla creazione e alla gestione di un AVL di utenti.
User* newUser (String username, String password) {

    User* user = (User*)malloc(sizeof(User));

    if (user) {
        strcpy(user->username, username);
        strcpy(user->password, password);
        user->punti = 0;
        user->prenotazioni_attive = NULL;
        user->h = 0;
        user->sx = user->dx = NULL;
        return user;
    }
    else
        return NULL;

}

int Altezza (User* T) {
    if (!T)
        return -1;
    else
        return T->h;
}

User* RotazioneSx (User* T) {

    User* NewRoot = T->sx;

    T->sx = NewRoot->dx;
    NewRoot->dx = T;
    T->h = 1 + max(Altezza(T->sx), Altezza(T->dx));

    return NewRoot;

}
User* RotazioneDx (User* T) {

    User* NewRoot = T->dx;

    T->dx = NewRoot->sx;
    NewRoot->sx = T;
    T->h = 1 + max(Altezza(T->sx), Altezza(T->dx));

    return NewRoot;

}

User* DoppiaRotazioneSx (User* T) {

    T->sx = RotazioneDx (T->sx);
    return RotazioneSx (T);

}
User* DoppiaRotazioneDx (User* T) {

    T->dx = RotazioneSx (T->dx);
    return RotazioneDx (T);

}

User* BilanciaSx (User* T) {

    if ((Altezza(T->sx) - Altezza(T->dx)) == 2) {

        if (Altezza((T->sx)->sx) > Altezza((T->sx)->dx))
            T = RotazioneSx(T);

        else
            T = DoppiaRotazioneSx(T);

    }

    else
        T->h = 1 + max(Altezza(T->sx), Altezza(T->dx));

    return T;

}
User* BilanciaDx (User* T) {

    if ((Altezza(T->dx) - Altezza(T->sx)) == 2) {

        if (Altezza((T->dx)->dx) > Altezza((T->dx)->sx))
            T = RotazioneDx(T);

        else
            T = DoppiaRotazioneDx(T);

    }

    else
        T->h = 1 + max(Altezza(T->sx), Altezza(T->dx));

    return T;

}

User* addUser (User* T, String username, String password) {

    if (T) {

        if (strcmp(T->username, username) == 0)
            return T;

        if (strcmp(T->username, username) == -1) {
            T->dx = addUser (T->dx, username, password);
            T = BilanciaDx (T);
        }

        if (strcmp(T->username, username) == 1) {
            T->sx = addUser (T->sx, username, password);
            T = BilanciaSx (T);
        }

    }

    else
        T = newUser(username, password);

    return T;

}
User* getUser (User* T, String username) {

    if (T) {

        if (strcmp(T->username, username) == -1)
            return getUser(T->dx, username);

        else if (strcmp(T->username, username) == 1)
            return getUser(T->sx, username);

        else
            return T;

    }

    else
        return T;

}

User* StaccaMin (User* T, User* P) {

   User* ret = NULL;
   User* NewRoot = NULL;

   if (T) {

       if (T->sx) {
           ret = StaccaMin (T->sx, T);
           NewRoot = BilanciaDx (T);
       }
       else {
           ret = T;
           NewRoot = T->dx;
       }

       if (P->sx == T)
           P->sx = NewRoot;
       else
           P->dx = NewRoot;

   }

   return ret;

}
User* CancellaRoot (User* T) {

   User* tmp = NULL;

   if (T) {

       if (!T->sx || !T->dx) {
           tmp = T;
           if (!T->sx)
               T = T->dx;
           else
               T = T->sx;
       }

       else {
           tmp = StaccaMin(T->dx, T);
           strcpy(T->username, tmp->username);
           strcpy(T->password, tmp->password);
           T->punti = tmp->punti;
           free(T->prenotazioni_attive);
           T->prenotazioni_attive = tmp->prenotazioni_attive;
           T = BilanciaSx(T);
       }

       free(tmp);

   }

   return T;

}
User* deleteUser (User* T, String username) {

   if (T) {

       if (strcmp(T->username, username) == 1) {
           T->sx = deleteUser(T->sx, username);
           T = BilanciaDx(T);
       }

       else if (strcmp(T->username, username) == -1) {
           T->dx = deleteUser(T->dx, username);
           T = BilanciaSx(T);
       }

       else
           T = CancellaRoot(T);

   }

   return T;

}

void printAVL (User* T) {

    if (T) {
        printAVL(T->sx);
        printf("%s\n", T->username);
        printAVL(T->dx);
    }

}

void freeUser (User* U) {

    free_list_Prenotazione(U->prenotazioni_attive);
    free(U);

}
void destroyAVL (User* T) {

    if(!T)
        return;

    destroyAVL(T->sx);
    destroyAVL(T->dx);
    freeUser(T);

}

#endif
