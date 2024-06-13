#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#define DEFAULT_PASS_SIZE 16

struct Data
{
  int dia;
  int mes;
  int ano;
};

struct Endereco
{
  char endereco[45];
  char cep[10];
  char bairro[20];
  char cidade[20];
  char estado[2];
};

struct Cliente
{
  char nome[25];
  int agencia;
  int numDaConta;
  float limiteDaConta;
  char cpf[15];
  struct Data nascimento;
  struct Data vencimento;
  char telefone[14];
  struct Endereco endereco;
  char senha[16];
  float saldo;
  char tipoConta[4];
  char excluido;
};

struct Funcionario
{
  char nomeFuncionario[25];
  char cpf[14];
  int codigoFuncionario;
  char cargo[25];
  struct Data nascimento;
  char telefoneContato[15];
  char senhaFuncionario[16];
  struct Endereco endereco;
  char excluido;
};

// Declaração de todas as funções utilizadas no projeto.
void enviarTitulo();                                                                              // Enviar título ASCII para o usuário antes das mensagens
void enviarMenuCliente(FILE *file, Cliente cliente);                                              // Envia o menu de clientes
void enviarMenuFuncionario();                                                                     // Envia o menu de funcionarios
void enviarMenuPrincipal();                                                                       // Envia o menu principal
int consultarCliente(FILE *file, Cliente cliente);                                                // Consulta os dados de um cliente
int inserirCliente(FILE *file, Cliente cliente);                                                  // Insere dados de um funcionário no arquivo
int alterarCliente(FILE *file, Cliente cliente);                                                  // Alterar os dados de um cliente do arquivo
int alterarSaldoCliente(FILE *file, Cliente cliente_antigo, Cliente cliente_novo);                // Alterar o saldo de um cliente no arquivo.
int consultarFuncionario(FILE *file, Funcionario funcionario);                                    // Consulta os dados de um funcionário
int inserirFuncionario(FILE *file, Funcionario funcionario);                                      // Insere dados de um funcionário no arquivo
int alterarFuncionario(FILE *file, Funcionario funcionario_antigo, Funcionario funcionario_novo); // Alterar os dados de um funcionário do arquivo
int validarSenhaAdmin(char *senhaDigitada);                                                       // Valida a senha de administrador digitada pelo usuário
void validarSenhaCliente(FILE *file, Cliente cliente);                                            // Valida o usuario e senha de um cliente
void validarSenhaFuncionario(FILE *file, Funcionario funcionario);                                // Valida o usuario e senha de um funcionario
void saldo(FILE *file, Cliente cliente);                                                          // Mostrar o saldo de um cliente.
void depositar(FILE *file, Cliente cliente);                                                      // Depositar um valor na conta de um cliente.
void sacar(FILE *file, Cliente cliente);                                                          // Sacar um valor da conta de um cliente.

// Função que mostra o saldo do cliente.
void saldo(FILE *file, Cliente cliente)
{
  // Variável que contem a posição do ponteiro do seek.
  int posicao;

  if ((posicao = consultarCliente(file, cliente)) != -1)
  {
    fseek(file, posicao * sizeof(cliente), SEEK_SET);
    fread(&cliente, sizeof(cliente), 1, file);

    enviarTitulo();
    printf("Saldo atual em sua conta R$%.2f\n\n", cliente.saldo);
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
    system("cls");

    enviarMenuCliente(file, cliente);
  }
}

// Função para depositar um valor na conta do cliente.
void depositar(FILE *file, Cliente cliente)
{
  Cliente cliente_alterado;
  int posicao;

  if ((posicao = consultarCliente(file, cliente)) != -1)
  {
    float valor;

    enviarTitulo();
    printf("Digite o valor a ser depositado: \n\nR$");
    scanf("%f", &valor);
    system("cls");

    // Adiciona o valor depositado ao valor da conta do
    cliente_alterado.saldo = cliente.saldo + valor;

    // Realiza a alteração do saldo do cliente no arquivo.
    alterarSaldoCliente(file, cliente, cliente_alterado);

    // Enviar a mensagem com o saldo do usuário
    enviarTitulo();
    printf("Conta: %s\n\n", cliente.nome);
    printf("Voce realizou um deposito de R$%.2f com sucesso. \n", valor);
    printf("Seu novo saldo: R$%.2f\n\n", cliente.saldo);
    printf("Pressione qualquer tecla para retornar ao menu...\n");
    getch();
    system("cls");

    // Enviar o menu de clientes novamente para o usuário.
    enviarMenuCliente(file, cliente);
  }
}

