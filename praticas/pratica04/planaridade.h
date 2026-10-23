#ifndef PLANARIDADE_H
#define PLANARIDADE_H

#include "conectividade.h"

int contar_arestas(GrafoLista *g);
int eh_planar_euler(GrafoLista *g);
int possui_k5(GrafoLista *g);
int possui_k33(GrafoLista *g);
int eh_planar(GrafoLista *g);

#endif