#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"
#include "fila.h"

void bfs_conectividade(Grafo* g, int origem, int aresta_a, int aresta_b) {
    int n = g->num_vertices;

    int* alcancado = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        alcancado[i] = 0;
    }

    Fila fila;
    fila_criar(&fila);

    alcancado[origem] = 1;
    fila_enfileirar(&fila, origem);

    while (!fila_vazia(&fila)) {
        int u = fila_desenfileirar(&fila);

        No* atual = g->vertices[u].lista_adj;
        while (atual != NULL) {
            int v = atual->destino;

            int aresta_removida =
                (u == aresta_a && v == aresta_b) ||
                (u == aresta_b && v == aresta_a);

            if (!aresta_removida && !alcancado[v]) {
                alcancado[v] = 1;
                fila_enfileirar(&fila, v);
            }
            atual = atual->prox;
        }
    }

    printf("\n=== BFS de conectividade a partir de [%d] %s ===\n",
           origem, g->vertices[origem].nome);
    if (aresta_a != -1) {
        printf("(simulando queda da linha %d--%d)\n", aresta_a, aresta_b);
    }
    printf("\nENERGIZADAS:\n");
    for (int i = 0; i < n; i++) {
        if (alcancado[i]) {
            printf("  [%d] %s\n", i, g->vertices[i].nome);
        }
    }
    printf("\nNO ESCURO:\n");
    int alguem_no_escuro = 0;
    for (int i = 0; i < n; i++) {
        if (!alcancado[i]) {
            printf("  [%d] %s\n", i, g->vertices[i].nome);
            alguem_no_escuro = 1;
        }
    }
    if (!alguem_no_escuro) {
        printf("  (ninguem - a rede continua totalmente conectada)\n");
    }

    free(alcancado);
}