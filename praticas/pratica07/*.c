#include <stdio.h>
#include <stdlib.h>

#define INF 999999

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

void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int vertice_relac, int *pred) {
    (void)g;
    int atual = vertice_relac;
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

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int v, i;
    for (v = 0; v < g->V; v++) {
        dist[v] = INF;
        pred[v] = -1;
    }
    dist[origem] = 0;
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
        if (!atualizou) break;
    }
    for (int u = 0; u < g->V; u++) {
        if (dist[u] != INF) {
            ArestaPonderada *adj = g->listaAdj[u].cabeca;
            while (adj != NULL) {
                int v_dest = adj->destino;
                int peso = adj->peso;
                if (dist[u] + peso < dist[v_dest]) {
                    reconstruir_ciclo_negativo(g, v_dest, pred);
                    return 0;
                }
                adj = adj->proximo;
            }
        }
    }
    return 1;
}

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
    }
    free(dist);
    free(pred);
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
