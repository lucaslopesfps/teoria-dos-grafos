#include <stdio.h>
#include <stdlib.h>
#include "hamiltoniano.h"

Grafo *criar_grafo(int num_vertices) {
    if (num_vertices <= 0) {
        return NULL;
    }

    Grafo *g = malloc(sizeof(Grafo));
    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->adj = calloc(num_vertices, sizeof(int *));

    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->adj[i] = calloc(num_vertices, sizeof(int));

        if (g->adj[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(g->adj[j]);
            }
            free(g->adj);
            free(g);
            return NULL;
        }
    }

    return g;
}

void adicionar_aresta(Grafo *g, int u, int v) {
    if (g == NULL || u < 0 || v < 0 ||
        u >= g->num_vertices || v >= g->num_vertices ||
        u == v) {
        return;
    }

    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void liberar_grafo(Grafo *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        free(g->adj[i]);
    }

    free(g->adj);
    free(g);
}

static int buscar_hamiltoniano(Grafo *g, int *caminho,
                               int *visitado, int pos,
                               int exigir_ciclo) {
    int n = g->num_vertices;

    if (pos == n) {
        if (!exigir_ciclo ||
            g->adj[caminho[n - 1]][caminho[0]]) {
            return 1;
        }

        return 0;
    }

    for (int v = 0; v < n; v++) {
        if (!visitado[v] &&
            g->adj[caminho[pos - 1]][v]) {

            caminho[pos] = v;
            visitado[v] = 1;

            if (buscar_hamiltoniano(g, caminho,
                                   visitado, pos + 1,
                                   exigir_ciclo)) {
                return 1;
            }

            visitado[v] = 0;
        }
    }

    return 0;
}

int encontrar_caminho_hamiltoniano_bt(Grafo *g,
                                      int *caminho) {
    if (g == NULL || caminho == NULL) {
        return 0;
    }

    int n = g->num_vertices;
    int *visitado = calloc(n, sizeof(int));

    if (visitado == NULL) {
        return 0;
    }

    int encontrou = 0;

    for (int inicio = 0; inicio < n; inicio++) {
        caminho[0] = inicio;
        visitado[inicio] = 1;

        if (buscar_hamiltoniano(g, caminho,
                               visitado, 1, 0)) {
            encontrou = 1;
            break;
        }

        visitado[inicio] = 0;
    }

    free(visitado);
    return encontrou;
}

int encontrar_ciclo_hamiltoniano_bt(Grafo *g,
                                    int *ciclo) {
    if (g == NULL || ciclo == NULL) {
        return 0;
    }

    int n = g->num_vertices;
    int *visitado = calloc(n, sizeof(int));

    if (visitado == NULL) {
        return 0;
    }

    ciclo[0] = 0;
    visitado[0] = 1;

    int encontrou = buscar_hamiltoniano(g, ciclo,
                                        visitado, 1, 1);

    if (encontrou) {
        ciclo[n] = ciclo[0];
    }

    free(visitado);
    return encontrou;
}
