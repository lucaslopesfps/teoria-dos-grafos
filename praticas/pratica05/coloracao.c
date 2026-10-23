#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

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

static int pode_usar_cor(GrafoLista *g, int vertice,
                         int cor, int *cores) {
    No *atual = g->lista[vertice];

    while (atual != NULL) {
        if (cores[atual->vertice] == cor) {
            return 0;
        }

        atual = atual->prox;
    }

    return 1;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;
    int *cores = malloc((size_t)n * sizeof(int));

    if (cores == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        cores[i] = -1;
    }

    int maior_cor = -1;

    for (int u = 0; u < n; u++) {
        int cor = 0;

        while (!pode_usar_cor(g, u, cor, cores)) {
            cor++;
        }

        cores[u] = cor;

        if (cor > maior_cor) {
            maior_cor = cor;
        }
    }

    *num_cores = maior_cor + 1;

    return cores;
}

static int grau_vertice(GrafoLista *g, int vertice) {
    int grau = 0;
    No *atual = g->lista[vertice];

    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }

    return grau;
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int n = g->num_vertices;

    int *cores = malloc((size_t)n * sizeof(int));
    int *ordem = malloc((size_t)n * sizeof(int));
    int *graus = malloc((size_t)n * sizeof(int));

    if (cores == NULL || ordem == NULL || graus == NULL) {
        free(cores);
        free(ordem);
        free(graus);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        cores[i] = -1;
        ordem[i] = i;
        graus[i] = grau_vertice(g, i);
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (graus[ordem[j]] > graus[ordem[i]]) {
                int temp = ordem[i];
                ordem[i] = ordem[j];
                ordem[j] = temp;
            }
        }
    }

    int cor_atual = 0;

    for (int i = 0; i < n; i++) {
        int u = ordem[i];

        if (cores[u] != -1) {
            continue;
        }

        cores[u] = cor_atual;

        for (int j = i + 1; j < n; j++) {
            int v = ordem[j];

            if (cores[v] == -1 &&
                pode_usar_cor(g, v, cor_atual, cores)) {
                cores[v] = cor_atual;
            }
        }

        cor_atual++;
    }

    *num_cores = cor_atual;

    free(ordem);
    free(graus);

    return cores;
}

int eh_bipartido(GrafoLista *g) {
    int n = g->num_vertices;
    int *cores = malloc((size_t)n * sizeof(int));
    int *fila = malloc((size_t)n * sizeof(int));

    if (cores == NULL || fila == NULL) {
        free(cores);
        free(fila);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        cores[i] = -1;
    }

    for (int inicio = 0; inicio < n; inicio++) {
        if (cores[inicio] != -1) {
            continue;
        }

        int frente = 0;
        int tras = 0;

        fila[tras++] = inicio;
        cores[inicio] = 0;

        while (frente < tras) {
            int u = fila[frente++];
            No *atual = g->lista[u];

            while (atual != NULL) {
                int v = atual->vertice;

                if (cores[v] == -1) {
                    cores[v] = 1 - cores[u];
                    fila[tras++] = v;
                } else if (cores[v] == cores[u]) {
                    free(cores);
                    free(fila);
                    return 0;
                }

                atual = atual->prox;
            }
        }
    }

    free(cores);
    free(fila);

    return 1;
}