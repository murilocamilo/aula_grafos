#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF 999999

// --- Estruturas ---
typedef struct {
    int V;
    int E;
    int **adj; // Matriz de adjacência (pesos)
} Grafo;

// --- Funções do Hamiltoniano ---
int tem_caminho_hamiltoniano_bt(Grafo *g, int vertice, int *caminho, int *visitado, int V) {
    if (vertice == V) return 1; // Todos os vértices visitados
    
    for (int v = 1; v < g->V; v++) {
        if (!visitado[v] && g->adj[caminho[vertice - 1]][v] != INF) {
            visitado[v] = 1;
            caminho[vertice] = v;
            if (tem_caminho_hamiltoniano_bt(g, vertice + 1, caminho, visitado, V)) return 1;
            visitado[v] = 0; // Backtrack
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano(Grafo *g, int *caminho) {
    int *visitado = (int*) calloc(g->V, sizeof(int));
    for (int i = 0; i < g->V; i++) {
        caminho[0] = i;
        visitado[i] = 1;
        if (tem_caminho_hamiltoniano_bt(g, 1, caminho, visitado, g->V)) {
            free(visitado);
            return 1;
        }
        visitado[i] = 0;
    }
    free(visitado);
    return 0;
}

// --- Funções do TSP ---
void permutar_tsp(Grafo *g, int *caminho, int *visitado, int pos, int custo_atual, int *melhor_caminho, int *melhor_custo) {
    if (pos == g->V) {
        // Verifica se fecha o ciclo
        if (g->adj[caminho[pos - 1]][caminho[0]] != INF) {
            int custo_total = custo_atual + g->adj[caminho[pos - 1]][caminho[0]];
            if (custo_total < *melhor_custo) {
                *melhor_custo = custo_total;
                for (int i = 0; i < g->V; i++) melhor_caminho[i] = caminho[i];
            }
        }
        return;
    }

    for (int v = 0; v < g->V; v++) {
        if (!visitado[v] && g->adj[caminho[pos - 1]][v] != INF) {
            visitado[v] = 1;
            caminho[pos] = v;
            int novo_custo = custo_atual + g->adj[caminho[pos - 1]][v];
            if (novo_custo < *melhor_custo) {
                permutar_tsp(g, caminho, visitado, pos + 1, novo_custo, melhor_caminho, melhor_custo);
            }
            visitado[v] = 0;
        }
    }
}

int tsp_forca_bruta(Grafo *g, int *melhor_caminho) {
    int *caminho = (int*) malloc(g->V * sizeof(int));
    int *visitado = (int*) calloc(g->V, sizeof(int));
    int melhor_custo = INT_MAX;

    caminho[0] = 0; // Fixa origem
    visitado[0] = 1;
    permutar_tsp(g, caminho, visitado, 1, 0, melhor_caminho, &melhor_custo);

    free(caminho);
    free(visitado);
    return melhor_custo == INT_MAX ? -1 : melhor_custo;
}

int tsp_vizinho_mais_proximo(Grafo *g, int *caminho) {
    int *visitado = (int*) calloc(g->V, sizeof(int));
    int custo = 0;
    int atual = 0;
    caminho[0] = 0;
    visitado[0] = 1;

    for (int i = 1; i < g->V; i++) {
        int proximo = -1, menor_dist = INF;
        for (int v = 0; v < g->V; v++) {
            if (!visitado[v] && g->adj[atual][v] != INF && g->adj[atual][v] < menor_dist) {
                menor_dist = g->adj[atual][v];
                proximo = v;
            }
        }
        if (proximo == -1) { free(visitado); return -1; }
        visitado[proximo] = 1;
        caminho[i] = proximo;
        custo += menor_dist;
        atual = proximo;
    }
    custo += g->adj[atual][0]; // Volta para origem
    free(visitado);
    return custo;
}

// --- Funções Auxiliares ---
Grafo* criar_grafo(int V) {
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    g->V = V;
    g->E = 0;
    g->adj = (int**) malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        g->adj[i] = (int*) malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) g->adj[i][j] = (i == j) ? 0 : INF;
    }
    return g;
}

void adicionar_aresta(Grafo *g, int u, int v, int peso) {
    g->adj[u][v] = peso;
    g->adj[v][u] = peso;
    g->E++;
}

void liberar_grafo(Grafo *g) {
    for (int i = 0; i < g->V; i++) free(g->adj[i]);
    free(g->adj);
    free(g);
}

Grafo* gerar_grafo_completo(int V) {
    Grafo *g = criar_grafo(V);
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            adicionar_aresta(g, i, j, 1 + (rand() % 10)); // Peso aleatório 1 a 10
        }
    }
    return g;
}

// --- Main ---
int main(void) {
    srand(42);
    
    printf("--- Experimento Caminho Hamiltoniano e TSP ---\n");
    printf("%-5s | %-20s | %-20s\n", "N", "TSP Forca Bruta (ms)", "TSP Vizinho (ms)");
    printf("----------------------------------------------------\n");

    for (int V = 5; V <= 12; V += 3) { // 5, 8, 12
        Grafo *g = gerar_grafo_completo(V);
        int *caminho = (int*) malloc(V * sizeof(int));

        clock_t inicio = clock();
        int custo_bt = tsp_forca_bruta(g, caminho);
        clock_t fim = clock();
        double tempo_bt = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

        inicio = clock();
        int custo_guloso = tsp_vizinho_mais_proximo(g, caminho);
        fim = clock();
        double tempo_guloso = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;

        printf("%-5d | %-20.4f | %-20.4f\n", V, tempo_bt, tempo_guloso);
        printf("  Custo BT: %d | Custo Vizinho: %d\n", custo_bt, custo_guloso);

        free(caminho);
        liberar_grafo(g);
    }

    return 0;
}
