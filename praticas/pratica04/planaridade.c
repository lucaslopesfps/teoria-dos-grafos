#include <stdio.h>
#include "planaridade.h"

static int existe_aresta(GrafoLista *g, int u, int v) {
    No *atual = g->lista[u];

    while (atual != NULL) {
        if (atual->vertice == v) {
            return 1;
        }

        atual = atual->prox;
    }

    return 0;
}

int contar_arestas(GrafoLista *g) {
    int total = 0;

    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->lista[i];

        while (atual != NULL) {
            total++;
            atual = atual->prox;
        }
    }

    return total / 2;
}

int eh_planar_euler(GrafoLista *g) {
    int n = g->num_vertices;
    int m = contar_arestas(g);

    if (n < 3) {
        return 1;
    }

    return m <= (3 * n - 6);
}

int possui_k5(GrafoLista *g) {
    int n = g->num_vertices;

    if (n < 5 || n > 10) {
        return 0;
    }

    for (int a = 0; a < n - 4; a++) {
        for (int b = a + 1; b < n - 3; b++) {
            for (int c = b + 1; c < n - 2; c++) {
                for (int d = c + 1; d < n - 1; d++) {
                    for (int e = d + 1; e < n; e++) {
                        int vertices[5] = {a, b, c, d, e};
                        int completo = 1;

                        for (int i = 0; i < 5 && completo; i++) {
                            for (int j = i + 1; j < 5; j++) {
                                if (!existe_aresta(g, vertices[i],
                                                  vertices[j])) {
                                    completo = 0;
                                    break;
                                }
                            }
                        }

                        if (completo) {
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int possui_k33(GrafoLista *g) {
    int n = g->num_vertices;

    if (n < 6 || n > 10) {
        return 0;
    }

    for (int a = 0; a < n - 5; a++) {
        for (int b = a + 1; b < n - 4; b++) {
            for (int c = b + 1; c < n - 3; c++) {
                for (int d = c + 1; d < n - 2; d++) {
                    for (int e = d + 1; e < n - 1; e++) {
                        for (int f = e + 1; f < n; f++) {
                            int v[6] = {a, b, c, d, e, f};

                            for (int mascara = 0; mascara < 64; mascara++) {
                                int grupo_a[3];
                                int grupo_b[3];
                                int qtd_a = 0;
                                int qtd_b = 0;

                                for (int i = 0; i < 6; i++) {
                                    if (mascara & (1 << i)) {
                                        if (qtd_a < 3) {
                                            grupo_a[qtd_a++] = v[i];
                                        } else {
                                            qtd_a++;
                                        }
                                    } else {
                                        if (qtd_b < 3) {
                                            grupo_b[qtd_b++] = v[i];
                                        } else {
                                            qtd_b++;
                                        }
                                    }
                                }

                                if (qtd_a != 3 || qtd_b != 3) {
                                    continue;
                                }

                                int completo = 1;

                                for (int i = 0; i < 3 && completo; i++) {
                                    for (int j = 0; j < 3; j++) {
                                        if (!existe_aresta(
                                                g, grupo_a[i],
                                                grupo_b[j])) {
                                            completo = 0;
                                            break;
                                        }
                                    }
                                }

                                if (completo) {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int eh_planar(GrafoLista *g) {
    if (!eh_planar_euler(g)) {
        return 0;
    }

    if (g->num_vertices <= 10) {
        if (possui_k5(g) || possui_k33(g)) {
            return 0;
        }
    }

    return 1;
}