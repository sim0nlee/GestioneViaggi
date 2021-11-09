//Per facilitare la lettura del codice, chiudere tutte le parentesi graffe delle etichette

#include "Grafo.h"
#include "Utenti_e_Prenotazioni.h"
#include "Ausiliari.h"

void MENU_PRINCIPALE (char* opNum) {

    system("cls");

    printf("*******************************************************************************\n");
    printf("*                                                                             *\n");
    printf("*                                W E L C O M E                                *\n");
    printf("*                                                                             *\n");
    printf("*******************************************************************************\n\n");

    printf("Scegli un'operazione da effettuare:\n\n     1) Accedi come amministratore\n     2) Registra un nuovo utente\n     3) Accedi come utente\n     4) Esci dal programma\n\n");
    scanf("%c", opNum);
    fflush(stdin);

}
void MENU_ADMIN (char* opNum) {

    system("cls");

    printf("Benvenuto, ADMIN. Cosa vuoi fare?\n\n");
    printf("     1) Aggiungi una destinazione\n     2) Cancella una destinazione\n     3) Aggiungi un volo\n     4) Cancella un volo\n     5) Visualizza il grafo delle tratte\n     6) Visualizza gli utenti registrati\n     7) Logout\n\n");
    scanf("%c", opNum);
    fflush(stdin);
    printf("\n");

}
void MENU_USER (char* opNum, User* currUser) {

    system("cls");

    printf("Benvenuto/a, %s! Cosa vorresti fare?\n\n", currUser->username);
    printf("     1) Visualizza destinazioni disponibili\n     2) Effettua una prenotazione\n     3) Cancella una prenotazione effettuata\n     4) Visualizza le tue prenotazioni attive\n     5) Visualizza i tuoi punti\n     6) Elimina il tuo account\n     7) Logout\n\n");
    scanf("%c", opNum);
    fflush(stdin);

}
void MENU_SCEGLI_DESTINAZIONE (char* opNum, Vertex* source) {

    system("cls");

    printf("CITTA' DI PARTENZA: %s\n\n", source->citta);
    printf("Ti proponiamo queste possibilita':\n\n");
    printf("     1) Visualizza le mete raggiungibili da questa citta'\n     2) Scegli una destinazione\n     3) Scegli la meta piu' economica\n     4) Scegli la meta piu' gettonata\n     5) Cambia citta' di partenza\n\n");
    scanf("%c", opNum);
    fflush(stdin);

}
void MENU_SCEGLI_TRATTA (char* opNum, Vertex* source, Vertex* target) {

    system("cls");

    printf("CITTA' DI PARTENZA: %s           CITTA' DI DESTINAZIONE: %s\n\n", source->citta, target->citta);
    printf("Che tratta preferisci?\n\n     1) Tratta piu' economica\n     2) Tratta piu' breve\n     3) Cambia meta\n\n");
    scanf("%c", opNum);
    fflush(stdin);

}

