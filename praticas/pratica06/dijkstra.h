
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

typedef struct {
    int num_vertices;
    No **adj;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade;
    int tamanho;
    int *posicao;
} MinHeap;

GrafoListaPonderado *criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso);
void liberar_grafo(GrafoListaPonderado *g);

MinHeap *criar_min_heap(int capacidade, int num_vertices);
void inserir_heap(MinHeap *heap, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *heap);
void diminuir_chave(MinHeap *heap, int vertice, int nova_distancia);
void liberar_min_heap(MinHeap *heap);

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
int *reconstruir_caminho(int origem, int destino, int *pred, int num_vertices);

#endif
