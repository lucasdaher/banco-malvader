#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>

// Definição de valores padrões
#define MAX_PASSWORD_SIZE 4
#define DEFAULT_SIZE 128
#define MAX_CONTAS 100

// Declaração das funções
void enviarMenuFuncionario();
void enviarMenuAberturaConta();
void salvarArquivoContaPoupanca();
void enviarMenuPrincipal();

// Declaração dos registros/structs dos usuários do sistema
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

struct ContaPoupanca contas[MAX_CONTAS];
int totalContas = 0;

// Função que recebe os dados da conta poupança que será criada
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

  printf("A conta foi criada com sucesso!\nVoltando para o menu de funcionarios.\n\n");

  // Salva a conta do usuario em um arquivo
  salvarArquivoContaPoupanca();
  // Envia novamente o menu de funcionarios ao final
  enviarMenuFuncionario();
}

void salvarDadosPoupanca(struct ContaPoupanca *conta)
{
  if (totalContas < MAX_CONTAS)
    contas[totalContas++] = *conta;
  else
    printf("Nao foi possivel adicionar mais contas.");
}

// Função que irá salvar os dados do usuário em um arquivo
void salvarArquivoContaPoupanca()
{
  // Criação e definição da leitura do arquivo
  FILE *file = fopen("contas-poupanca.txt", "w");

  // Verificação para caso o arquivo não seja gerado ou lido
  if (file == NULL)
    printf("Nao foi possivel ler ou gerar o arquivo de contas do tipo poupanca.");

  // Laço para registrar cada dado do usuário no arquivo de forma individual
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

  // Fechando o arquivo ao final da execução
  fclose(file);
}

// Função que remove uma conta utilizando o seu ID exclusivo
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

// Função para buscar os clientes por IDs
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

// Função para mostrar os dados de todas as contas criadas
void mostrarContas()
{
  // Laço de repetição que percorre todas as contas criadas e envia os dados de cada uma
  for (int i = 0; i < totalContas; i++)
  {
    // Solicitando os dados contidos dentro do vetor de contas
    printf("ID: %d\n", contas[i].id);
    printf("Agencia: %d\n", contas[i].agencia);
  }
}

// Realizar a criação da conta poupança.
void criarContaPoupanca()
{
  struct ContaPoupanca conta;
  adicionarContaPoupanca(&conta);
  salvarDadosPoupanca(&conta);
  salvarArquivoContaPoupanca();
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
    printf("\nMenu funcionario:\n");
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