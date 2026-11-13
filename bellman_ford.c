#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

// Define INF caso não esteja definido no header
#ifndef INF
#define INF 999999
#endif

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int v, i;
    
    // 1. Inicialização das distâncias e predecessores
    for (v = 0; v < g->V; v++) {
        dist[v] = INF;
        pred[v] = -1;
    }
    dist[origem] = 0;

    // 2. Relaxação das arestas V - 1 vezes
    for (i = 1; i <= g->V - 1; i++) {
        int atualizou = 0;
        for (int u = 0; u < g->V; u++) {
            if (dist[u] != INF) {
                ArestaPonderada *adj = g->listaAdj[u].cabeca;
                while (adj != NULL) {
                    int v_dest = adj->destino;
                    int peso = adj->peso;

                    if (dist[u] + peso < dist[v_dest]) {
                        dist[v_dest] = dist[u] + peso;
                        pred[v_dest] = u;
                        atualizou = 1;
                    }
                    adj = adj->proximo;
                }
            }
        }
        // Otimização: se nenhuma distância mudou nesta iteração, podemos parar
        if (!atualizou) break;
    }

    // 3. Verificação de ciclos de custo negativo (V-ésima iteração)
    for (int u = 0; u < g->V; u++) {
        if (dist[u] != INF) {
            ArestaPonderada *adj = g->listaAdj[u].cabeca;
            while (adj != NULL) {
                int v_dest = adj->destino;
                int peso = adj->peso;

                if (dist[u] + peso < dist[v_dest]) {
                    // Ciclo negativo detectado!
                    reconstruir_ciclo_negativo(g, v_dest, pred);
                    return 0; // Retorna 0 indicando falha devido a ciclo negativo
                }
                adj = adj->proximo;
            }
        }
    }

    return 1; // Sucesso, sem ciclos negativos
}

// A função de reconstrução fica aqui embaixo, FORA da bellman_ford
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int vertice_relac, int *pred) {
    (void)g; // Evita o aviso de parâmetro não utilizado no compilador
    
    int atual = vertice_relac;
    
    // Retrocede V vezes para garantir que estamos dentro do ciclo
    for (int i = 0; i < g->V; i++) {
        atual = pred[atual];
    }

    int inicio = atual;
    printf("Ciclo de custo negativo detectado envolvendo os vertices:\n");
    printf("%d", atual);
    
    atual = pred[atual];
    while (atual != inicio) {
        printf(" <- %d", atual);
        atual = pred[atual];
    }
    printf(" <- %d\n", inicio);
}