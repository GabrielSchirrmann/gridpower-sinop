#ifndef TOPOLOGIA_H
#define TOPOLOGIA_H

#include "grafo.h"

// Calcula quais vértices ficam ENERGIZADOS quando 'no_removido' é derrubado.
// Energizado = existe caminho até alguma fonte (TRANSMISSAO ou GERADORA),
// usando só vértices/arestas que não passam pelo nó derrubado.
// Use no_removido = -1 para a rede intacta (baseline).
//
// Retorna um vetor alocado de tamanho g->num_vertices:
//   energizado[i] == 1  -> tem energia
//   energizado[i] == 0  -> está no escuro
// O CHAMADOR deve liberar com free().

int* energizados_apos_queda(Grafo* g, int no_removido);
void simular_queda_no(Grafo* g, int no_removido);

#endif