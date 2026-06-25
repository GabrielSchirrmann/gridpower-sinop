#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "dijkstra.h"

void dijkstra(Grafo* g, int origem) {
    int n = g->num_vertices;

    double* dist = malloc(n * sizeof(double));
    int* prev = malloc(n * sizeof(int));
    int* visitado = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        dist[i] = DBL_MAX;
        prev[i] = -1;
        visitado[i] = 0;
    }
    dist[origem] = 0.0;

    for (int passo = 0; passo < n; passo++) {
        int u = -1;
        double menor = DBL_MAX;
        for (int i = 0; i < n; i++) {
            if (!visitado[i] && dist[i] < menor) {
                menor = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        visitado[u] = 1;

        No* atual = g->vertices[u].lista_adj;
        while (atual != NULL) {
            int v = atual->destino;
            double peso = atual->peso;

            if (!visitado[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                prev[v] = u;
            }
            atual = atual->prox;
        }
    }

    printf("\n=== Dijkstra a partir de [%d] %s ===\n\n",
           origem, g->vertices[origem].nome);

    for (int i = 0; i < n; i++) {
        printf("Ate [%d] %s: ", i, g->vertices[i].nome);

        if (dist[i] == DBL_MAX) {
            printf("inalcancavel\n");
        } else {
            printf("%.2f km | caminho: ", dist[i]);

            int caminho[100];
            int tam = 0;
            int atual_no = i;
            while (atual_no != -1) {
                caminho[tam] = atual_no;
                tam++;
                atual_no = prev[atual_no];
            }
            for (int j = tam - 1; j >= 0; j--) {
                printf("%d", caminho[j]);
                if (j > 0) printf(" -> ");
            }
            printf("\n");
        }
    }

    free(dist);
    free(prev);
    free(visitado);
}