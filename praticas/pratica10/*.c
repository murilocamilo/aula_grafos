#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF 999999

typedef struct {
    int V;
    int E;
    int **adj;
} Grafo;

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

int tem_caminho_hamiltoniano_bt(Grafo *g, int vertice, int *caminho, int *visitado) {
    if (vertice == g->V) return 1;
    for (int v = 0; v < g->V; v++) {
        if (!visitado[v] && g->adj[caminho[vertice - 1]][v] != INF) {
            visitado[v] = 1;
            caminho[vertice] = v;
            if (tem_caminho_hamiltoniano_bt(g, vertice + 1, caminho, visitado)) return 1;
            visitado[v] = 0;
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(Grafo *g, int *caminho) {
    int *visitado = (int*) calloc(g->V, sizeof(int));
    for (int i = 0; i < g->V; i++) {
        caminho[0] = i;
        visitado[i] = 1;
        if (tem_caminho_hamiltoniano_bt(g, 1, caminho, visitado)) {
            free(visitado);
            return 1;
        }
        visitado[i] = 0;
    }
    free(visitado);
    return 0;
}

Grafo* grafo_para_tsp(Grafo *g) {
    Grafo *novo = criar_grafo(g->V);
    for (int i = 0; i < g->V; i++) {
        for (int j = 0; j < g->V; j++) {
            if (i != j) novo->adj[i][j] = (g->adj[i][j] != INF) ? 1 : 2;
        }
    }
    return novo;
}

void permutar_tsp(Grafo *g, int *caminho, int *visitado, int pos, int custo_atual, int *melhor_caminho, int *melhor_custo) {
    if (pos == g->V) {
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
            if (novo_custo < *melhor_custo) permutar_tsp(g, caminho, visitado, pos + 1, novo_custo, melhor_caminho, melhor_custo);
            visitado[v] = 0;
        }
    }
}

int tsp_forca_bruta(Grafo *g, int *melhor_caminho) {
    int *caminho = (int*) malloc(g->V * sizeof(int));
    int *visitado = (int*) calloc(g->V, sizeof(int));
    int melhor_custo = INT_MAX;
    caminho[0] = 0; visitado[0] = 1;
    permutar_tsp(g, caminho, visitado, 1, 0, melhor_caminho, &melhor_custo);
    free(caminho); free(visitado);
    return melhor_custo == INT_MAX ? -1 : melhor_custo;
}

int tsp_vizinho_mais_proximo(Grafo *g, int *caminho) {
    int *visitado = (int*) calloc(g->V, sizeof(int));
    int custo = 0, atual = 0;
    caminho[0] = 0; visitado[0] = 1;
    for (int i = 1; i < g->V; i++) {
        int proximo = -1, menor_dist = INF;
        for (int v = 0; v < g->V; v++) {
            if (!visitado[v] && g->adj[atual][v] != INF && g->adj[atual][v] < menor_dist) {
                menor_dist = g->adj[atual][v]; proximo = v;
            }
        }
        if (proximo == -1) { free(visitado); return -1; }
        visitado[proximo] = 1; caminho[i] = proximo; custo += menor_dist; atual = proximo;
    }
    custo += g->adj[atual][0];
    free(visitado);
    return custo;
}

int tsp_insercao(Grafo *g, int *caminho) {
    int *visitado = (int*) calloc(g->V, sizeof(int));
    int custo = 0;
    caminho[0] = 0; caminho[1] = 1;
    visitado[0] = 1; visitado[1] = 1;
    int tam = 2;
    while (tam < g->V) {
        int melhor_v = -1, melhor_pos = -1, menor_custo_insercao = INF;
        for (int v = 0; v < g->V; v++) {
            if (!visitado[v]) {
                for (int i = 0; i < tam; i++) {
                    int u = caminho[i];
                    int w = caminho[(i + 1) % tam];
                    int custo_insercao = g->adj[u][v] + g->adj[v][w] - g->adj[u][w];
                    if (custo_insercao < menor_custo_insercao) {
                        menor_custo_insercao = custo_insercao;
                        melhor_v = v; melhor_pos = (i + 1) % tam;
                    }
                }
            }
        }
        if (melhor_v == -1) break;
        for (int i = tam; i > melhor_pos; i--) caminho[i] = caminho[i - 1];
        caminho[melhor_pos] = melhor_v;
        visitado[melhor_v] = 1;
        custo += menor_custo_insercao;
        tam++;
    }
    free(visitado);
    return custo;
}

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
    printf("%-5s | %-15s | %-15s | %-15s | %-10s\n", "N", "TSP BT (ms)", "TSP Vizinho(ms)", "TSP Insercao(ms)", "Custo BT");
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

        // Impressão usando todas as variáveis para evitar o erro de compilação
        printf("%-5d | %-15.4f | %-15.4f | %-15.4f | %d\n", V, tempo_bt, tempo_vizinho, tempo_insercao, custo_bt);
        printf("  Custos -> BT: %d | Vizinho: %d | Insercao: %d\n", custo_bt, custo_vizinho, custo_insercao);

        free(caminho);
        liberar_grafo(g);
        liberar_grafo(g_tsp);
    }
    return 0;
}
