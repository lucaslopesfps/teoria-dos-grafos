#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista *criar_grafo_lista(int n) {
    GrafoLista *g = malloc(sizeof(GrafoLista));

    if (g == NULL) {
        return NULL;
    }

    g->n = n;
    g->adj = calloc((size_t)n, sizeof(No *));

    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

static int vertice_valido(GrafoLista *g, int vertice) {
    return g != NULL && vertice >= 0 && vertice < g->n;
}

static int existe_na_lista(No *inicio, int destino) {
    No *atual = inicio;

    while (atual != NULL) {
        if (atual->destino == destino) {
            return 1;
        }

        atual = atual->prox;
    }

    return 0;
}

static void inserir_em_lista(No **inicio, int destino) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        return;
    }

    novo->destino = destino;
    novo->prox = *inicio;
    *inicio = novo;
}

void inserir_aresta_lista(GrafoLista *g, int origem, int destino) {
    if (!vertice_valido(g, origem) ||
        !vertice_valido(g, destino) ||
        origem == destino) {
        return;
    }

    if (existe_na_lista(g->adj[origem], destino)) {
        return;
    }

    inserir_em_lista(&g->adj[origem], destino);
    inserir_em_lista(&g->adj[destino], origem);
}

static void remover_da_lista(No **inicio, int destino) {
    No *atual = *inicio;
    No *anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == destino) {
            if (anterior == NULL) {
                *inicio = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }
}

void remover_aresta_lista(GrafoLista *g, int origem, int destino) {
    if (!vertice_valido(g, origem) ||
        !vertice_valido(g, destino)) {
        return;
    }

    remover_da_lista(&g->adj[origem], destino);
    remover_da_lista(&g->adj[destino], origem);
}

int grau_lista(GrafoLista *g, int vertice) {
    if (!vertice_valido(g, vertice)) {
        return -1;
    }

    int grau = 0;
    No *atual = g->adj[vertice];

    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }

    return grau;
}

int sao_adjacentes_lista(GrafoLista *g, int origem, int destino) {
    if (!vertice_valido(g, origem) ||
        !vertice_valido(g, destino)) {
        return 0;
    }

    return existe_na_lista(g->adj[origem], destino);
}

void liberar_grafo_lista(GrafoLista *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];

        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }

    free(g->adj);
    free(g);
}