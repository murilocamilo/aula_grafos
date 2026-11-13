#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

// Funções auxiliares locais
GrafoListaPonderado* criar_grafo(int V) {
    GrafoListaPonderado *g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->V = V;
    g->E = 0;
    g->listaAdj = (VerticeLista*) malloc(V * sizeof(VerticeLista));
    for (int i = 0; i < V; i++) {
        g->listaAdj[i].cabeca = NULL;
    }
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso) {
    ArestaPonderada *nova = (ArestaPonderada*) malloc(sizeof(ArestaPonderada));
    nova->destino = destino;
    nova->peso = peso;
    nova->proximo = g->listaAdj[origem].cabeca;
    g->listaAdj[origem].cabeca = nova;
    g->E++;
}

int main(void) {
    int V = 4;
    GrafoListaPonderado *g = criar_grafo(V);

    adicionar_aresta(g, 0, 1, 1);
    adicionar_aresta(g, 1, 2, 2);
    adicionar_aresta(g, 2, 3, 3);
    adicionar_aresta(g, 0, 3, 10);

    int *dist = (int*) malloc(V * sizeof(int));
    int *pred = (int*) malloc(V * sizeof(int));

    int origem = 0;
    int resultado = bellman_ford(g, origem, dist, pred);

    if (resultado) {
        printf("Caminhos mais curtos a partir do vertice %d:\n", origem);
        for (int i = 0; i < V; i++) {
            printf("Distancia ate %d: %d\n", i, dist[i]);
        }
    } else {
        printf("O grafo contem ciclo negativo.\n");
        // A função de imprimir o ciclo já foi chamada dentro da bellman_ford
    }

    free(dist);
    free(pred);
    
    // Liberação da memória do grafo
    for (int i = 0; i < V; i++) {
        ArestaPonderada *atual = g->listaAdj[i].cabeca;
        while (atual != NULL) {
            ArestaPonderada *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(g->listaAdj);
    free(g);

    return 0;
}