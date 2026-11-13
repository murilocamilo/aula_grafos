#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

typedef struct ArestaPonderada {
    int destino;
    int peso;
    struct ArestaPonderada *proximo;
} ArestaPonderada;

typedef struct {
    ArestaPonderada *cabeca;
} VerticeLista;

typedef struct {
    int V;
    int E;
    VerticeLista *listaAdj;
} GrafoListaPonderado;

// Protótipos das funções
int bellman_ford(GrafoListaPonderado *g, int origem, int dist[], int pred[]);
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int vertice_relac, int *pred);

#endif