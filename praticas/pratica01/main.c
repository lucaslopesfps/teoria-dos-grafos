#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main(void) {
    GrafoMatriz *matriz = criar_grafo_matriz(5);
    GrafoLista *lista = criar_grafo_lista(5);

    if (matriz == NULL || lista == NULL) {
        printf("Erro ao criar os grafos.\n");
        liberar_grafo_matriz(matriz);
        liberar_grafo_lista(lista);
        return 1;
    }

    inserir_aresta_matriz(matriz, 0, 1);
    inserir_aresta_matriz(matriz, 0, 2);
    inserir_aresta_matriz(matriz, 1, 2);
    inserir_aresta_matriz(matriz, 1, 3);
    inserir_aresta_matriz(matriz, 3, 4);

    inserir_aresta_lista(lista, 0, 1);
    inserir_aresta_lista(lista, 0, 2);
    inserir_aresta_lista(lista, 1, 2);
    inserir_aresta_lista(lista, 1, 3);
    inserir_aresta_lista(lista, 3, 4);

    printf("=== PRATICA 01 - REPRESENTACAO DE GRAFOS ===\n\n");

    printf("=== MATRIZ DE ADJACENCIA ===\n");
    printf("Grau do vertice 1: %d\n", grau_matriz(matriz, 1));
    printf("0 e 2 sao adjacentes? %s\n",
           sao_adjacentes_matriz(matriz, 0, 2) ? "Sim" : "Nao");
    printf("0 e 4 sao adjacentes? %s\n",
           sao_adjacentes_matriz(matriz, 0, 4) ? "Sim" : "Nao");

    printf("\nRemovendo a aresta 0 - 2...\n");
    remover_aresta_matriz(matriz, 0, 2);

    printf("0 e 2 sao adjacentes? %s\n",
           sao_adjacentes_matriz(matriz, 0, 2) ? "Sim" : "Nao");
    printf("Grau do vertice 0 apos remocao: %d\n",
           grau_matriz(matriz, 0));

    printf("\n=== LISTA DE ADJACENCIA ===\n");
    printf("Grau do vertice 1: %d\n", grau_lista(lista, 1));
    printf("0 e 2 sao adjacentes? %s\n",
           sao_adjacentes_lista(lista, 0, 2) ? "Sim" : "Nao");
    printf("0 e 4 sao adjacentes? %s\n",
           sao_adjacentes_lista(lista, 0, 4) ? "Sim" : "Nao");

    printf("\nRemovendo a aresta 0 - 2...\n");
    remover_aresta_lista(lista, 0, 2);

    printf("0 e 2 sao adjacentes? %s\n",
           sao_adjacentes_lista(lista, 0, 2) ? "Sim" : "Nao");
    printf("Grau do vertice 0 apos remocao: %d\n",
           grau_lista(lista, 0));

    liberar_grafo_matriz(matriz);
    liberar_grafo_lista(lista);

    return 0;
}