#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "cobertura.h"

Grafo* criar_grafo(int V) {
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    g->V = V;
    g->E = 0;
    g->adj = (int**) malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        g->adj[i] = (int*) calloc(V, sizeof(int));
    }
    return g;
}

void adicionar_aresta(Grafo *g, int u, int v) {
    if (g->adj[u][v] == 0) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
        g->E++;
    }
}

void liberar_grafo(Grafo *g) {
    for (int i = 0; i < g->V; i++) free(g->adj[i]);
    free(g->adj);
    free(g);
}

// Verifica se todas as arestas estão cobertas
int eh_cobertura(Grafo *g, int *cobertura, int tamanho) {
    for (int u = 0; u < g->V; u++) {
        for (int v = u + 1; v < g->V; v++) {
            if (g->adj[u][v]) {
                int u_coberto = 0, v_coberto = 0;
                for (int i = 0; i < tamanho; i++) {
                    if (cobertura[i] == u) u_coberto = 1;
                    if (cobertura[i] == v) v_coberto = 1;
                }
                if (!u_coberto && !v_coberto) return 0; // Aresta não coberta
            }
        }
    }
    return 1;
}

// Backtracking com poda
void bt_recursivo(Grafo *g, int *atual, int tam_atual, int vertice, int *melhor, int *menor_tam) {
    // Poda: se já temos mais vértices que a melhor solução, para.
    if (tam_atual >= *menor_tam) return;

    if (vertice == g->V) {
        if (eh_cobertura(g, atual, tam_atual)) {
            *menor_tam = tam_atual;
            for (int i = 0; i < tam_atual; i++) melhor[i] = atual[i];
        }
        return;
    }

    // Opção 1: Incluir o vértice atual
    atual[tam_atual] = vertice;
    bt_recursivo(g, atual, tam_atual + 1, vertice + 1, melhor, menor_tam);

    // Opção 2: Não incluir o vértice atual
    bt_recursivo(g, atual, tam_atual, vertice + 1, melhor, menor_tam);
}

int cobertura_minima_bt(Grafo *g, int *melhor_cobertura) {
    int *atual = (int*) malloc(g->V * sizeof(int));
    int menor_tam = INT_MAX;
    
    bt_recursivo(g, atual, 0, 0, melhor_cobertura, &menor_tam);
    
    free(atual);
    return menor_tam == INT_MAX ? 0 : menor_tam;
}

// Heurística gulosa: escolhe o vértice de maior grau
int cobertura_gulosa(Grafo *g, int *cobertura) {
    int *grau = (int*) calloc(g->V, sizeof(int));
    int *coberto = (int*) calloc(g->V, sizeof(int)); // Vértices já na cobertura
    int *aresta_coberta = (int*) calloc(g->V * g->V, sizeof(int));
    int tam = 0;

    // Calcula o grau inicial
    for (int i = 0; i < g->V; i++) {
        for (int j = 0; j < g->V; j++) {
            if (g->adj[i][j]) grau[i]++;
        }
    }

    int arestas_restantes = g->E;

    while (arestas_restantes > 0) {
        // Encontra o vértice de maior grau não escolhido
        int max_grau = -1, u = -1;
        for (int i = 0; i < g->V; i++) {
            if (!coberto[i] && grau[i] > max_grau) {
                max_grau = grau[i];
                u = i;
            }
        }

        if (u == -1) break;
        coberto[u] = 1;
        cobertura[tam++] = u;

        // Remove as arestas cobertas por u
        for (int v = 0; v < g->V; v++) {
            if (g->adj[u][v] && !aresta_coberta[u * g->V + v]) {
                aresta_coberta[u * g->V + v] = 1;
                aresta_coberta[v * g->V + u] = 1;
                arestas_restantes--;
                grau[u]--; // Reduz o grau do próprio u (opcional)
                grau[v]--; // Reduz o grau do vizinho v
            }
        }
    }

    free(grau);
    free(coberto);
    free(aresta_coberta);
    return tam;
}