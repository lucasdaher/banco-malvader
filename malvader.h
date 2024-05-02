#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>

#define PASSWORD_SIZE 20
#define DEFAULT_SIZE 100

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

void solicitarSenhaFuncionario(int type)
{
  struct Funcionario funcionario;
  // Funcionario
  char password[PASSWORD_SIZE];
  if (type == 1)
  {
    do
    {
      printf("Digite a sua senha: \n");
      scanf(" %c", &password);
    } while (password != funcionario.senhaFuncionario);
  }
  // Cliente
  else if (type == 2)
  {
  }
  else
  {
    printf("Usuario invalido, reinicie o programa...");
    exit(1);
  }
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
      solicitarSenhaFuncionario(1);
      break;

    default:
      printf("Opcao invalida, tente novamente.");
    }
  } while (option <= 0 || option > 3);
}
