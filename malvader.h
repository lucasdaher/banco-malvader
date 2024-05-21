#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>

#define MAX_PASSWORD_SIZE 6
#define DEFAULT_SIZE 128
#define MAX_CP_ACCOUNTS 15
#define MAX_CC_ACCOUNTS 15

// Declaração das funções
void enviarMenuFuncionario();

void enviarMenuAberturaConta();

void enviarMenuPrincipal();

struct Data {
    int dia;
    int mes;
    int ano;
};

struct Endereco {
    char endereco[DEFAULT_SIZE];
    int cep;
    char local[DEFAULT_SIZE];
    int numeroDaCasa;
    char bairro[DEFAULT_SIZE];
    char cidade[DEFAULT_SIZE];
    char estado[DEFAULT_SIZE];
};

struct ContaPoupanca {
    int id;
    int agencia;
    int numeroDaConta;
    char nomeCliente[DEFAULT_SIZE];
    int cpf;
    struct Data nascimento;
    int telefoneContato;
    struct Endereco endereco;
    char senhaDoClienteCp[MAX_PASSWORD_SIZE];
};

struct ContaCorrente {
    int id;
    int agencia;
    int numeroDaConta;
    float limiteDaConta;
    struct Data vencimento;
    char nomeCliente[DEFAULT_SIZE];
    int cpf;
    struct Data nascimento;
    int telefoneContato;
    struct Endereco endereco;
    char senhaDoClienteCc[MAX_PASSWORD_SIZE];
};

struct Funcionario {
    char codigoFuncionario[2];
    char cargo[DEFAULT_SIZE];
    char nomeFuncionario[DEFAULT_SIZE];
    char cpf[14]; // 000.000.000-00
    struct Data nascimento;
    char telefoneContato[15];
    char senhaFuncionario[MAX_PASSWORD_SIZE];
    struct Endereco endereco;
};

int *numContas;

// Função que realiza a validação da senha administrativa.
int validarSenhaAdmin(char *typedPass) {
    // Definindo a senha padrão para administrador.
    const char *passAdmin = "adm";

    // Verificação se a senha digitada está correta.
    if (strcmp(typedPass, passAdmin) != 0) {
        // Se o valor recebido for 1, o usuário não será autenticado por senha incorreta.
        return 1;
    }

    // Se o valor recebido for 0, o usuário será autenticado.
    return 0;
}

// Função que realiza uma busca de funcionários no arquivo de funcionários.
void buscarFuncionario(struct Funcionario *funcionario) {

    // Verifica se existem contas cadastradas.
    if (numContas == 0) {
        printf("Nenhuma conta foi encontrada.");
        return;
    }

    // Abre o arquivo de funcionários no modo de leitura.
    FILE *file = fopen("funcionarios.txt", "r+");

    // Verifica se o arquivo não conseguir ser gerado.
    if (file == NULL) {
        printf("Um erro ocorreu ao tentar abrir o arquivo.\n");
        return;
    }

    // Lendo o código do funcionário.
    fread(&funcionario->codigoFuncionario, sizeof(char), 2, file);
    printf("Codigo do funcionario: %s\n", funcionario->codigoFuncionario);

    // Lendo o nome do funcionário.
    fread(&funcionario->nomeFuncionario, sizeof(char), 50, file);
    printf("Nome do funcionario: %s\n", funcionario->nomeFuncionario);

    // Lendo o CPF do funcionario.
    fread(&funcionario->cpf, sizeof(char), 14, file);
    printf("CPF do funcionario: %s\n", funcionario->cpf);

    // Fechando o arquivo de funcionários.
    fclose(file);
}

