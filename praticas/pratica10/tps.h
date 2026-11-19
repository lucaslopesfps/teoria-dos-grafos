#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

typedef struct {
    int num_vertices;
    int **pesos;
} GrafoTSP;

GrafoTSP *criar_grafo_tsp(int num_vertices);

void liberar_grafo_tsp(GrafoTSP *g);

GrafoTSP *grafo_para_tsp(Grafo *g);

int tsp_forca_bruta(GrafoTSP *g, int *caminho);

int tsp_vizinho_mais_proximo(GrafoTSP *g, int *caminho);

int tsp_insercao(GrafoTSP *g, int *caminho);

#endif
