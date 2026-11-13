#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

// Função para gerar um grafo aleatório para o experimento
Grafo* gerar_grafo_aleatorio(int V, int arestas) {
    Grafo *g = criar_grafo(V);
    srand(42); // Semente fixa para resultados reproduzíveis
    int count = 0;
    while (count < arestas) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && g->adj[u][v] == 0) {
            adicionar_aresta(g, u, v);
            count++;
        }
    }
    return g;
}

int main(void) {
    int tamanhos[] = {5, 8, 10, 12, 15};
    int num_testes = 5;

    printf("--- Experimento de Cobertura de Vertices ---\n");
    printf("%-5s | %-15s | %-15s\n", "N", "BT (Tempo ms)", "Guloso (Tempo ms)");
    printf("---------------------------------------------\n");

    for (int i = 0; i < num_testes; i++) {
        int V = tamanhos[i];
        Grafo *g = gerar_grafo_aleatorio(V, V * 2); // Arestas = 2 * V (grafo esparso)

        int *cob_bt = (int*) malloc(V * sizeof(int));
        int *cob_guloso = (int*) malloc(V * sizeof(int));

        // Medir tempo do Backtracking
        clock_t inicio = clock();
        int tam_bt = cobertura_minima_bt(g, cob_bt);
        clock_t fim = clock();
        double tempo_bt = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

        // Medir tempo do Guloso
        inicio = clock();
        int tam_guloso = cobertura_gulosa(g, cob_guloso);
        fim = clock();
        double tempo_guloso = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

        printf("%-5d | %-15.4f | %-15.4f\n", V, tempo_bt, tempo_guloso);
        // printf("  Tamanho BT: %d | Tamanho Guloso: %d\n", tam_bt, tam_guloso);

        free(cob_bt);
        free(cob_guloso);
        liberar_grafo(g);
    }

    return 0;
}