int main() {

    Graph* G = NULL;
    G = initGraph(G);

    User* AVL_Utenti = NULL;

    char opNum, scelta;

    float costo_scanner; int durata_scanner;
    String password_scanner, username_scanner, city1_scanner, city2_scanner;
    int codice_scanner;

    int scanf_ret;

    User* currUser;
    Vertex *source, *target;


MENU_PRINCIPALE:; {

    MENU_PRINCIPALE(&opNum);

    switch (opNum) {

        case '1':
            goto ADMIN_LOGIN;

        case '2':
            goto REGISTRAZIONE;

        case '3':
            goto USER_LOGIN;

        case '4':
            goto EXIT;

        default:
            goto MENU_PRINCIPALE;

    }

}



ADMIN_LOGIN:; {

    system("cls");

    do {

        printf("Inserisci la password amministratore (scrivi 'back' per tornare al menu principale): ");
        gets(password_scanner);
        fflush(stdin);

        if (strcmp(password_scanner, "back") == 0)
            goto MENU_PRINCIPALE;

        else if (strcmp(password_scanner, ADMINPASSWORD) != 0)
            printf("\nPASSWORD ERRATA!\n\n");

    } while (strcmp(password_scanner, "admin") != 0);

}

    ADMIN_LOGGED:; {

        MENU_ADMIN(&opNum);

        switch (opNum) {

            case '1':     //Inserimento di una nuova citta'

                do {

                    printf("\nInserire il nome della nuova destinazione da aggiungere (scrivi 'back' per tornare indietro): ");
                    gets(city1_scanner);
                    fflush(stdin);

                    if (strcmp(city1_scanner, "back") == 0)
                        goto ADMIN_LOGGED;

                    else if (!isLiteral(city1_scanner))
                        printf("\nIl nome della citta' puo' contenere solo lettere, spazi e apostrofi.\n");

                    else if (!isalpha(city1_scanner[0]))
                        printf("\nIl nome della citta' deve iniziare con una lettera.\n");

                } while (!isLiteral(city1_scanner) || !isalpha(city1_scanner[0]));

                G = addVertex(G, city1_scanner);

                printf("\n\nOperazione eseguita.");
                waitKey();
                goto ADMIN_LOGGED;

            case '2':     //Cancellazione di una citta'

                printf("\nInserire il nome della destinazione da cancellare (scrivi 'back' per tornare indietro): ");
                gets(city1_scanner);
                fflush(stdin);

                if (strcmp(city1_scanner, "back") == 0)
                    goto ADMIN_LOGGED;

                G = deleteVertex (G, city1_scanner);

                printf("\n\nOperazione eseguita.");
                waitKey();
                goto ADMIN_LOGGED;

            case '3':     //Inserimento di una tratta tra due citta', ossia di un arco nel grafo

                printf("\n\nInserire la citta' di partenza (scrivi 'back' per tornare indietro): ");
                gets(city1_scanner);
                fflush(stdin);

                if (strcmp(city1_scanner, "back") == 0)
                    goto ADMIN_LOGGED;

                printf("Inserire la citta' di destinazione (scrivi 'back' per tornare indietro): ");
                gets(city2_scanner);
                fflush(stdin);

                if (strcmp(city2_scanner, "back") == 0)
                    goto ADMIN_LOGGED;

                do {
                    printf("Inserire il costo del volo (solo valori positivi, 0 per tornare indietro): ");
                    scanf_ret = scanf("%f", &costo_scanner);
                    fflush(stdin);
                    if (costo_scanner == 0) goto ADMIN_LOGGED;
                    if(!scanf_ret) {costo_scanner = NULLCOST;}
                } while (costo_scanner <= 0);

                do {
                    printf("Inserire la durata del volo in minuti (solo valori interi positivi, 0 per tornare indietro): ");
                    scanf_ret = scanf("%d", &durata_scanner);
                    fflush(stdin);
                    if (durata_scanner == 0) goto ADMIN_LOGGED;
                    if(!scanf_ret) {durata_scanner = NULLTIME;}
                } while (durata_scanner <= 0);

                G = addEdge(G, city1_scanner, city2_scanner, costo_scanner, durata_scanner, true);

                waitKey();
                goto ADMIN_LOGGED;

            case '4':     //Cancellazione di una tratta tra due citta'

                printf("\n\nInserire la citta' di partenza (scrivi 'back' per tornare indietro): ");
                gets(city1_scanner);
                fflush(stdin);

                if (strcmp(city1_scanner, "back") == 0)
                    goto ADMIN_LOGGED;

                printf("Inserire la citta' di destinazione (scrivi 'back' per tornare indietro): ");
                gets(city2_scanner);
                fflush(stdin);

                if (strcmp(city2_scanner, "back") == 0)
                    goto ADMIN_LOGGED;

                G = deleteEdge (G, city1_scanner, city2_scanner);

                printf("\n\nOperazione eseguita.");
                waitKey();
                goto ADMIN_LOGGED;

            case '5':     //Stampa del grafo

                system("cls");
                printGraph(G);
                waitKey();
                goto ADMIN_LOGGED;

            case '6':     //Stampa dell'AVL di utenti

                system("cls");
                if (!AVL_Utenti)
                    printf("Non sono presenti utenti registrati.");
                else
                    printAVL(AVL_Utenti);
                waitKey();
                goto ADMIN_LOGGED;

            case '7':     //Ritorno al menu principale

                if (!G || G->V == 0) {
                    printf("\n\nInserisci almeno una destinazione!");
                    waitKey();
                    goto ADMIN_LOGGED;
                }
                else
                    goto MENU_PRINCIPALE;

            default:

                goto ADMIN_LOGGED;

        }

    }



REGISTRAZIONE:; {

    system("cls");

    do {

        printf("Inserisci il nuovo username (scrivi 'back' per tornare al menu principale): ");
        gets(username_scanner);
        fflush(stdin);

        if (strcmp(username_scanner, "back") == 0)
            goto MENU_PRINCIPALE;

        else if (!isalpha(username_scanner[0]))
            printf("\nLo username deve iniziare con una lettera!\n\n");

        else if (getUser(AVL_Utenti, username_scanner) != NULL)
            printf("\nQuesto nome utente e' gia' in uso.\n\n");

    } while (!isalpha(username_scanner[0]) || getUser(AVL_Utenti, username_scanner) != NULL);

    system("cls");
    printf("Nuovo username: %s\n\n", username_scanner);

    do {

        printf("Crea una password per l'utente \"%s\": ", username_scanner);
        gets(password_scanner);
        fflush(stdin);

        if (!isalpha(password_scanner[0]))
            printf("\nLa password deve iniziare con una lettera!\n\n");

    } while (!isalpha(password_scanner[0]));

    system("cls");
    printf("Nuovo username: %s\nPassword: %s", username_scanner, password_scanner);

    AVL_Utenti = addUser (AVL_Utenti, username_scanner, password_scanner);

    printf("\n\nRegistrazione effettuata.");
    waitKey();
    goto MENU_PRINCIPALE;

}



USER_LOGIN:; {

    system("cls");

    do {

        printf("Immetti username (scrivi 'back' per tornare al menu principale): ");
        gets(username_scanner);
        fflush(stdin);

        if (strcmp(username_scanner, "back") == 0)
            goto MENU_PRINCIPALE;

        if (getUser(AVL_Utenti, username_scanner) == NULL)
            printf("\nUtente non presente.\n\n");

    } while (getUser(AVL_Utenti, username_scanner) == NULL);

    system("cls");
    printf("Username: %s\n\n", username_scanner);

    currUser = getUser(AVL_Utenti, username_scanner);

    do {

        printf("Immetti password (scrivi 'back' per tornare al menu principale): ");
        gets(password_scanner);
        fflush(stdin);

        if (strcmp(password_scanner, "back") == 0)
            goto MENU_PRINCIPALE;

        if (strcmp(password_scanner, currUser->password) != 0)
            printf("\nPASSWORD ERRATA!\n\n");

    } while (strcmp(password_scanner, currUser->password) != 0);

}

    USER_LOGGED:; {

        MENU_USER(&opNum, currUser);

        switch (opNum) {

            case '1':     //Stampa la lista di vertici del grafo, ovvero le possibili destinazioni
                system("cls");
                printf("Ecco la lista di citta' in cui operiamo:\n\n");
                print_list_Vertex(G->vertices);
                waitKey();
                goto USER_LOGGED;

            case '2':
                goto PRENOTA;

            case '3':
                goto CANCELLA_PRENOTAZIONE;

            case '4':     //Stampa la lista delle prenotazioni dell'utente
                system("cls");
                if (currUser->prenotazioni_attive)
                    print_list_Prenotazione(currUser->prenotazioni_attive);
                else
                    printf("Non sono presenti prenotazioni.");
                waitKey();
                goto USER_LOGGED;

            case '5':     //Visualizza i punti dell'utente
                printf("\n\nAl momento disponi di %d punti.\nOgni prenotazione effettuata ti dara' punti pari a un decimo del suo costo, arrotondati per difetto.\n100, 250 e 400 punti daranno accesso a sconti del 10%%, 25%% e 40%%.", currUser->punti);
                waitKey();
                goto USER_LOGGED;

            case '6':
                system("cls");
                printf("Se elimini il tuo account perderai le tue prenotazioni attive.\n\n");

                do {
                    printf("Procedere? (s/n): ");
                    scanf("%c", &scelta);
                } while (scelta != 's' && scelta != 'n');

                if (scelta == 's') {
                    AVL_Utenti = deleteUser(AVL_Utenti, username_scanner);
                    printf("\n\nAccount eliminato.");
                    waitKey();
                    goto MENU_PRINCIPALE;
                }
                else
                    goto USER_LOGGED;

            case '7':
                goto MENU_PRINCIPALE;

            default:
                goto USER_LOGGED;

        }

    }

        PRENOTA:; {

            system("cls");

            do {

                printf("Scegli la citta' da cui vuoi partire (scrivi 'back' per tornare indietro): ");
                gets(city1_scanner);
                fflush(stdin);

                if (strcmp(city1_scanner, "back") == 0)
                    goto USER_LOGGED;

                else if (getVertex(G->vertices, city1_scanner) == NULL)
                    printf("\nQuesta citta' non e' disponibile.\n\n");

            } while (getVertex(G->vertices, city1_scanner) == NULL);

            source = getVertex(G->vertices, city1_scanner);

            SCEGLI_DESTINAZIONE:; {

                MENU_SCEGLI_DESTINAZIONE(&opNum, source);

                if (opNum == '1') {

                    //Chiama DFS_Visit sulla citta' di partenza per visualizzare le citta' raggiungibili da essa

                    if (!source->adjacents) {
                        printf("\n\nNon partono voli da questa citta' !");
                        waitKey();
                        goto SCEGLI_DESTINAZIONE;
                    }

                    system("cls");
                    printf("Le seguenti citta' sono raggiungibili da %s coi nostri voli:\n\n", source->citta);
                    DFS_Reachables(G, source);
                    waitKey();
                    goto SCEGLI_DESTINAZIONE;

                }

                if (opNum == '2') {

                    //Scelta della citta' di destinazione e dovuti controlli
                    system("cls");

                    do {

                        printf("Scegli la citta' di destinazione (scrivi 'back' per tornare indietro): ");
                        gets(city2_scanner);
                        fflush(stdin);

                        if (strcmp(city2_scanner, "back") == 0)
                            goto SCEGLI_DESTINAZIONE;

                        else if (getVertex(G->vertices, city2_scanner) == NULL)
                            printf("\nQuesta citta' non e' disponibile.\n\n");

                        else {

                            target = getVertex(G->vertices, city2_scanner);

                            if (source == target)
                                printf("\nSei gia' qui!\n\n");

                            else if (!isReachable(G, source, target))
                                printf("\nSiamo spiacenti, non esiste questo volo.\n\n");

                        }

                    } while (getVertex(G->vertices, city2_scanner) == NULL || !isReachable(G, source, target) || source == target);

                    SCEGLI_TRATTA:; {

                        MENU_SCEGLI_TRATTA(&opNum, source, target);

                        //Permette di scegliere tra la tratta piu' economica e la tratta piu' breve
                        switch (opNum) {

                            case '1':
                                system("cls");
                                scelta = effettuaPrenotazione(currUser, G, source, target, COSTO);    //tratta economica
                                if (scelta == 'n')
                                    goto SCEGLI_TRATTA;
                                goto USER_LOGGED;

                            case '2':
                                system("cls");
                                scelta = effettuaPrenotazione(currUser, G, source, target, DURATA);    //tratta breve
                                if (scelta == 'n')
                                    goto SCEGLI_TRATTA;
                                goto USER_LOGGED;

                            case '3':
                                goto SCEGLI_DESTINAZIONE;

                            default:
                                goto SCEGLI_TRATTA;

                        }

                    }

                }

                if (opNum == '3') {

                    //La citta' piu' economica e' la prima nella lista di adiacenza della citta' di partenza

                    if (!source->adjacents) {
                        printf("\n\nNon partono voli da questa citta' !");
                        waitKey();
                        goto SCEGLI_DESTINAZIONE;
                    }

                    target = source->adjacents->currAdj;
                    printf("\n\nLa meta piu' economica e' %s.", target->citta);
                    waitKey();
                    system("cls");
                    scelta = effettuaPrenotazione(currUser, G, source, target, FAST);
                    if (scelta == 'n')
                        goto SCEGLI_DESTINAZIONE;
                    goto USER_LOGGED;

                }

                if (opNum == '4') {

                    //Prende la citta' piu' gettonata e torna alla scelta della tratta.

                    target = getPiuGettonata(G, G->vertices, source, source);

                    if (target == source) {
                        printf("\n\nNon e' stato possibile trovare una meta valida. Consultare la lista di destinazioni per scegliere una meta.");
                        waitKey();
                        goto SCEGLI_DESTINAZIONE;
                    }
                    else {
                        printf("\n\nLa meta piu' gettonata e' %s (Visite: %d).", target->citta, target->visite);
                        waitKey();
                        goto SCEGLI_TRATTA;
                    }

                }

                if (opNum == '5')
                    goto PRENOTA;

                else
                    goto SCEGLI_DESTINAZIONE;

            }

        }

        CANCELLA_PRENOTAZIONE:; {

            //Elimina una prenotazione dalla lista di prenotazioni attive dell'utente
            system("cls");

            if (!currUser->prenotazioni_attive) {
                printf("Non sono presenti prenotazioni.");
                waitKey();
                goto USER_LOGGED;
            }

            do {
                printf("Inserisci il codice della prenotazione da eliminare (>= 100, premi 0 per tornare indietro): ");
                scanf_ret = scanf("%d", &codice_scanner);
                fflush(stdin);
                if (!scanf_ret) {codice_scanner = -1;}
                if (codice_scanner == 0) {goto USER_LOGGED;}
            } while (codice_scanner < 100);

            printf("\nNon sono previsti rimborsi per prenotazioni disdette, ma non perderai i punti guadagnati.\n\n");

            do {
                printf("Procedere? (s/n): ");
                scanf("%c", &scelta);
            } while (scelta != 's' && scelta != 'n');

            if (scelta == 's') {
                currUser->prenotazioni_attive = removePrenotazione(currUser->prenotazioni_attive, codice_scanner);
                printf("\n\nPrenotazione %d cancellata, se esistente.\n\n", codice_scanner);
                waitKey();
                goto USER_LOGGED;
            }
            else
                goto USER_LOGGED;

        }



EXIT:; {

    //All'uscita si libera la memoria

    destroyAVL(AVL_Utenti);
    destroyGraph(G);

    printf("\n\nA presto, e grazie per averci scelto!");
    waitKey();

}

return 0;

}
