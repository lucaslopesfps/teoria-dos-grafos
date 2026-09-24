#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

#define MAX_VERTICES 100

typedef struct No {
    int vertice;
    struct No *prox;
} No;

typedef struct {
    int num_vertices;
    No **lista;
} GrafoLista;

GrafoLista *criar_grafo(int num_vertices);
void adicionar_aresta(GrafoLista *g, int origem, int destino);
void liberar_grafo(GrafoLista *g);

void dfs_articulacoes(GrafoLista *g, int u, int *visitado,
                      int *descoberta, int *low, int *pai,
                      int *articulacao, int *tempo);

void encontrar_articulacoes(GrafoLista *g);
void detectar_pontes(GrafoLista *g);

#endif