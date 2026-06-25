#include <stdio.h>
#include "grafo.h"
#include "dijkstra.h"
#include "bfs.h"

int main(void) {
    Grafo* g = ler_grafo("data/rede_sinop.csv");
    if (g == NULL) {
        return 1;
    }

    imprimir_grafo(g);

    dijkstra(g, 0);

    bfs_conectividade(g, 0, -1, -1);   // rede intacta
    bfs_conectividade(g, 0, 3, 4);     // cai a linha 3--4
    bfs_conectividade(g, 0, 0, 3);     // cai a linha 0--3 (no ciclo)

    liberar_grafo(g);
    return 0;
}