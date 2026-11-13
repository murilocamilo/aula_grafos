#ifndef COBERTURA_H
#define COBERTURA_H

// Estrutura para representar um grafo simples não direcionado
typedef struct {
    int V;              // Número de vértices
    int E;              // Número de arestas
    int **adj;          // Matriz de adjacência (1 se tem aresta, 0 se não)
} Grafo;

// Funções auxiliares
Grafo* criar_grafo(int V);
void adicionar_aresta(Grafo *g, int u, int v);
void liberar_grafo(Grafo *g);

// Funções da prática
int eh_cobertura(Grafo *g, int *cobertura, int tamanho);
int cobertura_minima_bt(Grafo *g, int *melhor_cobertura);
int cobertura_gulosa(Grafo *g, int *cobertura);

#endif