//Questa libreria contiene definizioni e funzioni ncessarie alla creazione e alla gestione del grafo delle tratte
//e alle procedure che operano su di esso

#ifndef GRAFO_H
#define GRAFO_H

#include "Ausiliari.h"

#define INF 999999999
#define NULLCOST -1
#define NULLTIME -1

typedef struct Vertex {
//  Ogni nodo del grafo rappresenta una citta' con un certo numero di visite, e contiene una chiave univoca.
//  Il campo Adj_list rappresenta la lista di adiacenza del nodo, e il campo next e' necessario per l'inserimento in lista
    String citta;
    int visite;
    int key;
    struct Adj_list* adjacents;
    struct Vertex* next;
} Vertex;

typedef struct Adj_list {
//  Una lista di adiacenza e' una lista di puntatori a vertice. Ogni nodo rappresenta un arco, ovvero un volo, con durata e costo
    Vertex *currAdj;
    float costo;
    int durata;
    struct Adj_list* next;
} Adj_list;

typedef struct Graph {
//  Il grafo e' rappresentato con una lista di vertici e un intero V che ne indica la dimensione
    int V;
    Vertex* vertices;
} Graph;

typedef struct Predecessore {
//  Questa struct e' necessaria a conservare le informazioni sui predecessori conservati dall'algoritmo di Dijkstra man mano che avanza
    Vertex* vertex;
    float costo;
    int durata;
} Predecessore;

typedef struct Path {
//  Questa struct rappresenta i nodi dei percorsi minimi calcolati da Dijkstra
    String citta;
    struct Path* next;
} Path;

typedef struct nodoHeap {
//  Sebbene questa struct sia identica alla struct Predecessore, il suo utilizzo e' necessario in quanto in questo caso costo e durata
//  non sono i pesi dell'arco del predecessore, ma i valori associati ad un vertice dall'algoritmo di Dijkstra man mano che avanza
    Vertex* v;
    float costo;
    int durata;
} nodoHeap;

typedef struct Heap {
//  Uno heap di vertici necessario per l'esecuzione di Dijkstra
    int currSize;
    int dimensione;
    int* pos;         //tiene traccia della posizione di un nodo nell'array
    nodoHeap** array;
} Heap;

typedef enum {white, black} color;

Graph* makeGraph (Graph* G) {

    //Alloca un grafo vuoto
    if (G)
        return G;

    G = (Graph*)malloc(sizeof(Graph));

    if (G) {
        G->V = 0;
        G->vertices = NULL;
    }

    return G;

}

Vertex* newVertex (String citta, int key) {

    //Crea un nuovo vertice
    Vertex* v = (Vertex*)malloc(sizeof(Vertex));
    if (v) {
        strcpy(v->citta, citta);
        v->key = key;
        v->visite = 0;
        v->adjacents = NULL;
        v->next = NULL;
    }
    return v;

}
Vertex* list_insert_Vertex (Vertex* list, String citta, int key) {

    //Inserisce un vertice nella lista di vertici del grafo, in ordine alfabetico
    if (!list)
        return newVertex(citta, key);

    else if (strcmp_literal(list->citta, citta) == 1) {
        Vertex* tmp = newVertex(citta, key);
        tmp->next = list;
        return tmp;
    }

    else if (list->next && strcmp_literal(list->next->citta, citta) == 1) {
        Vertex* tmp = newVertex(citta, key);
        tmp->next = list->next;
        list->next = tmp;
        return list;
    }

    else
        list->next = list_insert_Vertex(list->next, citta, key);

}
Vertex* list_delete_Vertex (Vertex* list, String citta) {

    //Elimina un vertice dalla lista di vertici del grafo
    if (list) {
        if (strcmp_literal(list->citta, citta) == 0) {
            Vertex* tmp = list;
            list = list->next;
            free(tmp);
        }
        else
            list->next = list_delete_Vertex(list->next, citta);
    }

    return list;

}
Vertex* getVertex (Vertex* v, String citta) {

    //Restituisce un puntatore all'elemento cercato nella lista di vertici del grafo
    if (!v)
        return NULL;

    else if (strcmp_literal(v->citta, citta) == 0)
        return v;

    else
        return getVertex(v->next, citta);

}

