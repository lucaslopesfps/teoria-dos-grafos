#include <stdio.h>
#include "busca_largura.h"
#include "busca_profundidade.h"

int main(void) {
    GrafoLista grafo;

    int dist[LIMITE];
    int pred[LIMITE];
    int visitado[LIMITE] = {0};
    int tempo_entrada[LIMITE];
    int tempo_saida[LIMITE];
    int tempo = 0;

    inicializar_grafo(&grafo, 8);

    inserir_aresta(&grafo, 0, 1);
    inserir_aresta(&grafo, 0, 2);
    inserir_aresta(&grafo, 1, 3);
    inserir_aresta(&grafo, 2, 3);
    inserir_aresta(&grafo, 3, 4);
    inserir_aresta(&grafo, 4, 5);
    inserir_aresta(&grafo, 5, 6);
    inserir_aresta(&grafo, 6, 7);

    printf("=== BUSCA EM LARGURA (BFS) ===\n");

    bfs(&grafo, 0, dist, pred);

    for (int i = 0; i < grafo.num_vertices; i++) {
        printf("Vertice %d: distancia = %d, predecessor = %d\n",
               i, dist[i], pred[i]);
    }

    printf("\n=== BUSCA EM PROFUNDIDADE (DFS) ===\n");

    dfs_recursiva(&grafo, 0, visitado,
                  tempo_entrada, tempo_saida, &tempo);

    for (int i = 0; i < grafo.num_vertices; i++) {
        printf("Vertice %d: entrada = %d, saida = %d\n",
               i, tempo_entrada[i], tempo_saida[i]);
    }

    printf("\n=== COMPONENTES CONEXOS ===\n");
    printf("Quantidade: %d\n", contar_componentes(&grafo));

    printf("\n=== DETECCAO DE CICLO ===\n");

    if (tem_ciclo(&grafo)) {
        printf("O grafo possui ciclo.\n");
    } else {
        printf("O grafo nao possui ciclo.\n");
    }

    printf("\n=== TESTE DE BIPARTICAO ===\n");

    if (eh_bipartido(&grafo)) {
        printf("O grafo e bipartido.\n");
    } else {
        printf("O grafo nao e bipartido.\n");
    }

    liberar_grafo(&grafo);

    return 0;
}