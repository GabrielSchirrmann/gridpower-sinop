#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

// Inicia uma fila vazia
void fila_criar(Fila* f) {
    f->inicio = NULL;
    f->fim = NULL;
}

// 1 se a fila está vazia 0 caso contrário
int fila_vazia(Fila* f) {
    return f->inicio == NULL;
}

// Põe um valor no final da fila
void fila_enfileirar(Fila* f, int valor) {
    NoFila* novo = malloc(sizeof(NoFila));
    novo->valor = valor;
    novo->prox = NULL;

    if (fila_vazia(f)){
        // Fila vazia: o novo nó é o inicio e o fim ao mesmo tempo
        f->inicio = novo;
        f->fim = novo;
    } else {
        // Fila com elementos: pendura no fim e atualiza o fim
        f->fim->prox = novo;
        f->fim = novo;
    }
}

// Tira o valor no ínicio da fila e retorna
int fila_desenfileirar(Fila* f){
    if (fila_vazia(f)){
        return -1; // Fila vazia, não há o que desenfileirar
    }

    NoFila* temp = f->inicio;   // Guarda o nó atual do inicio
    int valor = temp->valor;   // Salva o valor antes de liberar
    f->inicio = temp->prox;   // O inicio avança para o proximo

    // Se a fila ficou vazia, o fim também precisa virar NULL
    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    free(temp);   //Libera o nó removido
    return valor;
}