Adj_list* newAdj (Vertex* adj, float costo, int durata) {

    //Crea un nuovo nodo di adiacenza
    Adj_list* newAdj = (Adj_list*)malloc(sizeof(Adj_list));
    if (newAdj) {
        newAdj->currAdj = adj;
        newAdj->costo = costo;
        newAdj->durata = durata;
        newAdj->next = NULL;
    }
    return newAdj;

}
Adj_list* list_insert_Adj (Adj_list* list, Vertex* adj, float costo, int durata) {

    //Inserisce un nodo in una lista di adiacenza, in ordine di costo
    if (!list)
        return newAdj(adj, costo, durata);

    else if (list->costo >= costo) {
        Adj_list* tmp = newAdj(adj, costo, durata);
        tmp->next = list;
        return tmp;
    }

    else if (list->next && list->next->costo >= costo) {
        Adj_list* tmp = newAdj(adj, costo, durata);
        tmp->next = list->next;
        list->next = tmp;
        return list;
    }

    else
        list->next = list_insert_Adj(list->next, adj, costo, durata);

}
Adj_list* list_delete_Adj (Adj_list* list, Vertex* adj) {

    //Elimina un nodo da una lista di adiacenza
    if (list) {
        if (list->currAdj == adj) {
            Adj_list* tmp = list;
            list = list->next;
            free(tmp);
        }
        else
            list->next = list_delete_Adj(list->next, adj);
    }

    return list;

}
Adj_list* getAdj (Adj_list* list, Vertex* adj) {

    //Restituisce un puntatore all'elemento cercato in una lista di adiacenza
    if (!list)
        return NULL;

    else if (list->currAdj == adj)
        return list;

    else
        return getAdj(list->next, adj);

}

Graph* addVertex (Graph* G, String citta) {

    //Aggiunge al vertice al grafo dopo aver controllato che non sia gia' presente, inserendolo nella lista di vertici del grafo
    if (!G)
        G = makeGraph(G);

    if (getVertex(G->vertices, citta) == NULL) {
        G->vertices = list_insert_Vertex(G->vertices, citta, G->V);
        G->V++;
    }

    return G;

}
Graph* addEdge (Graph* G, String source_city, String target_city, float costo, int durata, bool display_message) {

//  Aggiunge un arco al grafo dopo aver controllato che i due estremi siano presenti e che l'arco non esista gia',
//  inserendo un adiacente contentente la citta' di destinazione alla lista di adiacenza della citta' di partenza

    if (!G || G->V == 0)
        return G;

    Vertex *v, *source = NULL, *target = NULL; int found = 0;
    for (v = G->vertices; v; v = v->next) {
        if (strcmp_literal(v->citta, source_city) == 0) {
            source = v;
            found++;
            if (found == 2) break;
        }
        if (strcmp_literal(v->citta, target_city) == 0) {
            target = v;
            found++;
            if (found == 2) break;
        }
    }

    if (!source) {
        printf("\n\n\"%s\" non trovata. Operazione fallita.\n", source_city);
        return G;
    }
    else if (!target) {
        printf("\n\n\"%s\" non trovata. Operazione fallita.\n", target_city);
        return G;
    }
    else if (getAdj(source->adjacents, target) == NULL) {
        source->adjacents = list_insert_Adj(source->adjacents, target, costo, durata);
        if (display_message == true)
            printf("\n\nOperazione eseguita.");
    }
    else
        printf("\n\nEsiste gia' una tratta tra queste due citta' !");

    return G;

}

void print_list_Adj (Adj_list* l) {

    //Stampa una lista di adiacenza
    if(l) {
        if (l->next)
            printf("%s (Costo: %.2f, Tempo: %d min.), ", l->currAdj->citta, l->costo, l->durata);
        else
            printf("%s (Costo: %.2f, Tempo: %d min.)", l->currAdj->citta, l->costo, l->durata);
        print_list_Adj(l->next);
    }

}
void print_list_Vertex (Vertex* list) {

    //Stampa la lista di vertici del grafo con relativo numero di prenotazioni effettuate
    if (list) {
        printf("%s (Visite: %d)\n", list->citta, list->visite);
        print_list_Vertex(list->next);
    }

}
void printGraph (Graph* G) {

    //Stampa il grafo esplicitando le liste di adiacenza
    if (!G || G->V == 0) {
        printf("Il grafo e' vuoto!");
        return;
    }

    Vertex* v;
    for (v = G->vertices; v != NULL; v = v->next) {
        printf("%s -> ", v->citta);
        print_list_Adj(v->adjacents);
        printf("\n\n");
    }

}

