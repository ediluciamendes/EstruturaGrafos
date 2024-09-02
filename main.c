#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Estrutura para representar uma aresta
typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta* prox;
} Aresta;

// Estrutura para representar um grafo
typedef struct Grafo {
    Aresta** adjacencias;
    int numVertices;
} Grafo;

// Função para criar um novo grafo
Grafo* criarGrafo(int numVertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->adjacencias = (Aresta**)malloc(numVertices * sizeof(Aresta*));

    for (int i = 0; i < numVertices; i++) {
        grafo->adjacencias[i] = NULL;
    }

    return grafo;
}

// Função para criar uma nova aresta
Aresta* novaAresta(int destino, int peso) {
    Aresta* aresta = (Aresta*)malloc(sizeof(Aresta));
    aresta->destino = destino;
    aresta->peso = peso;
    aresta->prox = NULL;
    return aresta;
}

// Função para inserir um vértice (incrementa o número de vértices)
void inserirVertice(Grafo* grafo) {
    grafo->numVertices++;
    grafo->adjacencias = (Aresta**)realloc(grafo->adjacencias, grafo->numVertices * sizeof(Aresta*));
    grafo->adjacencias[grafo->numVertices - 1] = NULL;
}

// Função para inserir uma aresta
void inserirAresta(Grafo* grafo, int origem, int destino, int peso) {
    Aresta* nova = novaAresta(destino, peso);
    nova->prox = grafo->adjacencias[origem];
    grafo->adjacencias[origem] = nova;

    // Como o grafo é não direcionado, adiciona a aresta na outra direção
    nova = novaAresta(origem, peso);
    nova->prox = grafo->adjacencias[destino];
    grafo->adjacencias[destino] = nova;
}

// Função para remover um vértice
void removerVertice(Grafo* grafo, int id) {
    // Remove todas as arestas ligadas ao vértice a ser removido
    for (int i = 0; i < grafo->numVertices; i++) {
        Aresta* atual = grafo->adjacencias[i];
        Aresta* anterior = NULL;

        while (atual != NULL) {
            if (atual->destino == id) {
                if (anterior == NULL) {
                    grafo->adjacencias[i] = atual->prox;
                } else {
                    anterior->prox = atual->prox;
                }
                free(atual);
                break;
            }
            anterior = atual;
            atual = atual->prox;
        }
    }

    // Remove o próprio vértice
    free(grafo->adjacencias[id]);
    for (int i = id; i < grafo->numVertices - 1; i++) {
        grafo->adjacencias[i] = grafo->adjacencias[i + 1];
    }
    grafo->numVertices--;
    grafo->adjacencias = (Aresta**)realloc(grafo->adjacencias, grafo->numVertices * sizeof(Aresta*));
}

// Função para remover uma aresta
void removerAresta(Grafo* grafo, int origem, int destino) {
    Aresta* atual = grafo->adjacencias[origem];
    Aresta* anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == destino) {
            if (anterior == NULL) {
                grafo->adjacencias[origem] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }

    // Remove a aresta na outra direção (grafo não direcionado)
    atual = grafo->adjacencias[destino];
    anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == origem) {
            if (anterior == NULL) {
                grafo->adjacencias[destino] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

// Função para visualizar o grafo
void visualizarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        printf("Vértice %d:", i);
        Aresta* atual = grafo->adjacencias[i];
        while (atual != NULL) {
            printf(" -> (%d, peso %d)", atual->destino, atual->peso);
            atual = atual->prox;
        }
        printf("\n");
    }
}

// Função para informar o grau de um vértice
void Grau(Grafo* grafo, int vertice) {
    int grau = 0;
    Aresta* atual = grafo->adjacencias[vertice];
    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }
    printf("O grau do vértice %d é %d\n", vertice, grau);
}

// Função para liberar a memória do grafo
void liberarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        Aresta* atual = grafo->adjacencias[i];
        while (atual != NULL) {
            Aresta* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(grafo->adjacencias);
    free(grafo);
}

