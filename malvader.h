#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#define MAX_PASSWORD_SIZE 6
#define DEFAULT_SIZE 128

struct Data
{
  char dia[2];
  char mes[2];
  char ano[4];
};

struct Endereco
{
  char endereco[45]; // Maximo de 45 caracteres
  char cep[9];
  char bairro[20];
  char cidade[20];
  char estado[4]; // O usuário deve informar em SIGLA
};

struct Funcionario
{
  char nomeFuncionario[25];
  char cpf[14];
  char codigoFuncionario[3];
  char cargo[25];
  struct Data nascimento;
  char telefoneContato[15];
  char senhaFuncionario[16];
  struct Endereco endereco;
  char excluido;
};

void enviarMenuCliente();
void limparMensagens(int quantidadeDeLinhas);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função para validar a senha do cliente.
int validarSenhaCliente(char *senhaDigitada)
{
  // Abre o arquivo no modo de leitura.
  FILE *file = fopen("arquivo.txt", "r");

  // Verifica se o arquivo foi aberto corretamente.
  if (file == NULL)
  {
    printf("Erro na abertura do arquivo.");
    // Tenta criar o arquivo de clientes caso ele não exista.
    file = fopen("arquivo.txt", "w");
  }

  // Pede pro usuário inserir a senha.
  printf("Digite a sua senha: \n");
  fflush(stdin); // Limpa o buffer do techado
  // Recebe a string que foi digitada
  gets(senhaDigitada);

  system("cls");

  // Declarando a variável que receberá a senha do cliente.
  char passCliente[100];

  // Busca a senha do cliente no arquivo que foi aberto anteriormente.
  fread(passCliente, sizeof(char), strlen(passCliente), file);

  // Verifica se a senha está correta.
  // if (strcmp(senhaDigitada, passCliente) != 0) // Compara duas strings

  // Remover essa linha e voltar a de cima, estou apenas testando...
  if (strcmp(senhaDigitada, "cliente123") != 0) // Compara duas strings
  {
    printf("\nSenha incorreta, saindo do programa...\n");
    system("pause");
    return 1;
  }

  // Mensagem de que a senha foi autenticada.
  printf("Voce foi autenticado com sucesso.\n");

  // Fechando o arquivo de clientes.
  fclose(file);

  return 0;
}

// Função que mostra o saldo do cliente.
void visualizarSaldo()
{
  // Adicionar a leitura do saldo do cliente em um arquivo
  // e adicionar o conteúdo do saldo na variável saldo.

  float saldo = 0;
  char senhaDigitada;

  // Autenticação da senha do cliente.
  validarSenhaCliente(&senhaDigitada);

  int contador;
  system("cls");
  for (int i = 0; i <= 10; i++)
  {
    printf("\nInformacoes do seu saldo em conta:\n", saldo);
    printf("\nSaldo da conta: R$%.2f\n\n", saldo);
    contador = 10 - i;
    if (contador == 1)
      printf("(!) Voce sera redirecionado(a) ao menu de clientes em %d segundo...\n\n\n\n", contador);
    else if (contador == 0)
      printf("(!) Voltando ao menu de clientes...\n");
    else
      printf("(!) Voce sera redirecionado(a) ao menu de clientes em %d segundos...\n\n\n\n", contador);

    sleep(1);
    system("cls");
  }

  enviarMenuCliente();

  // Envia o saldo para o cliente.

  // sleep(10); // Aguarda 10 segundos para retornar ao menu de clientes
}

// Função para realizar depósito.
void depositar(float *saldo, float valor)
{
  char senhaDigitada;
  validarSenhaCliente(&senhaDigitada);

  printf("\nDigite o valor a ser depositado: R$ ");
  scanf("%f", &valor);
  *saldo += valor;

  int contador;
  system("cls");
  for (int i = 0; i <= 10; i++)
  {
    printf("\n\nDesposito de R$%.2f realizado com sucesso! \n\nSeu novo saldo: R$%.2f\n\n", valor, *saldo);
    contador = 10 - i;
    if (contador == 1)
      printf("(!) Voce sera redirecionado(a) de volta ao menu em %d segundo...\n\n\n\n", contador);
    else if (contador == 0)
      printf("(!) Voltando para o menu de clientes...\n");
    else
      printf("(!) Voce sera redirecionado(a) de volta ao menu em %d segundos...\n\n\n\n", contador);

    sleep(1);
    system("cls");
  }
}

