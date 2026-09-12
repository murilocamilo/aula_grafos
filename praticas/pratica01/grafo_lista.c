#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista* criar_grafo_lista(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = (No**) malloc(n * sizeof(No*));
    for (int i = 0; i < n; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

static void aux_inserir_no(No **cabeca, int dest) {
    No *novo = (No*) malloc(sizeof(No));
    novo->destino = dest;
    novo->prox = *cabeca;
    *cabeca = novo;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        if (!sao_adjacentes_lista(g, u, v)) {
            aux_inserir_no(&(g->adj[u]), v);
            aux_inserir_no(&(g->adj[v]), u);
        }
    }
}

static void aux_remover_no(No **cabeca, int dest) {
    No *atual = *cabeca;
    No *ant = NULL;
    while (atual != NULL && atual->destino != dest) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        if (ant == NULL) *cabeca = atual->prox;
        else ant->prox = atual->prox;
        free(atual);
    }
}

void remover_aresta_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        aux_remover_no(&(g->adj[u]), v);
        aux_remover_no(&(g->adj[v]), u);
    }
}

int grau_lista(GrafoLista *g, int u) {
    if (u < 0 || u >= g->n) return -1;
    int grau = 0;
    No *atual = g->adj[u];
    while (atual) {
        grau++;
        atual = atual->prox;
    }
    return grau;
}

int sao_adjacentes_lista(GrafoLista *g, int u, int v) {
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) return 0;
    No *atual = g->adj[u];
    while (atual) {
        if (atual->destino == v) return 1;
        atual = atual->prox;
    }
    return 0;
}

void liberar_grafo_lista(GrafoLista *g) {
    if (!g) return;
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adj);
    free(g);
}