void free_list_Adj (Adj_list* list) {

    //Libera una lista di adiacenza
    if (list) {
        free_list_Adj(list->next);
        free(list);
    }

}
void free_list_Vertex (Vertex* list) {

    //Libera la lista di vertici
    if (list) {
        free_list_Vertex (list->next);
        free(list);
    }

}

Graph* fixKeys (Graph* G) {

    //Scorre la lista di vertici del grafo e fa si che ogni vertice abbia una key univoca compresa tra 1 e V
    if (!G || G->V == 0)
        return G;

    Vertex* v; int key = 0;
    for (v = G->vertices; v; v = v->next) {
        v->key = key;
        key++;
    }

    return G;

}
Graph* deleteVertex (Graph* G, String citta) {

//  Rimuove un vertice dal grafo eliminando la sua lista di adiacenza, ogni riferimento ad esso dalle liste di
//  adiacenza di altri vertici, e infine eliminandolo dalla lista di vertici del grafo. Dopo l'eliminazione si
//  effettua una chiamata a fixKeys per mantenere consistenti le chiavi di ciascun vertice.

    if (!G || G->V == 0)
        return G;

    Vertex* to_delete = getVertex(G->vertices, citta);

    if (to_delete == NULL)
        return G;

    free_list_Adj(to_delete->adjacents);

    Vertex* v;
    for (v = G->vertices; v; v = v->next)
        v->adjacents = list_delete_Adj(v->adjacents, to_delete);

    G->vertices = list_delete_Vertex(G->vertices, citta);
    G->V--;
    G = fixKeys(G);

    return G;

}
Graph* deleteEdge (Graph* G, String source_citta, String target_citta) {

//  Elimina un arco dal grafo dopo essersi assicurato che sorgente e destinazione esistano
//  (nel caso l'arco non esista la funzione termina normalmente senza apportare modifiche al grafo)

    if (!G || G->V == 0)
        return G;

    Vertex *v, *source = NULL, *target = NULL; int found = 0;
    for (v = G->vertices; v; v = v->next) {
        if (strcmp_literal(v->citta, source_citta) == 0) {
            source = v;
            found++;
            if (found == 2) break;
        }
        if (strcmp_literal(v->citta, target_citta) == 0) {
            target = v;
            found++;
            if (found == 2) break;
        }
    }

    if (!source || !target)
        return G;

    else
        source->adjacents = list_delete_Adj(source->adjacents, target);

    return G;

}

void destroyGraph (Graph* G) {

    //Dealloca il grafo liberando prima le liste di adiacenza di ogni vertice, poi i vertici, e infine il grafo stesso
    if (!G)
        return;

    Vertex* v;
    for(v = G->vertices; v; v = v->next)
        free_list_Adj (v->adjacents);

    free_list_Vertex(G->vertices);
    free(G);

}

