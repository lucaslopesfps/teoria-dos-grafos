#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

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
    novo->proximo = g->lista[origem];
    g->lista[origem] = novo;
}

void liberar_grafo(GrafoLista *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->lista[i];

        while (atual != NULL) {
            No *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }

    free(g->lista);
    free(g);
}

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int *grau_entrada = calloc((size_t)n, sizeof(int));
    int *fila = malloc((size_t)n * sizeof(int));
    int *ordem = malloc((size_t)n * sizeof(int));

    if (grau_entrada == NULL || fila == NULL || ordem == NULL) {
        free(grau_entrada);
        free(fila);
        free(ordem);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        No *atual = g->lista[i];

        while (atual != NULL) {
            grau_entrada[atual->vertice]++;
            atual = atual->proximo;
        }
    }

    int inicio = 0;
    int fim = 0;

    for (int i = 0; i < n; i++) {
        if (grau_entrada[i] == 0) {
            fila[fim++] = i;
        }
    }

    int quantidade = 0;

    while (inicio < fim) {
        int u = fila[inicio++];
        ordem[quantidade++] = u;

        No *atual = g->lista[u];

        while (atual != NULL) {
            int v = atual->vertice;
            grau_entrada[v]--;

            if (grau_entrada[v] == 0) {
                fila[fim++] = v;
            }

            atual = atual->proximo;
        }
    }

    free(grau_entrada);
    free(fila);

    if (quantidade != n) {
        free(ordem);
        *tamanho = 0;
        return NULL;
    }

    *tamanho = quantidade;
    return ordem;
}

static int dfs_topologica(GrafoLista *g, int vertice, int *estado,
                          int *ordem, int *posicao) {
    estado[vertice] = 1;

    No *atual = g->lista[vertice];

    while (atual != NULL) {
        int v = atual->vertice;

        if (estado[v] == 1) {
            return 0;
        }

        if (estado[v] == 0) {
            if (!dfs_topologica(g, v, estado, ordem, posicao)) {
                return 0;
            }
        }

        atual = atual->proximo;
    }

    estado[vertice] = 2;
    ordem[(*posicao)--] = vertice;

    return 1;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int n = g->num_vertices;
    int *estado = calloc((size_t)n, sizeof(int));
    int *ordem = malloc((size_t)n * sizeof(int));

    if (estado == NULL || ordem == NULL) {
        free(estado);
        free(ordem);
        return NULL;
    }

    int posicao = n - 1;

    for (int i = 0; i < n; i++) {
        if (estado[i] == 0) {
            if (!dfs_topologica(g, i, estado, ordem, &posicao)) {
                free(estado);
                free(ordem);
                *tamanho = 0;
                return NULL;
            }
        }
    }

    free(estado);

    *tamanho = n;
    return ordem;
}

int eh_dag(GrafoLista *g) {
    int tamanho = 0;
    int *ordem = ordenacao_topologica_kahn(g, &tamanho);

    if (ordem == NULL) {
        return 0;
    }

    free(ordem);
    return 1;
}