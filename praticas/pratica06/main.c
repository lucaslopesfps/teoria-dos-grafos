
#include "dijkstra.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int num_vertices = 6;
    GrafoListaPonderado *g = criar_grafo(num_vertices);

    if (g == NULL) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return 1;
    }

    adicionar_aresta(g, 0, 1, 4);
    adicionar_aresta(g, 0, 2, 2);
    adicionar_aresta(g, 1, 2, 1);
    adicionar_aresta(g, 1, 3, 5);
    adicionar_aresta(g, 2, 3, 8);
    adicionar_aresta(g, 2, 4, 10);
    adicionar_aresta(g, 3, 4, 2);
    adicionar_aresta(g, 3, 5, 6);
    adicionar_aresta(g, 4, 5, 3);

    int dist[6];
    int pred[6];
    int origem = 0;

    dijkstra(g, origem, dist, pred);

    printf("Menores caminhos a partir do vertice %d:\n\n", origem);

    for (int destino = 0; destino < num_vertices; destino++) {
        printf("Vertice %d: ", destino);

        if (dist[destino] == INT_MAX) {
            printf("inalcancavel\n");
            continue;
        }

        printf("distancia = %d | caminho: ", dist[destino]);

        int *caminho = reconstruir_caminho(
            origem, destino, pred, num_vertices
        );

        if (caminho == NULL) {
            printf("nao foi possivel reconstruir\n");
            continue;
        }

        int tamanho = caminho[0];

        for (int i = 1; i <= tamanho; i++) {
            printf("%d", caminho[i]);

            if (i < tamanho) {
                printf(" -> ");
            }
        }

        printf("\n");
        free(caminho);
    }

    liberar_grafo(g);
    return 0;
}

