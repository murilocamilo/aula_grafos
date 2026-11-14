#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

Grafo* gerar_grafo_teste(int V) {
    Grafo *g = criar_grafo(V);
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (rand() % 2 == 0) adicionar_aresta(g, i, j, 1 + (rand() % 10));
        }
    }
    return g;
}

int main(void) {
    srand(42);
    printf("--- Experimento Pratica 10 ---\n");
    printf("%-5s | %-15s | %-15s | %-15s | %-15s\n", "N", "TSP BT (ms)", "TSP Vizinho(ms)", "TSP Insercao(ms)", "Custo BT");
    printf("-----------------------------------------------------------------------------------------\n");

    int tamanhos[] = {5, 8, 10, 12};
    for (int i = 0; i < 4; i++) {
        int V = tamanhos[i];
        Grafo *g = gerar_grafo_teste(V);
        Grafo *g_tsp = grafo_para_tsp(g);

        int *caminho = (int*) malloc(V * sizeof(int));

        clock_t inicio = clock();
        int custo_bt = tsp_forca_bruta(g_tsp, caminho);
        clock_t fim = clock();
        double tempo_bt = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

        inicio = clock();
        int custo_vizinho = tsp_vizinho_mais_proximo(g_tsp, caminho);
        fim = clock();
        double tempo_vizinho = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

        inicio = clock();
        int custo_insercao = tsp_insercao(g_tsp, caminho);
        fim = clock();
        double tempo_insercao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

        printf("%-5d | %-15.4f | %-15.4f | %-15.4f | %d\n", V, tempo_bt, tempo_vizinho, tempo_insercao, custo_bt);

        free(caminho);
        liberar_grafo(g);
        liberar_grafo(g_tsp);
    }
    return 0;
}
