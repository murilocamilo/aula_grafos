#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#define INF INT_MAX

// Estrutura do Nó de Adjacência para Grafo Ponderado
typedef struct NoAdj {
    int destino;
    int peso;
    struct NoAdj *prox;
} NoAdj;

// Estrutura do Grafo representado por Lista de Adjacência
typedef struct {
    int num_vertices;
    NoAdj **listas;
} GrafoListaPonderado;

// Protótipos - Grafo
GrafoListaPonderado* criar_grafo(int num_vertices);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

// Protótipos - Algoritmo de Dijkstra (Sem Min-Heap)
int encontrar_minimo_distancia(int *dist, int *visitado, int V);
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif