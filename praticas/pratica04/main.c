#include <stdio.h>
#include "conectividade.h"
#include "planaridade.h"

int main(void) {
    printf("=== PRATICA 04 - CONECTIVIDADE E PLANARIDADE ===\n\n");

    /* Grafo para testar articulacoes e pontes */
    GrafoLista *g = criar_grafo(5);

    if (g == NULL) {
        printf("Erro ao criar o grafo.\n");
        return 1;
    }

    adicionar_aresta(g, 0, 1);
    adicionar_aresta(g, 1, 2);
    adicionar_aresta(g, 2, 0);
    adicionar_aresta(g, 1, 3);
    adicionar_aresta(g, 3, 4);

    printf("=== CONECTIVIDADE ===\n");

    encontrar_articulacoes(g);

    printf("Pontes:\n");
    detectar_pontes(g);

    printf("\n=== PLANARIDADE ===\n");

    printf("Vertices: %d\n", g->num_vertices);
    printf("Arestas: %d\n", contar_arestas(g));

    if (eh_planar_euler(g)) {
        printf("O grafo satisfaz a condicao de Euler.\n");
    } else {
        printf("O grafo nao satisfaz a condicao de Euler.\n");
    }

    if (eh_planar(g)) {
        printf("Resultado da heuristica: grafo planar.\n");
    } else {
        printf("Resultado da heuristica: grafo nao planar.\n");
    }

    liberar_grafo(g);

    /* Teste com K5 */
    printf("\n=== TESTE K5 ===\n");

    GrafoLista *k5 = criar_grafo(5);

    if (k5 == NULL) {
        printf("Erro ao criar K5.\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            adicionar_aresta(k5, i, j);
        }
    }

    printf("Vertices: %d\n", k5->num_vertices);
    printf("Arestas: %d\n", contar_arestas(k5));

    if (possui_k5(k5)) {
        printf("K5 detectado.\n");
    }

    if (eh_planar(k5)) {
        printf("Resultado da heuristica: grafo planar.\n");
    } else {
        printf("Resultado da heuristica: grafo nao planar.\n");
    }

    liberar_grafo(k5);

    return 0;
}