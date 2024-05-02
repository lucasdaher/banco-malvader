#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>

#define MAX_PASSWORD_SIZE 4
#define DEFAULT_SIZE 128
#define MAX_CONTAS 100

void enviarMenuFuncionario();
void enviarMenuAberturaConta();
void salvarArquivoContaPoupanca();
void enviarMenuPrincipal();

struct Data
{
  int dia;
  int mes;
  int ano;
};

struct Funcionario
{
  int codigoFuncionario;
  char cargo[DEFAULT_SIZE];
  char nomeFuncionario[DEFAULT_SIZE];
  int cpf;
  struct Data nascimento;
  char telefoneContato[15];
  char enderecoFuncionario[DEFAULT_SIZE];
  char cep[9];
  char local[DEFAULT_SIZE];
  int numeroDaCasa;
  char bairro[DEFAULT_SIZE];
  char cidade[DEFAULT_SIZE];
  char estado[DEFAULT_SIZE];
  char senhaFuncionario[MAX_PASSWORD_SIZE];
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
  char endereco[DEFAULT_SIZE];
  int cep;
  char local[DEFAULT_SIZE];
  int numeroDaCasa;
  char bairro[DEFAULT_SIZE];
  char cidade[DEFAULT_SIZE];
  char estado[DEFAULT_SIZE];
  char senhaDoClienteCp[MAX_PASSWORD_SIZE];
};

struct ContaCorrente
{
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

struct ContaPoupanca contas[MAX_CONTAS];
int totalContas = 0;

void adicionarContaPoupanca(struct ContaPoupanca *conta)
{
  // Adicionar um identificador unico para a conta do usuario
  conta->id = totalContas + 1;

  printf("Digite a agencia: \n");
  scanf("%d", &conta->agencia);

  printf("Digite o numero da conta: \n");
  scanf("%d", &conta->numeroDaConta);

  printf("Digite o nome do cliente: \n");
  scanf(" %[^\n]", &conta->nomeCliente);

  printf("Digite o CPF (apenas numeros): \n");
  scanf("%d", &conta->cpf);

  printf("Digite a data de nascimento seguindo o formato > (12 07 2002) '12 de Julho de 2002': \n");
  scanf("%d %d %d", &conta->nascimento.dia, &conta->nascimento.mes, &conta->nascimento.ano);

  printf("Digite o telefone para contato (apenas numeros): \n");
  scanf("%d", &conta->telefoneContato);

  printf("Digite o endereco do cliente: \n");
  scanf(" %[^\n]", &conta->endereco);

  printf("Digite o CEP (apenas numeros): \n");
  scanf("%d", &conta->cep);

  printf("Digite o local: \n");
  scanf(" %[^\n]", &conta->local);

  printf("Digite o numero da casa: \n");
  scanf("%d", &conta->numeroDaCasa);

  printf("Digite o bairro: \n");
  scanf(" %[^\n]", &conta->bairro);

  printf("Digite a cidade: \n");
  scanf(" %[^\n]", &conta->cidade);

  printf("Digite o estado: \n");
  scanf(" %[^\n]", &conta->estado);

  printf("Digite uma senha para o cliente (somente numeros): \n");
  scanf(" %[^\n]", &conta->senhaDoClienteCp);

  printf("\n\nRedirecionando voce de volta para o menu de funcionarios...\n");

  salvarArquivoContaPoupanca();
  enviarMenuFuncionario();
}

void salvarDadosPoupanca(struct ContaPoupanca *conta)
{
  if (totalContas < MAX_CONTAS)
    contas[totalContas++] = *conta;
  else
    printf("Nao foi possivel adicionar mais contas.");
}

void salvarArquivoContaPoupanca()
{
  FILE *file = fopen("contas-poupanca.txt", "w");

  if (file == NULL)
    printf("Nao foi possivel ler ou gerar o arquivo de contas do tipo poupanca.");

  for (int i = 0; i < totalContas; i++)
  {
    fprintf(file, "ID: %d\n", contas[i].id);
    fprintf(file, "Agencia: %d\n", contas[i].agencia);
    fprintf(file, "Numero da conta: %d\n", contas[i].numeroDaConta);
    fprintf(file, "Nome Cliente: %s\n", contas[i].nomeCliente);
    fprintf(file, "CPF: %d\n", contas[i].cpf);
    fprintf(file, "Data de nascimento: %d/%d/%d\n", contas[i].nascimento.dia, contas[i].nascimento.mes, contas[i].nascimento.ano);
    fprintf(file, "Telefone para contato: %d\n", contas[i].telefoneContato);
    fprintf(file, "Endereco: %s\n", contas[i].endereco);
    fprintf(file, "CEP: %d\n", contas[i].cep);
    fprintf(file, "Local: %s\n", contas[i].local);
    fprintf(file, "Numero da Casa: %d\n", contas[i].numeroDaCasa);
    fprintf(file, "Bairro: %s\n", contas[i].bairro);
    fprintf(file, "Cidade: %s\n", contas[i].cidade);
    fprintf(file, "Estado: %s\n", contas[i].estado);
    fprintf(file, "Senha do Cliente: %s\n", contas[i].senhaDoClienteCp);
  }

  fclose(file);
}

void removerContaPorId(int id)
{
  struct ContaPoupanca contasTemp[MAX_CONTAS];
  int totalContasTemp = 0;

  // Copiar todas as contas, exceto a conta com o ID especificado, para contasTemp
  for (int i = 0; i < totalContas; i++)
  {
    if (contas[i].id != id)
    {
      contasTemp[totalContasTemp++] = contas[i];
    }
  }

  // Se o número de contas não mudou, a conta com o ID especificado não foi encontrada
  if (totalContasTemp == totalContas)
  {
    printf("Conta com ID %d nao encontrada.\n", id);
    return;
  }

  // Copiar contasTemp de volta para contas
  memcpy(contas, contasTemp, totalContasTemp * sizeof(struct ContaPoupanca));
  totalContas = totalContasTemp;

  // Salvar as contas restantes de volta no arquivo
  salvarArquivoContaPoupanca();
}

void buscarClientePorId(int id)
{
  for (int i = 0; i < totalContas; i++)
  {
    if (contas[i].id == id)
    {
      printf("Nome do cliente: %s\n", contas[i].nomeCliente);
      return;
    }
  }

  printf("Cliente com ID %d nao encontrado.\n", id);
}

void mostrarContas()
{
  for (int i = 0; i < totalContas; i++)
  {
    printf("ID: %d\n", contas[i].id);
    printf("Agencia: %d\n", contas[i].agencia);
  }
}

void criarContaPoupanca()
{
  struct ContaPoupanca conta;
  adicionarContaPoupanca(&conta);
  salvarDadosPoupanca(&conta);
  salvarArquivoContaPoupanca();
}

void enviarMenuAberturaConta()
{
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
      criarContaPoupanca();
      break;

    case 2:
      break;

    case 3:
      printf("Voltando para o menu do funcionario... \n");
      system("pause");
      enviarMenuFuncionario();
      break;

    default:
      printf("Opcao invalida, tente novamente... \n");
    }
  } while (option <= 0 || option > 3);
}

