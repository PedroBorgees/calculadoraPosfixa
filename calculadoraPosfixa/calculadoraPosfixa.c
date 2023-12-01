#include "calculadoraPosfixa.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

// Inicializa uma pilha vazia
void inicializar(Pilha *pilha) {
    pilha->topo = NULL;
}

// Empilha um novo valor na pilha
void empilhar(Pilha *pilha, double valor) {
    No *novoNo = (No *)malloc(sizeof(No)); // Alocando memória para um novo nó
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    novoNo->dado = valor; // Definindo o valor do novo nó
    novoNo->proximo = pilha->topo; // Colocando o novo nó no topo da pilha
    pilha->topo = novoNo;
}

// Desempilha um valor da pilha
double desempilhar(Pilha *pilha) {
    if (pilha->topo == NULL) {
        printf("Erro: Pilha vazia\n");
        exit(EXIT_FAILURE);
    }
    No *temp = pilha->topo; // Armazenando temporariamente o nó do topo
    double valor = temp->dado; // Capturando o valor do nó
    pilha->topo = temp->proximo; // Removendo o nó do topo da pilha
    free(temp); // Liberando a memória do nó removido
    return valor;
}

// Verificar se um caractere é um operador
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ||
            c == 'r' || c == 'l' || c == 's' || c == 'c' || c == 't');
}

// Avalia uma expressão matemática em notação pós-fixada
double avaliarExpressaoPosfixada(const char *expressao) {
    Pilha pilha;
    inicializar(&pilha);

    for (int i = 0; expressao[i] != '\0'; ++i) {
        if (isdigit(expressao[i]) || expressao[i] == '.') {
            char buffer[64];
            int j = 0;
            while (isdigit(expressao[i]) || expressao[i] == '.') {
                buffer[j++] = expressao[i++];
            }
            buffer[j] = '\0';
            double valor = atof(buffer);
            empilhar(&pilha, valor);
            --i; // Ajuste do índice após processar o número
        } else if (expressao[i] == ' ' || expressao[i] == '\t') {
            continue;
        } else if (isOperator(expressao[i])) {
            double operando1, operando2 = 0;
            double resultado = 0;

            // Tratamento para operadores trigonométricos
            if (expressao[i] == 'c' && strncmp(&expressao[i], "cos", 3) == 0) {
                i += 2; // Avança o índice para passar "cos"
                if (pilha.topo == NULL) {
                    printf("Erro: Pilha vazia antes de 'cos'\n");
                    exit(EXIT_FAILURE);
                }
                operando1 = desempilhar(&pilha);
                resultado = cos(operando1);
            } else if (expressao[i] == 's' && strncmp(&expressao[i], "sen", 3) == 0) {
                i += 2; // Avança o índice para passar "sen"
                if (pilha.topo == NULL) {
                    printf("Erro: Pilha vazia antes de 'sen'\n");
                    exit(EXIT_FAILURE);
                }
                operando1 = desempilhar(&pilha);
                resultado = sin(operando1);
            } else if (expressao[i] == 't' && strncmp(&expressao[i], "tan", 3) == 0) {
                i += 2; // Avança o índice para passar "tan"
                if (pilha.topo == NULL) {
                    printf("Erro: Pilha vazia antes de 'tan'\n");
                    exit(EXIT_FAILURE);
                }
                operando1 = desempilhar(&pilha);
                resultado = tan(operando1);
            } else if (expressao[i] == 'l' && strncmp(&expressao[i], "log", 3) == 0) {
                i += 2; // Avança o índice para passar "log"
                if (pilha.topo == NULL || pilha.topo->proximo == NULL) {
                    printf("Erro: Pilha vazia ou insuficiente para operação 'log'\n");
                    exit(EXIT_FAILURE);
                }
                operando2 = desempilhar(&pilha); // base
                operando1 = desempilhar(&pilha); // número
                if (operando2 <= 0.0 || operando1 <= 0.0 || operando2 == 1.0) {
                    printf("Erro: Base ou número inválido para logaritmo\n");
                    exit(EXIT_FAILURE);
                }
                resultado = log(operando1) / log(operando2); // log base change formula
                empilhar(&pilha, resultado);
            } else {
                // Tratamento para outros operadores
                if (pilha.topo == NULL || pilha.topo->proximo == NULL) {
                    printf("Erro: Pilha vazia ou insuficiente para operação binária\n");
                    exit(EXIT_FAILURE);
                }
                operando2 = desempilhar(&pilha);
                operando1 = desempilhar(&pilha);

                switch (expressao[i]) {
                    case '+': resultado = operando1 + operando2; break;
                    case '-': resultado = operando1 - operando2; break;
                    case '*': resultado = operando1 * operando2; break;
                    case '/':
                        if (operando2 == 0.0) {
                            printf("Erro: Divisão por zero\n");
                            exit(EXIT_FAILURE);
                        }
                        resultado = operando1 / operando2;
                        break;
                    case '^': resultado = pow(operando1, operando2); break;
                    // Adicione mais casos se necessário
                }
            }
            empilhar(&pilha, resultado);
        }
    }

    if (pilha.topo == NULL) {
        printf("Erro: Pilha vazia ao final da operação\n");
        exit(EXIT_FAILURE);
    }

    double finalResult = desempilhar(&pilha);
    return finalResult;
}


void inicializarString(PilhaString *pilha) {
    pilha->topo = NULL;
}

void empilharString(PilhaString *pilha, const char *valor) {
    NoString *novoNo = (NoString *)malloc(sizeof(NoString));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória\n");
        exit(EXIT_FAILURE);
    }
    strcpy(novoNo->dado, valor); // Copia a string
    novoNo->proximo = pilha->topo;
    pilha->topo = novoNo;
}

