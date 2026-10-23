#include "bellman_ford.h"
#include <stdio.h>
#include <stdlib.h>

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int V = g->V;

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        pred[i] = -1;
    }
    dist[origem] = 0;

    
    for (int i = 0; i < V - 1; i++) {
        for (int u = 0; u < V; u++) {
            No *atual = g->listaAdj[u];
            while (atual != NULL) {
                int v = atual->dest;
                int peso = atual->peso;

                if (dist[u] != INF && dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso;
                    pred[v] = u;
                }
                atual = atual->prox;
            }
        }
    }

    for (int u = 0; u < V; u++) {
        No *atual = g->listaAdj[u];
        while (atual != NULL) {
            int v = atual->dest;
            int peso = atual->peso;

            if (dist[u] != INF && dist[u] + peso < dist[v]) {
                pred[v] = u; 
                reconstruir_ciclo_negativo(g, v, pred);
                return 0; 
            }
            atual = atual->prox;
        }
    }

    return 1; 
}

void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int vertice_inicio, int *pred) {
    int V = g->V;
    int atual = vertice_inicio;

    for (int i = 0; i < V; i++) {
        atual = pred[atual];
    }

    int inicio_ciclo = atual;
    int *ciclo = (int *)malloc((V + 1) * sizeof(int));
    int tam = 0;

    ciclo[tam++] = atual;
    int temp = pred[atual];
    
    while (temp != inicio_ciclo) {
        ciclo[tam++] = temp;
        temp = pred[temp];
    }
    ciclo[tam++] = inicio_ciclo; 

    printf("Ciclo negativo detectado:\n");
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d", ciclo[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    free(ciclo);
}