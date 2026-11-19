#include <stdlib.h>
#include <limits.h>
#include "tps.h"

GrafoTSP *criar_grafo_tsp(int num_vertices) {
    if (num_vertices <= 0) {
        return NULL;
    }

    GrafoTSP *g = malloc(sizeof(GrafoTSP));
    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->pesos = calloc(num_vertices, sizeof(int *));

    if (g->pesos == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        g->pesos[i] = calloc(num_vertices, sizeof(int));

        if (g->pesos[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(g->pesos[j]);
            }
            free(g->pesos);
            free(g);
            return NULL;
        }
    }

    return g;
}

void liberar_grafo_tsp(GrafoTSP *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        free(g->pesos[i]);
    }

    free(g->pesos);
    free(g);
}

GrafoTSP *grafo_para_tsp(Grafo *g) {
    if (g == NULL) {
        return NULL;
    }

    int n = g->num_vertices;
    GrafoTSP *tsp = criar_grafo_tsp(n);

    if (tsp == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                tsp->pesos[i][j] = 0;
            } else if (g->adj[i][j]) {
                tsp->pesos[i][j] = 1;
            } else {
                tsp->pesos[i][j] = 2;
            }
        }
    }

    return tsp;
}

static void buscar_tsp(GrafoTSP *g, int *atual,
                       int *melhor, int *visitado,
                       int pos, int custo,
                       int *menor_custo) {
    int n = g->num_vertices;

    if (custo >= *menor_custo) {
        return;
    }

    if (pos == n) {
        int total = custo + g->pesos[atual[n - 1]][0];

        if (total < *menor_custo) {
            *menor_custo = total;

            for (int i = 0; i < n; i++) {
                melhor[i] = atual[i];
            }

            melhor[n] = 0;
        }

        return;
    }

    for (int v = 1; v < n; v++) {
        if (!visitado[v]) {
            visitado[v] = 1;
            atual[pos] = v;

            buscar_tsp(g, atual, melhor, visitado,
                       pos + 1,
                       custo + g->pesos[atual[pos - 1]][v],
                       menor_custo);

            visitado[v] = 0;
        }
    }
}

int tsp_forca_bruta(GrafoTSP *g, int *caminho) {
    if (g == NULL || caminho == NULL) {
        return -1;
    }

    int n = g->num_vertices;

    if (n == 1) {
        caminho[0] = 0;
        caminho[1] = 0;
        return 0;
    }

    int *atual = malloc((n + 1) * sizeof(int));
    int *visitado = calloc(n, sizeof(int));

    if (atual == NULL || visitado == NULL) {
        free(atual);
        free(visitado);
        return -1;
    }

    atual[0] = 0;
    visitado[0] = 1;

    int menor_custo = INT_MAX;

    buscar_tsp(g, atual, caminho, visitado,
               1, 0, &menor_custo);

    free(atual);
    free(visitado);

    return menor_custo == INT_MAX ? -1 : menor_custo;
}

int tsp_vizinho_mais_proximo(GrafoTSP *g,
                             int *caminho) {
    if (g == NULL || caminho == NULL) {
        return -1;
    }

    int n = g->num_vertices;
    int *visitado = calloc(n, sizeof(int));

    if (visitado == NULL) {
        return -1;
    }

    caminho[0] = 0;
    visitado[0] = 1;

    int custo = 0;

    for (int i = 1; i < n; i++) {
        int anterior = caminho[i - 1];
        int proximo = -1;
        int menor_peso = INT_MAX;

        for (int v = 0; v < n; v++) {
            if (!visitado[v] &&
                g->pesos[anterior][v] < menor_peso) {
                menor_peso = g->pesos[anterior][v];
                proximo = v;
            }
        }

        if (proximo == -1) {
            free(visitado);
            return -1;
        }

        caminho[i] = proximo;
        visitado[proximo] = 1;
        custo += menor_peso;
    }

    caminho[n] = 0;
    custo += g->pesos[caminho[n - 1]][0];

    free(visitado);
    return custo;
}

int tsp_insercao(GrafoTSP *g, int *caminho) {
    if (g == NULL || caminho == NULL) {
        return -1;
    }

    int n = g->num_vertices;
    int *visitado = calloc(n, sizeof(int));

    if (visitado == NULL) {
        return -1;
    }

    caminho[0] = 0;
    caminho[1] = 0;
    visitado[0] = 1;

    int quantidade = 1;

    while (quantidade < n) {
        int escolhido = -1;
        int menor_distancia = INT_MAX;

        for (int v = 0; v < n; v++) {
            if (visitado[v]) {
                continue;
            }

            for (int i = 0; i < quantidade; i++) {
                int u = caminho[i];
                int distancia = g->pesos[u][v];

                if (distancia < menor_distancia) {
                    menor_distancia = distancia;
                    escolhido = v;
                }
            }
        }

        if (escolhido == -1) {
            free(visitado);
            return -1;
        }

        int melhor_posicao = 1;
        int menor_aumento = INT_MAX;

        for (int i = 0; i < quantidade; i++) {
            int u = caminho[i];
            int v = caminho[i + 1];

            int aumento =
                g->pesos[u][escolhido] +
                g->pesos[escolhido][v] -
                g->pesos[u][v];

            if (aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_posicao = i + 1;
            }
        }

        for (int i = quantidade; i >= melhor_posicao; i--) {
            caminho[i + 1] = caminho[i];
        }

        caminho[melhor_posicao] = escolhido;
        visitado[escolhido] = 1;
        quantidade++;
    }

    int custo = 0;

    for (int i = 0; i < n; i++) {
        custo += g->pesos[caminho[i]][caminho[i + 1]];
    }

    free(visitado);
    return custo;
}
