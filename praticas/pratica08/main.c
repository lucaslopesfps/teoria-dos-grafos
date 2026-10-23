
#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

int main(void) {
    int num_vertices = 5;
    int num_arestas = 7;

    Aresta arestas[] = {
        {0, 1, 2},
        {0, 3, 6},
        {1, 2, 3},
        {1, 3, 8},
        {1, 4, 5},
        {2, 4, 7},
        {3, 4, 9}
    };

    Aresta *resultado = malloc(
        (size_t)(num_vertices - 1) * sizeof(Aresta)
    );

    if (resultado == NULL) {
        printf("Erro de memoria.\n");
        return 1;
    }

    int peso_kruskal = 0;

    printf("=== Kruskal ===\n");

    if (kruskal(num_vertices, arestas, num_arestas,
                resultado, &peso_kruskal)) {
        for (int i = 0; i < num_vertices - 1; i++) {
            printf("%d -- %d (peso %d)\n",
                   resultado[i].u,
                   resultado[i].v,
                   resultado[i].peso);
        }

        printf("Peso total: %d\n", peso_kruskal);
    } else {
        printf("Nao foi possivel encontrar uma AGM.\n");
    }

    GrafoListaPonderado *g = criar_grafo(num_vertices);

    if (g == NULL) {
        free(resultado);
        printf("Erro ao criar grafo.\n");
        return 1;
    }

    for (int i = 0; i < num_arestas; i++) {
        adicionar_aresta(
            g,
            arestas[i].u,
            arestas[i].v,
            arestas[i].peso
        );
    }

    int *pai = malloc((size_t)num_vertices * sizeof(int));

    if (pai == NULL) {
        liberar_grafo(g);
        free(resultado);
        printf("Erro de memoria.\n");
        return 1;
    }

    int peso_prim = 0;

    printf("\n=== Prim ===\n");

    if (prim(g, 0, pai, &peso_prim)) {
        for (int i = 0; i < num_vertices; i++) {
            if (pai[i] != -1) {
                printf("%d -- %d\n", pai[i], i);
            }
        }

        printf("Peso total: %d\n", peso_prim);
    } else {
        printf("Nao foi possivel encontrar uma AGM.\n");
    }

    free(pai);
    free(resultado);
    liberar_grafo(g);

    return 0;
}
