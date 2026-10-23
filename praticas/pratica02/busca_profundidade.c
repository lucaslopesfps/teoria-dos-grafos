#include <stdlib.h>
#include "busca_profundidade.h"


void dfs_recursiva(GrafoLista *g, int u, int *visitado,
                   int *tempo_entrada, int *tempo_saida, int *tempo) {
    visitado[u] = 1;
    (*tempo)++;
    tempo_entrada[u] = *tempo;

    No *atual = g->adjacencia[u];

    while (atual != NULL) {
        int v = atual->vertice;

        if (!visitado[v]) {
            dfs_recursiva(g, v, visitado,
                          tempo_entrada, tempo_saida, tempo);
        }

        atual = atual->proximo;
    }

    (*tempo)++;
    tempo_saida[u] = *tempo;
}


int contar_componentes(GrafoLista *g) {
    int visitado[LIMITE] = {0};
    int tempo_entrada[LIMITE];
    int tempo_saida[LIMITE];
    int tempo = 0;
    int componentes = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            componentes++;

            dfs_recursiva(g, i, visitado,
                          tempo_entrada, tempo_saida, &tempo);
        }
    }

    return componentes;
}

static int ciclo_dfs(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1;

    No *atual = g->adjacencia[u];

    while (atual != NULL) {
        int v = atual->vertice;

        if (!visitado[v]) {
            if (ciclo_dfs(g, v, u, visitado)) {
                return 1;
            }
        } else if (v != pai) {
            return 1;
        }

        atual = atual->proximo;
    }

    return 0;
}


int tem_ciclo(GrafoLista *g) {
    int visitado[LIMITE] = {0};

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            if (ciclo_dfs(g, i, -1, visitado)) {
                return 1;
            }
        }
    }

    return 0;
}