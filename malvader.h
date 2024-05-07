#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>

// Definição de valores padrões
#define MAX_PASSWORD_SIZE 4
#define DEFAULT_SIZE 128
#define MAX_CP_ACCOUNTS 15
#define MAX_CC_ACCOUNTS 15

// Declaração das funções
void visualizarContaPoupanca(struct ContaPoupanca cps[], int numContas, char nomeCliente[]);
void removerContaPoupanca(struct ContaPoupanca cps[], int *numContas);
void cadastrarContaPoupanca(struct ContaPoupanca cps[], int *numContas);
void enviarMenuFuncionario();
void enviarMenuAberturaConta();
void enviarMenuPrincipal();

struct Data
{
  int dia;
  int mes;
  int ano;
};

struct Endereco
{
  char endereco[DEFAULT_SIZE];
  int cep;
  char local[DEFAULT_SIZE];
  int numeroDaCasa;
  char bairro[DEFAULT_SIZE];
  char cidade[DEFAULT_SIZE];
  char estado[DEFAULT_SIZE];
};

struct Funcionario
{
  int codigoFuncionario;
  char cargo[DEFAULT_SIZE];
  char nomeFuncionario[DEFAULT_SIZE];
  int cpf;
  struct Data nascimento;
  char telefoneContato[15];
  char senhaFuncionario[MAX_PASSWORD_SIZE];
  struct Endereco endereco;
};

