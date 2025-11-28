#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código implementa fila circular de 5 peças e pilha de reserva com 3 peças.

//Constantes

#define TAM_FILA  5
#define TAM_PILHA 3

// --------- Struct ---------

typedef struct {
    char tipo;  // tipo da peça: 'I', 'O', 'T', 'L', etc.
    int  id;    // identificador único
} Peca;

// Protótipos (fila)

void inicializarFila(Peca fila[], int *inicio, int *fim, int *qtd);
int  filaCheia(int qtd);
int  filaVazia(int qtd);
int  enqueue(Peca fila[], int *inicio, int *fim, int *qtd, Peca p);
int  dequeue(Peca fila[], int *inicio, int *fim, int *qtd, Peca *p);
void mostrarFila(Peca fila[], int inicio, int qtd);

// Protótipos (pilha)

void inicializarPilha(Peca pilha[], int *topo);
int  pilhaCheia(int topo);
int  pilhaVazia(int topo);
int  push(Peca pilha[], int *topo, Peca p);
int  pop(Peca pilha[], int *topo, Peca *p);
void mostrarPilha(Peca pilha[], int topo);

// Protótipos gerais

Peca gerarPeca(void);
int  lerOpcao(void);

// Variável global para IDs

static int proximoId = 1;



// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.