// Função que altera o saldo de um cliente no arquivo.
int alterarSaldoCliente(FILE *file, Cliente cliente, Cliente cliente_alterado)
{
  int posicao;

  // Verifica se o arquivo conseguiu ser aberto com sucesso
  if (file != NULL)
  {
    posicao = consultarCliente(file, cliente);
    if (posicao != -1)
    {
      fseek(file, posicao * sizeof(Cliente), SEEK_SET);
      fread(&cliente, sizeof(cliente), 1, file);

      // Copia os dados contidos no registro antigo e envia para o novo
      cliente.saldo = cliente_alterado.saldo;

      fseek(file, posicao * sizeof(Cliente), SEEK_SET);
      fwrite(&cliente, sizeof(cliente_alterado), 1, file);

      // Enviando mensagem de confirmação...
      return 1;
    }
  }
  return 0;
}

// Função para realizar saques.
void sacar(FILE *file, Cliente cliente)
{
  file = fopen("clientes.txt", "r+");
  float saldo;

  if (file == NULL)
  {
    printf("O arquivo nao foi encontrado, tentando gerar um novo.\n");
    file = fopen("clientes.txt", "w+");
  }

  float valor;
  enviarTitulo();
  printf("Digite o valor a ser retirado: \n\nR$");
  scanf("%f", &valor);
  system("cls");

  if (saldo < valor)
  {
    enviarTitulo();
    printf("Voce nao possui saldo suficiente para realizar este saque.\n");
    printf("Por questoes de seguranca, estamos saindo do programa...\n");
    exit(1);
    system("cls");
    return;
  }

  // Recebe o valor e adiciona ao saldo do usuário.
  saldo -= valor;

  enviarTitulo();
  printf("Voce realizou um saque de R$%.2f com sucesso. \nSeu novo saldo: R$%.2f\n\n", valor, saldo);
  printf("Pressione qualquer tecla para voltar ao menu de clientes.\n");
  getch();
  system("cls");

  // Enviar o menu de clientes novamente para o usuário.
  enviarMenuCliente(file, cliente);
}

// Função que realiza a consulta nos arquivos sobre um cliente
int consultarCliente(FILE *file, Cliente cliente)
{
  Cliente cliente_lido;
  int posicao;

  // Caso o arquivo tenha sido aberto e lido com sucesso
  if (file != NULL)
  {
    // Define o ponteiro de busca para o ínicio do código
    fseek(file, 0L, SEEK_SET);

    // Define a posição inicial para 0
    posicao = 0;

    while (fread(&cliente_lido, sizeof(cliente_lido), 1, file))
    {
      if (strcmpi(cliente_lido.nome, cliente.nome) == 0 &&
          (cliente_lido.excluido == 0))
        return posicao;
      posicao++;
    };
  }
  return -1;
}

// Função que insere os dados do cliente em um arquivo
int inserirCliente(FILE *file, Cliente cliente)
{
  Cliente cliente_lido;
  int posicao;

  if (file != NULL)
  {
    posicao = 0;

    // Procurar se a estrutura do cliente existe no arquivo.
    if (consultarCliente(file, cliente))
    {
      // Definindo o ponteiro de busca no início do arquivo
      fseek(file, 0L, SEEK_SET);

      // Vai rodar enquanto não chegar ao fim do arquivo
      while (fread(&cliente_lido, sizeof(cliente_lido), 1, file))
      {
        if (cliente_lido.excluido == 1)
          break;
        posicao++;
      };

      fseek(file, posicao * sizeof(cliente), SEEK_SET);
      cliente.excluido = 0;

      if (fwrite(&cliente, sizeof(cliente), 1, file))
      {
        enviarTitulo();
        printf("O cliente foi cadastrado com sucesso.\n");
        printf("Pressione qualquer tecla para concluir o cadastro...\n");
        getch();
        return 1;
      }
    }
  }
  return 0;
}

// Função que exclui um funcionário dos registros.
int excluirCliente(FILE *file, Cliente cliente)
{
  int posicao;
  // Executa somente se o arquivo existir.
  if (file != NULL)
  {
    posicao = consultarCliente(file, cliente);
    // Verifica a existência do usuário dentro dos arquivos.
    if (posicao != -1)
    {
      fseek(file, posicao * sizeof(cliente), SEEK_SET);
      cliente.excluido = 1;
      if (fwrite(&cliente, sizeof(cliente), 1, file))
      {
        enviarTitulo();
        printf("Voce excluiu o cliente %s dos registros com sucesso.\n", cliente.nome);
        printf("Pressione qualquer tecla para finalizar...\n");
        system("cls");
        return 1;
      }
    }
  }
  return 0;
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
        // Verifica se o funcionário já foi excluido.
        // Caso tenha sido, ele para a busca e retorna erro.
        if (funcionario_lido.excluido == 1)
          break;
        // Adiciona uma posição sempre que o laço se repete.
        posicao++;
      };

      fseek(file, posicao * sizeof(funcionario), SEEK_SET);
      funcionario.excluido = 0;

      if (fwrite(&funcionario, sizeof(funcionario), 1, file))
      {
        enviarTitulo();
        printf("O funcionario foi cadastrado com sucesso.\n");
        printf("Pressione qualquer tecla para concluir o cadastro...\n");
        getch();
        return 1;
      }
    }
  }
  // Retorno padrão
  return 0;
}