void enviarMenuFuncionario()
{
  int option;
  do
  {
    printf("\nMenu funcionario:\n\n");
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
      printf("Iniciando processo de abertura de conta...");
      // Enviar a mensagem com o menu de abertura de conta.
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
      exit(1);
      break;

    default:
      printf("Opcao invalida, tente novamente...");
    }

  } while (option <= 0 || option > 7);
}

void solicitarSenhaFuncionario()
{
  struct Funcionario funcionario;
  // Funcionario
  char password[DEFAULT_SIZE];
  strcpy(funcionario.senhaFuncionario, "teste123");
  do
  {
    printf("Digite a sua senha: \n");
    scanf("%s", password);

    if (strcmp(password, funcionario.senhaFuncionario) != 0) // verifica se as duas strings são iguais
      printf("\nA senha digitada esta incorreta, tente novamente. \n\n");

    if (strcmp(password, funcionario.senhaFuncionario) == 0)
    {
      enviarMenuFuncionario();
    }

  } while (strcmp(password, funcionario.senhaFuncionario) != 0); // strcmp = string compare
}

void enviarMenuPrincipal()
{
  int option;
  do
  {
    printf("Bem-vindo ao Banco Malvader!\n\n");
    printf("Escolha uma opcao do menu principal: \n\n");
    printf("1) Funcionario\n");
    printf("2) Cliente\n");
    printf("3) Sair do Programa\n\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
      solicitarSenhaFuncionario();
      break;

    default:
      printf("Opcao invalida, tente novamente.");
    }
  } while (option <= 0 || option > 3);
}