char* desempilharString(PilhaString *pilha) {
    if (pilha->topo == NULL) {
        printf("Erro: Pilha vazia\n");
        exit(EXIT_FAILURE);
    }
    NoString *temp = pilha->topo;
    char *valor = strdup(temp->dado); // Duplica a string
    pilha->topo = temp->proximo;
    free(temp);
    return valor;
}

// Função para converter expressão pós-fixada para pré-fixada
char* converterParaPreFixada(const char *expressaoPosfixada) {
    PilhaString pilhaSubexpressoes;
    inicializarString(&pilhaSubexpressoes);

    for (int i = 0; expressaoPosfixada[i] != '\0'; ++i) {
        if (isdigit(expressaoPosfixada[i]) || expressaoPosfixada[i] == '.') {
            char buffer[64];
            int j = 0;
            while (isdigit(expressaoPosfixada[i]) || expressaoPosfixada[i] == '.') {
                buffer[j++] = expressaoPosfixada[i++];
            }
            buffer[j] = '\0';
            --i; 

            empilharString(&pilhaSubexpressoes, buffer);
        } else if (expressaoPosfixada[i] == ' ' || expressaoPosfixada[i] == '\t') {
            continue;
        } else if (isOperator(expressaoPosfixada[i])) {
            // Processa operadores trigonométricos como uma unidade
            if (strncmp(&expressaoPosfixada[i], "cos", 3) == 0) {
                i += 2; // Pula os caracteres adicionais de "cos"
                char* operando = desempilharString(&pilhaSubexpressoes);
                char subexpressao[256];
                sprintf(subexpressao, "cos %s", operando);
                empilharString(&pilhaSubexpressoes, subexpressao);
                free(operando);
            } else if (strncmp(&expressaoPosfixada[i], "sen", 3) == 0) {
                i += 2; // Pula os caracteres adicionais de "sen"
                char* operando = desempilharString(&pilhaSubexpressoes);
                char subexpressao[256];
                sprintf(subexpressao, "sen %s", operando);
                empilharString(&pilhaSubexpressoes, subexpressao);
                free(operando);
            } else if (strncmp(&expressaoPosfixada[i], "tan", 3) == 0) {
                i += 2; // Pula os caracteres adicionais de "tan"
                char* operando = desempilharString(&pilhaSubexpressoes);
                char subexpressao[256];
                sprintf(subexpressao, "tan %s", operando);
                empilharString(&pilhaSubexpressoes, subexpressao);
                free(operando);
            } else if (strncmp(&expressaoPosfixada[i], "log", 3) == 0) {
                i += 2; // Pula os caracteres adicionais de "log"
                if (pilhaSubexpressoes.topo == NULL) {
                    printf("Erro: Pilha vazia para operador 'log'\n");
                    exit(EXIT_FAILURE);
                }
                char* operando = desempilharString(&pilhaSubexpressoes);
                char subexpressao[256];
                sprintf(subexpressao, "log %s", operando);
                empilharString(&pilhaSubexpressoes, subexpressao);
                free(operando);
                printf("Subexpressão 'log' empilhada: %s\n", subexpressao);
            } else {
                // Processa outros operadores
                if (pilhaSubexpressoes.topo == NULL || pilhaSubexpressoes.topo->proximo == NULL) {
                    printf("Erro: Não há operandos suficientes na pilha para a operação '%c'\n", expressaoPosfixada[i]);
                    exit(EXIT_FAILURE);
                }
                char* operando2 = desempilharString(&pilhaSubexpressoes);
                char* operando1 = desempilharString(&pilhaSubexpressoes);

                char subexpressao[256];
                sprintf(subexpressao, "%c %s %s", expressaoPosfixada[i], operando1, operando2);
                empilharString(&pilhaSubexpressoes, subexpressao);
                free(operando1);
                free(operando2);
            }
        }
    }

    if (pilhaSubexpressoes.topo == NULL) {
        printf("Erro: Nada para desempilhar ao converter para pré-fixada\n");
        exit(EXIT_FAILURE);
    }

    char* expressaoPrefixada = desempilharString(&pilhaSubexpressoes);
    return expressaoPrefixada;
}

int main() {
    const char *expressoes[] = {
    "3 4 + 5 *", // Teste 1: (3 + 4) * 5
    "7 2 * 4 +", // Teste 2: 7 * 2 + 4
    "8 5 2 4 + * +", // Teste 3: 8 + (5 * (2 + 4))
    "6 2 / 3 + 4 *", // Teste 4: (6 / 2 + 3) * 4
    "9 5 2 8 * 4 + * +", // Teste 5: 9 + (5 * (2 + (8 * 4)))
    "2 3 log 5 /", // Teste 6: log(2 + 3) / 5
    "10 3 log 2 +", // Teste 7: 3^log(10) + 2
    "45 60 + 30 cos *", // Teste 8: (45 + 60) * cos(30)
    "0.5 45 sen 2 ^ +", // Teste 9: 0.5 + sen(45)^2
    "3 4 + 5 tan *" // Teste 10: (3 + 4) * tan(5)
};

    for (size_t i = 0; i < sizeof(expressoes) / sizeof(expressoes[0]); ++i) {
        printf("Expressão pós-fixada: %s\n", expressoes[i]);

        double resultado = avaliarExpressaoPosfixada(expressoes[i]);
        printf("Resultado: %f\n", resultado);

        char* expressaoPrefixada = converterParaPreFixada(expressoes[i]);
        printf("Expressão pré-fixada: %s\n", expressaoPrefixada);
        free(expressaoPrefixada); // Liberando a memória alocada

        printf("\n");
    }

    return 0;
} 