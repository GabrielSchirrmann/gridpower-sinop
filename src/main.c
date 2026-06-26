#include <stdio.h>
#include "grafo.h"
#include "menu.h"

int main(int argc, char** argv) {
    const char* caminho = (argc > 1) ? argv[1] : "data/rede_sinop.csv";

    Grafo* g = ler_grafo(caminho);
    if (g == NULL) {
        return 1;
    }

    executar_menu(g);

    liberar_grafo(g);
    return 0;
}