int main() {

    Peca fila[TAM_FILA];
    Peca pilha[TAM_PILHA];

    int inicioFila, fimFila, qtdFila;
    int topoPilha;
    int opcao;

    srand((unsigned) time(NULL));

    inicializarFila(fila, &inicioFila, &fimFila, &qtdFila);
    inicializarPilha(pilha, &topoPilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        enqueue(fila, &inicioFila, &fimFila, &qtdFila, nova);
    }

    printf("=== Desafio Tetris Stack - Nivel Mestre ===\n");

    do {
        printf("\n-----------------------------\n");
        printf("Estado atual:\n");
        mostrarFila(fila, inicioFila, qtdFila);
        mostrarPilha(pilha, topoPilha);

        printf("\nMenu:\n");
        printf("1 - Jogar peca (dequeue da fila)\n");
        printf("2 - Reservar peca (fila -> pilha)\n");
        printf("3 - Usar peca reservada (pop da pilha)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");

        opcao = lerOpcao();

        if (opcao == 1) {
            // Jogar peça: remove da frente da fila e insere nova no fim
            Peca jogada;
            if (!dequeue(fila, &inicioFila, &fimFila, &qtdFila, &jogada)) {
                printf("Fila vazia, nao ha peca para jogar.\n");
            } else {
                printf("Jogando peca: tipo %c, id %d\n", jogada.tipo, jogada.id);
                Peca nova = gerarPeca();
                if (!enqueue(fila, &inicioFila, &fimFila, &qtdFila, nova)) {
                    // Em teoria nao deve acontecer, pois sempre mantemos 5
                    printf("Erro ao inserir nova peca na fila.\n");
                }
            }
        } else if (opcao == 2) {
            // Reservar peça: tira da frente da fila e coloca na pilha
            if (pilhaCheia(topoPilha)) {
                printf("Pilha de reserva cheia.\n");
            } else {
                Peca reservada;
                if (!dequeue(fila, &inicioFila, &fimFila, &qtdFila, &reservada)) {
                    printf("Fila vazia, nao ha peca para reservar.\n");
                } else {
                    if (!push(pilha, &topoPilha, reservada)) {
                        printf("Nao foi possivel reservar a peca.\n");
                    } else {
                        printf("Peca reservada: tipo %c, id %d\n", reservada.tipo, reservada.id);
                        // Repor fila com nova peça
                        Peca nova = gerarPeca();
                        enqueue(fila, &inicioFila, &fimFila, &qtdFila, nova);
                    }
                }
            }
        } else if (opcao == 3) {
            // Usar peça reservada: pop da pilha
            Peca usada;
            if (!pop(pilha, &topoPilha, &usada)) {
                printf("Pilha vazia, nao ha peca reservada para usar.\n");
            } else {
                printf("Usando peca reservada: tipo %c, id %d\n", usada.tipo, usada.id);
            }
        } else if (opcao == 4) {
            // Trocar frente da fila com topo da pilha
            if (filaVazia(qtdFila)) {
                printf("Fila vazia, nao ha peca na frente.\n");
            } else if (pilhaVazia(topoPilha)) {
                printf("Pilha vazia, nao ha peca no topo.\n");
            } else {
                int indiceFrente = inicioFila;
                Peca tmp = fila[indiceFrente];
                fila[indiceFrente] = pilha[topoPilha];
                pilha[topoPilha] = tmp;
                printf("Troca realizada entre frente da fila e topo da pilha.\n");
            }
        } else if (opcao == 5) {
            // Trocar os 3 primeiros da fila com as 3 peças da pilha
            if (qtdFila < 3) {
                printf("Fila nao tem 3 pecas para trocar.\n");
            } else if (topoPilha != TAM_PILHA - 1) {
                // topo == 2 significa que tem exatamente 3 elementos na pilha (0,1,2)
                printf("Pilha precisa ter exatamente 3 pecas para essa troca.\n");
            } else {
                for (int i = 0; i < 3; i++) {
                    int indiceFila = (inicioFila + i) % TAM_FILA;
                    Peca tmp = fila[indiceFila];
                    fila[indiceFila] = pilha[i];
                    pilha[i] = tmp;
                }
                printf("Troca realizada entre 3 primeiros da fila e as 3 pecas da pilha.\n");
            }
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    printf("Encerrando o programa.\n");
    return 0;
}

// --------- Implementacao da fila circular ---------

void inicializarFila(Peca fila[], int *inicio, int *fim, int *qtd) {
    (void)fila; // evita warning, fila já é usada nas outras funcoes
    *inicio = 0;
    *fim    = 0;
    *qtd    = 0;
}

int filaCheia(int qtd) {
    return qtd == TAM_FILA;
}

int filaVazia(int qtd) {
    return qtd == 0;
}

int enqueue(Peca fila[], int *inicio, int *fim, int *qtd, Peca p) {
    (void)inicio; // inicio nao e usado diretamente aqui
    if (filaCheia(*qtd)) {
        return 0;
    }
    fila[*fim] = p;
    *fim = (*fim + 1) % TAM_FILA;
    (*qtd)++;
    return 1;
}

int dequeue(Peca fila[], int *inicio, int *fim, int *qtd, Peca *p) {
    (void)fim; // fim nao e usado diretamente aqui
    if (filaVazia(*qtd)) {
        return 0;
    }
    *p = fila[*inicio];
    *inicio = (*inicio + 1) % TAM_FILA;
    (*qtd)--;
    return 1;
}

void mostrarFila(Peca fila[], int inicio, int qtd) {
    printf("\nFila de pecas (frente -> tras):\n");
    if (qtd == 0) {
        printf("[vazia]\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        int idx = (inicio + i) % TAM_FILA;
        printf("[%d] tipo %c, id %d\n", i, fila[idx].tipo, fila[idx].id);
    }
}

// --------- Implementacao da pilha ---------

void inicializarPilha(Peca pilha[], int *topo) {
    (void)pilha;
    *topo = -1;
}

int pilhaCheia(int topo) {
    return topo == TAM_PILHA - 1;
}

int pilhaVazia(int topo) {
    return topo == -1;
}

int push(Peca pilha[], int *topo, Peca p) {
    if (pilhaCheia(*topo)) {
        return 0;
    }
    (*topo)++;
    pilha[*topo] = p;
    return 1;
}

int pop(Peca pilha[], int *topo, Peca *p) {
    if (pilhaVazia(*topo)) {
        return 0;
    }
    *p = pilha[*topo];
    (*topo)--;
    return 1;
}

void mostrarPilha(Peca pilha[], int topo) {
    printf("\nPilha de reserva (topo -> base):\n");
    if (topo == -1) {
        printf("[vazia]\n");
        return;
    }
    for (int i = topo; i >= 0; i--) {
        printf("[%d] tipo %c, id %d\n", i, pilha[i].tipo, pilha[i].id);
    }
}

// --------- Funcoes auxiliares ---------

Peca gerarPeca(void) {
    // Tipos simples de exemplo
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    int indice = rand() % 7;

    Peca p;
    p.tipo = tipos[indice];
    p.id   = proximoId++;
    return p;
}

int lerOpcao(void) {
    int op;
    printf("Escolha uma opcao: ");
    if (scanf("%d", &op) != 1) {
        // Limpar entrada em caso de erro
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        return -1;
    }
    // Limpar resto da linha
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    return op;
}

    // 🧩 Nível Novato: Fila de Peças Futuras
    //
    // - Crie uma struct Peca com os campos: tipo (char) e id (int).
    // - Implemente uma fila circular com capacidade para 5 peças.
    // - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
    // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
    // - Exiba a fila após cada ação com uma função mostrarFila().
    // - Use um menu com opções como:
    //      1 - Jogar peça (remover da frente)
    //      0 - Sair
    // - A cada remoção, insira uma nova peça ao final da fila.



    // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
    //
    // - Implemente uma pilha linear com capacidade para 3 peças.
    // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
    // - Permita enviar uma peça da fila para a pilha (reserva).
    // - Crie um menu com opção:
    //      2 - Enviar peça da fila para a reserva (pilha)
    //      3 - Usar peça da reserva (remover do topo da pilha)
    // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
    // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


    // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
    //
    // - Implemente interações avançadas entre as estruturas:
    //      4 - Trocar a peça da frente da fila com o topo da pilha
    //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
    // - Para a opção 4:
    //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
    //      Troque os elementos diretamente nos arrays.
    // - Para a opção 5:
    //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
    //      Use a lógica de índice circular para acessar os primeiros da fila.
    // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
    // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
    // - O menu deve ficar assim:
    //      4 - Trocar peça da frente com topo da pilha
    //      5 - Trocar 3 primeiros da fila com os 3 da pilha