// Função auxiliar para DFS
void auxDFS(Grafo* grafo, int vertice, bool* visitado) {
    visitado[vertice] = true;
    Aresta* atual = grafo->adjacencias[vertice];

    while (atual != NULL) {
        if (!visitado[atual->destino]) {
            auxDFS(grafo, atual->destino, visitado);
        }
        atual = atual->prox;
    }
}

// Função para verificar se o grafo é conexo
bool verificarConexo(Grafo* grafo) {
    if (grafo->numVertices == 0) return true;

    bool* visitado = (bool*)calloc(grafo->numVertices, sizeof(bool));
    auxDFS(grafo, 0, visitado);

    for (int i = 0; i < grafo->numVertices; i++) {
        if (!visitado[i]) {
            free(visitado);
            return false;
        }
    }
    free(visitado);
    return true;
}

// Função para realizar a busca em largura (BFS)
void buscalargura(Grafo* grafo, int verticeInicial) {
    bool* visitado = (bool*)calloc(grafo->numVertices, sizeof(bool));
    int* fila = (int*)malloc(grafo->numVertices * sizeof(int));
    int frente = 0, tras = 0;

    visitado[verticeInicial] = true;
    fila[tras++] = verticeInicial;

    while (frente != tras) {
        int vertice = fila[frente++];
        printf("%d ", vertice);

        Aresta* atual = grafo->adjacencias[vertice];
        while (atual != NULL) {
            if (!visitado[atual->destino]) {
                visitado[atual->destino] = true;
                fila[tras++] = atual->destino;
            }
            atual = atual->prox;
        }
    }
    printf("\n");
    free(visitado);
    free(fila);
}

