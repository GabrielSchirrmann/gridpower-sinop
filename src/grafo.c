#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "haversine.h"

int vertice_eh_fonte(Grafo* g, int i) {
    return strcmp(g->vertices[i].tipo, "TRANSMISSAO") == 0 ||
        strcmp(g->vertices[i].tipo, "GERADORA") == 0;
}

Grafo* ler_grafo(const char* caminho) {
    FILE* arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'\n", caminho);
        return NULL;
    }

    Grafo* g = malloc(sizeof(Grafo));
    fscanf(arquivo, "%d %d", &g->num_vertices, &g->num_arestas);
    g->vertices = malloc(g->num_vertices * sizeof(Vertice));

    for (int i = 0; i < g->num_vertices; i++) {
        fscanf(arquivo, "%d %s %s %lf %lf",
               &g->vertices[i].id,
               g->vertices[i].nome,
               g->vertices[i].tipo,
               &g->vertices[i].lat,
               &g->vertices[i].lon);
        g->vertices[i].lista_adj = NULL;
    }

    for (int i = 0; i < g->num_arestas; i++) {
        int origem, destino, tensao;
        fscanf(arquivo, "%d %d %d", &origem, &destino, &tensao);

        double peso = haversine(g->vertices[origem].lat, g->vertices[origem].lon,
                                g->vertices[destino].lat, g->vertices[destino].lon);

        No* no1 = malloc(sizeof(No));
        no1->destino = destino;
        no1->peso = peso;
        no1->prox = g->vertices[origem].lista_adj;
        g->vertices[origem].lista_adj = no1;

        No* no2 = malloc(sizeof(No));
        no2->destino = origem;
        no2->peso = peso;
        no2->prox = g->vertices[destino].lista_adj;
        g->vertices[destino].lista_adj = no2;
    }

    fclose(arquivo);
    return g;
}

void imprimir_grafo(Grafo* g) {
    printf("Grafo com %d vertices e %d arestas:\n\n",
           g->num_vertices, g->num_arestas);
    for (int i = 0; i < g->num_vertices; i++) {
        printf("[%d] %s (%s)\n", g->vertices[i].id,
               g->vertices[i].nome, g->vertices[i].tipo);

        No* atual = g->vertices[i].lista_adj;
        while (atual != NULL) {
            printf("   -> vizinho %d (peso %.2f km)\n",
                   atual->destino, atual->peso);
            atual = atual->prox;
        }
    }
}

void liberar_grafo(Grafo* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        No* atual = g->vertices[i].lista_adj;
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->vertices);
    free(g);
}