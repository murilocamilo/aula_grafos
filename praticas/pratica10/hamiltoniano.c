#include <stdio.h>
#include <stdlib.h>
#include "hamiltoniano.h"

int bt_hamiltoniano(Grafo *g, int vertice, int *caminho, int *visitado) {
    if (vertice == g->V) return 1; // Caminho completo encontrado
    
    for (int v = 0; v < g->V; v++) {
        if (!visitado[v] && g->adj[caminho[vertice - 1]][v] != 999999) {
            visitado[v] = 1;
            caminho[vertice] = v;
            if (bt_hamiltoniano(g, vertice + 1, caminho, visitado)) return 1;
            visitado[v] = 0; // Backtrack
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(Grafo *g, int *caminho) {
    int *visitado = (int*) calloc(g->V, sizeof(int));
    for (int i = 0; i < g->V; i++) {
        caminho[0] = i;
        visitado[i] = 1;
        if (bt_hamiltoniano(g, 1, caminho, visitado)) {
            free(visitado);
            return 1;
        }
        visitado[i] = 0;
    }
    free(visitado);
    return 0;
}
