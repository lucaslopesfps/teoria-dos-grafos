
#include "dijkstra.h"

#include <limits.h>
#include <stdlib.h>

static void trocar(NoHeap *a, NoHeap *b)
{
    NoHeap temp = *a;
    *a = *b;
    *b = temp;
}

GrafoListaPonderado *criar_grafo(int num_vertices)
{
    if (num_vertices <= 0) {
        return NULL;
    }

    GrafoListaPonderado *g = malloc(sizeof(*g));
    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = num_vertices;
    g->adj = calloc((size_t)num_vertices, sizeof(*g->adj));

    if (g->adj == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso)
{
    if (g == NULL || origem < 0 || origem >= g->num_vertices ||
        destino < 0 || destino >= g->num_vertices || peso < 0) {
        return;
    }

    No *novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return;
    }

    novo->destino = destino;
    novo->peso = peso;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;
}

void liberar_grafo(GrafoListaPonderado *g)
{
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->adj[i];

        while (atual != NULL) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }

    free(g->adj);
    free(g);
}

MinHeap *criar_min_heap(int capacidade, int num_vertices)
{
    if (capacidade <= 0 || num_vertices <= 0) {
        return NULL;
    }

    MinHeap *heap = malloc(sizeof(*heap));
    if (heap == NULL) {
        return NULL;
    }

    heap->dados = malloc((size_t)capacidade * sizeof(*heap->dados));
    heap->posicao = malloc((size_t)num_vertices * sizeof(*heap->posicao));

    if (heap->dados == NULL || heap->posicao == NULL) {
        free(heap->dados);
        free(heap->posicao);
        free(heap);
        return NULL;
    }

    heap->capacidade = capacidade;
    heap->tamanho = 0;

    for (int i = 0; i < num_vertices; i++) {
        heap->posicao[i] = -1;
    }

    return heap;
}

static void subir_heap(MinHeap *heap, int indice)
{
    while (indice > 0) {
        int pai = (indice - 1) / 2;

        if (heap->dados[pai].distancia <= heap->dados[indice].distancia) {
            break;
        }

        int v1 = heap->dados[pai].vertice;
        int v2 = heap->dados[indice].vertice;

        trocar(&heap->dados[pai], &heap->dados[indice]);
        heap->posicao[v1] = indice;
        heap->posicao[v2] = pai;

        indice = pai;
    }
}

static void descer_heap(MinHeap *heap, int indice)
{
    while (1) {
        int menor = indice;
        int esquerda = 2 * indice + 1;
        int direita = 2 * indice + 2;

        if (esquerda < heap->tamanho &&
            heap->dados[esquerda].distancia <
                heap->dados[menor].distancia) {
            menor = esquerda;
        }

        if (direita < heap->tamanho &&
            heap->dados[direita].distancia <
                heap->dados[menor].distancia) {
            menor = direita;
        }

        if (menor == indice) {
            break;
        }

        int v1 = heap->dados[indice].vertice;
        int v2 = heap->dados[menor].vertice;

        trocar(&heap->dados[indice], &heap->dados[menor]);
        heap->posicao[v1] = menor;
        heap->posicao[v2] = indice;

        indice = menor;
    }
}

void inserir_heap(MinHeap *heap, int vertice, int distancia)
{
    if (heap == NULL || heap->tamanho >= heap->capacidade ||
        vertice < 0) {
        return;
    }

    int indice = heap->tamanho++;
    heap->dados[indice].vertice = vertice;
    heap->dados[indice].distancia = distancia;
    heap->posicao[vertice] = indice;

    subir_heap(heap, indice);
}

NoHeap extrair_minimo(MinHeap *heap)
{
    NoHeap vazio = {-1, INT_MAX};

    if (heap == NULL || heap->tamanho == 0) {
        return vazio;
    }

    NoHeap minimo = heap->dados[0];
    heap->posicao[minimo.vertice] = -1;
    heap->tamanho--;

    if (heap->tamanho > 0) {
        heap->dados[0] = heap->dados[heap->tamanho];
        heap->posicao[heap->dados[0].vertice] = 0;
        descer_heap(heap, 0);
    }

    return minimo;
}

void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia)
{
    if (heap == NULL || vertice < 0 ||
        vertice >= heap->capacidade) {
        return;
    }

    int indice = heap->posicao[vertice];

    if (indice < 0 || indice >= heap->tamanho ||
        nova_distancia >= heap->dados[indice].distancia) {
        return;
    }

    heap->dados[indice].distancia = nova_distancia;
    subir_heap(heap, indice);
}

void liberar_min_heap(MinHeap *heap)
{
    if (heap == NULL) {
        return;
    }

    free(heap->dados);
    free(heap->posicao);
    free(heap);
}

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred)
{
    if (g == NULL || dist == NULL || pred == NULL ||
        origem < 0 || origem >= g->num_vertices) {
        return;
    }

    int n = g->num_vertices;
    MinHeap *heap = criar_min_heap(n, n);

    if (heap == NULL) {
        return;
    }

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        pred[i] = -1;
    }

    dist[origem] = 0;

    for (int i = 0; i < n; i++) {
        inserir_heap(heap, i, dist[i]);
    }

    diminuir_chave(heap, origem, 0);

    while (heap->tamanho > 0) {
        NoHeap atual = extrair_minimo(heap);
        int u = atual.vertice;

        if (u < 0 || dist[u] == INT_MAX) {
            break;
        }

        for (No *aresta = g->adj[u]; aresta != NULL;
             aresta = aresta->prox) {
            int v = aresta->destino;
            int peso = aresta->peso;

            if (heap->posicao[v] == -1 ||
                dist[u] > INT_MAX - peso) {
                continue;
            }

            int nova_distancia = dist[u] + peso;

            if (nova_distancia < dist[v]) {
                dist[v] = nova_distancia;
                pred[v] = u;
                diminuir_chave(heap, v, nova_distancia);
            }
        }
    }

    liberar_min_heap(heap);
}

int *reconstruir_caminho(int origem, int destino, int *pred,
                         int num_vertices)
{
    if (pred == NULL || num_vertices <= 0 ||
        origem < 0 || origem >= num_vertices ||
        destino < 0 || destino >= num_vertices) {
        return NULL;
    }

    int *caminho = malloc((size_t)num_vertices * sizeof(*caminho));
    if (caminho == NULL) {
        return NULL;
    }

    int tamanho = 0;
    int atual = destino;

    while (atual != -1 && tamanho < num_vertices) {
        caminho[tamanho++] = atual;

        if (atual == origem) {
            int *resultado = malloc((size_t)(tamanho + 1) *
                                    sizeof(*resultado));

            if (resultado == NULL) {
                free(caminho);
                return NULL;
            }

            resultado[0] = tamanho;

            for (int i = 0; i < tamanho; i++) {
                resultado[i + 1] = caminho[tamanho - 1 - i];
            }

            free(caminho);
            return resultado;
        }

        atual = pred[atual];
    }

    free(caminho);
    return NULL;
}