void enviarMenuCadastroFuncionario(struct Funcionario *funcionario) {
    // Abrindo o arquivo de funcionários.
    FILE *file = fopen("funcionarios.txt", "a+"); // Tenta abrir o arquivo de funcionários como append.

    // Verificando se um erro ocorreu com o arquivo ao tentar gerar...
    if (file == NULL) {
        printf("Um erro ocorreu ao tentar abrir o arquivo, tentando gerar um novo...\n");
        file = fopen("funcionarios.txt", "w+"); // Caso o arquivo não seja gerado, irá tentar criar um.
    }

    printf("Digite o codigo do funcionario: \n");
    // Realiza a limpeza do buffer do teclado.
    fflush(stdin);
    // Recebe o valor digitado pelo usuário e insere em CPF.
    gets(funcionario->codigoFuncionario);
    // Tenta gravar a informação no arquivo de funcionários.
    fwrite(&funcionario->codigoFuncionario, sizeof(char), 2, file);

    // Caso o arquivo seja lido e ocorra tudo certo.
    printf("Digite o nome do funcionario: \n");
    // Realiza a limpeza do buffer do teclado.
    fflush(stdin);
    // Recebe o valor digitado pelo usuário e insere em NOME.
    gets(funcionario->nomeFuncionario);
    // Tenta gravar a informação no arquivo de funcionários.
    fwrite(&funcionario->nomeFuncionario, sizeof(char), strlen(funcionario->nomeFuncionario), file);

    printf("Digite o CPF do funcionario (000.000.000-00): \n");
    // Realiza a limpeza do buffer do teclado.
    fflush(stdin);
    // Recebe o valor digitado pelo usuário e insere em CPF.
    gets(funcionario->cpf);
    // Tenta gravar a informação no arquivo de funcionários.
    fwrite(&funcionario->cpf, sizeof(char), 14, file);

    // Enviando a mensagem de cadastro concluído com sucesso.
    printf("O cadastro do funcionario foi concluido com sucesso.\n");

    // Fechando o arquivo de funcionários ao final.
    fclose(file);
}

// Função que cadastra um funcionário
void cadastrarFuncionario() {
    // Variável que irá armazenar a senha digitada pelo usuário
    char adminPass[DEFAULT_SIZE];
    printf("Digite a senha de administrador: \n");
    scanf(" %[^\n]", adminPass);

    // Verifica se a senha digitada é inválida
    if (validarSenhaAdmin(adminPass) != 0) {
        printf("A senha digitada esta incorreta, saindo do sistema...\n");
        system("pause"); // Pausando o programa.
        exit(1); // Saindo do programa retornando 1.
    }

    // Verifica se a senha digitada é válida
    if (validarSenhaAdmin(adminPass) == 0) {
        // Solicita a struct Funcionario em uma variavel funcionario
        struct Funcionario funcionario{};
        printf("Autenticado com sucesso!\n");
        // Envia o menu de cadastro para funcionarios.
        enviarMenuCadastroFuncionario(&funcionario);
    }
}

void criarContaCorrente() {
    printf("Em desenvolvimento...");
}

void criarContaPoupanca() {
    printf("Em desenvolvimento...");
}

// Função que envia o menu de abertura de conta
void enviarMenuAberturaConta() {
    // Variavel que vai armazenar a opcao desejada pelo usuario
    int option;
    do {
        printf("\nMenu funcionario:\n\n");
        printf("1) Conta Poupanca - CP\n");
        printf("2) Conta Corrente - CC\n");
        printf("3) Voltar\n");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Iniciando processo de criacao de conta poupanca... \n\n");
                // Envia o processo de criação de conta poupança para o funcionário
                // adicionar função de criação de conta
                criarContaPoupanca();
                break;

            case 2:
                criarContaCorrente();
                break;

            case 3:
                printf("Voltando para o menu do funcionario... \n");
                enviarMenuFuncionario();
                break;

            default:
                printf("Opcao invalida, tente novamente... \n");
        }
        // Executa o código acima enquanto option não for (1,2 ou 3)
    } while (option <= 0 || option > 3);
}

