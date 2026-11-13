#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "agm.h"

DSU *criar_dsu(int n) {
    DSU *d = malloc(sizeof(DSU));
    if (d == NULL) {
        return NULL;
    }

    d->pai = malloc((size_t)n * sizeof(int));
    d->rank = calloc((size_t)n, sizeof(int));

    if (d->pai == NULL || d->rank == NULL) {
        free(d->pai);
        free(d->rank);
        free(d);
        return NULL;
    }

    d->n = n;

    for (int i = 0; i < n; i++) {
        d->pai[i] = i;
    }

    return d;
}

void liberar_dsu(DSU *d) {
    if (d != NULL) {
        free(d->pai);
        free(d->rank);
        free(d);
    }
}

int find(DSU *d, int x) {
    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]);
    }

    return d->pai[x];
}

void unir(DSU *d, int x, int y) {
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);

    if (raiz_x == raiz_y) {
        return;
    }

    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
        d->pai[raiz_y] = raiz_x;
    } else {
        d->pai[raiz_y] = raiz_x;
        d->rank[raiz_x]++;
    }
}

static int comparar_arestas(const void *a, const void *b) {
    const Aresta *aresta_a = a;
    const Aresta *aresta_b = b;

    if (aresta_a->peso < aresta_b->peso) {
        return -1;
    }

    if (aresta_a->peso > aresta_b->peso) {
        return 1;
    }

    return 0;
}

int kruskal(
    int num_vertices,
    Aresta *arestas,
    int num_arestas,
    Aresta *agm,
    int *peso_total
) {
    if (num_vertices <= 0 || num_arestas < 0 ||
        arestas == NULL || agm == NULL || peso_total == NULL) {
        return 0;
    }

    DSU *d = criar_dsu(num_vertices);
    if (d == NULL) {
        return 0;
    }

    qsort(arestas, (size_t)num_arestas, sizeof(Aresta),
          comparar_arestas);

    int quantidade = 0;
    *peso_total = 0;

    for (int i = 0; i < num_arestas && quantidade < num_vertices - 1; i++) {
        int u = arestas[i].u;
        int v = arestas[i].v;

        if (u < 0 || u >= num_vertices ||
            v < 0 || v >= num_vertices) {
            liberar_dsu(d);
            return 0;
        }

        if (find(d, u) != find(d, v)) {
            unir(d, u, v);
            agm[quantidade++] = arestas[i];
            *peso_total += arestas[i].peso;
        }
    }

    liberar_dsu(d);

    return quantidade == num_vertices - 1;
}

GrafoListaPonderado *criar_grafo(int num_vertices) {
    if (num_vertices <= 0) {
        return NULL;
    }

    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->adj = calloc((size_t)num_vertices, sizeof(No *));

    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

void adicionar_aresta(
    GrafoListaPonderado *g,
    int origem,
    int destino,
    int peso
) {
    if (g == NULL || origem < 0 || origem >= g->num_vertices ||
        destino < 0 || destino >= g->num_vertices) {
        return;
    }

    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return;
    }

    novo->destino = destino;
    novo->peso = peso;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;

    novo = malloc(sizeof(No));
    if (novo == NULL) {
        return;
    }

    novo->destino = origem;
    novo->peso = peso;
    novo->prox = g->adj[destino];
    g->adj[destino] = novo;
}

void liberar_grafo(GrafoListaPonderado *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->adj[i];

        while (atual != NULL) {
            No *prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }

    free(g->adj);
    free(g);
}

typedef struct {
    int vertice;
    int chave;
} ItemHeap;

typedef struct {
    ItemHeap *dados;
    int *posicao;
    int tamanho;
} MinHeap;

static void trocar(MinHeap *h, int i, int j) {
    ItemHeap temp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = temp;

    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;

        if (h->dados[pai].chave <= h->dados[i].chave) {
            break;
        }

        trocar(h, pai, i);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    while (1) {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < h->tamanho &&
            h->dados[esq].chave < h->dados[menor].chave) {
            menor = esq;
        }

        if (dir < h->tamanho &&
            h->dados[dir].chave < h->dados[menor].chave) {
            menor = dir;
        }

        if (menor == i) {
            break;
        }

        trocar(h, i, menor);
        i = menor;
    }
}

static MinHeap *criar_heap(int n) {
    MinHeap *h = malloc(sizeof(MinHeap));
    if (h == NULL) {
        return NULL;
    }

    h->dados = malloc((size_t)n * sizeof(ItemHeap));
    h->posicao = malloc((size_t)n * sizeof(int));

    if (h->dados == NULL || h->posicao == NULL) {
        free(h->dados);
        free(h->posicao);
        free(h);
        return NULL;
    }

    h->tamanho = n;

    for (int i = 0; i < n; i++) {
        h->dados[i].vertice = i;
        h->dados[i].chave = INT_MAX;
        h->posicao[i] = i;
    }

    return h;
}

static ItemHeap extrair_minimo(MinHeap *h) {
    ItemHeap minimo = h->dados[0];

    h->posicao[minimo.vertice] = -1;
    h->tamanho--;

    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
        descer(h, 0);
    }

    return minimo;
}

static void diminuir_chave(MinHeap *h, int vertice, int chave) {
    int i = h->posicao[vertice];

    if (i < 0 || chave >= h->dados[i].chave) {
        return;
    }

    h->dados[i].chave = chave;
    subir(h, i);
}

static void liberar_heap(MinHeap *h) {
    if (h != NULL) {
        free(h->dados);
        free(h->posicao);
        free(h);
    }
}

int prim(
    GrafoListaPonderado *g,
    int origem,
    int *pai,
    int *peso_total
) {
    if (g == NULL || pai == NULL || peso_total == NULL ||
        origem < 0 || origem >= g->num_vertices) {
        return 0;
    }

    int n = g->num_vertices;
    MinHeap *h = criar_heap(n);

    if (h == NULL) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        pai[i] = -1;
    }

    diminuir_chave(h, origem, 0);
    *peso_total = 0;
    int visitados = 0;

    while (h->tamanho > 0) {
        ItemHeap atual = extrair_minimo(h);

        if (atual.chave == __INT_MAX__) {
            liberar_heap(h);
            return 0;
        }

        *peso_total += atual.chave;
        visitados++;

        for (No *a = g->adj[atual.vertice]; a != NULL; a = a->prox) {
            int v = a->destino;

            if (h->posicao[v] != -1 && a->peso < h->dados[h->posicao[v]].chave) {
                pai[v] = atual.vertice;
                diminuir_chave(h, v, a->peso);
            }
        }
    }

    liberar_heap(h);
    return visitados == n;
}
