#ifndef GRAFO_H
#define GRAFO_H

// ---- Estrutura de um nó da lista de adjacência (uma aresta) ----
typedef struct No {
    int destino;
    double peso;
    struct No* prox;
} No;

// ---- Estrutura de um vértice ----
typedef struct {
    int id;
    char nome[50];
    char tipo[20];
    double lat;
    double lon;
    No* lista_adj;
} Vertice;

// ---- Estrutura do grafo inteiro ----
typedef struct {
    int num_vertices;
    int num_arestas;
    Vertice* vertices;
} Grafo;

// ---- operações do grafo ----
Grafo* ler_grafo(const char* caminho);
void imprimir_grafo(Grafo* g);
void liberar_grafo(Grafo* g);

int vertice_eh_fonte(Grafo* g, int i);

#endif