#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

#define LIMITE 10

typedef struct No {
    int vertice;
    struct No *proximo;
} No;

typedef struct {
    No *adjacencia[LIMITE];
    int num_vertices;
} GrafoLista;

typedef struct {
    int *dados;
    int capacidade;
    int inicio;
    int fim;
    int tamanho;
} Fila;

void inicializar_grafo(GrafoLista *grafo, int numero);
void inserir_aresta(GrafoLista *grafo, int u, int v);
void liberar_grafo(GrafoLista *grafo);

void bfs(GrafoLista *g, int origem, int *dist, int *pred);

int eh_bipartido(GrafoLista *g);

#endif