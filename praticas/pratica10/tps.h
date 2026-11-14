#ifndef TPS_H
#define TPS_H

#define INF 999999

typedef struct {
    int V;
    int E;
    int **adj;
} Grafo;

Grafo* criar_grafo(int V);
void adicionar_aresta(Grafo *g, int u, int v, int peso);
void liberar_grafo(Grafo *g);
Grafo* grafo_para_tsp(Grafo *g);

int tsp_forca_bruta(Grafo *g, int *melhor_caminho);
int tsp_vizinho_mais_proximo(Grafo *g, int *caminho);
int tsp_insercao(Grafo *g, int *caminho);

#endif