// Função para realizar saques.
void sacar(float *saldo, float valor)
{
  char senhaDigitada;
  validarSenhaCliente(&senhaDigitada);

  // Verifica se o saldo é maior que o valor digitado
  if (*saldo >= valor)
  {
    // Retira o valor do saldo do usuário
    *saldo -= valor;
    printf("\nSaque de %.2f realizado com sucesso! \nNovo saldo: R$ %.2f\n\n", valor, *saldo);
  }
  else
  {
    printf("\nSaldo insuficiente. \nNao foi possivel sacar.");
  }
}

void enviarMenuCliente()
{
  float saldo = 1000.0;
  float valor;

  int opcao;
  do
  {
    printf("Menu Cliente: \n");
    printf("\n1) Saldo\n");
    printf("2) Deposito\n");
    printf("3) Saque\n");
    printf("4) Extrato\n");
    printf("5) Consultar limite\n");
    printf("6) Sair\n\n");
    scanf("%d", &opcao);
    system("cls");

    switch (opcao)
    {
    case 1:
      visualizarSaldo();
      break;
    case 2:
      depositar(&saldo, valor);
      break;
    case 3:
      printf("\nDigite o valor a ser sacado: R$ ");
      scanf("%f");
      sacar(&saldo, valor);
    default:
      printf("\nVoce selecionou uma opcao invalida, tente outra...");
      break;
    }

  } while (opcao <= 0 || opcao > 6);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void limparMensagens(int quantidadeDeLinhas)
{
  for (int i = 0; i < quantidadeDeLinhas; i++)
  {
    printf("\n");
  }
}

int consultarFuncionario(FILE *file, Funcionario funcionario);

int inserirFuncionario(FILE *file, Funcionario funcionario);

int validarSenhaAdmin(char *senhaDigitada);

int alterarFuncionario(FILE *file, Funcionario funcionario_antigo, Funcionario funcionario_novo);

void enviarMenuFuncionario();

void solicitarSenhaFuncionario(int tipoDeMenu);

void enviarMenuPrincipal();

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
  struct Endereco endereco;
  char senhaDoClienteCc[MAX_PASSWORD_SIZE];
};

// Função que realiza a consulta nos arquivos sobre um funcionário
int consultarFuncionario(FILE *file, Funcionario funcionario)
{
  Funcionario funcionario_lido;
  int posicao;

  // Caso o arquivo tenha sido aberto e lido com sucesso
  if (file != NULL)
  {
    // Define o ponteiro de busca para o ínicio do código
    fseek(file, 0L, SEEK_SET);

    // Define a posição inicial para 0
    posicao = 0;

    while (fread(&funcionario_lido, sizeof(funcionario_lido), 1, file))
    {
      if (strcmpi(funcionario_lido.nomeFuncionario, funcionario.nomeFuncionario) == 0 &&
          (funcionario_lido.excluido == 0))
        return posicao;
      posicao++;
    };
  }
  return -1;
}

// Função que insere os dados do funcionário em um arquivo
int inserirFuncionario(FILE *file, Funcionario funcionario)
{
  Funcionario funcionario_lido;
  int posicao;

  if (file != NULL)
  {
    posicao = 0;

    // Procurar se a estrutura do funcionário existe no arquivo.
    if (consultarFuncionario(file, funcionario))
    {
      // Definindo o ponteiro de busca no início do arquivo
      fseek(file, 0L, SEEK_SET);

      // Vai rodar enquanto não chegar ao fim do arquivo
      while (fread(&funcionario_lido, sizeof(funcionario_lido), 1, file))
      {
        if (funcionario_lido.excluido == 1)
          break;
        posicao++;
      };

      fseek(file, posicao * sizeof(funcionario), SEEK_SET);
      funcionario.excluido = 0;

      if (fwrite(&funcionario, sizeof(funcionario), 1, file))
      {
        printf("O funcionario foi cadastrado com sucesso.\n\n");
        getch();
        return 1;
      }
    }
  }
  return 0;
}

