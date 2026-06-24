#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "grafo.h"

// Calcula a distância em km entre dois pontos (lat/lon) na superfície da Terra.
double haversine(double lat1, double lon1, double lat2, double lon2) {
    double R = 6371.0;  // raio da Terra em km

    // converte graus para radianos
    double rad = 3.14159265358979323846 / 180.0;
    double dlat = (lat2 - lat1) * rad;
    double dlon = (lon2 - lon1) * rad;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1 * rad) * cos(lat2 * rad) *
               sin(dlon / 2) * sin(dlon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c;
}

// Lê o grafo de um arquivo CSV e monta a lista de adjacência na memória.
Grafo* ler_grafo(const char* caminho) {
    FILE* arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        printf("ERRO: nao foi possivel abrir o arquivo '%s'\n", caminho);
        return NULL;
    }

    // aloca o grafo
    Grafo* g = malloc(sizeof(Grafo));

    // lê o cabeçalho: numero de vertices e arestas
    fscanf(arquivo, "%d %d", &g->num_vertices, &g->num_arestas);

    // aloca o vetor de vertices
    g->vertices = malloc(g->num_vertices * sizeof(Vertice));

    // ---- lê os vertices ----
    for (int i = 0; i < g->num_vertices; i++) {
        fscanf(arquivo, "%d %s %s %lf %lf",
               &g->vertices[i].id,
               g->vertices[i].nome,
               g->vertices[i].tipo,
               &g->vertices[i].lat,
               &g->vertices[i].lon);
        g->vertices[i].lista_adj = NULL;
    }

    // ---- lê as arestas ----
    for (int i = 0; i < g->num_arestas; i++) {
        int origem, destino, tensao;
        fscanf(arquivo, "%d %d %d", &origem, &destino, &tensao);

        // calcula a distância real entre os dois vertices
        double peso = haversine(g->vertices[origem].lat, g->vertices[origem].lon,
                                g->vertices[destino].lat, g->vertices[destino].lon);

        // insere no sentido origem -> destino
        No* no1 = malloc(sizeof(No));
        no1->destino = destino;
        no1->peso = peso;
        no1->prox = g->vertices[origem].lista_adj;
        g->vertices[origem].lista_adj = no1;

        // insere no sentido destino -> origem (aresta não-dirigida!)
        No* no2 = malloc(sizeof(No));
        no2->destino = origem;
        no2->peso = peso;
        no2->prox = g->vertices[destino].lista_adj;
        g->vertices[destino].lista_adj = no2;
    }

    fclose(arquivo);
    return g;
}

int main(void) {
    Grafo* g = ler_grafo("data/rede_sinop.csv");
    if (g == NULL) {
        return 1;  // erro na leitura, encerra
    }

    // imprime o grafo
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

    // libera a memória
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

    return 0;
}