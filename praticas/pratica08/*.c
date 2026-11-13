#include <stdio.h>
#include <stdlib.h>

#define INF 999999

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

int find(DSU *d, int x) {
    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]);
    }
    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);
    if (raiz_x == raiz_y) return 0;
    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
        d->pai[raiz_y] = raiz_x;
    } else {
        d->pai[raiz_y] = raiz_x;
        d->rank[raiz_x]++;
    }
    return 1;
}

int comparar_arestas(const void *a, const void *b) {
    Aresta *a1 = (Aresta *)a;
    Aresta *a2 = (Aresta *)b;
    return a1->peso - a2->peso;
}

void kruskal(Aresta *arestas, int V, int E) {
    qsort(arestas, E, sizeof(Aresta), comparar_arestas);
    DSU d;
    d.n = V;
    d.pai = (int *)malloc(V * sizeof(int));
    d.rank = (int *)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        d.pai[i] = i;
        d.rank[i] = 0;
    }
    printf("\n--- Kruskal ---\n");
    int peso_total = 0;
    int arestas_incluidas = 0;
    for (int i = 0; i < E && arestas_incluidas < V - 1; i++) {
        if (unir(&d, arestas[i].u, arestas[i].v)) {
            printf("Aresta: %d - %d | Peso: %d\n", arestas[i].u, arestas[i].v, arestas[i].peso);
            peso_total += arestas[i].peso;
            arestas_incluidas++;
        }
    }
    printf("Peso total da AGM (Kruskal): %d\n", peso_total);
    free(d.pai);
    free(d.rank);
}

void prim(int **grafo, int V) {
    int *chave = (int *)malloc(V * sizeof(int));
    int *pai = (int *)malloc(V * sizeof(int));
    int *mstSet = (int *)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        chave[i] = INF;
        mstSet[i] = 0;
        pai[i] = -1;
    }
    chave[0] = 0;
    printf("\n--- Prim ---\n");
    int peso_total = 0;
    for (int count = 0; count < V - 1; count++) {
        int min = INF, u = -1;
        for (int v = 0; v < V; v++) {
            if (mstSet[v] == 0 && chave[v] < min) {
                min = chave[v];
                u = v;
            }
        }
        if (u == -1) break;
        mstSet[u] = 1;
        for (int v = 0; v < V; v++) {
            if (grafo[u][v] && mstSet[v] == 0 && grafo[u][v] < chave[v]) {
                pai[v] = u;
                chave[v] = grafo[u][v];
            }
        }
    }
    for (int i = 1; i < V; i++) {
        printf("Aresta: %d - %d | Peso: %d\n", pai[i], i, grafo[i][pai[i]]);
        peso_total += grafo[i][pai[i]];
    }
    printf("Peso total da AGM (Prim): %d\n", peso_total);
    free(chave);
    free(pai);
    free(mstSet);
}

int main(void) {
    int V = 4;
    int E = 5;
    Aresta *arestas = (Aresta *)malloc(E * sizeof(Aresta));
    arestas[0] = (Aresta){0, 1, 10};
    arestas[1] = (Aresta){0, 2, 6};
    arestas[2] = (Aresta){0, 3, 5};
    arestas[3] = (Aresta){1, 3, 15};
    arestas[4] = (Aresta){2, 3, 4};
    kruskal(arestas, V, E);
    free(arestas);

    int **grafo = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        grafo[i] = (int *)calloc(V, sizeof(int));
    }
    grafo[0][1] = grafo[1][0] = 10;
    grafo[0][2] = grafo[2][0] = 6;
    grafo[0][3] = grafo[3][0] = 5;
    grafo[1][3] = grafo[3][1] = 15;
    grafo[2][3] = grafo[3][2] = 4;
    prim(grafo, V);
    for (int i = 0; i < V; i++) free(grafo[i]);
    free(grafo);
    return 0;
}
