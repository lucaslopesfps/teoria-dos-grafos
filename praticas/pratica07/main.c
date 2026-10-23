#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

void adicionar_aresta(GrafoListaPonderado *g, int origem, int dest, int peso) {
    No *novo = (No *)malloc(sizeof(No));
    novo->dest = dest;
    novo->peso = peso;
    novo->prox = g->listaAdj[origem];
    g->listaAdj[origem] = novo;
}

int main() {
    int V = 5;
    GrafoListaPonderado g;
    g.V = V;
    g.listaAdj = (No **)malloc(V * sizeof(No *));
    for (int i = 0; i < V; i++) {
        g.listaAdj[i] = NULL;
    }

    adicionar_aresta(&g, 0, 1, -1);
    adicionar_aresta(&g, 1, 2, 2);
    adicionar_aresta(&g, 2, 3, -3);
    adicionar_aresta(&g, 3, 1, 1); 
    adicionar_aresta(&g, 0, 4, 4);

    int origem = 0;
    int *dist = (int *)malloc(V * sizeof(int));
    int *pred = (int *)malloc(V * sizeof(int));

    printf("Executando Bellman-Ford a partir do vértice %d...\n", origem);
    int resultado = bellman_ford(&g, origem, dist, pred);

    if (resultado) {
        printf("\nMenores distâncias a partir da origem:\n");
        for (int i = 0; i < V; i++) {
            if (dist[i] == INF)
                printf("Vértice %d: INF\n", i);
            else
                printf("Vértice %d: %d\n", i, dist[i]);
        }
    } else {
        printf("\nO grafo contém ciclo(s) de custo negativo!\n");
    }

    for (int i = 0; i < V; i++) {
        No *atual = g.listaAdj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g.listaAdj);
    free(dist);
    free(pred);

    return 0;
}