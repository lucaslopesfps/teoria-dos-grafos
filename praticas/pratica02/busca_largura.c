#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"


static void inicializar_fila(Fila *f, int capacidade) {
    f->dados = malloc(capacidade * sizeof(int));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

static int fila_vazia(Fila *f) {
    return f->tamanho == 0;
}

static void enfileirar(Fila *f, int valor) {
    if (f->tamanho < f->capacidade) {
        f->dados[f->fim] = valor;
        f->fim = (f->fim + 1) % f->capacidade;
        f->tamanho++;
    }
}

static int desenfileirar(Fila *f) {
    int valor = f->dados[f->inicio];

    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;

    return valor;
}

static void liberar_fila(Fila *f) {
    free(f->dados);
}


void inicializar_grafo(GrafoLista *grafo, int numero) {
    grafo->num_vertices = numero;

    for (int i = 0; i < LIMITE; i++) {
        grafo->adjacencia[i] = NULL;
    }
}

void inserir_aresta(GrafoLista *grafo, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->vertice = v;
    novo->proximo = grafo->adjacencia[u];
    grafo->adjacencia[u] = novo;

    novo = malloc(sizeof(No));
    novo->vertice = u;
    novo->proximo = grafo->adjacencia[v];
    grafo->adjacencia[v] = novo;
}

void liberar_grafo(GrafoLista *grafo) {
    for (int i = 0; i < grafo->num_vertices; i++) {
        No *atual = grafo->adjacencia[i];

        while (atual != NULL) {
            No *aux = atual;
            atual = atual->proximo;
            free(aux);
        }

        grafo->adjacencia[i] = NULL;
    }
}


void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    Fila fila;

    inicializar_fila(&fila, g->num_vertices);

    for (int i = 0; i < g->num_vertices; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    dist[origem] = 0;
    enfileirar(&fila, origem);

    while (!fila_vazia(&fila)) {
        int u = desenfileirar(&fila);
        No *atual = g->adjacencia[u];

        while (atual != NULL) {
            int v = atual->vertice;

            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(&fila, v);
            }

            atual = atual->proximo;
        }
    }

    liberar_fila(&fila);
}


int eh_bipartido(GrafoLista *g) {
    int cor[LIMITE];

    for (int i = 0; i < g->num_vertices; i++) {
        cor[i] = -1;
    }

    for (int inicio = 0; inicio < g->num_vertices; inicio++) {
        if (cor[inicio] != -1) {
            continue;
        }

        Fila fila;
        inicializar_fila(&fila, g->num_vertices);

        cor[inicio] = 0;
        enfileirar(&fila, inicio);

        while (!fila_vazia(&fila)) {
            int u = desenfileirar(&fila);
            No *atual = g->adjacencia[u];

            while (atual != NULL) {
                int v = atual->vertice;

                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    enfileirar(&fila, v);
                } else if (cor[v] == cor[u]) {
                    liberar_fila(&fila);
                    return 0;
                }

                atual = atual->proximo;
            }
        }

        liberar_fila(&fila);
    }

    return 1;
}