Graph* initGraph (Graph* G) {

    //Inizializza il grafo con le destinazioni e le tratte
    G = addVertex(G, "Napoli");
    G = addVertex(G, "Roma");
    G = addVertex(G, "Milano");
    G = addVertex(G, "Zurigo");
    G = addVertex(G, "Parigi");
    G = addVertex(G, "Monaco");
    G = addVertex(G, "Londra");
    G = addVertex(G, "Madrid");
    G = addVertex(G, "Barcellona");
    G = addVertex(G, "Dublino");
    G = addVertex(G, "Manchester");
    G = addVertex(G, "Lisbona");
    G = addVertex(G, "Copenhagen");
    G = addVertex(G, "Oslo");
    G = addVertex(G, "Francoforte");
    G = addVertex(G, "Vienna");
    G = addVertex(G, "Istanbul");
    G = addVertex(G, "Amsterdam");
    G = addVertex(G, "Mosca");
    G = addVertex(G, "Stoccolma");

    G = addEdge(G, "Napoli", "Roma", 54.16, 30, false);
    G = addEdge(G, "Napoli", "Milano", 83.23, 65, false);
    G = addEdge(G, "Napoli", "Barcellona", 200.23, 90, false);
    G = addEdge(G, "Roma", "Milano", 46.89, 40, false);
    G = addEdge(G, "Roma", "Barcellona", 110.41, 90, false);
    G = addEdge(G, "Roma", "Monaco", 93.60, 85, false);
    G = addEdge(G, "Milano", "Francoforte", 91.58, 75, false);
    G = addEdge(G, "Milano", "Zurigo", 52.33, 30, false);
    G = addEdge(G, "Milano", "Parigi", 104.65, 90, false);
    G = addEdge(G, "Milano", "Vienna", 206.30, 40, false);
    G = addEdge(G, "Zurigo", "Milano", 51.11, 30, false);
    G = addEdge(G, "Zurigo", "Vienna", 88.14, 55, false);
    G = addEdge(G, "Zurigo", "Oslo", 811.64, 95, false);
    G = addEdge(G, "Parigi", "Monaco", 100.21, 90, false);
    G = addEdge(G, "Parigi", "Londra", 74.92, 45, false);
    G = addEdge(G, "Parigi", "Stoccolma", 610.88, 150, false);
    G = addEdge(G, "Monaco", "Milano", 80.51, 40, false);
    G = addEdge(G, "Monaco", "Parigi", 107, 90, false);
    G = addEdge(G, "Monaco", "Zurigo", 60.19, 35, false);
    G = addEdge(G, "Londra", "Francoforte", 96.11, 55, false);
    G = addEdge(G, "Londra", "Milano", 133.18, 120, false);
    G = addEdge(G, "Madrid", "Parigi", 124.43, 110, false);
    G = addEdge(G, "Madrid", "Lisbona", 50.90, 30, false);
    G = addEdge(G, "Madrid", "Barcellona", 62.74, 35, false);
    G = addEdge(G, "Madrid", "Monaco", 301.10, 150, false);
    G = addEdge(G, "Barcellona", "Napoli", 120.18, 110, false);
    G = addEdge(G, "Barcellona", "Madrid", 55.85, 35, false);
    G = addEdge(G, "Dublino", "Manchester", 62.03, 35, false);
    G = addEdge(G, "Dublino", "Londra", 73.33, 40, false);
    G = addEdge(G, "Manchester", "Londra", 55.37, 30, false);
    G = addEdge(G, "Manchester", "Dublino", 55, 35, false);
    G = addEdge(G, "Lisbona", "Barcellona", 89.98, 60, false);
    G = addEdge(G, "Lisbona", "Madrid", 61.79, 30, false);
    G = addEdge(G, "Copenhagen", "Amsterdam", 81.19, 45, false);
    G = addEdge(G, "Copenhagen", "Oslo", 77.63, 50, false);
    G = addEdge(G, "Oslo", "Amsterdam", 107.15, 80, false);
    G = addEdge(G, "Oslo", "Stoccolma", 66.01, 40, false);
    G = addEdge(G, "Oslo", "Copenhagen", 97.76, 50, false);
    G = addEdge(G, "Francoforte", "Vienna", 75.11, 40, false);
    G = addEdge(G, "Francoforte", "Amsterdam", 104.19, 70, false);
    G = addEdge(G, "Vienna", "Mosca", 206.11, 180, false);
    G = addEdge(G, "Vienna", "Istanbul", 136.85, 110, false);
    G = addEdge(G, "Istanbul", "Francoforte", 201.49, 180, false);
    G = addEdge(G, "Istanbul", "Mosca", 213.22, 200, false);
    G = addEdge(G, "Amsterdam", "Copenhagen", 76.54, 45, false);
    G = addEdge(G, "Amsterdam", "Stoccolma", 130.75, 100, false);
    G = addEdge(G, "Amsterdam", "Istanbul", 700.17, 180, false);
    G = addEdge(G, "Mosca", "Stoccolma", 175.69, 160, false);
    G = addEdge(G, "Mosca", "Istanbul", 231.65, 200, false);
    G = addEdge(G, "Stoccolma", "Copenhagen", 80.30, 50, false);
    G = addEdge(G, "Stoccolma", "Mosca", 185, 160, false);

    return G;

}


