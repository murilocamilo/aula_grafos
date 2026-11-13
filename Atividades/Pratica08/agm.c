#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

#define INF 999999

// --- Implementação do Union-Find ---
int find(DSU *d, int x) {
    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]); // Compressão de caminho
    }
    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);

    if (raiz_x == raiz_y) return 0; // Já estão no mesmo conjunto (ciclo)

    // União por rank
    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
        d->pai[raiz_y] = raiz_x;
    } else {
        d->pai[raiz_y] = raiz_x;
        d->rank[raiz_x]++;
    }
    return 1;
}

// --- Função de comparação para o qsort (Kruskal) ---
int comparar_arestas(const void *a, const void *b) {
    Aresta *a1 = (Aresta *)a;
    Aresta *a2 = (Aresta *)b;
    return a1->peso - a2->peso;
}

// --- Kruskal O(E log E) ---
void kruskal(Aresta *arestas, int V, int E) {
    // 1. Ordenar as arestas por peso
    qsort(arestas, E, sizeof(Aresta), comparar_arestas);

    // 2. Inicializar DSU
    DSU d;
    d.n = V;
    d.pai = (int *)malloc(V * sizeof(int));
    d.rank = (int *)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        d.pai[i] = i;
        d.rank[i] = 0;
    }

    printf("\n--- Kruskal ---\n");
    int peso_total = 0;
    int arestas_incluidas = 0;

    for (int i = 0; i < E && arestas_incluidas < V - 1; i++) {
        if (unir(&d, arestas[i].u, arestas[i].v)) {
            printf("Aresta: %d - %d | Peso: %d\n", arestas[i].u, arestas[i].v, arestas[i].peso);
            peso_total += arestas[i].peso;
            arestas_incluidas++;
        }
    }
    printf("Peso total da AGM (Kruskal): %d\n", peso_total);

    free(d.pai);
    free(d.rank);
}

// --- Prim O((V+E) log V) usando busca linear como Min-Heap simplificado ---
void prim(int **grafo, int V) {
    int *chave = (int *)malloc(V * sizeof(int));
    int *pai = (int *)malloc(V * sizeof(int));
    int *mstSet = (int *)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        chave[i] = INF;
        mstSet[i] = 0;
        pai[i] = -1;
    }

    chave[0] = 0; // Começa do vértice 0

    printf("\n--- Prim ---\n");
    int peso_total = 0;

    for (int count = 0; count < V - 1; count++) {
        // Encontrar o vértice com a menor chave que ainda não está na AGM
        int min = INF, u = -1;
        for (int v = 0; v < V; v++) {
            if (mstSet[v] == 0 && chave[v] < min) {
                min = chave[v];
                u = v;
            }
        }

        if (u == -1) break;
        mstSet[u] = 1;

        // Atualizar as chaves dos vértices adjacentes
        for (int v = 0; v < V; v++) {
            if (grafo[u][v] && mstSet[v] == 0 && grafo[u][v] < chave[v]) {
                pai[v] = u;
                chave[v] = grafo[u][v];
            }
        }
    }

    for (int i = 1; i < V; i++) {
        printf("Aresta: %d - %d | Peso: %d\n", pai[i], i, grafo[i][pai[i]]);
        peso_total += grafo[i][pai[i]];
    }
    printf("Peso total da AGM (Prim): %d\n", peso_total);

    free(chave);
    free(pai);
    free(mstSet);
}