// Função que envia o menu do funcionario
void enviarMenuFuncionario() {
    // Variavel que vai receber a opcao desejada pelo usuario
    int option;
    do {
        printf("\n\nMenu funcionario:\n");
        printf("1) Abertura de Conta\n");
        printf("2) Encerramento de Conta\n");
        printf("3) Consultar Dados\n");
        printf("4) Alterar Dados\n");
        printf("5) Cadastro de Funcionarios\n");
        printf("6) Gerar Relatorios (Indisponivel)\n");
        printf("7) Sair\n\n");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Iniciando processo de abertura de conta...\n");
                // Chamando a função que envia o menu de abertura de conta
                enviarMenuAberturaConta();
                break;

            case 2:
                printf("Em desenvolvimento...");
                exit(1);
                break;

            case 3:
                printf("Em desenvolvimento...");
                exit(1);
                break;

            case 4:
                printf("Em desenvolvimento...");
                exit(1);
                break;

            case 5:
                cadastrarFuncionario();
                break;

            case 6:
                printf("Em desenvolvimento...");
                exit(1);
                break;

            case 7:
                printf("Saindo do programa... \n");
                system("pause");
                exit(1);
                break;

            default:
                printf("Opcao invalida, tente novamente...");
        }

        // Enquanto a opção não for (1,2,3,4,5,6 ou 7) executa o código acima
    } while (option <= 0 || option > 7);
}

// Função para solicitar a senha do funcionario
void solicitarSenhaFuncionario() {
    // Definindo a váriavel de senha que o usuário irá digitar
    char password[DEFAULT_SIZE];

    int acesso = 0;
    do {
        // Bloqueando o acesso do funcionário até a autenticação da senha
        acesso = 0;

        printf("Digite a sua senha: \n");
        scanf("%s", password); // Não precisa do & por se tratar de uma String

        // Faz a verificação se a senha digitada é válida como (Admin || Funcionário)
        if (validarSenhaAdmin(password) != 0)
            printf("\nA senha digitada esta incorreta, tente novamente. \n\n");

        // Se a senha digitada for a senha correta, o usuario recebe o menu de funcionários
        if (validarSenhaAdmin(password) == 0) {
            // Senha autenticada e o acesso será liberado
            acesso = 1;

            // Requisitando a função para enviar o menu de funcionários
            enviarMenuFuncionario();
        }
    } while (acesso == 0);
}