//LE SEGUENTI SONO FUNZIONI DI OPERAZIONE SU HEAP STRETTAMENTE NECESSARIE ALL'ESECUZIONE DI DIJKSTRA

nodoHeap* new_nodoHeap (Vertex* v, float costo, int durata) {

    //Alloca un nuovo nodo per l'heap
    nodoHeap* new = (nodoHeap*)malloc(sizeof(nodoHeap));
    if (new) {
        new->v = v;
        new->costo = costo;
        new->durata = durata;
    }
    return new;

}
Heap* makeHeap (int dimensione) {

    //Alloca un nuovo heap
    Heap* new = (Heap*)malloc(sizeof(Heap));
    if (new) {
        new->currSize = 0;
        new->dimensione = dimensione;
        new->pos = (int*)malloc(dimensione * sizeof(int));
        new->array = (nodoHeap**)malloc(dimensione * sizeof(nodoHeap*));
    }
    return new;

}
void swap_nodoHeap (nodoHeap** a, nodoHeap** b) {

    //Inverte la posizione di due puntatori a nodo per Heapify
    nodoHeap* t = *a;
    *a = *b;
    *b = t;

}
void Heapify (Heap* Heap, int i, variante variante) {

    //In base alla variante passata in ingresso, la procedura Hepify opera sull'heap tenendo conto dei costi o delle durate

    int min, l, r;

    min = i;
    l = 2*i + 1;
    r = 2*i + 2;

    if (variante == COSTO) {

        if (l < Heap->currSize && Heap->array[l]->costo < Heap->array[min]->costo)
            min = l;

        if (r < Heap->currSize && Heap->array[r]->costo < Heap->array[min]->costo)
            min = r;

    }

    if (variante == DURATA) {

        if (l < Heap->currSize && Heap->array[l]->durata < Heap->array[min]->durata)
            min = l;

        if (r < Heap->currSize && Heap->array[r]->durata < Heap->array[min]->durata)
            min = r;

    }

    if (min != i) {

        nodoHeap* min_node = Heap->array[min];
        nodoHeap* i_node = Heap->array[i];

        Heap->pos[min_node->v->key] = i;
        Heap->pos[i_node->v->key] = min;

        swap_nodoHeap(&Heap->array[min], &Heap->array[i]);

        Heapify(Heap, min, variante);

    }

}
nodoHeap* getMin (Heap* Heap, variante variante) {

    //Estrae il minimo costo o la minima durata dall'heap

    if (Heap->currSize == 0)
        return NULL;

    nodoHeap* radice = Heap->array[0];

    nodoHeap* ultimo = Heap->array[Heap->currSize - 1];
    Heap->array[0] = ultimo;

    Heap->pos[radice->v->key] = Heap->currSize - 1;
    Heap->pos[ultimo->v->key] = 0;

    Heap->currSize--;

    Heapify(Heap, 0, variante);

    return radice;

}
void decreaseKey (Heap* Heap, Vertex* v, float costo, int durata, variante variante) {

    //Aggiorna il valore del costo o della durata del vertice contenuto nel nodo, e ripristina la proprieta' di minHeap

    int i = Heap->pos[v->key];

    if (variante == COSTO) {

        Heap->array[i]->costo = costo;

        while (i > 0 && Heap->array[i]->costo < Heap->array[(i-1)/2]->costo) {

            Heap->pos[Heap->array[i]->v->key] = (i-1)/2;
            Heap->pos[Heap->array[(i-1)/2]->v->key] = i;
            swap_nodoHeap(&Heap->array[i], &Heap->array[(i-1)/2]);

            i = (i-1)/2;

        }

    }

    if (variante == DURATA) {

        Heap->array[i]->durata = durata;

        while (i && Heap->array[i]->durata < Heap->array[(i-1)/2]->durata) {

            Heap->pos[Heap->array[i]->v->key] = (i-1)/2;
            Heap->pos[Heap->array[(i-1)/2]->v->key] = i;
            swap_nodoHeap(&Heap->array[i], &Heap->array[(i-1)/2]);

            i = (i-1)/2;

        }

    }

}
bool isInHeap (Heap* Heap, Vertex* v) {

    //Controlla la presenza di un nodo nello Heap
    if (Heap->pos[v->key] < Heap->currSize)
        return true;
    else
        return false;

}
bool isEmpty(Heap* Heap) {
    //Controlla che l'Heap sia vuoto
    return (Heap->currSize == 0);
}
void destroyHeap (Heap* Heap) {

    //Libera la memoria occupata dall'Heap
    int i;
    for (i=0; i < Heap->dimensione; i++)
        free(Heap->array[i]);

    free(Heap->array);
    free(Heap->pos);
    free(Heap);


}

