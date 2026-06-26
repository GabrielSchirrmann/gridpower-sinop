#include <stdio.h>
#include <stdlib.h>
#include "topologia.h"
#include "fila.h"

int* energizados_apos_queda(Grafo* g, int no_removido) {
    int n = g->num_vertices;

    int* energizado = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        energizado[i] = 0;                 // todo mundo começa no escuro
    }

    Fila fila;
    fila_criar(&fila);

    // Multi-fonte: toda fonte que não seja o nó derrubado
    // entra na fila como ponto de partida da busca.
    for (int i = 0; i < n; i++) {
        if (i != no_removido && vertice_eh_fonte(g, i)) {
            energizado[i] = 1;
            fila_enfileirar(&fila, i);
        }                                  // fecha o if
    }                                      // fecha o for

    while (!fila_vazia(&fila)) {
        int u = fila_desenfileirar(&fila);

        No* atual = g->vertices[u].lista_adj;
        while (atual != NULL) {
            int v = atual->destino;
            if (v != no_removido && !energizado[v]) {
                energizado[v] = 1;
                fila_enfileirar(&fila, v);
            }
            atual = atual->prox;
        }
    }

    return energizado;
}                                          // <-- energizados_apos_queda termina AQUI

void simular_queda_no(Grafo* g, int no_removido) {
    int n = g->num_vertices;
    int* energizado = energizados_apos_queda(g, no_removido);

    printf("\n=== Simulacao: queda do vertice ");
    if (no_removido < 0) {
        printf("(NENHUM - rede intacta) ===\n");
    } else {
        printf("[%d] %s ===\n", no_removido, g->vertices[no_removido].nome);
    }

    printf("\nENERGIZADAS:\n");
    int qtd_lig = 0;
    for (int i = 0; i < n; i++) {
        if (energizado[i]) {
            printf("  [%d] %s\n", i, g->vertices[i].nome);
            qtd_lig++;
        }
    }

    printf("\nNO ESCURO:\n");
    int qtd_esc = 0;
    for (int i = 0; i <n; i++) {
        if (!energizado[i]) {
            printf("  [%d] %s%s\n", i, g->vertices[i].nome,
                (i == no_removido) ? " (derrubado)" : "");
            qtd_esc++;
        }
    }
    if (qtd_esc == 0) {
        printf("  (ninguem - a rede continua totalmente energizada)\n");

    }
    printf("\nResumo: %d energizada(s), %d no escuro.\n", qtd_lig, qtd_esc);

    free(energizado);
}