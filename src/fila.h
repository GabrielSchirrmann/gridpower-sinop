#ifndef FILA_H
#define FILA_H

// Um nó da fila (aguarda um id de vértice))
typedef struct NoFila {
    int valor;
    struct NoFila* prox;
} NoFila;

// A fila em si: ponteiros para o inicio e o fim
typedef struct {
    NoFila* inicio;
    NoFila* fim;
} Fila;

//---- Operações da fila ----
void fila_criar(Fila* f);   // Inicia uma fila vazia
void fila_enfileirar(Fila* f, int valor);   /// Põe no fim da fila
int  fila_desenfileirar(Fila* f);   // Retira do inicio e retorna o valor
int  fila_vazia(Fila* f);   // Retorna 1 se a fila estiver vazia, 0 caso contrário

#endif
