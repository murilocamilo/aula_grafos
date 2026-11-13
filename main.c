#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

int main(void) {
    int V = 4;
    
    // --- Teste do Kruskal ---
    int E = 5;
    Aresta *arestas = (Aresta *)malloc(E * sizeof(Aresta));
    arestas[0] = (Aresta){0, 1, 10};
    arestas[1] = (Aresta){0, 2, 6};
    arestas[2] = (Aresta){0, 3, 5};
    arestas[3] = (Aresta){1, 3, 15};
    arestas[4] = (Aresta){2, 3, 4};

    kruskal(arestas, V, E);
    free(arestas);

    // --- Teste do Prim ---
    // Criando matriz de adjacência para o Prim
    int **grafo = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        grafo[i] = (int *)calloc(V, sizeof(int));
    }

    // Preenchendo a matriz (grafo não direcionado)
    grafo[0][1] = grafo[1][0] = 10;
    grafo[0][2] = grafo[2][0] = 6;
    grafo[0][3] = grafo[3][0] = 5;
    grafo[1][3] = grafo[3][1] = 15;
    grafo[2][3] = grafo[3][2] = 4;

    prim(grafo, V);

    // Liberando a matriz
    for (int i = 0; i < V; i++) free(grafo[i]);
    free(grafo);

    return 0;
}