// Função para realizar a busca em profundidade (DFS)
void buscaprofundidade(Grafo* grafo, int verticeInicial) {
    bool* visitado = (bool*)calloc(grafo->numVertices, sizeof(bool));
    auxDFS(grafo, verticeInicial, visitado);
    for (int i = 0; i < grafo->numVertices; i++) {
        if (visitado[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    free(visitado);
}

// Função para encontrar o caminho mínimo usando Dijkstra
void dijkstra(Grafo* grafo, int origem) {
    int* distancia = (int*)malloc(grafo->numVertices * sizeof(int));
    bool* visitado = (bool*)calloc(grafo->numVertices, sizeof(bool));

    for (int i = 0; i < grafo->numVertices; i++) {
        distancia[i] = INT_MAX;
    }
    distancia[origem] = 0;

    for (int count = 0; count < grafo->numVertices - 1; count++) {
        int min = INT_MAX, minIndex;

        for (int v = 0; v < grafo->numVertices; v++) {
            if (!visitado[v] && distancia[v] <= min) {
                min = distancia[v];
                minIndex = v;
            }
        }

        visitado[minIndex] = true;

        Aresta* atual = grafo->adjacencias[minIndex];
        while (atual != NULL) {
            int v = atual->destino;
            if (!visitado[v] && distancia[minIndex] != INT_MAX &&
                distancia[minIndex] + atual->peso < distancia[v]) {
                distancia[v] = distancia[minIndex] + atual->peso;
            }
            atual = atual->prox;
        }
    }

    printf("Distâncias a partir do vértice %d:\n", origem);
    for (int i = 0; i < grafo->numVertices; i++) {
        printf("%d: %d\n", i, distancia[i]);
    }

    free(distancia);
    free(visitado);
}

// Função para encontrar a árvore geradora mínima usando Prim
void prim(Grafo* grafo) {
    int* chave = (int*)malloc(grafo->numVertices * sizeof(int));
    int* parent = (int*)malloc(grafo->numVertices * sizeof(int));
    bool* naMST = (bool*)calloc(grafo->numVertices, sizeof(bool));

    for (int i = 0; i < grafo->numVertices; i++) {
        chave[i] = INT_MAX;
        parent[i] = -1;
    }

    chave[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < grafo->numVertices - 1; count++) {
        int min = INT_MAX, minIndex;

        for (int v = 0; v < grafo->numVertices; v++) {
            if (!naMST[v] && chave[v] < min) {
                min = chave[v];
                minIndex = v;
            }
        }

        naMST[minIndex] = true;

        Aresta* atual = grafo->adjacencias[minIndex];
        while (atual != NULL) {
            int v = atual->destino;
            if (!naMST[v] && atual->peso < chave[v]) {
                parent[v] = minIndex;
                chave[v] = atual->peso;
            }
            atual = atual->prox;
        }
    }

    printf("Árvore Geradora Mínima (Prim):\n");
    for (int i = 1; i < grafo->numVertices; i++) {
        printf("%d - %d\n", parent[i], i);
    }

    free(chave);
    free(parent);
    free(naMST);
}

// Função para converter o grafo em uma matriz de adjacência
void converterMatrizAdjacencia(Grafo* grafo) {
    int** matrizAdj = (int**)malloc(grafo->numVertices * sizeof(int*));
    for (int i = 0; i < grafo->numVertices; i++) {
        matrizAdj[i] = (int*)calloc(grafo->numVertices, sizeof(int));
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        Aresta* atual = grafo->adjacencias[i];
        while (atual != NULL) {
            matrizAdj[i][atual->destino] = atual->peso;
            atual = atual->prox;
        }
    }

    printf("Matriz de Adjacência:\n");
    for (int i = 0; i < grafo->numVertices; i++) {
        for (int j = 0; j < grafo->numVertices; j++) {
            printf("%d ", matrizAdj[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        free(matrizAdj[i]);
    }
    free(matrizAdj);
}


int main() {
    Grafo* grafo = criarGrafo(0);
    int opcao, vertice, origem, destino, peso;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Inserir Vértice\n");
        printf("2. Inserir Aresta\n");
        printf("3. Remover Vértice\n");
        printf("4. Remover Aresta\n");
        printf("5. Visualizar Grafo\n");
        printf("6. Informar Grau de um Vértice\n");
        printf("7. Verificar se o Grafo é Conexo\n");
        printf("8. Converter para Matriz de Adjacência\n");
        printf("9. Caminhamento em Amplitude (BFS)\n");
        printf("10. Caminhamento em Profundidade (DFS)\n");
        printf("11. Caminho Mínimo (Dijkstra)\n");
        printf("12. Árvore Geradora Mínima (Prim)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirVertice(grafo);
                printf("Vértice inserido!\n");
                break;
            case 2:
                printf("Digite a origem, destino e peso da aresta: ");
                scanf("%d %d %d", &origem, &destino, &peso);
                inserirAresta(grafo, origem, destino, peso);
                printf("Aresta inserida!\n");
                break;
            case 3:
                printf("Digite o vértice que queira remover: ");
                scanf("%d", &vertice);
                removerVertice(grafo, vertice);
                printf("Vértice removido!\n");
                break;
            case 4:
                printf("Digite a origem e destino da aresta que deseja remover: ");
                scanf("%d %d", &origem, &destino);
                removerAresta(grafo, origem, destino);
                printf("Aresta removida com sucesso!\n");
                break;
            case 5:
                visualizarGrafo(grafo);
                break;
            case 6:
                printf("Digite o vértice que deseja saber o grau: ");
                scanf("%d", &vertice);
                Grau(grafo, vertice);
                break;
            case 7:
                if (verificarConexo(grafo)) {
                    printf("O grafo é conexo.\n");
                } else {
                    printf("O grafo não é conexo.\n");
                }
                break;
            case 8:
                converterMatrizAdjacencia(grafo);
                break;
            case 9:
                printf("Digite o vértice inicial para o caminhamento em amplitude: ");
                scanf("%d", &vertice);
                buscalargura(grafo, vertice);
                break;
            case 10:
                printf("Digite o vértice inicial para busca em profundidade: ");
                scanf("%d", &vertice);
                buscaprofundidade(grafo, vertice);
                break;
            case 11:
                printf("Digite o vértice de origem para Dijkstra: ");
                scanf("%d", &origem);
                dijkstra(grafo, origem);
                break;
            case 12:
                prim(grafo);
                break;
            case 0:
                liberarGrafo(grafo);
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
