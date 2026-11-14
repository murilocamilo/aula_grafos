#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct {
    int V;
    int E;
    int **adj;
} Grafo;

Grafo* criar_grafo(int V) {
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    g->V = V; g->E = 0;
    g->adj = (int**) malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) g->adj[i] = (int*) calloc(V, sizeof(int));
    return g;
}

void adicionar_aresta(Grafo *g, int u, int v) {
    if (g->adj[u][v] == 0) { g->adj[u][v] = 1; g->adj[v][u] = 1; g->E++; }
}

void liberar_grafo(Grafo *g) {
    for (int i = 0; i < g->V; i++) free(g->adj[i]);
    free(g->adj); free(g);
}

int eh_cobertura(Grafo *g, int *cobertura, int tamanho) {
    for (int u = 0; u < g->V; u++) {
        for (int v = u + 1; v < g->V; v++) {
            if (g->adj[u][v]) {
                int u_coberto = 0, v_coberto = 0;
                for (int i = 0; i < tamanho; i++) {
                    if (cobertura[i] == u) u_coberto = 1;
                    if (cobertura[i] == v) v_coberto = 1;
                }
                if (!u_coberto && !v_coberto) return 0;
            }
        }
    }
    return 1;
}

void bt_recursivo(Grafo *g, int *atual, int tam_atual, int vertice, int *melhor, int *menor_tam) {
    if (tam_atual >= *menor_tam) return;
    if (vertice == g->V) {
        if (eh_cobertura(g, atual, tam_atual)) {
            *menor_tam = tam_atual;
            for (int i = 0; i < tam_atual; i++) melhor[i] = atual[i];
        }
        return;
    }
    atual[tam_atual] = vertice;
    bt_recursivo(g, atual, tam_atual + 1, vertice + 1, melhor, menor_tam);
    bt_recursivo(g, atual, tam_atual, vertice + 1, melhor, menor_tam);
}

int cobertura_minima_bt(Grafo *g, int *melhor_cobertura) {
    int *atual = (int*) malloc(g->V * sizeof(int));
    int menor_tam = INT_MAX;
    bt_recursivo(g, atual, 0, 0, melhor_cobertura, &menor_tam);
    free(atual);
    return menor_tam == INT_MAX ? 0 : menor_tam;
}

int cobertura_gulosa(Grafo *g, int *cobertura) {
    int *grau = (int*) calloc(g->V, sizeof(int));
    int *coberto = (int*) calloc(g->V, sizeof(int));
    int *aresta_coberta = (int*) calloc(g->V * g->V, sizeof(int));
    int tam = 0;
    for (int i = 0; i < g->V; i++) for (int j = 0; j < g->V; j++) if (g->adj[i][j]) grau[i]++;
    int arestas_restantes = g->E;
    while (arestas_restantes > 0) {
        int max_grau = -1, u = -1;
        for (int i = 0; i < g->V; i++) if (!coberto[i] && grau[i] > max_grau) { max_grau = grau[i]; u = i; }
        if (u == -1) break;
        coberto[u] = 1; cobertura[tam++] = u;
        for (int v = 0; v < g->V; v++) {
            if (g->adj[u][v] && !aresta_coberta[u * g->V + v]) {
                aresta_coberta[u * g->V + v] = 1; aresta_coberta[v * g->V + u] = 1;
                arestas_restantes--; grau[u]--; grau[v]--;
            }
        }
    }
    free(grau); free(coberto); free(aresta_coberta);
    return tam;
}

Grafo* gerar_grafo_aleatorio(int V, int arestas) {
    Grafo *g = criar_grafo(V);
    srand(42);
    int count = 0;
    while (count < arestas) {
        int u = rand() % V; int v = rand() % V;
        if (u != v && g->adj[u][v] == 0) { adicionar_aresta(g, u, v); count++; }
    }
    return g;
}

int main(void) {
    int tamanhos[] = {5, 8, 10, 12, 15};
    printf("--- Experimento de Cobertura de Vertices ---\n");
    printf("%-5s | %-15s | %-15s\n", "N", "BT (Tempo ms)", "Guloso (Tempo ms)");
    printf("---------------------------------------------\n");
    for (int i = 0; i < 5; i++) {
        int V = tamanhos[i];
        Grafo *g = gerar_grafo_aleatorio(V, V * 2);
        int *cob_bt = (int*) malloc(V * sizeof(int));
        int *cob_guloso = (int*) malloc(V * sizeof(int));
        clock_t inicio = clock();
        int tam_bt = cobertura_minima_bt(g, cob_bt);
        clock_t fim = clock();
        double tempo_bt = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;
        inicio = clock();
        int tam_guloso = cobertura_gulosa(g, cob_guloso);
        fim = clock();
        double tempo_guloso = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;
        printf("%-5d | %-15.4f | %-15.4f\n", V, tempo_bt, tempo_guloso);
        printf("  Tamanho BT: %d | Tamanho Guloso: %d\n", tam_bt, tam_guloso);
        free(cob_bt); free(cob_guloso); liberar_grafo(g);
    }
    return 0;
}
