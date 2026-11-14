#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

// --- Estruturas ---
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

// --- Funções do Grafo ---
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

// --- Min-Heap ---
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

// --- Dijkstra ---
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

// --- MAIN ---
int main() {
    int V = 6;
    GrafoListaPonderado *g = criar_grafo(V);
    adicionar_aresta(g, 0, 1, 4); adicionar_aresta(g, 1, 0, 4);
    adicionar_aresta(g, 0, 2, 2); adicionar_aresta(g, 2, 0, 2);
    adicionar_aresta(g, 1, 2, 1); adicionar_aresta(g, 2, 1, 1);
    adicionar_aresta(g, 1, 3, 5); adicionar_aresta(g, 3, 1, 5);
    adicionar_aresta(g, 1, 4, 8); adicionar_aresta(g, 4, 1, 8);
    adicionar_aresta(g, 2, 3, 4); adicionar_aresta(g, 3, 2, 4);
    adicionar_aresta(g, 2, 4, 10); adicionar_aresta(g, 4, 2, 10);
    adicionar_aresta(g, 3, 4, 2); adicionar_aresta(g, 4, 3, 2);
    adicionar_aresta(g, 3, 5, 1); adicionar_aresta(g, 5, 3, 1);
    adicionar_aresta(g, 4, 5, 3); adicionar_aresta(g, 5, 4, 3);

    int dist[V]; int pred[V]; int origem = 0;
    dijkstra(g, origem, dist, pred);
    printf("--- Distancias minimas a partir da origem (A) ---\n");
    for (int i = 0; i < V; i++) {
        if (dist[i] == INF) printf("Vertice %c: Inalcancavel\n", 'A' + i);
        else printf("Vertice %c: Custo %d\n", 'A' + i, dist[i]);
    }
    printf("\n--- Reconstrucao dos Caminhos ---\n");
    for (int i = 0; i < V; i++) {
        printf("Caminho ate %c: ", 'A' + i);
        reconstruir_caminho(origem, i, pred);
    }
    liberar_grafo(g);
    return 0;
}
