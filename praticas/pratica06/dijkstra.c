#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "dijkstra.h"

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao;
} MinHeap;

MinHeap* criar_min_heap(int capacidade) {
    MinHeap *h = (MinHeap*) malloc(sizeof(MinHeap));
    h->capacidade = capacidade;
    h->tamanho = 0;
    h->dados = (NoHeap*) malloc(capacidade * sizeof(NoHeap));
    h->posicao = (int*) malloc(capacidade * sizeof(int));
    for (int i = 0; i < capacidade; i++) h->posicao[i] = -1;
    return h;
}

void trocar(MinHeap *h, int i, int j) {
    NoHeap temp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = temp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

void subir(MinHeap *h, int i) {
    while (i > 0 && h->dados[i].distancia < h->dados[(i - 1) / 2].distancia) {
        trocar(h, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

void descer(MinHeap *h, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia) menor = esq;
    if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia) menor = dir;
    if (menor != i) {
        trocar(h, i, menor);
        descer(h, menor);
    }
}

void inserir_min_heap(MinHeap *h, int vertice, int distancia) {
    if (h->tamanho == h->capacidade) return;
    h->dados[h->tamanho] = (NoHeap){vertice, distancia};
    h->posicao[vertice] = h->tamanho;
    h->tamanho++;
    subir(h, h->tamanho - 1);
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap raiz = h->dados[0];
    h->posicao[raiz.vertice] = -1;
    h->tamanho--;
    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
        descer(h, 0);
    }
    return raiz;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    int i = h->posicao[vertice];
    if (i == -1) return;
    h->dados[i].distancia = nova_distancia;
    subir(h, i);
}

GrafoListaPonderado* criar_grafo(int V) {
    GrafoListaPonderado *g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->V = V;
    g->E = 0;
    g->listaAdj = (VerticeLista*) malloc(V * sizeof(VerticeLista));
    for (int i = 0; i < V; i++) g->listaAdj[i].cabeca = NULL;
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

void liberar_grafo(GrafoListaPonderado *g) {
    for (int i = 0; i < g->V; i++) {
        ArestaPonderada *atual = g->listaAdj[i].cabeca;
        while (atual != NULL) {
            ArestaPonderada *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(g->listaAdj);
    free(g);
}

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    MinHeap *h = criar_min_heap(g->V);
    for (int i = 0; i < g->V; i++) {
        dist[i] = INF;
        pred[i] = -1;
        inserir_min_heap(h, i, INF);
    }
    dist[origem] = 0;
    diminuir_chave(h, origem, 0);

    while (h->tamanho > 0) {
        NoHeap u = extrair_minimo(h);
        if (u.distancia == INF) break;

        ArestaPonderada *adj = g->listaAdj[u.vertice].cabeca;
        while (adj != NULL) {
            int v = adj->destino;
            if (dist[u.vertice] + adj->peso < dist[v]) {
                dist[v] = dist[u.vertice] + adj->peso;
                pred[v] = u.vertice;
                diminuir_chave(h, v, dist[v]);
            }
            adj = adj->proximo;
        }
    }
    free(h->dados);
    free(h->posicao);
    free(h);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (destino == origem) { printf("%d\n", origem); return; }
    if (pred[destino] == -1) { printf("Sem caminho\n"); return; }
    int caminho[100]; int tam = 0; int atual = destino;
    while (atual != -1 && tam < 100) {
        caminho[tam++] = atual;
        if (atual == origem) break;
        atual = pred[atual];
    }
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d", caminho[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}