// Função para enviar o menu principal
void enviarMenuPrincipal() {
    char malvader[18][190] = {
            "MMMMMMMM               MMMMMMMM               AAA               LLLLLLLLLLL     VVVVVVVV           VVVVVVVV   AAA               DDDDDDDDDDDDD      EEEEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRR   ",
            "M:::::::M             M:::::::M              A:::A              L:::::::::L     V::::::V           V::::::V  A:::A              D::::::::::::DDD   E::::::::::::::::::::ER::::::::::::::::R  ",
            "M::::::::M           M::::::::M             A:::::A             L:::::::::L     V::::::V           V::::::V A:::::A             D:::::::::::::::DD E::::::::::::::::::::ER::::::RRRRRR:::::R ",
            "M:::::::::M         M:::::::::M            A:::::::A            LL:::::::LL     V::::::V           V::::::VA:::::::A            DDD:::::DDDDD:::::DEE::::::EEEEEEEEE::::ERR:::::R     R:::::R",
            "M::::::::::M       M::::::::::M           A:::::::::A             L:::::L        V:::::V           V:::::VA:::::::::A             D:::::D    D:::::D E:::::E       EEEEEE  R::::R     R:::::R",
            "M:::::::::::M     M:::::::::::M          A:::::A:::::A            L:::::L         V:::::V         V:::::VA:::::A:::::A            D:::::D     D:::::DE:::::E               R::::R     R:::::R",
            "M:::::::M::::M   M::::M:::::::M         A:::::A A:::::A           L:::::L          V:::::V       V:::::VA:::::A A:::::A           D:::::D     D:::::DE::::::EEEEEEEEEE     R::::RRRRRR:::::R ",
            "M::::::M M::::M M::::M M::::::M        A:::::A   A:::::A          L:::::L           V:::::V     V:::::VA:::::A   A:::::A          D:::::D     D:::::DE:::::::::::::::E     R:::::::::::::RR  ",
            "M::::::M  M::::M::::M  M::::::M       A:::::A     A:::::A         L:::::L            V:::::V   V:::::VA:::::A     A:::::A         D:::::D     D:::::DE:::::::::::::::E     R::::RRRRRR:::::R ",
            "M::::::M   M:::::::M   M::::::M      A:::::AAAAAAAAA:::::A        L:::::L             V:::::V V:::::VA:::::AAAAAAAAA:::::A        D:::::D     D:::::DE::::::EEEEEEEEEE     R::::R     R:::::R",
            "M::::::M   M:::::::M   M::::::M      A:::::AAAAAAAAA:::::A        L:::::L             V:::::V V:::::VA:::::AAAAAAAAA:::::A        D:::::D     D:::::DE::::::EEEEEEEEEE     R::::R     R:::::R",
            "M::::::M    M:::::M    M::::::M     A:::::::::::::::::::::A       L:::::L              V:::::V:::::VA:::::::::::::::::::::A       D:::::D     D:::::DE:::::E               R::::R     R:::::R",
            "M::::::M     MMMMM     M::::::M    A:::::AAAAAAAAAAAAA:::::A      L:::::L         LLLLLLV:::::::::VA:::::AAAAAAAAAAAAA:::::A      D:::::D    D:::::D E:::::E       EEEEEE  R::::R     R:::::R",
            "M::::::M               M::::::M   A:::::A             A:::::A   LL:::::::LLLLLLLLL:::::L V:::::::VA:::::A             A:::::A   DDD:::::DDDDD:::::DEE::::::EEEEEEEE:::::ERR:::::R     R:::::R",
            "M::::::M               M::::::M  A:::::A               A:::::A  L::::::::::::::::::::::L  V:::::VA:::::A               A:::::A  D:::::::::::::::DD E::::::::::::::::::::ER::::::R     R:::::R",
            "M::::::M               M::::::M A:::::A                 A:::::A L::::::::::::::::::::::L   V:::VA:::::A                 A:::::A D::::::::::::DDD   E::::::::::::::::::::ER::::::R     R:::::R",
            "MMMMMMMM               MMMMMMMMAAAAAAA                   AAAAAAALLLLLLLLLLLLLLLLLLLLLLLL    VVVAAAAAAA                   AAAAAAADDDDDDDDDDDDD      EEEEEEEEEEEEEEEEEEEEEERRRRRRRR     RRRRRRR",
    };

    printf("\n");
    // Laço de repetição que envia todas as linhas do array acima
    for (int i = 0; i < 18; i++) {
        printf("%s\n", malvader[i]);
    }

    // Variavel que vai receber a opção do menu que o usuário selecionar
    int option;
    do {
        printf("Bem-vindo(a) ao Malvader Bank!\n\n");
        printf("Escolha uma opcao do menu principal: \n");
        printf("1) Funcionario\n");
        printf("2) Cliente\n");
        printf("3) Sair do Programa\n\n");
        // Armazena a opção escolhida pelo usuario no endereço de memória de option
        scanf("%d", &option);

        switch (option) {
            case 1:
                // Chamando a função que solicita a senha para o funcionário ao acessar o menu
                solicitarSenhaFuncionario();
                break;

            case 2:
                // Enviar a autenticação da conta do cliente
                break;

            case 3:
                printf("Saindo do programa...");
                system("pause");
                exit(1);
                break;

                // Caso o usuario digite algo que não seja aceito ou que não exista
            default:
                printf("Opcao invalida, tente novamente.");
        }

        // O bloco de código acima será executado enquanto a opção não for (1, 2 ou 3)
    } while (option <= 0 || option > 3);
}