# Power Grid Simulator

Simulador de resiliência de redes elétricas escrito em C. A partir de um grafo
da rede de transmissão/distribuição, o programa calcula a **rota de menor
distância** entre pontos (Dijkstra) e simula **falhas** — queda de uma linha ou
de uma subestação inteira — para descobrir **quem fica no escuro**.

O dataset de exemplo modela a região de **Sinop (Mato Grosso, Brasil)** com
coordenadas geográficas reais. Como a região é apenas um arquivo de dados, o
mesmo motor roda qualquer outra rede no mesmo formato.

> Projeto desenvolvido como trabalho final de **Estruturas de Dados (Grafos)**.

## O que o programa faz

- **Modela a rede como grafo não-direcionado**, usando lista de adjacência.
- **Dijkstra** — menor rota (em km) de um vértice de origem para todos os
  outros, com reconstrução do caminho via vetor de predecessores.
- **BFS de conectividade** — simula a queda de uma **linha** (aresta) e verifica
  se a rede se dividiu, listando quem perdeu conexão.
- **Queda de vértice** (extensão) — simula a queda de uma **subestação ou torre**
  inteira (o nó e todas as suas linhas) usando BFS multi-fonte, e reporta quem
  ficou energizado e quem ficou no escuro.

## Conceitos de grafo aplicados ao tema

| Elemento do grafo | No mundo da rede elétrica |
|---|---|
| **Vértice** | Usina geradora, subestação, ponto de transmissão, junção |
| **Aresta** | Linha de transmissão / cabo de alta tensão |
| **Peso** | Distância física do cabo em km (calculada por Haversine) |
| **Fonte** | Vértice que injeta energia: tipo `TRANSMISSAO` ou `GERADORA` |

Um vértice é considerado **energizado** quando existe caminho até alguma fonte.
Na simulação de falha, o nó/linha derrubado é removido da busca; quem perde o
caminho até toda fonte fica **no escuro**.

## Estrutura do projeto

| Arquivo | Responsabilidade |
|---|---|
| `src/grafo.[ch]` | Estrutura do grafo e leitura (parser) do arquivo CSV |
| `src/haversine.[ch]` | Distância geográfica entre duas coordenadas |
| `src/fila.[ch]` | Fila encadeada usada pela BFS |
| `src/dijkstra.[ch]` | Caminho mínimo (distância + predecessores) |
| `src/bfs.[ch]` | Conectividade após queda de **linha** (aresta) |
| `src/topologia.[ch]` | Conectividade após queda de **vértice** (nó) |
| `src/menu.[ch]` | Menu interativo no terminal |
| `src/main.c` | Lê o grafo e entrega o controle ao menu |
| `data/rede_sinop.csv` | Rede de exemplo (Sinop-MT) |

## Formato do arquivo de dados

`data/rede_sinop.csv`

```
<num_vertices> <num_arestas>
<id> <nome> <tipo> <lat> <lon>          # uma linha por vertice
...
<origem> <destino> <tensao_kV>          # uma linha por aresta
...
```

Exemplo:

```
10 10
0 SE_Sinop_Eletronorte TRANSMISSAO -11.9834849 -55.5204356
...
0 1 138
4 8 34
```

Tipos de vertice: `TRANSMISSAO`, `SUBESTACAO`, `INDUSTRIAL`, `GERADORA`, `JUNCAO`.
O nome nao pode conter espacos (a leitura usa `%s`); use `_` como separador.

## Como compilar e rodar

Requer `gcc` e `make` (em Windows, use o WSL).

```bash
make            # compila e gera o executavel ./grid
make run        # compila e ja executa
make clean      # remove binario e arquivos objeto
```

Por padrao o programa carrega `data/rede_sinop.csv`. Para usar outra rede no
mesmo formato, passe o caminho como argumento:

```bash
./grid                       # usa a rede de Sinop
./grid data/outra_rede.csv   # usa outra rede
```

No menu, escolha a operacao: imprimir a rede, rodar Dijkstra, derrubar um
vertice ou derrubar uma linha.

## Gerenciamento de memoria

Toda a memoria e alocada dinamicamente (`malloc`) e liberada (`free`) ao final.
Validado com Valgrind, sem vazamentos e sem acessos invalidos:

```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

## Licenca

Distribuido sob a licenca MIT — veja o arquivo `LICENSE`.