#ifndef AGM_H
#define AGM_H

// Estruturas obrigatórias pedidas no roteiro
typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

// Funções do Union-Find
int find(DSU *d, int x);
int unir(DSU *d, int x, int y);

// Funções dos algoritmos
void kruskal(Aresta *arestas, int V, int E);
void prim(int **grafo, int V);

#endif