// Adicionar validação pela senha do funcionário contida no arquivo
// Função que realiza a validação da senha administrativa.
int validarSenhaAdmin(char *senhaDigitada)
{
  // Definindo a senha padrão para administrador.
  const char *passAdmin = "adm";

  // Verificação se a senha digitada está correta.
  if (strcmp(senhaDigitada, passAdmin) != 0)
  {
    // Se o valor recebido for 1, o usuário não será autenticado por senha incorreta.
    return 1;
  }

  // Se o valor recebido for 0, o usuário será autenticado.
  return 0;
}

// Função que altera os dados de um funcionário
int alterarFuncionario(FILE *file, Funcionario funcionario_antigo, Funcionario funcionario_novo)
{
  int posicao;

  // Verifica se o arquivo conseguiu ser aberto com sucesso
  if (file != NULL)
  {
    posicao = consultarFuncionario(file, funcionario_antigo);
    if (posicao != -1)
    {
      fseek(file, posicao * sizeof(Funcionario), SEEK_SET);
      fread(&funcionario_antigo, sizeof(funcionario_antigo), 1, file);

      ///////////////////////////////////////////////////////////////////////
      // Adicionar todos os dados que serão atualizados nas linhas abaixo: //
      ///////////////////////////////////////////////////////////////////////

      // Copia os dados contidos no registro antigo e envia para o novo
      strcpy(funcionario_antigo.nomeFuncionario, funcionario_novo.nomeFuncionario);
      strcpy(funcionario_antigo.cpf, funcionario_novo.cpf);

      fseek(file, posicao * sizeof(Funcionario), SEEK_SET);
      fwrite(&funcionario_antigo, sizeof(funcionario_novo), 1, file);

      printf("Os dados deste funcionario foram alterados com sucesso.\n");
      return 1;
    }
  }
  return 0;
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
      // adicionar função de criação de conta
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
  // Declaração do arquivo
  FILE *file;
  // Variavel que vai receber a opcao desejada pelo usuario
  Funcionario funcionario, funcionario_alterado;

  // Variavel da opção que será digitada pelo usuário
  int option;

  // Tenta abrir o arquivo no modo de leitura e escrita
  file = fopen("funcionarios.txt", "r+");

  // Caso o arquivo não seja encontrado ou aberto por algum motivo
  // O programa tentará criar um arquivo novo.
  if (file == NULL)
  {
    printf("Arquivo não encontrado, recriando o arquivo...");
    file = fopen("funcionarios.txt", "w+");
  }

  // Caso o arquivo exista e seja aberto com sucesso
  if (file != NULL)
  {
    do
    {
      // Envia o menu de funcionários
      printf("\n\nMenu funcionario:\n");
      printf("1) Abertura de Conta\n");
      printf("2) Encerramento de Conta\n");
      printf("3) Consultar Dados\n");
      printf("4) Alterar Dados\n");
      printf("5) Cadastro de Funcionarios\n");
      printf("6) Gerar Relatorios (Indisponivel)\n");
      printf("7) Sair\n\n");
      scanf("%d", &option);

      // Limpa todos os streams de entrada que estiverem abertos.
      fflush(file);

      // Verificação e ação para cada valor digitado pelo usuário ao escolher a opção do menu
      switch (option)
      {
      case 1:
        printf("Iniciando processo de abertura de conta...\n");

        // Chamando a função que envia o menu de abertura de conta
        enviarMenuAberturaConta();
        break;

      case 2:
        printf("Em desenvolvimento...");
        system("pause");
        break;

      case 3:
        int posicao;
        fflush(stdin);
        printf("Digite o nome do funcionario desejado: \n");
        gets(funcionario.nomeFuncionario);

        if ((posicao = consultarFuncionario(file, funcionario)) != -1)
        {
          fseek(file, posicao * sizeof(funcionario), SEEK_SET);
          fread(&funcionario, sizeof(funcionario), 1, file);

          printf("\nMostrando informacoes do(a) funcionario(a) %s:\n\n", funcionario.nomeFuncionario);
          printf("---------------------------------------------\n");
          printf("Nome: %s\n", funcionario.nomeFuncionario);
          printf("Codigo: %s\n", funcionario.codigoFuncionario);
          printf("Cargo: %s\n", funcionario.cargo);
          printf("CPF: %s\n", funcionario.cpf);
          printf("Data de Nascimento: %s/%s/%s\n", funcionario.nascimento.dia, funcionario.nascimento.mes,
                 funcionario.nascimento.ano);
          printf("Telefone: %s\n", funcionario.telefoneContato);
          printf("Endereco: %s\n", funcionario.endereco.endereco);
          printf("CEP: %s\n", funcionario.endereco.cep);
          printf("Bairro: %s\n", funcionario.endereco.bairro);
          printf("Cidade: %s\n", funcionario.endereco.cidade);
          printf("Estado: %s\n", funcionario.endereco.estado);
          printf("Senha: %s\n", funcionario.senhaFuncionario);
          printf("---------------------------------------------\n");

          printf("\n(!) Voce sera redirecionado ao menu de funcionario em alguns segundos (15s)...\n");

          // Aguardar 15 segundos para executar as próximas linhas de código
          sleep(15);

          // Requisita a função que realiza a limpeza das mensagens
          limparMensagens(100);

          // Envia o menu de funcionario novamente para o usuario
          enviarMenuFuncionario();
        }

        getch();
        break;

        // Alterar dados
      case 4:
        do
        {
          printf("\nAlterar dados: \n\n");
          printf("1) Alterar Conta\n");
          printf("2) Alterar Funcionario\n");
          printf("3) Alterar Cliente\n");
          printf("4) Voltar\n\n");
          scanf("%d", &option);

          fflush(stdin); // Limpa o buffer do teclado

          switch (option)
          {
          case 1:
            // solicitarSenhaFuncionario(faltando parametros);
            break;

          case 2:
            char password[DEFAULT_SIZE];

            int acesso = 0;
            do
            {
              acesso = 0;

              printf("Digite a senha de administrador: \n");
              scanf("%s", password);

              // Faz a verificação se a senha digitada é válida como (Admin || Funcionário)
              if (validarSenhaAdmin(password) != 0)
                printf("\nA senha digitada esta incorreta, tente novamente. \n\n");

              // Se a senha digitada for a senha correta, o usuario recebe o menu de funcionários
              if (validarSenhaAdmin(password) == 0)
              {
                // Senha autenticada e o acesso será liberado
                acesso = 1;

                printf("Digite o nome do funcionario que tera os dados alterados: \n");
                fflush(stdin);
                gets(funcionario.nomeFuncionario);

                printf("Digite o novo nome do funcionario: \n");
                fflush(stdin);
                gets(funcionario_alterado.nomeFuncionario);

                printf("Digite o novo CPF do funcionario: \n");
                fflush(stdin);
                gets(funcionario_alterado.cpf);

                alterarFuncionario(file, funcionario, funcionario_alterado);
              }
            } while (acesso == 0);
            break;
          }
        } while (option <= 0 || option > 4);
        break;

      case 5:
        printf("Digite o nome do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.nomeFuncionario);

        printf("Digite o codigo do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.codigoFuncionario);

        printf("Digite o cargo do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.cargo);

        printf("Digite o CPF do funcionario no formato (000.000.000-00): \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.cpf);

        printf("Digite o dia da data de nascimento do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.nascimento.dia);

        printf("Digite o mes da data de nascimento do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.nascimento.mes);

        printf("Digite o ano da data de nascimento do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.nascimento.ano);

        printf("Digite o telefone para contato: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.telefoneContato);

        printf("Digite o endereco do funcionario (Maximo de 45 caracteres): \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.endereco.endereco);

        printf("Digite o CEP do funcionario no formato (00000-000): \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.endereco.cep);

        printf("Digite o bairro do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.endereco.bairro);

        printf("Digite a cidade do funcionario: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.endereco.cidade);

        printf("Digite o estado do funcionario (Coloque em sigla: ex: DF): \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.endereco.estado);

        printf("Digite a senha do funcionario (Maximo de 16 caracteres): \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.senhaFuncionario);

        // Requisita a função que insere os dados digitados no arquivo de funcionários
        inserirFuncionario(file, funcionario);
        break;

      case 6:
        printf("Em desenvolvimento...");
        system("pause");
        break;

      case 7:
        printf("Saindo do programa... \n");
        system("pause");
        break;

      default:
        printf("Opcao invalida, tente novamente...");
      }

      // Enquanto a opção não for (1,2,3,4,5,6 ou 7) executa o código acima
    } while (option <= 0 || option > 7);
    fclose(file);
  }
}

void solicitarSenhaCliente()
{
  char password;
  int acesso = 0;

  do
  {
    acesso = 0;

    if (validarSenhaCliente(&password) == 1)
    {
      return;
    }

    acesso = 1;
    enviarMenuCliente();

  } while (acesso == 0);
}

// Função para solicitar a senha do funcionario
void solicitarSenhaFuncionario(int tipoDeMenu)
{
  // Definindo a váriavel de senha que o usuário irá digitar
  char password[DEFAULT_SIZE];

  int acesso = 0;
  do
  {
    // Bloqueando o acesso do funcionário até a autenticação da senha
    acesso = 0;

    printf("Digite a sua senha: \n");
    scanf("%s", password); // Não precisa do & por se tratar de uma String

    // Faz a verificação se a senha digitada é válida como (Admin || Funcionário)
    if (validarSenhaAdmin(password) != 0)
    {
      printf("\nA senha digitada esta incorreta, tente novamente. \n\n");
      system("cls");
    }

    // Se a senha digitada for a senha correta, o usuario recebe o menu de funcionários
    if (validarSenhaAdmin(password) == 0)
    {
      // Senha autenticada e o acesso será liberado
      acesso = 1;

      if (tipoDeMenu == 1)
      {
        // Requisitando a função para enviar o menu de funcionários
        enviarMenuFuncionario();
      }
      else if (tipoDeMenu == 2)
      {
        enviarMenuAberturaConta(); // Envia o menu de abertura de conta
      }
    }
  } while (acesso == 0);
}

// Função para enviar o menu principal
void enviarMenuPrincipal()
{
  char title[7][91] = {
      "::::    ::::      :::     :::    :::     :::     :::     :::::::::  :::::::::: :::::::::  ",
      "+:+:+: :+:+:+   :+: :+:   :+:    :+:     :+:   :+: :+:   :+:    :+: :+:        :+:    :+: ",
      "+:+ +:+:+ +:+  +:+   +:+  +:+    +:+     +:+  +:+   +:+  +:+    +:+ +:+        +:+    +:+ ",
      "+#+  +:+  +#+ +#++:++#++: +#+    +#+     +:+ +#++:++#++: +#+    +:+ +#++:++#   +#++:++#:  ",
      "+#+       +#+ +#+     +#+ +#+     +#+   +#+  +#+     +#+ +#+    +#+ +#+        +#+    +#+ ",
      "#+#       #+# #+#     #+# #+#      #+#+#+#   #+#     #+# #+#    #+# #+#        #+#    #+# ",
      "###       ### ###     ### ########## ###     ###     ### #########  ########## ###    ### "};

  char subtitle[7][91] = {
      "                     :::::::::      :::     ::::    ::: :::    :::                        ",
      "                     :+:    :+:   :+: :+:   :+:+:   :+: :+:   :+:                         ",
      "                     +:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+                          ",
      "                     +#++:++#+  +#++:++#++: +#+ +:+ +#+ +#++:++                           ",
      "                     +#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+                          ",
      "                     #+#    #+# #+#     #+# #+#   #+#+# #+#   #+#                         ",
      "                     #########  ###     ### ###    #### ###    ###                        "};

  // char malvader[18][190] = {
  //     "MMMMMMMM               MMMMMMMM               AAA               LLLLLLLLLLL     VVVVVVVV           VVVVVVVV   AAA               DDDDDDDDDDDDD      EEEEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRR   ",
  //     "M:::::::M             M:::::::M              A:::A              L:::::::::L     V::::::V           V::::::V  A:::A              D::::::::::::DDD   E::::::::::::::::::::ER::::::::::::::::R  ",
  //     "M::::::::M           M::::::::M             A:::::A             L:::::::::L     V::::::V           V::::::V A:::::A             D:::::::::::::::DD E::::::::::::::::::::ER::::::RRRRRR:::::R ",
  //     "M:::::::::M         M:::::::::M            A:::::::A            LL:::::::LL     V::::::V           V::::::VA:::::::A            DDD:::::DDDDD:::::DEE::::::EEEEEEEEE::::ERR:::::R     R:::::R",
  //     "M::::::::::M       M::::::::::M           A:::::::::A             L:::::L        V:::::V           V:::::VA:::::::::A             D:::::D    D:::::D E:::::E       EEEEEE  R::::R     R:::::R",
  //     "M:::::::::::M     M:::::::::::M          A:::::A:::::A            L:::::L         V:::::V         V:::::VA:::::A:::::A            D:::::D     D:::::DE:::::E               R::::R     R:::::R",
  //     "M:::::::M::::M   M::::M:::::::M         A:::::A A:::::A           L:::::L          V:::::V       V:::::VA:::::A A:::::A           D:::::D     D:::::DE::::::EEEEEEEEEE     R::::RRRRRR:::::R ",
  //     "M::::::M M::::M M::::M M::::::M        A:::::A   A:::::A          L:::::L           V:::::V     V:::::VA:::::A   A:::::A          D:::::D     D:::::DE:::::::::::::::E     R:::::::::::::RR  ",
  //     "M::::::M  M::::M::::M  M::::::M       A:::::A     A:::::A         L:::::L            V:::::V   V:::::VA:::::A     A:::::A         D:::::D     D:::::DE:::::::::::::::E     R::::RRRRRR:::::R ",
  //     "M::::::M   M:::::::M   M::::::M      A:::::AAAAAAAAA:::::A        L:::::L             V:::::V V:::::VA:::::AAAAAAAAA:::::A        D:::::D     D:::::DE::::::EEEEEEEEEE     R::::R     R:::::R",
  //     "M::::::M   M:::::::M   M::::::M      A:::::AAAAAAAAA:::::A        L:::::L             V:::::V V:::::VA:::::AAAAAAAAA:::::A        D:::::D     D:::::DE::::::EEEEEEEEEE     R::::R     R:::::R",
  //     "M::::::M    M:::::M    M::::::M     A:::::::::::::::::::::A       L:::::L              V:::::V:::::VA:::::::::::::::::::::A       D:::::D     D:::::DE:::::E               R::::R     R:::::R",
  //     "M::::::M     MMMMM     M::::::M    A:::::AAAAAAAAAAAAA:::::A      L:::::L         LLLLLLV:::::::::VA:::::AAAAAAAAAAAAA:::::A      D:::::D    D:::::D E:::::E       EEEEEE  R::::R     R:::::R",
  //     "M::::::M               M::::::M   A:::::A             A:::::A   LL:::::::LLLLLLLLL:::::L V:::::::VA:::::A             A:::::A   DDD:::::DDDDD:::::DEE::::::EEEEEEEE:::::ERR:::::R     R:::::R",
  //     "M::::::M               M::::::M  A:::::A               A:::::A  L::::::::::::::::::::::L  V:::::VA:::::A               A:::::A  D:::::::::::::::DD E::::::::::::::::::::ER::::::R     R:::::R",
  //     "M::::::M               M::::::M A:::::A                 A:::::A L::::::::::::::::::::::L   V:::VA:::::A                 A:::::A D::::::::::::DDD   E::::::::::::::::::::ER::::::R     R:::::R",
  //     "MMMMMMMM               MMMMMMMMAAAAAAA                   AAAAAAALLLLLLLLLLLLLLLLLLLLLLLL    VVVAAAAAAA                   AAAAAAADDDDDDDDDDDDD      EEEEEEEEEEEEEEEEEEEEEERRRRRRRR     RRRRRRR",
  // };

  printf("------------------------------------------------------------------------------------------\n\n");
  printf("\n");
  // Laço de repetição que envia todas as linhas do array acima
  for (int i = 0; i < 7; i++)
  {
    printf("%s\n", title[i]);
  }
  printf("\n");
  for (int i = 0; i < 7; i++)
  {
    printf("%s\n", subtitle[i]);
  }
  printf("\n");
  printf("------------------------------------------------------------------------------------------\n\n");

  // Variavel que vai receber a opção do menu que o usuário selecionar
  int option;
  do
  {
    // printf("Bem-vindo(a) ao Malvader Bank!\n\n");
    printf("Escolha uma opcao do menu principal: \n");
    printf("1) Funcionario\n");
    printf("2) Cliente\n");
    printf("3) Sair\n\n");
    // Armazena a opção escolhida pelo usuario no endereço de memória de option
    scanf("%d", &option);
    system("cls");

    switch (option)
    {
    case 1:
      // Chamando a função que solicita a senha para o funcionário ao acessar o menu
      solicitarSenhaFuncionario(1); // 1 significa que será aberto o menu de funcionarios
      break;

    case 2:
      // Enviar a autenticação da conta do cliente
      solicitarSenhaCliente();
      system("cls");
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