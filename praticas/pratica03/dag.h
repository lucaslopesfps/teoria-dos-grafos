#ifndef DAG_H
#define DAG_H

typedef struct No {
    int vertice;
    struct No *proximo;
} No;

typedef struct {
    int num_vertices;
    No **lista;
} GrafoLista;

GrafoLista *criar_grafo(int num_vertices);
void adicionar_aresta(GrafoLista *g, int origem, int destino);
void liberar_grafo(GrafoLista *g);

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);
int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);
int eh_dag(GrafoLista *g);

#endif