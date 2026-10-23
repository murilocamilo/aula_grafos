#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

int main() {
    int V = 6;
    GrafoListaPonderado *g = criar_grafo(V);

    // Mapeamento: A=0, B=1, C=2, D=3, E=4, F=5
    // Grafo não-direcionado com os pesos da imagem[cite: 1]
    adicionar_aresta(g, 0, 1, 4); adicionar_aresta(g, 1, 0, 4); // A - B (4)[cite: 1]
    adicionar_aresta(g, 0, 2, 2); adicionar_aresta(g, 2, 0, 2); // A - C (2)[cite: 1]
    adicionar_aresta(g, 1, 2, 1); adicionar_aresta(g, 2, 1, 1); // B - C (1)[cite: 1]
    adicionar_aresta(g, 1, 3, 5); adicionar_aresta(g, 3, 1, 5); // B - D (5)[cite: 1]
    adicionar_aresta(g, 1, 4, 8); adicionar_aresta(g, 4, 1, 8); // B - E (8)[cite: 1]
    adicionar_aresta(g, 2, 3, 4); adicionar_aresta(g, 3, 2, 4); // C - D (4)[cite: 1]
    adicionar_aresta(g, 2, 4, 10); adicionar_aresta(g, 4, 2, 10); // C - E (10)[cite: 1]
    adicionar_aresta(g, 3, 4, 2); adicionar_aresta(g, 4, 3, 2); // D - E (2)[cite: 1]
    adicionar_aresta(g, 3, 5, 1); adicionar_aresta(g, 5, 3, 1); // D - F (1)[cite: 1]
    adicionar_aresta(g, 4, 5, 3); adicionar_aresta(g, 5, 4, 3); // E - F (3)[cite: 1]

    int dist[V];
    int pred[V];
    int origem = 0; // Vértice 'A'

    dijkstra(g, origem, dist, pred);

    printf("--- Distancias minimas a partir da origem (A) ---\n");
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF) {
            printf("Vertice %c: Inalcancavel\n", 'A' + i);
        } else {
            printf("Vertice %c: Custo %d\n", 'A' + i, dist[i]);
        }
    }

    printf("\n--- Reconstrucao dos Caminhos ---\n");
    for (int i = 0; i < V; i++) {
        printf("Caminho ate %c: ", 'A' + i);
        reconstruir_caminho(origem, i, pred);
    }

    liberar_grafo(g);
    return 0;
}