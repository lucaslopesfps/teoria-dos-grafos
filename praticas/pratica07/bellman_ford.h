#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#define INF 1000000000 

typedef struct No {
    int dest;
    int peso;
    struct No* prox;
} No;

typedef struct {
    int V;          
    No** listaAdj;  
} GrafoListaPonderado;

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int vertice_inicio, int *pred);

#endif