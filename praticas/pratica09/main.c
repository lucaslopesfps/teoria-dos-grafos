#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

static void imprimir_solucao(int *solucao, int n) {
    printf("{ ");

    for (int i = 0; i < n; i++) {
        if (solucao[i]) {
            printf("%d ", i);
        }
    }

    printf("}");
}

static Grafo *gerar_grafo(int n) {
    int quantidade = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (j == i + 1 || (i + j) % 3 == 0) {
                quantidade++;
            }
        }
    }

    Grafo *g = criar_grafo(n, quantidade);

    if (g == NULL) {
        return NULL;
    }

    int k = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (j == i + 1 || (i + j) % 3 == 0) {
                g->arestas[k].u = i;
                g->arestas[k].v = j;
                k++;
            }
        }
    }

    return g;
}

int main(void) {
    int tamanhos[] = {5, 8, 10, 12, 15};
    int quantidade_testes = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("=== PRATICA 09 - COBERTURA DE VERTICES ===\n");

    for (int t = 0; t < quantidade_testes; t++) {
        int n = tamanhos[t];

        Grafo *g = gerar_grafo(n);

        if (g == NULL) {
            printf("Erro ao criar grafo com %d vertices.\n", n);
            return 1;
        }

        int *solucao_bt = calloc((size_t)n, sizeof(int));
        int *solucao_gulosa = calloc((size_t)n, sizeof(int));

        if (solucao_bt == NULL || solucao_gulosa == NULL) {
            printf("Erro de memoria.\n");
            free(solucao_bt);
            free(solucao_gulosa);
            liberar_grafo(g);
            return 1;
        }

        clock_t inicio = clock();
        int qtd_bt = cobertura_minima_bt(g, solucao_bt);
        clock_t fim = clock();

        double tempo_bt = (double)(fim - inicio) / CLOCKS_PER_SEC;

        inicio = clock();
        int qtd_gulosa = cobertura_gulosa(g, solucao_gulosa);
        fim = clock();

        double tempo_gulosa = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("\nGrafo com %d vertices e %d arestas\n",
               n, g->num_arestas);

        printf("Backtracking: %d vertices | Tempo: %.6f s\n",
               qtd_bt, tempo_bt);
        printf("Solucao: ");
        imprimir_solucao(solucao_bt, n);
        printf("\n");

        printf("Guloso:       %d vertices | Tempo: %.6f s\n",
               qtd_gulosa, tempo_gulosa);
        printf("Solucao: ");
        imprimir_solucao(solucao_gulosa, n);
        printf("\n");

        printf("Cobertura BT valida: %s\n",
               eh_cobertura(g, solucao_bt) ? "SIM" : "NAO");

        printf("Cobertura gulosa valida: %s\n",
               eh_cobertura(g, solucao_gulosa) ? "SIM" : "NAO");

        free(solucao_bt);
        free(solucao_gulosa);
        liberar_grafo(g);
    }

    return 0;
}
