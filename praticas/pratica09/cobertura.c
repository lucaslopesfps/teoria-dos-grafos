#include <stdio.h>
#include <stdlib.h>
#include "cobertura.h"

Grafo *criar_grafo(int num_vertices, int num_arestas) {
    if (num_vertices <= 0 || num_arestas < 0) {
        return NULL;
    }

    Grafo *g = malloc(sizeof(Grafo));

    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->num_arestas = num_arestas;
    g->arestas = NULL;

    if (num_arestas > 0) {
        g->arestas = malloc((size_t)num_arestas * sizeof(Aresta));

        if (g->arestas == NULL) {
            free(g);
            return NULL;
        }
    }

    return g;
}

void liberar_grafo(Grafo *g) {
    if (g != NULL) {
        free(g->arestas);
        free(g);
    }
}

int eh_cobertura(Grafo *g, int *selecionados) {
    if (g == NULL || selecionados == NULL) {
        return 0;
    }

    for (int i = 0; i < g->num_arestas; i++) {
        int u = g->arestas[i].u;
        int v = g->arestas[i].v;

        if (u < 0 || u >= g->num_vertices ||
            v < 0 || v >= g->num_vertices) {
            return 0;
        }

        if (!selecionados[u] && !selecionados[v]) {
            return 0;
        }
    }

    return 1;
}

static void buscar_cobertura(
    Grafo *g,
    int *atual,
    int *melhor,
    int indice,
    int quantidade,
    int *menor
) {
    if (quantidade >= *menor) {
        return;
    }

    if (eh_cobertura(g, atual)) {
        *menor = quantidade;

        for (int i = 0; i < g->num_vertices; i++) {
            melhor[i] = atual[i];
        }

        return;
    }

    if (indice == g->num_vertices) {
        return;
    }

    atual[indice] = 1;

    buscar_cobertura(
        g, atual, melhor, indice + 1,
        quantidade + 1, menor
    );

    atual[indice] = 0;

    buscar_cobertura(
        g, atual, melhor, indice + 1,
        quantidade, menor
    );
}

int cobertura_minima_bt(Grafo *g, int *solucao) {
    if (g == NULL || solucao == NULL) {
        return -1;
    }

    int n = g->num_vertices;

    int *atual = calloc((size_t)n, sizeof(int));

    if (atual == NULL) {
        return -1;
    }

    int menor = n + 1;

    for (int i = 0; i < n; i++) {
        solucao[i] = 0;
    }

    buscar_cobertura(g, atual, solucao, 0, 0, &menor);

    free(atual);

    return menor == n + 1 ? -1 : menor;
}

int cobertura_gulosa(Grafo *g, int *solucao) {
    if (g == NULL || solucao == NULL) {
        return -1;
    }

    int n = g->num_vertices;
    int quantidade = 0;

    for (int i = 0; i < n; i++) {
        solucao[i] = 0;
    }

    while (!eh_cobertura(g, solucao)) {
        int melhor_vertice = -1;
        int maior_grau = 0;

        for (int v = 0; v < n; v++) {
            if (solucao[v]) {
                continue;
            }

            int grau = 0;

            for (int i = 0; i < g->num_arestas; i++) {
                int u = g->arestas[i].u;
                int w = g->arestas[i].v;

                if (!solucao[u] && !solucao[w] &&
                    (u == v || w == v)) {
                    grau++;
                }
            }

            if (grau > maior_grau) {
                maior_grau = grau;
                melhor_vertice = v;
            }
        }

        if (melhor_vertice == -1) {
            return -1;
        }

        solucao[melhor_vertice] = 1;
        quantidade++;
    }

    return quantidade;
}
