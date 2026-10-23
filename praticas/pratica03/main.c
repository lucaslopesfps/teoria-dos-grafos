#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

static void exibir_ordem(const char *nome, int *ordem, int tamanho) {
    printf("%s: ", nome);

    for (int i = 0; i < tamanho; i++) {
        printf("%d", ordem[i]);

        if (i < tamanho - 1) {
            printf(" -> ");
        }
    }

    printf("\n");
}

int main(void) {
    GrafoLista *g = criar_grafo(6);

    if (g == NULL) {
        printf("Erro ao criar o grafo.\n");
        return 1;
    }

    adicionar_aresta(g, 5, 2);
    adicionar_aresta(g, 5, 0);
    adicionar_aresta(g, 4, 0);
    adicionar_aresta(g, 4, 1);
    adicionar_aresta(g, 2, 3);
    adicionar_aresta(g, 3, 1);

    printf("=== ORDENACAO TOPOLOGICA ===\n");

    if (eh_dag(g)) {
        printf("O grafo e um DAG.\n");
    } else {
        printf("O grafo possui ciclo.\n");
    }

    int tamanho_kahn = 0;
    int *ordem_kahn = ordenacao_topologica_kahn(g, &tamanho_kahn);

    if (ordem_kahn != NULL) {
        exibir_ordem("Kahn", ordem_kahn, tamanho_kahn);
        free(ordem_kahn);
    } else {
        printf("Kahn: ordenacao impossivel (grafo com ciclo).\n");
    }

    int tamanho_dfs = 0;
    int *ordem_dfs = ordenacao_topologica_dfs(g, &tamanho_dfs);

    if (ordem_dfs != NULL) {
        exibir_ordem("DFS", ordem_dfs, tamanho_dfs);
        free(ordem_dfs);
    } else {
        printf("DFS: ordenacao impossivel (grafo com ciclo).\n");
    }

    printf("\n=== TESTE COM CICLO ===\n");

    adicionar_aresta(g, 1, 5);

    if (eh_dag(g)) {
        printf("O grafo e um DAG.\n");
    } else {
        printf("O grafo possui ciclo. Ordenacao impossivel.\n");
    }

    liberar_grafo(g);

    return 0;
}