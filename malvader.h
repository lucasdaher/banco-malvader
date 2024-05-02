#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>

#define PASSWORD_SIZE 20
#define DEFAULT_SIZE 128

struct Data
{
  int dia;
  int mes;
  int ano;
};

struct ClienteCp
{
  int agencia;
  int numeroDaConta;
  char nomeCliente[DEFAULT_SIZE];
  struct Data nascimento;
  char telefoneContato[15];
  char endereco[DEFAULT_SIZE];
  char cep[DEFAULT_SIZE];
  char local[DEFAULT_SIZE];
  int numeroDaCasa;
  char bairro[DEFAULT_SIZE];
  char cidade[DEFAULT_SIZE];
  char estado[DEFAULT_SIZE];
  char senhaDoClienteCp[DEFAULT_SIZE];
};

struct ClienteCc
{
  int agencia;
  int numeroDaConta;
  float limiteDaConta;
  struct Data vencimento;
  char nomeCliente[DEFAULT_SIZE];
  char cpf[14];
  struct Data nascimento;
  char telefoneContato[15];
  char endereco[DEFAULT_SIZE];
  char cep[9];
  char local[DEFAULT_SIZE];
  int numeroCasa;
  char bairro[DEFAULT_SIZE];
  char cidade[DEFAULT_SIZE];
  char estado[DEFAULT_SIZE];
  char senhaDoClienteCc[DEFAULT_SIZE];
};

struct Funcionario
{
  int codigoFuncionario;
  char cargo[DEFAULT_SIZE];
  char nomeFuncionario[DEFAULT_SIZE];
  char cpf[14];
  struct Data nascimento;
  char telefoneContato[15];
  char enderecoFuncionario[DEFAULT_SIZE];
  char cep[9];
  char local[DEFAULT_SIZE];
  int numeroDaCasa;
  char bairro[DEFAULT_SIZE];
  char cidade[DEFAULT_SIZE];
  char estado[DEFAULT_SIZE];
  char senhaFuncionario[DEFAULT_SIZE];
};

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