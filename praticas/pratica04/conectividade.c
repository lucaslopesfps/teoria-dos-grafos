#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

GrafoLista *criar_grafo(int num_vertices) {
    GrafoLista *g = malloc(sizeof(GrafoLista));

    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->lista = calloc((size_t)num_vertices, sizeof(No *));

    if (g->lista == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

void adicionar_aresta(GrafoLista *g, int origem, int destino) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        return;
    }

    novo->vertice = destino;
    novo->prox = g->lista[origem];
    g->lista[origem] = novo;

    novo = malloc(sizeof(No));

    if (novo == NULL) {
        return;
    }

    novo->vertice = origem;
    novo->prox = g->lista[destino];
    g->lista[destino] = novo;
}

void liberar_grafo(GrafoLista *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->lista[i];

        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }

    free(g->lista);
    free(g);
}

void dfs_articulacoes(GrafoLista *g, int u, int *visitado,
                      int *descoberta, int *low, int *pai,
                      int *articulacao, int *tempo) {
    visitado[u] = 1;
    descoberta[u] = low[u] = ++(*tempo);

    int filhos = 0;
    No *atual = g->lista[u];

    while (atual != NULL) {
        int v = atual->vertice;

        if (!visitado[v]) {
            filhos++;
            pai[v] = u;

            dfs_articulacoes(g, v, visitado, descoberta,
                             low, pai, articulacao, tempo);

            if (low[v] < low[u]) {
                low[u] = low[v];
            }

            if (pai[u] == -1 && filhos > 1) {
                articulacao[u] = 1;
            }

            if (pai[u] != -1 && low[v] >= descoberta[u]) {
                articulacao[u] = 1;
            }
        } else if (v != pai[u]) {
            if (descoberta[v] < low[u]) {
                low[u] = descoberta[v];
            }
        }

        atual = atual->prox;
    }
}

void encontrar_articulacoes(GrafoLista *g) {
    int n = g->num_vertices;

    int *visitado = calloc((size_t)n, sizeof(int));
    int *descoberta = calloc((size_t)n, sizeof(int));
    int *low = calloc((size_t)n, sizeof(int));
    int *pai = malloc((size_t)n * sizeof(int));
    int *articulacao = calloc((size_t)n, sizeof(int));

    if (visitado == NULL || descoberta == NULL || low == NULL ||
        pai == NULL || articulacao == NULL) {
        free(visitado);
        free(descoberta);
        free(low);
        free(pai);
        free(articulacao);
        return;
    }

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
    }

    int tempo = 0;

    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            dfs_articulacoes(g, i, visitado, descoberta,
                             low, pai, articulacao, &tempo);
        }
    }

    printf("Vertices de corte: ");

    for (int i = 0; i < n; i++) {
        if (articulacao[i]) {
            printf("%d ", i);
        }
    }

    printf("\n");

    free(visitado);
    free(descoberta);
    free(low);
    free(pai);
    free(articulacao);
}

static void dfs_pontes(GrafoLista *g, int u, int *visitado,
                       int *descoberta, int *low, int *pai,
                       int *tempo) {
    visitado[u] = 1;
    descoberta[u] = low[u] = ++(*tempo);

    No *atual = g->lista[u];

    while (atual != NULL) {
        int v = atual->vertice;

        if (!visitado[v]) {
            pai[v] = u;

            dfs_pontes(g, v, visitado, descoberta,
                       low, pai, tempo);

            if (low[v] < low[u]) {
                low[u] = low[v];
            }

            if (low[v] > descoberta[u]) {
                printf("Ponte: %d - %d\n", u, v);
            }
        } else if (v != pai[u]) {
            if (descoberta[v] < low[u]) {
                low[u] = descoberta[v];
            }
        }

        atual = atual->prox;
    }
}

void detectar_pontes(GrafoLista *g) {
    int n = g->num_vertices;

    int *visitado = calloc((size_t)n, sizeof(int));
    int *descoberta = calloc((size_t)n, sizeof(int));
    int *low = calloc((size_t)n, sizeof(int));
    int *pai = malloc((size_t)n * sizeof(int));

    if (visitado == NULL || descoberta == NULL ||
        low == NULL || pai == NULL) {
        free(visitado);
        free(descoberta);
        free(low);
        free(pai);
        return;
    }

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
    }

    int tempo = 0;

    for (int i = 0; i < n; i++) {
        if (!visitado[i]) {
            dfs_pontes(g, i, visitado, descoberta,
                       low, pai, &tempo);
        }
    }

    free(visitado);
    free(descoberta);
    free(low);
    free(pai);
}