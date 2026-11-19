
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hamiltoniano.h"
#include "tps.h"

static void imprimir_caminho(int *caminho, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d", caminho[i]);

        if (i < tamanho - 1) {
            printf(" -> ");
        }
    }

    printf("\n");
}

static void executar_experimento(int n) {
    Grafo *g = criar_grafo(n);

    if (g == NULL) {
        printf("Erro ao criar grafo.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (j == i + 1 || (i + j) % 3 == 0) {
                adicionar_aresta(g, i, j);
            }
        }
    }

    GrafoTSP *tsp = grafo_para_tsp(g);

    int *caminho = malloc((n + 1) * sizeof(int));
    int *ciclo = malloc((n + 1) * sizeof(int));
    int *rota = malloc((n + 1) * sizeof(int));

    if (tsp == NULL || caminho == NULL ||
        ciclo == NULL || rota == NULL) {
        printf("Erro de alocacao de memoria.\n");

        free(caminho);
        free(ciclo);
        free(rota);
        liberar_grafo_tsp(tsp);
        liberar_grafo(g);
        return;
    }

    printf("\n====================================\n");
    printf("GRAFO COM %d VERTICES\n", n);
    printf("====================================\n");

    clock_t inicio, fim;
    double tempo;
    int resultado;

    inicio = clock();
    resultado = encontrar_caminho_hamiltoniano_bt(g, caminho);
    fim = clock();

    tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nCaminho Hamiltoniano:\n");
    printf("Existe: %s\n", resultado ? "SIM" : "NAO");

    if (resultado) {
        imprimir_caminho(caminho, n);
    }

    printf("Tempo: %.6f s\n", tempo);

    inicio = clock();
    resultado = encontrar_ciclo_hamiltoniano_bt(g, ciclo);
    fim = clock();

    tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nCiclo Hamiltoniano:\n");
    printf("Existe: %s\n", resultado ? "SIM" : "NAO");

    if (resultado) {
        imprimir_caminho(ciclo, n + 1);
    }

    printf("Tempo: %.6f s\n", tempo);

    inicio = clock();
    resultado = tsp_forca_bruta(tsp, rota);
    fim = clock();

    tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTSP Forca Bruta:\n");
    printf("Custo: %d\n", resultado);

    if (resultado >= 0) {
        imprimir_caminho(rota, n + 1);
    }

    printf("Tempo: %.6f s\n", tempo);

    inicio = clock();
    resultado = tsp_vizinho_mais_proximo(tsp, rota);
    fim = clock();

    tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTSP Vizinho Mais Proximo:\n");
    printf("Custo: %d\n", resultado);

    if (resultado >= 0) {
        imprimir_caminho(rota, n + 1);
    }

    printf("Tempo: %.6f s\n", tempo);

    inicio = clock();
    resultado = tsp_insercao(tsp, rota);
    fim = clock();

    tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTSP Insercao:\n");
    printf("Custo: %d\n", resultado);

    if (resultado >= 0) {
        imprimir_caminho(rota, n + 1);
    }

    printf("Tempo: %.6f s\n", tempo);

    free(caminho);
    free(ciclo);
    free(rota);

    liberar_grafo_tsp(tsp);
    liberar_grafo(g);
}

int main(void) {
    int tamanhos[] = {5, 8, 10, 12};
    int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("PRATICA 10 - CAMINHO HAMILTONIANO E TSP\n");

    for (int i = 0; i < quantidade; i++) {
        executar_experimento(tamanhos[i]);
    }

    return 0;
}