// Função que exclui um funcionário dos registros.
int excluirFuncionario(FILE *file, Funcionario funcionario)
{
  int posicao;
  if (file != NULL)
  {
    posicao = consultarFuncionario(file, funcionario);
    // Caso seja válido
    if (posicao != -1)
    {
      fseek(file, posicao * sizeof(funcionario), SEEK_SET);
      funcionario.excluido = 1;
      if (fwrite(&funcionario, sizeof(funcionario), 1, file))
      {
        enviarTitulo();
        printf("Voce excluiu este funcionario dos registros com sucesso.\n");
        printf("Pressione qualquer tecla para finalizar...\n");
        system("cls");
        return 1;
      }
    }
  }
  return 0;
}

// Função que altera os dados de um funcionário.
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

      // Copia os dados contidos no registro antigo e envia para o novo
      strcpy(funcionario_antigo.nomeFuncionario, funcionario_novo.nomeFuncionario);
      strcpy(funcionario_antigo.cpf, funcionario_novo.cpf);

      fseek(file, posicao * sizeof(Funcionario), SEEK_SET);
      fwrite(&funcionario_antigo, sizeof(funcionario_novo), 1, file);

      enviarTitulo();
      printf("Os dados deste funcionario foram alterados com sucesso.\n");
      system("cls");
      return 1;
    }
  }
  return 0;
}

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

