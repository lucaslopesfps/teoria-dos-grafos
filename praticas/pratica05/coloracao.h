#ifndef COLORACAO_H
#define COLORACAO_H

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

int *coloracao_gulosa(GrafoLista *g, int *num_cores);
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores);
int eh_bipartido(GrafoLista *g);

#endif