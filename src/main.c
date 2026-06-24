#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "grafo.h"
#include "fila.h"

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

// Encontra a rota de menor distância da origem até todos os outros vértices.
void dijkstra(Grafo* g, int origem) {
    int n = g->num_vertices;

    // ---- as três estruturas que a rubrica pede ----
    double* dist = malloc(n * sizeof(double));
    int* prev = malloc(n * sizeof(int));
    int* visitado = malloc(n * sizeof(int));

    // ---- inicialização ----
    for (int i = 0; i < n; i++) {
        dist[i] = DBL_MAX;   // "infinito": ninguém foi alcançado ainda
        prev[i] = -1;        // ninguém tem predecessor ainda
        visitado[i] = 0;     // ninguém foi visitado ainda
    }
    dist[origem] = 0.0;      // distância da origem até ela mesma é zero

    // ---- loop principal: repete uma vez para cada vértice ----
    for (int passo = 0; passo < n; passo++) {

        // (1) acha o vértice não-visitado com menor dist
        int u = -1;
        double menor = DBL_MAX;
        for (int i = 0; i < n; i++) {
            if (!visitado[i] && dist[i] < menor) {
                menor = dist[i];
                u = i;
            }
        }

        // se não achou ninguém alcançável, encerra
        if (u == -1) break;

        // (2) marca como visitado
        visitado[u] = 1;

        // (3) relaxa todas as arestas de u
        No* atual = g->vertices[u].lista_adj;
        while (atual != NULL) {
            int v = atual->destino;
            double peso = atual->peso;

            if (!visitado[v] && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                prev[v] = u;
            }
            atual = atual->prox;
        }
    }

    // ---- imprime os resultados ----
    printf("\n=== Dijkstra a partir de [%d] %s ===\n\n",
        origem, g->vertices[origem].nome);

    for (int i = 0; i < n; i++) {
        printf("Ate [%d] %s: ", i, g->vertices[i].nome);

        if (dist[i] == DBL_MAX) {
            printf("inalcancavel\n");
        } else {
            printf("%.2f km | caminho: ", dist[i]);

            // reconstrói o caminho de trás pra frente
            int caminho[100];
            int tam = 0;
            int atual_no = i;
            while (atual_no != -1) {
                caminho[tam] = atual_no;
                tam++;
                atual_no = prev[atual_no];
            }
            // imprime invertido (da origem ao destino)
            for (int j = tam - 1; j >= 0; j--) {
                printf("%d", caminho[j]);
                if (j > 0) printf(" -> ");
            }
            printf("\n");
        }
    }

    // ---- libera a memória das três estruturas ----
    free(dist);
    free(prev);
    free(visitado);
}

// Verifica quais vértices continuam alcançáveis a partir da origem,
// IGNORANDO uma aresta removida (simula a queda de uma linha de transmissão).
// Para não remover nenhuma aresta, passe aresta_a = -1 e aresta_b = -1.
void bfs_conectividade(Grafo* g, int origem, int aresta_a, int aresta_b) {
    int n = g->num_vertices;

    // vetor que marca quem foi alcançado pela energia
    int* alcancado = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        alcancado[i] = 0;  // ninguém alcançado ainda
    }

    // ---- prepara a fila ----
    Fila fila;
    fila_criar(&fila);

    // a origem é o ponto de partida: marca e enfileira
    alcancado[origem] = 1;
    fila_enfileirar(&fila, origem);

    // ---- propagação ----
    while (!fila_vazia(&fila)) {
        int u = fila_desenfileirar(&fila);

        // olha todos os vizinhos de u
        No* atual = g->vertices[u].lista_adj;
        while (atual != NULL) {
            int v = atual->destino;

            // PULA a aresta removida (nos dois sentidos)
            int aresta_removida =
                (u == aresta_a && v == aresta_b) ||
                (u == aresta_b && v == aresta_a);

            if (!aresta_removida && !alcancado[v]) {
                alcancado[v] = 1;
                fila_enfileirar(&fila, v);
            }
            atual = atual->prox;
        }
    }

    // ---- relatório ----
    printf("\n=== BFS de conectividade a partir de [%d] %s ===\n",
        origem, g->vertices[origem].nome);
    if (aresta_a != -1) {
        printf("(simulando queda da linha %d--%d)\n", aresta_a, aresta_b);
    }
    printf("\nENERGIZADAS:\n");
    for (int i = 0; i < n; i++) {
        if (alcancado[i]) {
            printf("  [%d] %s\n", i, g->vertices[i].nome);
        }
    }
    printf("\nNO ESCURO:\n");
    int alguem_no_escuro = 0;
    for (int i = 0; i < n; i++) {
        if (!alcancado[i]) {
            printf("  [%d] %s\n", i, g->vertices[i].nome);
            alguem_no_escuro = 1;
        }
    }
    if (!alguem_no_escuro) {
        printf("  (ninguem - a rede continua totalmente conectada)\n");
    }

    free(alcancado);
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

dijkstra(g, 0);  // calcula rotas a partir da SE_Sinop_Eletronorte

// cenário 1: rede intacta
    bfs_conectividade(g, 0, -1, -1);

    // cenário 2: cai a linha 3--4 (Distrito--Inpasa)
    bfs_conectividade(g, 0, 3, 4);

    // cenário 3: cai a linha 0--3 (dentro do ciclo)
    bfs_conectividade(g, 0, 0, 3);

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
