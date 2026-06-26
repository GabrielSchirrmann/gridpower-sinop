#include <stdio.h>
#include "menu.h"
#include "dijkstra.h"
#include "bfs.h"
#include "topologia.h"

static int ler_inteiro(const char* prompt) {
    int x;
    printf("%s", prompt);
    if (scanf("%d", &x) != 1){
        // Tratamento de erro para entrada invalida
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
            return -2; // Retorna um valor de erro para entrada inválida
    }
    return x;
}

static void listar_vertices(Grafo* g) {
    printf("\nVertices da rede:\n");
    for (int i = 0; i < g->num_vertices; i++){
        printf("   [%d] %-22s %s%s\n", i, g->vertices[i].nome,
        g->vertices[i].tipo,
    vertice_eh_fonte(g,i) ? "   (fonte)" : "");
    }
}

void executar_menu(Grafo* g) {
    int opc = -1;
    while (opc != 0) {
        printf("\n========= GridPower Sinop =========\n");
        printf("1) Imprimir a rede (lista de adjacencia)\n");
        printf("2) Dijkstra: menor rota a partir de um vertice\n");
        printf("3) Derrubar um VERTICE (torre/subestacao)\n");
        printf("4) Derrubar uma LINHA (aresta)\n");
        printf("0) Sair\n");

        opc = ler_inteiro("Opcao: ");

switch (opc) {
            case 1:
                imprimir_grafo(g);
                break;

            case 2: {
                listar_vertices(g);
                int o = ler_inteiro("Origem: ");
                if (o >= 0 && o < g->num_vertices) dijkstra(g, o);
                else printf("Vertice invalido.\n");
                break;
            }

            case 3: {
                listar_vertices(g);
                int no = ler_inteiro("Vertice a derrubar: ");
                if (no >= 0 && no < g->num_vertices) simular_queda_no(g, no);
                else printf("Vertice invalido.\n");
                break;
            }

            case 4: {
                listar_vertices(g);
                int a = ler_inteiro("Linha - extremidade A: ");
                int b = ler_inteiro("Linha - extremidade B: ");
                if (a >= 0 && a < g->num_vertices && b >= 0 && b < g->num_vertices)
                    bfs_conectividade(g, 0, a, b);
                else printf("Vertices invalidos.\n");
                break;
            }

            case 0:
                printf("Ate mais!\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }
    }
}