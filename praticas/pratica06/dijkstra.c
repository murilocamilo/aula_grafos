#include "dijkstra.h"
#include <stdlib.h>
#include <stdio.h>

// --- Funções de Grafo ---

GrafoListaPonderado* criar_grafo(int num_vertices) {
    GrafoListaPonderado *g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->num_vertices = num_vertices;
    g->listas = (NoAdj**) malloc(num_vertices * sizeof(NoAdj*));
    for (int i = 0; i < num_vertices; i++) {
        g->listas[i] = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    NoAdj *novo = (NoAdj*) malloc(sizeof(NoAdj));
    novo->destino = v;
    novo->peso = peso;
    novo->prox = g->listas[u];
    g->listas[u] = novo;
}

void liberar_grafo(GrafoListaPonderado *g) {
    for (int i = 0; i < g->num_vertices; i++) {
        NoAdj *atual = g->listas[i];
        while (atual != NULL) {
            NoAdj *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->listas);
    free(g);
}

// --- Algoritmo de Dijkstra Sem Min-Heap ---

// Função auxiliar para encontrar o vértice não visitado de menor distância (Busca Linear O(V))
int encontrar_minimo_distancia(int *dist, int *visitado, int V) {
    int min = INF;
    int min_index = -1;

    for (int v = 0; v < V; v++) {
        if (!visitado[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int V = g->num_vertices;
    int *visitado = (int*) malloc(V * sizeof(int));

    // Inicialização das distâncias e antecessores
    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        pred[v] = -1;
        visitado[v] = 0;
    }

    dist[origem] = 0;

    for (int count = 0; count < V - 1; count++) {
        // Seleciona o vértice de menor distância ainda não processado
        int u = encontrar_minimo_distancia(dist, visitado, V);

        if (u == -1 || dist[u] == INF) break; // Vértices restantes inacessíveis

        visitado[u] = 1;

        // Relaxamento das arestas adjacentes
        NoAdj *adj = g->listas[u];
        while (adj != NULL) {
            int v = adj->destino;
            int peso = adj->peso;

            if (!visitado[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                pred[v] = u;
            }
            adj = adj->prox;
        }
    }

    free(visitado);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (origem == destino) {
        printf("%c", 'A' + origem);
        return;
    }

    if (pred[destino] == -1) {
        printf("Não existe caminho entre %c e %c\n", 'A' + origem, 'A' + destino);
        return;
    }

    int caminho[1000];
    int tam = 0;
    int atual = destino;

    while (atual != -1) {
        caminho[tam++] = atual;
        atual = pred[atual];
    }

    // Imprime o caminho com letras (A, B, C...)
    for (int i = tam - 1; i >= 0; i--) {
        printf("%c", 'A' + caminho[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}