// Função para validar e liberar o acesso do cliente ao menu.
void validarSenhaCliente(FILE *file, Cliente cliente)
{
  int acesso;
  int posicao;

  char password[DEFAULT_PASS_SIZE];

  do
  {
    // Bloqueando o acesso do usuário enquanto ele não valida a senha.
    acesso = 0;

    if ((posicao = consultarCliente(file, cliente)) == -1)
    {
      enviarTitulo();
      printf("Este cliente nao foi encontrado no arquivo...\n");
      exit(1);
      system("cls");
      return;
    }

    if ((posicao = consultarCliente(file, cliente)) != -1)
    {
      fseek(file, posicao * sizeof(cliente), SEEK_SET);
      fread(&cliente, sizeof(cliente), 1, file);

      enviarTitulo();
      printf("Digite a senha: \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(password);
      system("cls");

      if (strcmp(password, cliente.senha) == 0)
      {
        acesso = 1;
        enviarTitulo();
        printf("Autenticado com sucesso. \n");
        system("cls");

        enviarMenuCliente(file, cliente);
      }
      else
      {
        acesso = 0;
        enviarTitulo();
        printf("A senha digitada esta incorreta.\n");
        exit(1); // Ao digitar a senha incorreta, o usuário sai do programa.
      }
    }
  } while (acesso == 0);
}

// Função para validar e liberar o acesso do funcionário ao menu.
void validarSenhaFuncionario(FILE *file, Funcionario funcionario)
{
  int acesso;
  int posicao;

  char password[DEFAULT_PASS_SIZE];
  char passwordAdm[DEFAULT_PASS_SIZE] = "adm";

  do
  {
    acesso = 0;

    if ((posicao = consultarFuncionario(file, funcionario)) != -1)
    {
      fseek(file, posicao * sizeof(funcionario), SEEK_SET);
      fread(&funcionario, sizeof(funcionario), 1, file);

      enviarTitulo();
      printf("Digite a senha: \n");
      fflush(stdin);
      gets(password);
      system("cls");

      if ((posicao = consultarFuncionario(file, funcionario)) == -1)
      {
        enviarTitulo();
        printf("Este funcionario nao esta cadastrado.\n");
        sleep(3); // Aguarda 3 segundos para enviar o menu principal.
        enviarMenuPrincipal();
        return;
      }

      // Compara a senha digitada pela senha do funcionario
      if (strcmp(password, funcionario.senhaFuncionario) == 0) // Se der erro irá retornar 1, 0 é válido.
      {
        acesso = 1;
        enviarTitulo();
        printf("Autenticado(a) com sucesso.\n");
        system("cls");

        enviarMenuFuncionario();
      }
      else if (strcmp(password, passwordAdm) == 0)
      {
        acesso = 1;
        enviarTitulo();
        printf("Autenticado(a) utilizando a senha de administrador.\n");
        system("cls");

        enviarMenuFuncionario();
      }
      else
      {
        acesso = 0;
        enviarTitulo();
        printf("A senha digitada esta incorreta.\n");
        exit(1);
      }
    }

  } while (acesso == 0); // Enquanto o acesso não for liberado.
}

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

// Função que envia o título em ASCII para o usuário antes de alguma mensagem.
void enviarTitulo()
{
  char title[7][91] = {
      "::::     ::::     :::     :::    :::     :::     :::     :::::::::  :::::::::: :::::::::  ",
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

  printf("------------------------------------------------------------------------------------------\n\n");
  printf("\n");
  // Enviar o titulo linha por linha
  for (int i = 0; i < 7; i++)
  {
    printf("%s\n", title[i]);
  }
  printf("\n");
  // Enviar o subtitulo linha por linha
  for (int i = 0; i < 7; i++)
  {
    printf("%s\n", subtitle[i]);
  }
  printf("\n");
  printf("------------------------------------------------------------------------------------------\n\n");
}

// Função que envia o menu de clientes após autenticação.
void enviarMenuCliente(FILE *file, Cliente cliente)
{
  // Abre o arquivo de clientes.
  file = fopen("clientes.txt", "r+");

  // Verificar se o arquivo existe e foi lido.
  if (file == NULL)
  {
    printf("O registro de clientes não foi encontrado no sistema, gerando um novo.\n");
    file = fopen("clientes.txt", "w+"); // Tentativa de criar o arquivo
  }

  // Verifica se o arquivo foi aberto e executa o contexto.
  if (file != NULL)
  {
    enviarTitulo();
    int opcao;

    do
    {
      printf("Escolha uma opcao do menu de clientes: \n\n");
      printf("1) Saldo\n");
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
        printf("Estamos coletando suas informacoes para continuar, aguarde...\n");
        saldo(file, cliente);
        break;

      case 2:
        printf("Estamos coletando suas informacoes para continuar, aguarde...\n");
        depositar(file, cliente);
        break;

      case 3:
        sacar(file, cliente);
        break;

      case 6:
        enviarTitulo();
        printf("Voce escolheu sair do programa, desconectando da sua conta... \n");
        exit(1);

      default:
        printf("\nVoce selecionou uma opcao invalida, tente outra...\n");
        break;
      }
    } while (opcao <= 0 || opcao > 6);
  }
}

// Função que envia o menu de abertura de conta.
void enviarMenuAberturaConta()
{
  FILE *file = fopen("clientes.txt", "r+");
  Cliente cliente;

  if (file == NULL)
  {
    printf("Arquivo nao encontrado, tentando gerar um novo...\n");
    file = fopen("clientes.txt", "w+");
  }

  // Variavel que vai armazenar a opcao desejada pelo usuario
  int option;

  if (file != NULL)
  {
    do
    {
      enviarTitulo();
      printf("Menu funcionario:\n\n");
      printf("1) Conta Poupanca\n");
      printf("2) Conta Corrente\n");
      printf("3) Voltar\n\n");
      scanf("%d", &option);
      system("cls");

      switch (option)
      {
      case 1:

        // enviarTitulo();
        // printf("Informe o numero da agencia: \n");
        // fflush(stdin);
        // scanf("%d", &cliente.agencia);
        // system("cls");

        // enviarTitulo();
        // printf("Informe o numero da conta: \n");
        // fflush(stdin);
        // scanf("%d", &cliente.numDaConta);
        // system("cls");

        // enviarTitulo();
        // printf("Informe o CPF do cliente: \n");
        // fflush(stdin);
        // gets(cliente.cpf);
        // system("cls");

        // enviarTitulo();
        // printf("Informe a data de nascimento do cliente (dia, mes e ano - 11/22/3333): \n");
        // scanf("%d %d %d", &cliente.nascimento.dia, &cliente.nascimento.mes, &cliente.nascimento.ano);
        // system("cls");

        // enviarTitulo();
        // printf("Informe o telefone de contato do cliente: \n");
        // fflush(stdin);
        // gets(cliente.telefone);
        // system("cls");

        // enviarTitulo();
        // printf("Informe o endereco do cliente: \n");
        // fflush(stdin);
        // gets(cliente.endereco.endereco);
        // system("cls");

        // enviarTitulo();
        // printf("Informe o CEP do cliente: \n");
        // fflush(stdin);
        // gets(cliente.endereco.cep);
        // system("cls");

        // enviarTitulo();
        // printf("Informe o bairro do cliente: \n");
        // fflush(stdin);
        // gets(cliente.endereco.bairro);
        // system("cls");

        // enviarTitulo();
        // printf("Informe a cidade do cliente: \n");
        // fflush(stdin);
        // gets(cliente.endereco.cidade);
        // system("cls");

        // enviarTitulo();
        // printf("Informe o estado do cliente (em SIGLA): \n");
        // fflush(stdin);
        // gets(cliente.endereco.estado);
        // system("cls");

        enviarTitulo();
        printf("Informe o nome do cliente: \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(cliente.nome);
        system("cls");

        enviarTitulo();
        printf("Informe a senha do cliente: \n");
        fflush(stdin);
        gets(cliente.senha);
        system("cls");

        enviarTitulo();
        printf("Digite o saldo inicial do cliente: \n");
        scanf("%f", &cliente.saldo);
        system("cls");

        enviarTitulo();
        printf("Confirme que esta criando uma conta poupanca \n");
        printf("Digite 'CP' no campo abaixo para continuar: \n");
        fflush(stdin);
        gets(cliente.tipoConta);
        system("cls");

        inserirCliente(file, cliente);
        break;

      case 2:
        printf("Iniciando processo de criacao de conta corrente... \n\n");

        printf("Informe o numero da agencia: \n");
        scanf("%d", &cliente.agencia);
        system("cls");

        printf("Informe o numero da conta: \n");
        fflush(stdin);
        scanf("%d", &cliente.numDaConta);
        system("cls");

        printf("Informe o limite da conta: \n");
        fflush(stdin);
        scanf("%f", &cliente.limiteDaConta);
        system("cls");

        printf("Informe o nome do cliente: \n");
        fflush(stdin);
        gets(cliente.nome);
        system("cls");

        printf("Informe o cpf do cliente: \n");
        fflush(stdin);
        gets(cliente.cpf);
        system("cls");

        printf("Informe a data de nascimento do cliente (dia, mes e ano - 11/22/3333): \n");
        scanf("%d %d %d", &cliente.nascimento.dia, &cliente.nascimento.mes, &cliente.nascimento.ano);
        system("cls");

        printf("Informe o telefone de contato do cliente: \n");
        fflush(stdin);
        gets(cliente.telefone);
        system("cls");

        printf("Informe o endereço do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.endereco);
        system("cls");

        printf("Informe o CEP do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.cep);
        system("cls");

        printf("Informe o bairro do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.bairro);
        system("cls");

        printf("Informe a cidade do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.cidade);
        system("cls");

        printf("Informe o estado do cliente (em SIGLA): \n");
        fflush(stdin);
        gets(cliente.endereco.estado);
        system("cls");

        printf("Informe a senha do cliente: \n");
        fflush(stdin);
        gets(cliente.senha);
        system("cls");

        break;

      case 3:
        enviarTitulo();
        printf("Voltando para o menu do funcionario... \n");
        system("cls");
        enviarMenuFuncionario();
        break;

      default:
        enviarTitulo();
        printf("Voce selecionou uma opcao invalida, tente outra...\n");
        system("cls");
      }
      // Executa o código acima enquanto option não for (1,2 ou 3)
    } while (option <= 0 || option > 3);
    fclose(file);
  }
  fclose(file);
}

// Função que envia o menu de funcionários.
void enviarMenuFuncionario()
{
  // Declaração do arquivo para funcionarios.
  FILE *file;

  // Declaração do arquivo para clientes.
  FILE *fileClientes;

  // Variável que vai receber a opção desejada pelo usuário.
  Funcionario funcionario, funcionario_alterado;

  // Variável que vai receber a opção desejada pelo usuário.
  Cliente cliente, cliente_alterado;

  // Variável da opção que será digitada pelo usuário.
  int option;

  // Tenta abrir o arquivo no modo de leitura e escrita.
  file = fopen("funcionarios.txt", "r+");

  // Tenta abrir o arquivo no modo de leitura e escrita.
  fileClientes = fopen("clientes.txt", "r+");

  // Caso o arquivo não exista, o programa tentará criar um novo...
  if (file == NULL)
  {
    printf("Arquivo de funcionarios nao encontrado, recriando o arquivo...");
    file = fopen("funcionarios.txt", "w+");
  }

  // Caso o arquivo não exista, o programa tentará criar um novo...
  if (fileClientes == NULL)
  {
    printf("Arquivo de clientes nao encontrado, recriando o arquivo...");
    fileClientes = fopen("clientes.txt", "w+");
  }

  enviarTitulo();
  // Entra no laço de repetição enquanto o usuário não digitar uma opção.
  do
  {
    printf("Menu funcionario:\n\n");
    printf("1) Abertura de Conta\n");     // Falta terminar
    printf("2) Encerramento de Conta\n"); // Falta corrigir e finalizar
    printf("3) Consultar Dados\n");       // Consultar dados de clientes está com erro
    printf("4) Alterar Dados\n");         // Falta terminar
    printf("5) Cadastro de Funcionarios\n");
    printf("6) Sair\n\n");
    scanf("%d", &option);
    system("cls");

    // Limpa o buffer do teclado.
    fflush(file);

    switch (option)
    {
    case 1:
      enviarMenuAberturaConta();
      break;

    case 2:
      do
      {
        switch (option)
        {
        case 1:
          enviarTitulo();
          printf("Digite o nome do funcionario que tera a conta encerrada: \n");
          fflush(stdin); // Limpa o buffer do teclado
          gets(funcionario.nomeFuncionario);
          excluirFuncionario(file, funcionario); // Requisita a função que exclui um funcionário
          system("cls");
          break;

        case 2:
          enviarTitulo();
          printf("Digite o nome do cliente que tera a conta encerrada: \n");
          fflush(stdin); // Limpa o buffer do teclado
          gets(cliente.nome);
          system("cls");

          excluirCliente(file, cliente); // Requisita a função que exclui um funcionário
          break;

        default:
          break;
        }
      } while (option <= 0 || option > 3);
      break;

    case 3:
      int posicao;
      do
      {
        enviarTitulo();
        printf("Consultar dados: \n\n");
        printf("1) Funcionario\n");
        printf("2) Cliente\n\n");
        scanf("%d", &option);
        system("cls");

        switch (option)
        {
        case 1:
          enviarTitulo();
          fflush(stdin);
          printf("Digite o nome do funcionario desejado: \n");
          gets(funcionario.nomeFuncionario);
          system("cls");

          if ((posicao = consultarFuncionario(file, funcionario)) == -1)
          {
            enviarTitulo();
            printf("O funcionario nao foi encontrado, voltando ao menu...\n");
            sleep(3); // Espera 3 segundos para executar a próxima linha.
            system("cls");
            enviarMenuFuncionario();
          }

          if ((posicao = consultarFuncionario(file, funcionario)) != -1)
          {
            fseek(file, posicao * sizeof(funcionario), SEEK_SET);
            fread(&funcionario, sizeof(funcionario), 1, file);

            enviarTitulo();
            fflush(stdin);
            printf("Mostrando informacoes do(a) funcionario(a) %s:\n\n", funcionario.nomeFuncionario);
            printf("Codigo: %d\n", funcionario.codigoFuncionario);
            printf("Cargo: %s\n", funcionario.cargo);
            printf("CPF: %s\n", funcionario.cpf);
            printf("Data de Nascimento: %d/%d/%d\n", funcionario.nascimento.dia, funcionario.nascimento.mes,
                   funcionario.nascimento.ano);
            printf("Telefone: %s\n", funcionario.telefoneContato);
            printf("Endereco: %s, %s, %s, %s, %s\n", funcionario.endereco.endereco, funcionario.endereco.cep,
                   funcionario.endereco.bairro, funcionario.endereco.cidade,
                   funcionario.endereco.estado);
            printf("Senha: %s\n", funcionario.senhaFuncionario);

            printf("\nPressione qualquer tecla para voltar ao menu...\n");
            getch();
            system("cls");
            enviarMenuFuncionario();
          }
          break;

        case 2:
          enviarTitulo();
          printf("Digite o nome do cliente desejado: \n");
          fflush(stdin); // Limpa o buffer do teclado
          gets(cliente.nome);
          system("cls");

          // Verifica se o cliente existe ou não.
          if ((posicao = consultarCliente(file, cliente)) == -1)
          {
            enviarTitulo();
            printf("O cliente nao foi encontrado, voltando ao menu...\n");
            sleep(3); // Espera 3 segundos para executar a próxima linha.
            system("cls");
            enviarMenuFuncionario();
            return;
          }

          // Caso o cliente exista ele irá executar o código.
          if ((posicao = consultarCliente(file, cliente)) != -1)
          {
            fseek(file, posicao * sizeof(cliente), SEEK_SET);
            fread(&cliente, sizeof(cliente), 1, file);

            enviarTitulo();
            printf("Mostrando informacoes do(a) cliente %s:\n\n", cliente.nome);
            printf("Tipo de conta: %d", cliente.agencia);
            printf("Saldo: %f", cliente.saldo);
            printf("Senha: %s", cliente.senha);
            system("cls");
            getch();
            enviarMenuFuncionario();
          }
          break;

        default:
          printf("A opcao especificada esta invalida, tente outra...\n");
          break;
        }
      } while (option <= 0 || option > 3);

      getch();
      break;

      // Alterar dados
    case 4:
      int acesso;
      do
      {
        enviarTitulo();
        printf("Alterar dados: \n\n");
        printf("1) Alterar Conta\n");
        printf("2) Alterar Funcionario\n");
        printf("3) Alterar Cliente\n");
        printf("4) Voltar\n\n");
        scanf("%d", &option);
        system("cls");

        fflush(stdin); // Limpa o buffer do teclado

        switch (option)
        {
        case 1:
          // Criar funcionalidade que altera o tipo de conta de um cliente.
          // Conta Poupança = CP | Conta Corrente = CC
          break;

        case 2:
          char password[DEFAULT_PASS_SIZE];

          do
          {
            // Acesso bloqueado enquanto o usuário não se autenticar.
            acesso = 0;

            enviarTitulo();
            printf("Digite a senha de administrador: \n");
            fflush(stdin);
            gets(password);
            system("cls");

            // Faz a verificação se a senha digitada é válida como (Admin || Funcionário)
            if (validarSenhaAdmin(password) != 0)
            {
              enviarTitulo();
              printf("\nA senha digitada esta incorreta, voltando ao menu de funcionarios. \n\n");
              system("cls");
              sleep(5);
              enviarMenuFuncionario();
            }

            // Se a senha digitada for a senha correta, o usuario recebe o menu de funcionários.
            if (validarSenhaAdmin(password) == 0)
            {
              // Após o usuário for autenticado, o acesso será liberado.
              acesso = 1;

              enviarTitulo();
              printf("Digite o nome do funcionario que tera os dados alterados: \n");
              fflush(stdin);
              gets(funcionario.nomeFuncionario);
              system("cls");

              // Dados do funcionário que serão alterados devem estar abaixo dessa linha.
              enviarTitulo();
              printf("Digite o novo nome do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.nomeFuncionario);
              system("cls");

              enviarTitulo();
              printf("Digite o novo CPF do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.cpf);
              system("cls");

              enviarTitulo();
              printf("Digite o novo codigo do funcionario: \n");
              scanf("%d", &funcionario_alterado.codigoFuncionario);
              system("cls");

              enviarTitulo();
              printf("Digite o novo cargo do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.cargo);
              system("cls");

              enviarTitulo();
              printf("Digite o dia, mes e ano novo do funcionario - 11 22 3333: \n");
              scanf("%d %d %d", &funcionario_alterado.nascimento.dia, &funcionario_alterado.nascimento.mes, &funcionario_alterado.nascimento.ano);
              system("cls");

              enviarTitulo();
              printf("Digite o novo numero de telefone do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.telefoneContato);
              system("cls");

              enviarTitulo();
              printf("Digite a nova senha do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.senhaFuncionario);
              system("cls");

              enviarTitulo();
              printf("Digite o novo endereco do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.endereco.endereco);
              system("cls");

              enviarTitulo();
              printf("Digite o novo CEP do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.endereco.cep);
              system("cls");

              enviarTitulo();
              printf("Digite o novo bairro do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.endereco.bairro);
              system("cls");

              enviarTitulo();
              printf("Digite o novo cidade do funcionario: \n");
              fflush(stdin);
              gets(funcionario_alterado.endereco.cidade);
              system("cls");

              enviarTitulo();
              printf("Digite o novo estado do funcionario (Ex.: DF): \n");
              fflush(stdin);
              gets(funcionario_alterado.endereco.estado);
              system("cls");

              // Requisita a função que altera os dados de um funcionário.
              alterarFuncionario(file, funcionario, funcionario_alterado);
            }
          } while (acesso == 0);
          break;

        case 3:
          do
          {
            acesso = 0;

            enviarTitulo();
            printf("Digite o nome do cliente que tera os dados alterados: \n");
            fflush(stdin);
            gets(cliente.nome);
            system("cls");

            // Dados do cliente que serão alterados devem estar abaixo dessa linha.
            enviarTitulo();
            printf("Digite o novo nome do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.nome);
            system("cls");

            enviarTitulo();
            printf("Digite o novo numero da agencia: \n");
            scanf("%d", &cliente_alterado.agencia);
            system("cls");

            enviarTitulo();
            printf("Digite o novo numero da conta: \n");
            scanf("%d", &cliente_alterado.numDaConta);
            system("cls");

            enviarTitulo();
            printf("Digite o novo numero da agencia: \n");
            scanf("%f", &cliente_alterado.limiteDaConta);
            system("cls");

            enviarTitulo();
            printf("Digite o novo numero de CPF: \n");
            fflush(stdin);
            gets(cliente_alterado.cpf);
            system("cls");

            enviarTitulo();
            printf("Digite o dia, mes e ano da data de nascimento nova do cliente - 11 22 3333: \n");
            scanf("%d %d %d", &cliente_alterado.nascimento.dia, &cliente_alterado.nascimento.mes, &cliente_alterado.nascimento.ano);
            system("cls");

            enviarTitulo();
            printf("Digite o dia, mes e ano da data de vencimento nova do cliente - 11 22 3333: \n");
            scanf("%d %d %d", &cliente_alterado.vencimento.dia, &cliente_alterado.vencimento.mes, &cliente_alterado.vencimento.ano);
            system("cls");

            enviarTitulo();
            printf("Digite o novo numero de telefone do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.telefone);
            system("cls");

            enviarTitulo();
            printf("Digite o novo endereco do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.endereco.endereco);
            system("cls");

            enviarTitulo();
            printf("Digite o novo CEP do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.endereco.cep);
            system("cls");

            enviarTitulo();
            printf("Digite o novo bairro do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.endereco.bairro);
            system("cls");

            enviarTitulo();
            printf("Digite o novo cidade do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.endereco.cidade);
            system("cls");

            enviarTitulo();
            printf("Digite o novo estado do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.endereco.estado);
            system("cls");

            enviarTitulo();
            printf("Digite a nova senha do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.senha);
            system("cls");

            enviarTitulo();
            printf("Digite o novo tipo de conta do cliente: \n");
            fflush(stdin);
            gets(cliente_alterado.tipoConta);
            system("cls");

          } while (acesso == 0);
          break;

        default:
          enviarTitulo();
          printf("A opcao informada esta invalida, tente outra.\n");
          system("cls");
        }
      } while (option <= 0 || option > 4);

      break;

    case 5:
      enviarTitulo();
      printf("Digite o nome do funcionario: \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.nomeFuncionario);
      system("cls");

      enviarTitulo();
      printf("Digite o codigo do funcionario: \n");
      fflush(stdin); // Limpa o buffer do teclado
      scanf("%d", &funcionario.codigoFuncionario);
      system("cls");

      enviarTitulo();
      printf("Digite o cargo do funcionario: \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.cargo);
      system("cls");

      enviarTitulo();
      printf("Digite o CPF do funcionario no formato (000.000.000-00): \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.cpf);
      system("cls");

      enviarTitulo();
      printf("Digite o dia, mes e ano - 11 22 3333: \n");
      scanf("%d %d %d", &funcionario.nascimento.dia, &funcionario.nascimento.mes, &funcionario.nascimento.ano);
      system("cls");

      enviarTitulo();
      printf("Digite o telefone para contato: \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.telefoneContato);
      system("cls");

      enviarTitulo();
      printf("Digite o endereco do funcionario (Maximo de 45 caracteres): \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.endereco.endereco);
      system("cls");

      enviarTitulo();
      printf("Digite o CEP do funcionario no formato (00000-000): \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.endereco.cep);
      system("cls");

      enviarTitulo();
      printf("Digite o bairro do funcionario: \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.endereco.bairro);
      system("cls");

      enviarTitulo();
      printf("Digite a cidade do funcionario: \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.endereco.cidade);
      system("cls");

      enviarTitulo();
      printf("Digite o estado do funcionario (Coloque em sigla: ex: DF): \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.endereco.estado);
      system("cls");

      // Verificar se a senha atende os requisitos e coletar as informações.
      do
      {
        enviarTitulo();
        printf("Digite a senha do funcionario (Maximo de 16 caracteres): \n");
        fflush(stdin); // Limpa o buffer do teclado
        gets(funcionario.senhaFuncionario);

        if (strlen(funcionario.senhaFuncionario) == 0)
        {
          printf("A senha nao pode ser vazia.\n");
          system("cls");
        }
        else if (strlen(funcionario.senhaFuncionario) > 16)
        {
          printf("A senha excede o limite de 16 caracteres.\n");
          printf("Pressione qualquer tecla para tentar novamente...\n");
          fflush(stdin); // Limpar o buffer do teclado.
          system("cls");
        }
      } while (strlen(funcionario.senhaFuncionario) == 0 || strlen(funcionario.senhaFuncionario) > 16);

      // Requisita a função que insere os dados digitados no arquivo de funcionários
      inserirFuncionario(file, funcionario);
      break;

    case 6:
      enviarMenuPrincipal();
      break;

    default:
      enviarTitulo();
      printf("\nVoce selecionou uma opcao invalida, tente outra...\n");
      system("cls");
      break;
    }

  } while (option <= 0 || option > 7);

  fclose(file);
}

// Função para enviar o menu principal.
void enviarMenuPrincipal()
{
  FILE *file;
  Funcionario funcionario;

  FILE *fileCliente;
  Cliente cliente;

  int option;

  file = fopen("funcionarios.txt", "r+");
  fileCliente = fopen("clientes.txt", "r+");

  if (file == NULL)
  {
    printf("Arquivo de funcionarios nao foi encontrado, recriando o arquivo...\n");
    file = fopen("funcionarios.txt", "w+");
  }

  if (fileCliente == NULL)
  {
    printf("Arquivo de clientes nao foi encontrado, recriando o arquivo...\n");
    fileCliente = fopen("clientes.txt", "w+");
  }

  do
  {
    enviarTitulo();
    printf("Escolha uma opcao do menu principal: \n\n");
    printf("1) Funcionario\n");
    printf("2) Cliente\n");
    printf("3) Sair\n\n");
    scanf("%d", &option);
    system("cls");

    switch (option)
    {
    case 1:
      enviarTitulo();
      printf("Digite o nome do funcionario: \n");
      fflush(stdin); // Limpa o buffer do teclado
      gets(funcionario.nomeFuncionario);
      if (strcmp(funcionario.nomeFuncionario, "admin") == 0)
      {
        enviarMenuFuncionario();
        return;
      }
      system("cls");

      validarSenhaFuncionario(file, funcionario); // Requisita a função para validação da senha do funcionário.
      break;

    case 2:
      enviarTitulo();
      printf("Digite o nome do cliente: \n");
      fflush(stdin);      // Limpa o buffer do teclado
      gets(cliente.nome); // Recebe o nome do cliente em formato "STRING" (Cadeia de caracteres).
      system("cls");

      validarSenhaCliente(fileCliente, cliente); // Requisita a função para validação da senha do cliente.
      break;

    case 3:
      enviarTitulo();
      printf("Saindo do programa...");
      exit(1);
      system("cls");
      break;

      // Caso o usuario digite algo que não seja aceito ou que não exista
    default:
      printf("\nVoce selecionou uma opcao invalida, tente outra...\n");
    }

    fclose(file);
    // O bloco de código acima será executado enquanto a opção não for (1, 2 ou 3)
  } while (option <= 0 || option > 3);

  fclose(file);
}