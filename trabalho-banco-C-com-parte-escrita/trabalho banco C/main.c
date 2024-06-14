#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Conta {
    char nome[100];
    char cpf[12];
    char dataNascimento[11];
    int numeroConta;
    char senha[20];
    float saldo;
    float emprestimo;
    struct Conta *proxima;
} Conta;

Conta *contas = NULL;

void cadastrarConta() {
    Conta *novaConta = (Conta *)malloc(sizeof(Conta));
    if (novaConta == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    printf("Cadastro de Nova Conta\n");
    printf("Nome: ");
    scanf(" %[^\n]", novaConta->nome);
    printf("CPF: ");
    scanf("%s", novaConta->cpf);
    printf("Data de Nascimento (dd/mm/yyyy): ");
    scanf("%s", novaConta->dataNascimento);
    printf("Numero da Conta: ");
    scanf("%d", &novaConta->numeroConta);
    printf("Senha: ");
    scanf("%s", novaConta->senha);
    novaConta->saldo = 0.0;
    novaConta->emprestimo = 0.0;
    novaConta->proxima = contas;
    contas = novaConta;

    printf("Conta cadastrada com sucesso!\n");
}

Conta* login() {
    int numeroConta;
    char senha[20];
    printf("Login\n");
    printf("Numero da Conta: ");
    scanf("%d", &numeroConta);
    printf("Senha: ");
    scanf("%s", senha);

    Conta *conta = contas;
    while (conta != NULL) {
        if (conta->numeroConta == numeroConta && strcmp(conta->senha, senha) == 0) {
            printf("Login bem-sucedido!\n");
            return conta;
        }
        conta = conta->proxima;
    }
    printf("Numero da conta ou senha incorretos.\n");
    return NULL;
}

void depositar(Conta *conta) {
    float valor;
    printf("Valor do deposito: ");
    scanf("%f", &valor);
    if (valor <= 0) {
        printf("Valor invalido.\n");
        return;
    }
    conta->saldo += valor;
    printf("Deposito realizado com sucesso.\n");
}

void sacar(Conta *conta) {
    float valor;
    printf("Valor do saque: ");
    scanf("%f", &valor);
    if (valor > conta->saldo) {
        printf("Saldo insuficiente.\n");
        return;
    }
    conta->saldo -= valor;
    printf("Saque realizado com sucesso.\n");
}

void transferir(Conta *conta) {
    int numeroContaDestino;
    float valor;
    printf("Numero da conta destino: ");
    scanf("%d", &numeroContaDestino);
    printf("Valor da transferencia: ");
    scanf("%f", &valor);

    Conta *contaDestino = contas;
    while (contaDestino != NULL) {
        if (contaDestino->numeroConta == numeroContaDestino) {
            break;
        }
        contaDestino = contaDestino->proxima;
    }

    if (contaDestino == NULL) {
        printf("Conta destino nao encontrada.\n");
        return;
    }

    if (valor > conta->saldo) {
        printf("Saldo insuficiente.\n");
        return;
    }

    conta->saldo -= valor;
    contaDestino->saldo += valor;
    printf("Transferencia realizada com sucesso.\n");
}

void realizarEmprestimo(Conta *conta) {
    float valor;
    printf("Valor do emprestimo: ");
    scanf("%f", &valor);
    float valorComJuros = valor * 1.10;
    conta->emprestimo += valorComJuros;
    conta->saldo += valor;
    printf("Emprestimo realizado com sucesso. Valor a ser pago: %.2f\n", valorComJuros);
}

void pagarEmprestimo(Conta *conta) {
    if (conta->emprestimo == 0) {
        printf("Nao ha emprestimo a pagar.\n");
        return;
    }
    if (conta->saldo >= conta->emprestimo) {
        conta->saldo -= conta->emprestimo;
        conta->emprestimo = 0;
        printf("Emprestimo quitado com sucesso.\n");
    } else {
        printf("Saldo insuficiente para quitar o emprestimo.\n");
    }
}

void consultarSaldo(Conta *conta) {
    printf("Saldo atual: %.2f\n", conta->saldo);
    if (conta->emprestimo > 0) {
        printf("Valor devido em emprestimos: %.2f\n", conta->emprestimo);
    }
}

void fecharConta(Conta *conta) {
    if (conta->emprestimo > 0) {
        printf("Nao e possivel fechar a conta com emprestimos pendentes.\n");
        return;
    }

    Conta **ptr = &contas;
    while (*ptr != NULL) {
        if (*ptr == conta) {
            *ptr = conta->proxima;
            free(conta);
            printf("Conta fechada com sucesso.\n");
            return;
        }
        ptr = &(*ptr)->proxima;
    }
    printf("Conta nao encontrada.\n");
}

void menuEmprestimo(Conta *conta) {
    int opcao;

    while (1) {
        printf("\nMenu de Emprestimo\n");
        printf("1. Realizar emprestimo\n");
        printf("2. Pagar emprestimo\n");
        printf("3. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                realizarEmprestimo(conta);
                break;
            case 2:
                pagarEmprestimo(conta);
                break;
            case 3:
                return;
            default:
                printf("Opcao invalida.\n");
        }
    }
}

void menuAposLogin(Conta *contaLogada) {
    int opcao;

    while (1) {
        printf("\nMenu Login\n");
        printf("1. Consultar saldo\n");
        printf("2. Depositar\n");
        printf("3. Sacar\n");
        printf("4. Transferir\n");
        printf("5. Emprestimo\n");
        printf("6. Fechar conta\n");
        printf("7. Logout\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                consultarSaldo(contaLogada);
                break;
            case 2:
                depositar(contaLogada);
                break;
            case 3:
                sacar(contaLogada);
                break;
            case 4:
                transferir(contaLogada);
                break;
            case 5:
                menuEmprestimo(contaLogada);
                break;
            case 6:
                fecharConta(contaLogada);
                return; // Volta ao menu inicial apos fechar a conta
            case 7:
                return; // Logout, volta ao menu inicial
            default:
                printf("Opcao invalida.\n");
        }
    }
}

void menuInicial() {
    int opcao;
    Conta *contaLogada = NULL;

    while (1) {
        printf("\nMenu Inicial\n");
        printf("1. Cadastrar nova conta\n");
        printf("2. Login em conta existente\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarConta();
                break;
            case 2:
                contaLogada = login();
                if (contaLogada) {
                    menuAposLogin(contaLogada);
                }
                break;
            case 3:
                exit(0);
            default:
                printf("Opcao invalida.\n");
        }
    }
}

int main() {
    menuInicial();
    return 0;
}
