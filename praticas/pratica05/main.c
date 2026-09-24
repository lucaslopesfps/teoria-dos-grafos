#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

static void mostrar_coloracao(const char *nome, int *cores,
                              int num_vertices, int num_cores) {
    printf("%s\n", nome);

    for (int i = 0; i < num_vertices; i++) {
        printf("Vertice %d -> Cor %d\n", i, cores[i]);
    }

    printf("Numero de cores utilizadas: %d\n\n", num_cores);
}

int main(void) {
    GrafoLista *g = criar_grafo(6);

    if (g == NULL) {
        printf("Erro ao criar o grafo.\n");
        return 1;
    }

    adicionar_aresta(g, 0, 1);
    adicionar_aresta(g, 0, 2);
    adicionar_aresta(g, 1, 2);
    adicionar_aresta(g, 1, 3);
    adicionar_aresta(g, 2, 4);
    adicionar_aresta(g, 3, 4);
    adicionar_aresta(g, 3, 5);
    adicionar_aresta(g, 4, 5);

    printf("=== PRATICA 05 - COLORACAO DE GRAFOS ===\n\n");

    int num_cores_gulosa = 0;
    int *cores_gulosa = coloracao_gulosa(g, &num_cores_gulosa);

    if (cores_gulosa == NULL) {
        liberar_grafo(g);
        return 1;
    }

    mostrar_coloracao("=== COLORACAO GULOSA ===",
                      cores_gulosa,
                      g->num_vertices,
                      num_cores_gulosa);

    int num_cores_welsh = 0;
    int *cores_welsh = coloracao_welsh_powell(g, &num_cores_welsh);

    if (cores_welsh == NULL) {
        free(cores_gulosa);
        liberar_grafo(g);
        return 1;
    }

    mostrar_coloracao("=== WELSH-POWELL ===",
                      cores_welsh,
                      g->num_vertices,
                      num_cores_welsh);

    printf("=== TESTE DE BIPARTICAO ===\n");

    if (eh_bipartido(g)) {
        printf("O grafo e bipartido.\n");
    } else {
        printf("O grafo nao e bipartido.\n");
    }

    free(cores_gulosa);
    free(cores_welsh);
    liberar_grafo(g);

    return 0;
}