#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main() {
    int n = 5;

    // --- Teste Matriz de Adjacência ---
    printf("=== Testando Grafo Matriz ===\n");
    GrafoMatriz *gm = criar_grafo_matriz(n);
    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 0, 2);
    inserir_aresta_matriz(gm, 1, 3);

    printf("Grau do vertice 0 (Matriz): %d\n", grau_matriz(gm, 0));
    printf("0 e 1 sao adjacentes? %s\n", sao_adjacentes_matriz(gm, 0, 1) ? "Sim" : "Nao");
    printf("0 e 3 sao adjacentes? %s\n", sao_adjacentes_matriz(gm, 0, 3) ? "Sim" : "Nao");

    remover_aresta_matriz(gm, 0, 1);
    printf("Apos remover (0,1), grau de 0: %d\n", grau_matriz(gm, 0));
    liberar_grafo_matriz(gm);

    // --- Teste Lista de Adjacência ---
    printf("\n=== Testando Grafo Lista ===\n");
    GrafoLista *gl = criar_grafo_lista(n);
    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 0, 2);
    inserir_aresta_lista(gl, 1, 3);

    printf("Grau do vertice 0 (Lista): %d\n", grau_lista(gl, 0));
    printf("0 e 1 sao adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 1) ? "Sim" : "Nao");
    printf("0 e 3 sao adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 3) ? "Sim" : "Nao");

    remover_aresta_lista(gl, 0, 1);
    printf("Apos remover (0,1), grau de 0: %d\n", grau_lista(gl, 0));
    liberar_grafo_lista(gl);

    return 0;
}