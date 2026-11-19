#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

typedef struct {
    int num_vertices;
    int **adj;
} Grafo;

Grafo *criar_grafo(int num_vertices);

void adicionar_aresta(Grafo *g, int u, int v);

void liberar_grafo(Grafo *g);

int encontrar_caminho_hamiltoniano_bt(Grafo *g, int *caminho);

int encontrar_ciclo_hamiltoniano_bt(Grafo *g, int *ciclo);

#endif
