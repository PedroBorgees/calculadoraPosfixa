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