//QUI TERMINANO LE FUNZIONI DELL' HEAP


Predecessore* newPredecessore (Vertex* v, float costo, int durata) {

    //Alloca dinamicamente un nuovo predecessore
    Predecessore* new = (Predecessore*)malloc(sizeof(Predecessore));
    if (new) {
        new->vertex = v;
        new->costo = costo;
        new->durata = durata;
    }
    return new;

}
void free_Predecessori (Graph* G, Predecessore** p) {

    //Libera l'array di predecessori

    int i;

    for (i = 0; i < G->V; i++)
        free(p[i]);

    free(p);

}

Predecessore** Dijkstra (Graph* G, Vertex* s, Vertex* t, variante variante) {

//  La procedura Dijkstra alloca l'heap e un array dei predecessori da conservare in memoria per la
//  ricostruzione dei percorsi minimi, e a seconda della variante passata in ingresso calcolera' il
//  percorso di minor costo o durata

    Heap* Heap = makeHeap(G->V);

    Predecessore** p = (Predecessore**)malloc(G->V * sizeof(Predecessore*));

    Vertex *u, *v, *curr;
    Adj_list l;

    if (variante == COSTO) {

        float costo[G->V];

        for (v = G->vertices; v; v = v->next) {
            costo[v->key] = INF;
            p[v->key] = NULL;
            Heap->array[v->key] = new_nodoHeap(v, costo[v->key], NULLTIME);
            Heap->pos[v->key] = v->key;
        }

        costo[s->key] = 0;
        decreaseKey(Heap, s, costo[s->key], NULLTIME, 0);

        Heap->currSize = G->V;

        while (!isEmpty(Heap)) {

            nodoHeap* nodoHeap = getMin(Heap, variante);
            u = nodoHeap->v;

            if (u == t) {
                destroyHeap(Heap);
                return p;
            }

            Adj_list* l;
            for (l = u->adjacents; l; l = l->next) {
                curr = l->currAdj;
                if (isInHeap(Heap, curr) && costo[u->key] + l->costo < costo[curr->key] ) {
                    costo[curr->key] = costo[u->key] + l->costo;
                    decreaseKey(Heap, curr, costo[curr->key], NULLTIME, 0);
                    free(p[curr->key]);
                    p[curr->key] = newPredecessore(u, l->costo, l->durata);
                }
            }

        }

    }

    if (variante == DURATA) {

        int durata[G->V];

        for (v = G->vertices; v; v = v->next) {
            durata[v->key] = INF;
            p[v->key] = NULL;
            Heap->array[v->key] = new_nodoHeap(v, NULLCOST, durata[v->key]);
            Heap->pos[v->key] = v->key;
        }

        durata[s->key] = 0;
        decreaseKey(Heap, s, NULLCOST, durata[s->key], 1);

        Heap->currSize = G->V;

        while (!isEmpty(Heap)) {

            nodoHeap* nodoHeap = getMin(Heap, variante);
            u = nodoHeap->v;

            if (u == t) {
                destroyHeap(Heap);
                return p;
            }

            Adj_list* l;
            for (l = u->adjacents; l; l = l->next) {
                curr = l->currAdj;
                if (isInHeap(Heap, curr) && durata[u->key] + l->durata < durata[curr->key] ) {
                    durata[curr->key] = durata[u->key] + l->durata;
                    decreaseKey(Heap, curr, NULLCOST, durata[curr->key], 1);
                    free(p[curr->key]);
                    p[curr->key] = newPredecessore(u, l->costo, l->durata);
                }
            }

        }

    }

    destroyHeap(Heap);
    return p;

}

