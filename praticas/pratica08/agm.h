
#ifndef AGM_H
#define AGM_H

typedef struct {
    int *pai;
    int *rank;
    int n;
} DSU;

typedef struct {
    int u;
    int v;
    int peso;
} Aresta;

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

typedef struct {
    int num_vertices;
    No **adj;
} GrafoListaPonderado;

int find(DSU *d, int x);
void unir(DSU *d, int x, int y);

DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);

int kruskal(
    int num_vertices,
    Aresta *arestas,
    int num_arestas,
    Aresta *agm,
    int *peso_total
);

int prim(
    GrafoListaPonderado *g,
    int origem,
    int *pai,
    int *peso_total
);

GrafoListaPonderado *criar_grafo(int num_vertices);
void adicionar_aresta(
    GrafoListaPonderado *g,
    int origem,
    int destino,
    int peso
);
void liberar_grafo(GrafoListaPonderado *g);

#endif
