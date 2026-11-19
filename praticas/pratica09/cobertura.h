
#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int u;
    int v;
} Aresta;

typedef struct {
    int num_vertices;
    int num_arestas;
    Aresta *arestas;
} Grafo;

Grafo *criar_grafo(int num_vertices, int num_arestas);
void liberar_grafo(Grafo *g);

int eh_cobertura(Grafo *g, int *selecionados);

int cobertura_minima_bt(Grafo *g, int *solucao);

int cobertura_gulosa(Grafo *g, int *solucao);

#endif