struct ContaPoupanca
{
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

struct ContaCorrente
{
  int id;
  int agencia;
  int numeroDaConta;
  float limiteDaConta;
  struct Data vencimento;
  char nomeCliente[DEFAULT_SIZE];
  int cpf;
  struct Data nascimento;
  int telefoneContato;
  char endereco[DEFAULT_SIZE];
  int cep;
  char local[DEFAULT_SIZE];
  int numeroCasa;
  char bairro[DEFAULT_SIZE];
  char cidade[DEFAULT_SIZE];
  char estado[DEFAULT_SIZE];
  char senhaDoClienteCc[MAX_PASSWORD_SIZE];
};

struct ContaPoupanca cps[MAX_CP_ACCOUNTS];
int numContas = 0;

// Função que visualiza os dados de uma conta poupança de um cliente
void visualizarContaPoupanca(struct ContaPoupanca cps[], int numContas, char nomeCliente[])
{
  printf("Contas do cliente %s: \n", nomeCliente);
  for (int i = 0; i < numContas; i++)
  {
    if (strcmp(cps[i].nomeCliente, nomeCliente) == 0)
    {
      printf("Conta %d - Saldo: R$0\n", cps[i].id); // adicionar saldo quando tiver o sistema feito
    }
  }
}

// Função que remove uma conta poupança de um cliente
void removerContaPoupanca(struct ContaPoupanca cps[], int *numContas)
{
  if (*numContas == 0)
  {
    printf("Nao existem contas cadastradas. \n");
    return;
  }

  int numeroConta;
  printf("Digite o numero da conta que deseja remover:\n");
  scanf("%d", &numeroConta);

  int indiceConta = -1;
  for (int i = 1; i < *numContas; i++)
  {
    if (cps[i].id == numeroConta)
    {
      indiceConta = i;
      break;
    }
  }

  if (indiceConta == -1)
  {
    printf("Conta com numero %d nao foi encontrada... \n", numeroConta);
    return;
  }

  printf("Conta %d excluida com sucesso.\n", cps[indiceConta].id);
  cps[indiceConta] = cps[(*numContas) - 1];
  (*numContas)--;
}

// Função que efetua o cadastro de uma conta poupança
void cadastrarContaPoupanca(struct ContaPoupanca cps[], int *numContas)
{
  if (*numContas >= MAX_CP_ACCOUNTS)
  {
    printf("Limite de contas atingido.\n");
    return;
  }

  struct ContaPoupanca *cp;
  printf("Digite o numero da conta: \n");
  scanf("%d", &cp->id);

  for (int i = 0; i < *numContas; i++)
  {
    if (cps[i].id == cp->id)
    {
      printf("Ja existe uma conta com este numero.\n");
      return;
    }
  }

  printf("Digite a agencia: \n");
  scanf("%d", &cp->agencia);

  printf("Digite o numero da conta: \n");
  scanf("%d", &cp->numeroDaConta);

  printf("Digite o nome do cliente: \n");
  scanf(" %[^\n]", &cp->nomeCliente);

  printf("Digite o CPF (apenas numeros): \n");
  scanf("%d", &cp->cpf);

  printf("Digite a data de nascimento seguindo o formato (12 07 2002)\n");
  scanf("%d %d %d", &cp->nascimento.dia, &cp->nascimento.mes, &cp->nascimento.ano);

  printf("Digite o telefone para contato (apenas numeros): \n");
  scanf("%d", &cp->telefoneContato);

  printf("Digite o endereco do cliente: \n");
  scanf(" %[^\n]", &cp->endereco);

  printf("Digite o CEP (apenas numeros): \n");
  scanf("%d", &cp->endereco.cep);

  printf("Digite o local: \n");
  scanf(" %[^\n]", &cp->endereco.local);

  printf("Digite o numero da casa: \n");
  scanf("%d", &cp->endereco.numeroDaCasa);

  printf("Digite o bairro: \n");
  scanf(" %[^\n]", &cp->endereco.bairro);

  printf("Digite a cidade: \n");
  scanf(" %[^\n]", &cp->endereco.cidade);

  printf("Digite o estado: \n");
  scanf(" %[^\n]", &cp->endereco.estado);

  printf("Digite uma senha para o cliente (somente numeros): \n");
  scanf(" %[^\n]", &cp->senhaDoClienteCp);

  cps[*numContas] = *cp;
  (*numContas)++;
  printf("A conta foi cadastrada com sucesso.\n\n");
  enviarMenuFuncionario();
}

// Função que envia o menu de abertura de conta
void enviarMenuAberturaConta()
{
  // Variavel que vai armazenar a opcao desejada pelo usuario
  int option;
  do
  {
    printf("\nMenu funcionario:\n\n");
    printf("1) Conta Poupanca - CP\n");
    printf("2) Conta Corrente - CC\n");
    printf("3) Voltar\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
      printf("Iniciando processo de criacao de conta poupanca... \n\n");
      // Envia o processo de criação de conta poupança para o funcionário
      cadastrarContaPoupanca(cps, &numContas);
      break;

    case 2:
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
void enviarMenuFuncionario()
{
  // Variavel que vai receber a opcao desejada pelo usuario
  int option;
  do
  {
    printf("\n\nMenu funcionario:\n");
    printf("1) Abertura de Conta\n");
    printf("2) Encerramento de Conta\n");
    printf("3) Consultar Dados\n");
    printf("4) Alterar Dados\n");
    printf("5) Cadastro de Funcionarios\n");
    printf("6) Gerar Relatorios (Indisponivel)\n");
    printf("7) Sair\n\n");
    scanf("%d", &option);

    switch (option)
    {
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
      printf("Em desenvolvimento...");
      exit(1);
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

// Função para gerar uma senha administrativa ao iniciar o processo de login
void criarSenhaAdminFuncionario()
{
  // Requisitando um arquivo para armazenar as senhas administrativas
  FILE *file = fopen("pass-admin.txt", "w");

  // Verificando se acontecer alguma falha no arquivo
  if (file == NULL)
  {
    printf("Nao foi possivel ler ou criar o arquivo pass-admin.txt!");
    system("pause");
    exit(1);
  }

  // Definindo a variavel que vai receber a senha
  char funcionarioPass[DEFAULT_SIZE];
  // Definindo o conteúdo da variavel e a senha de administrador
  strcpy(funcionarioPass, "administrador1");
  fputs(funcionarioPass, file); // Salvando a STRING que contem a senha do funcionário

  // printf("Definindo a senha de administrador como \n%s\n\n", funcionarioPass);

  // Fechando o arquivo de senhas
  fclose(file);
}

// Função para solicitar a senha do funcionario
// Pendente: verificar se a senha do usuário está armazenada no arquivo de dados dos funcionarios
void solicitarSenhaFuncionario()
{
  // Gerar a senha de administrador caso ela não exista
  criarSenhaAdminFuncionario();

  // Definindo a váriavel de senha que o usuário irá digitar
  char password[DEFAULT_SIZE];

  // String copy: Adiciona a string no conteudo de "senhaFuncionario"
  // strcpy(funcionario.senhaFuncionario, "teste123");

  int acesso = 0;
  do
  {
    // Bloqueando o acesso do funcionário até a inserção da senha
    acesso = 0;

    printf("Digite a sua senha: \n");
    scanf("%s", password); // Não precisa do & por se tratar de uma String

    // Inicializa o arquivo de senhas administrativas no modo read
    FILE *adminFile = fopen("pass-admin.txt", "r");
    char senhaAdmin[DEFAULT_SIZE];

    // Lê a senha do arquivo uma vez.
    fgets(senhaAdmin, DEFAULT_SIZE, adminFile);

    // Remove a quebra de linha do final da senha, se houver
    senhaAdmin[strcspn(senhaAdmin, "\n")] = 0;

    // Faz a verificação se a senha digitada é válida como (Admin || Funcionário)
    if (strcmp(password, senhaAdmin) != 0)
      printf("\nA senha digitada esta incorreta, tente novamente. \n\n");

    // Se a senha digitada for a senha correta, o usuario recebe o menu de funcionários
    if (strcmp(password, senhaAdmin) == 0)
    {
      // Liberando o acesso para o funcionário
      acesso = 1;

      // Chamando a função para enviar o menu de funcionários
      enviarMenuFuncionario();
    }

    // Fecha o arquivo de senhas administrativas
    fclose(adminFile);
  } while (acesso == 0);
}

// Função para enviar o menu principal
void enviarMenuPrincipal()
{
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
  for (int i = 0; i < 18; i++)
  {
    printf("%s\n", malvader[i]);
  }

  // Variavel que vai receber a opção do menu que o usuário selecionar
  int option;
  do
  {
    printf("Bem-vindo(a) ao Malvader Bank!\n\n");
    printf("Escolha uma opcao do menu principal: \n");
    printf("1) Funcionario\n");
    printf("2) Cliente\n");
    printf("3) Sair do Programa\n\n");
    // Armazena a opção escolhida pelo usuario no endereço de memória de option
    scanf("%d", &option);

    switch (option)
    {
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