Path* newNode (String citta) {

    //Crea un nuovo nodo del percorso
    Path* node = (Path*)malloc(sizeof(Path));
    if (node) {
        strcpy(node->citta, citta);
        node->next = NULL;
    }
    return node;

}
Path* pushNode (Path* path, String citta) {

    //Inserice un nodo in cima al percorso
    Path* tmp = newNode(citta);
    tmp->next = path;
    return tmp;

}

Path* buildPath (Predecessore** p, Vertex* s, Vertex* t, float* costo, int* durata) {

//  Costruisce il percorso minimo tra due vertici partendo dall'ultimo e inserendo sempre in testa,
//  tramite l'array di predecessori, e aggiornando man mano il costo e la durata totali del percorso.

    Path* path = NULL;

    while (t && p[t->key]) {
        path = pushNode(path, t->citta);
        *costo += p[t->key]->costo;
        *durata += p[t->key]->durata;
        t = p[t->key]->vertex;
    }
    path = pushNode(path, s->citta);

    return path;

}
void printPath (Path* path) {

    //Stampa il percorso
    if (path) {
        printf("%s ", path->citta);
        if (path->next) {printf("-> ");}
        printPath(path->next);
    }

}
void freePath (Path* path) {

    //Libera la lista di vertici che rappresenta un percorso
    if (path) {
        freePath(path->next);
        free(path);
    }

}

bool isReachable_Visit (Vertex* s, Vertex* t, color c[]) {

    //Una variante di DFS_Visit per controllare se un vertice e' raggiungibile da un altro.
    //Viene invocata dalla funzione principale sotto.

    if (s == t)
        return true;

    c[s->key] = black;

    Adj_list* l; Vertex* v; int ret;

    for (l = s->adjacents; l; l = l->next) {
        v = l->currAdj;
        if (c[v->key] == white)
            ret = isReachable_Visit (v, t, c);
        if (ret == true)
            return true;
    }

    return false;

}
bool isReachable (Graph* G, Vertex* s, Vertex* t) {

    //Controlla che sorgente e destinazione esistano, alloca ed inizializza l'array di colori e invoca la visita
    if (!s || !t)
        return 0;

    color c[G->V];

    int i; bool ret;

    for (i=0; i < G->V; i++)
        c[i] = white;

    ret = isReachable_Visit(s, t, c);

    return ret;

}

void DFS_Reachables_Visit (Vertex* s, color* c, int flag) {

    //Una variante di DFS_Visit per visualizzare i vertici raggiungibili da un dato vertice.
    //Viene invocata dalla funzione principale sotto.

    if (flag)
        printf("%s (Visite: %d)\n", s->citta, s->visite);

    c[s->key] = black;

    Adj_list* l; Vertex* v;

    for (l = s->adjacents; l; l = l->next) {
        v = l->currAdj;
        if (c[v->key] == white)
            DFS_Reachables_Visit (v, c, 1);
    }

}
void DFS_Reachables (Graph* G, Vertex* s) {

    //Controlla che la sorgente esista, alloca ed inizializza l'array di colori e invoca la visita.
    if (!s)
        return;

    color c[G->V];

    int i;
    for (i=0; i < G->V; i++)
        c[i] = white;

    DFS_Reachables_Visit(s, c, 0);

}

Vertex* getPiuGettonata (Graph* G, Vertex* v, Vertex* source, Vertex* max) {

    //Restituisce la meta piu' gettonata raggiungibile da una data citta di partenza.
    //In caso di pareggio restituisce l'ultima trovata.

    if (!v)
        return max;

    else if (v->visite >= max->visite && isReachable(G, source, v) && v != source)
        max = v;

    return getPiuGettonata(G, v->next, source, max);

}

#endif
