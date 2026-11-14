#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#define INF 2147483647

typedef struct ArestaPonderada {
    int destino;
    int peso;
    struct ArestaPonderada *proximo;
} ArestaPonderada;

typedef struct {
    ArestaPonderada *cabeca;
} VerticeLista;

typedef struct {
    int V;
    int E;
    VerticeLista *listaAdj;
} GrafoListaPonderado;

GrafoListaPonderado* criar_grafo(int V);
void adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso);
void liberar_grafo(GrafoListaPonderado *g);
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif
