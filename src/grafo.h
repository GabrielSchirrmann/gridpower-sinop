#ifndef GRAFO_H
#define GRAFO_H

//----Estrutura de nó da lista de adjecência (uma aresta)----
typedef struct No {
    int destino;   //índice do vértice vizinho 
    double peso;   // distância (km) calculada pelo Haversine
    struct No*prox;   // ponteiro pro próximo vizinho na lista
} No;

// ---- Estrutura de um vértice ----
typedef struct {
    int id;
    char nome[50];
    char tipo[20];
    double lat;
    double lon;
    No* lista_adj; // ponteiro para a lista de vizinhos
} Vertice;

// ---- Estrutura do grafo inteiro ----
typedef struct {
    int num_vertices;
    int num_arestas;
    Vertice* vertices; // vetor de vértices (alocado dinamicamente)
} Grafo;

#endif