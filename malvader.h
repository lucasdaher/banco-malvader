#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <time.h>

// Valor padrão máximo de caracteres que uma senha poderá ter.
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

struct Transacao
{
  char nome[25];
  char tipo[15];
  float valor;
};

void enviarTitulo();                                                                              // Enviar título customizado para o usuário antes das mensagens
void enviarMenuCliente(FILE *file, Cliente cliente);                                              // Envia o menu de clientes
void enviarMenuFuncionario();                                                                     // Envia o menu de funcionarios
void enviarMenuPrincipal();                                                                       // Envia o menu principal
void formatarTipoConta(Cliente cliente);                                                          // Formatar a sigla do tipo de conta
int consultarCliente(FILE *file, Cliente cliente);                                                // Consulta os dados de um cliente
int inserirCliente(FILE *file, Cliente cliente);                                                  // Insere dados de um funcionário no arquivo
int alterarCliente(FILE *file, Cliente cliente_antigo, Cliente cliente_novo);                     // Alterar os dados de um cliente do arquivo
int consultarFuncionario(FILE *file, Funcionario funcionario);                                    // Consulta os dados de um funcionário
int inserirFuncionario(FILE *file, Funcionario funcionario);                                      // Insere dados de um funcionário no arquivo
int alterarFuncionario(FILE *file, Funcionario funcionario_antigo, Funcionario funcionario_novo); // Alterar os dados de um funcionário do arquivo
int validarSenhaAdmin(char *senhaDigitada);                                                       // Valida a senha de administrador digitada pelo usuário
void validarSenhaCliente(FILE *file, Cliente cliente);                                            // Valida o usuario e senha de um cliente
void validarSenhaFuncionario(FILE *file, Funcionario funcionario);                                // Valida o usuario e senha de um funcionario
void saldo(Cliente cliente);                                                                      // Mostrar o saldo de um cliente.
void depositar(Cliente cliente);                                                                  // Depositar um valor na conta de um cliente.
void sacar(Cliente cliente);                                                                      // Sacar um valor da conta de um cliente.
int criarMovimento(Cliente cliente, char tipo[15], double valor);                                 // Cria uma movimentação na conta de um cliente.

// Função que cria uma movimentação no extrato do cliente.
int criarMovimento(Cliente cliente, char tipo[15], double valor)
{
  Transacao transacao;
  FILE *file;

  // Cria a pasta de movimentações caso ela não exista.
  mkdir("movimentos");

  // Salva o arquivo com o nome informado a cima em uma pasta movimento.
  char path[256];
  snprintf(path, 256, "movimentos/%s.txt", cliente.nome);

  // Tenta abrir o arquivo.
  file = fopen(path, "a+");

  // Verifica se o arquivo NÃO existe e tenta criar um.
  if (file == NULL)
  {
    // Tenta gerar um novo arquivo para o cliente caso ele não exista.
    file = fopen(path, "a+");
  }

  // Recebe o valor da transação.
  transacao.valor = valor;

  // Define o tipo de transação para saque
  strcpy(transacao.tipo, tipo);

  // Define o nome do cliente da transação com o nome do cliente que realizou a ação.
  strcpy(transacao.nome, cliente.nome);

  // Insere os dados no arquivo
  fprintf(file, "%s ", transacao.nome);
  fprintf(file, "%s ", transacao.tipo);
  fprintf(file, "%.2lf", transacao.valor);
  fprintf(file, "\n");

  // Fecha e salva os dados informados.
  fclose(file);

  // Declaração dos arquivos de origem e destino.
  FILE *origem, *destino;

  // Tenta abrir o arquivo de origem no modo de leitura.
  origem = fopen(path, "r");

  // Verifica se o arquivo NÃO EXISTE.
  if (origem == NULL)
  {
    printf("Nao foi possível abrir o arquivo de origem.\n");
    return -1;
  }

  // Tenta abrir o arquivo de destino no modo de reset.
  destino = fopen("extrato.txt", "w");

  // Verifica se o arquivo NÃO EXISTE.
  if (destino == NULL)
  {
    printf("Nao foi possível abrir o arquivo de destino.\n");
    return -1;
  }

  // Declaração da variável que receberá a linha que será copiada.
  char line[256];

  // Laço que irá receber todas as linhas do arquivo.
  while (fgets(line, sizeof(line), origem))
  {
    // Recebe todos os dados que estão inseridos na linha do arquivo.
    sscanf(line, "%s %s %f", transacao.nome, transacao.tipo, &transacao.valor);

    // Insere os dados recebidos no arquivo de origem para o arquivo de destino.
    fprintf(destino, "%s %s %.2f\n", transacao.nome, transacao.tipo, transacao.valor);
  }

  // Fecha os arquivos
  fclose(origem);
  fclose(destino);

  // Retorna 0 em caso de sucesso.
  return 0;
}

// Função que mostra o saldo do cliente.
void saldo(Cliente cliente)
{
  // Tenta abrir o arquivo de clientes.
  FILE *file = fopen("clientes.txt", "r+");

  // Verifica se o arquivo não existir e tenta gerar um novo.
  if (file == NULL)
  {
    printf("O arquivo de clientes nao foi encontrado. Tentando gerar um novo...\n");
    // Tenta gerar um novo arquivo caso não exista.
    file = fopen("clientes.txt", "w+");
  }

  // Variável que contem a posição do ponteiro do seek.
  int posicao;

  // Verifica se o cliente existe nos arquivos.
  if ((posicao = consultarCliente(file, cliente)) != -1)
  {
    // Movimenta o ponteiro de busca dentro do arquivo.
    fseek(file, posicao * sizeof(cliente), SEEK_SET);
    // Realiza a leitura do registro salvo dentro do arquivo.
    fread(&cliente, sizeof(cliente), 1, file);

    // Envia a resposta para o usuário.
    enviarTitulo();
    printf("Saldo atual em sua conta R$%.2f\n\n", cliente.saldo);
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
    system("cls");

    // Retorna o usuário para o menu de clientes ao pressionar qualquer tecla.
    enviarMenuCliente(file, cliente);
  }
}

// Função para depositar um valor na conta do cliente.
void depositar(Cliente cliente)
{
  // Tenta abrir o arquivo de clientes.
  FILE *file = fopen("clientes.txt", "r+");

  // Verifica se o arquivo não existir e tenta gerar um novo.
  if (file == NULL)
  {
    printf("O arquivo de clientes nao foi encontrado, tentando gerar um novo...\n");
    // Tenta gerar um novo arquivo caso não exista.
    file = fopen("clientes.txt", "w+");
  }

  // Variável que armazenará o valor informado pelo usuário.
  float valor;

  // Variável que armazenará a posição do cliente dentro do arquivo lido.
  int posicao;

  enviarTitulo();
  printf("Seu saldo atual R$%.2f\n", cliente.saldo);
  printf("Digite o valor que deseja depositar: \n");
  printf("R$");
  scanf("%f", &valor);
  system("cls");

  // Verifica a existência do cliente nos arquivos.
  if ((posicao = consultarCliente(file, cliente)) == -1)
  {
    enviarTitulo();
    printf("A sua conta esta com problema, tente novamente mais tarde.\n");
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
    system("cls");

    // Enviar o menu de cliente.
    enviarMenuCliente(file, cliente);
    return;
  }

  // Verifica se o valor informado pelo usuário é menor do que 0 e envia um erro.
  if (valor < 0)
  {
    enviarTitulo();
    printf("O valor deve ser maior que 0.\n");
    printf("Pressione qualquer tecla para tentar novamente...\n");
    getch();
    system("cls");
    enviarMenuCliente(file, cliente);
    return;
  }

  // Soma o valor que o cliente já possui com o valor informado.
  cliente.saldo += valor;

  // Movimenta o ponteiro até as informações encontradas do cliente.
  fseek(file, posicao * sizeof(cliente), SEEK_SET);

  // Escreve as novas informações sobre as antigas dentro do arquivo.
  fwrite(&cliente, sizeof(cliente), 1, file);

  // Fecha e salva o arquivo que foi aberto anteriormente.
  fclose(file);

  // Envia a resposta de sucesso ao usuário.
  criarMovimento(cliente, "Deposito", valor);
  enviarTitulo();
  printf("Voce realizou um deposito no valor de R$%.2f com sucesso.\n", valor);
  printf("Seu saldo atualizado: R$%.2f\n\n", cliente.saldo);
  printf("Pressione qualquer tecla para retornar ao menu...\n");
  getch();
  system("cls");

  // Ao pressionar qualquer tecla o usuário retorna ao menu de clientes.
  enviarMenuCliente(file, cliente);
}

// Função para sacar um valor da conta do cliente.
void sacar(Cliente cliente)
{
  // Tenta abrir o arquivo de clientes.
  FILE *file = fopen("clientes.txt", "r+");

  // Verifica se o arquivo não existir e tenta gerar um novo.
  if (file == NULL)
  {
    printf("O arquivo de clientes nao foi encontrado. Tentando gerar um novo...\n");
    // Tenta gerar um arquivo no modo de escrita e leitura.
    file = fopen("clientes.txt", "w+");
  }

  // Variável que receberá o valor desejado para o saque.
  float valor;

  // Variável que armazenará a posição que o cliente se encontra dentro do arquivo.
  int posicao;

  // Requisita que o usuário informe um valor para sacar.
  enviarTitulo();
  printf("Seu saldo atual R$%.2f\n", cliente.saldo);
  printf("Digite o valor que deseja sacar: \n");
  printf("R$");
  scanf("%f", &valor);
  system("cls");

  // Verifica a existência do cliente nos arquivos.
  if ((posicao = consultarCliente(file, cliente)) == -1)
  {
    enviarTitulo();
    printf("A sua conta esta com problema, tente novamente mais tarde.\n");
    printf("Pressione qualquer tecla para voltar ao menu...\n");
    getch();
    system("cls");

    // Enviar o menu de cliente.
    enviarMenuCliente(file, cliente);
    return;
  }

  // Verifica se o valor informado é menor do que 0 e envia um erro.
  if (valor < 0)
  {
    enviarTitulo();
    printf("O valor deve ser maior que 0.\n");
    printf("Pressione qualquer tecla para tentar novamente...\n");
    getch();
    system("cls");
    enviarMenuCliente(file, cliente);
    return;
  }

  // Verifica se o valor digitado é maior que o saldo do cliente e retorna um erro.
  if (valor > cliente.saldo)
  {
    enviarTitulo();
    printf("Voce nao possui saldo suficiente para sacar.\n");
    printf("Pressione qualquer tecla para tentar novamente...\n");
    getch();
    system("cls");
    enviarMenuCliente(file, cliente);
    return;
  }

  // Caso todas as condições sejam atendidas, o valor será sacado da conta do cliente.
  cliente.saldo -= valor;

  // Movimenta o ponteiro até as informações encontradas do cliente.
  fseek(file, posicao * sizeof(cliente), SEEK_SET);

  // Escreve as novas informações sobre as antigas dentro do arquivo.
  fwrite(&cliente, sizeof(cliente), 1, file);

  // Fecha e salva o arquivo de clientes.
  fclose(file);

  // Envia resposta ao usuário
  criarMovimento(cliente, "Saque", valor);
  enviarTitulo();
  printf("Voce realizou um saque no valor de R$%.2f com sucesso.\n", valor);
  printf("Seu saldo atualizado: R$%.2f\n\n", cliente.saldo);
  printf("Pressione qualquer tecla para retornar ao menu...\n");
  getch();
  system("cls");

  enviarMenuCliente(file, cliente);
}

// Função que realiza a consulta nos arquivos sobre um cliente
int consultarCliente(FILE *file, Cliente cliente)
{
  // Variável que será responsável por trazer os dados lidos dentro do arquivo.
  Cliente cliente_lido;

  // Variável que recebe a posição do dado encontrado dentro do arquivo.
  int posicao;

  // Verifica se o arquivo conseguiu ser aberto e executado com sucesso.
  if (file != NULL)
  {
    // Movimenta o ponteiro para o início do arquivo.
    fseek(file, 0L, SEEK_SET);

    // Define a posição inicial para 0
    posicao = 0;

    // Irá realizar a leitura no arquivo enquanto não chegar ao fim do arquivo ou encontrar o cliente especificado.
    while (fread(&cliente_lido, sizeof(cliente_lido), 1, file))
    {
      // Compara se o cliente que foi lido no arquivo é igual ao cliente que foi digitado pelo usuário.
      if (strcmpi(cliente_lido.nome, cliente.nome) == 0 &&
          (cliente_lido.excluido == 0))
        // Caso o cliente seja encontrado, será retornado a posição que ele se encontra dentro do arquivo.
        return posicao;
      // Caso não encontre, continuará aumentando a posição até chegar ao EoF do arquivo.
      posicao++;
    };
  }
  // Retorna erro caso não seja encontrado.
  return -1;
}

// Função que insere os dados do cliente em um arquivo
int inserirCliente(FILE *file, Cliente cliente)
{
  // Variável que será responsável por trazer os dados lidos dentro do arquivo.
  Cliente cliente_lido;

  // Variável que recebe a posição do dado encontrado dentro do arquivo.
  int posicao;

  // Verifica se o arquivo conseguiu ser aberto e executado com sucesso.
  if (file != NULL)
  {
    // Define a posição inicial da variável no início do arquivo (0).
    posicao = 0;

    // Verifica se o cliente existe dentro do arquivo de clientes.
    if (consultarCliente(file, cliente))
    {
      // Definindo o ponteiro de busca no início do arquivo
      fseek(file, 0L, SEEK_SET);

      // Vai rodar enquanto não chegar ao fim do arquivo
      while (fread(&cliente_lido, sizeof(cliente_lido), 1, file))
      {
        // Verifica se o cliente foi excluido do arquivo e trava a busca, retornando erro.
        if (cliente_lido.excluido == 1)
          break;
        // Caso o cliente não tenha sido excluido, a posição aumenta até chegar ao EoF.
        posicao++;
      };

      // Movimenta o ponteiro do fseek até chegar ao fim de uma única struct Cliente.
      fseek(file, posicao * sizeof(cliente), SEEK_SET);

      // Define a conta do cliente como excluido = false (remove a exclusão de conta se ela já existe anteriormente).
      cliente.excluido = 0;

      // Salva os dados fornecidos dentro do arquivo de clientes.
      fwrite(&cliente, sizeof(cliente), 1, file);

      // Envia a resposta de sucesso do programa.
      enviarTitulo();
      printf("O(a) cliente foi cadastrado(a) com sucesso.\n");
      printf("Pressione qualquer tecla para concluir e salvar...\n");
      getch();
      system("cls");

      // Fecha o arquivo aberto anteriormente.
      fclose(file);

      // Ao pressionar qualquer tecla o usuário será movido de volta ao menu de funcionários.
      enviarMenuFuncionario();
      // Retorna 0 em caso de sucesso.
      return 0;
    }
  }
  // Retorna -1 em caso de erro.
  return -1;
}

// Função que exclui um funcionário dos registros.
int excluirCliente(FILE *file, Cliente cliente)
{
  // Variável que irá receber a posição que o cliente se encontra dentro do arquivo.
  int posicao;

  // Verifica se o arquivo conseguiu ser executado e lido com sucesso.
  if (file != NULL)
  {
    // Verifica se o cliente existe no arquivo.
    if ((posicao = consultarCliente(file, cliente)) != -1)
    {
      // Movimenta o ponteiro de busca do arquivo para o tamanho da struct Cliente.
      fseek(file, posicao * sizeof(cliente), SEEK_SET);

      // Define o cliente como excluido.
      cliente.excluido = 1;

      // Insere a alteração de exclusão nos dados salvos do arquivo de clientes.
      fwrite(&cliente, sizeof(cliente), 1, file);

      // Envia a resposta do programa para o usuário.
      enviarTitulo();
      printf("Voce excluiu o cliente %s dos registros com sucesso.\n", cliente.nome);
      printf("Pressione qualquer tecla para finalizar e salvar...\n");
      getch();
      system("cls");

      // Fecha o arquivo aberto anteriormente.
      fclose(file);

      // Ao pressionar qualquer tecla o usuário será movido ao menu de funcionários.
      enviarMenuFuncionario();
      // Retorna 0 em caso de sucesso.
      return 0;
    }
  }
  // Retorna -1 em caso de erro.
  return -1;
}

// Função que altera os dados de um funcionário.
int alterarCliente(FILE *file, Cliente cliente_antigo, Cliente cliente_novo)
{
  // Variável que receberá a posição do cliente no arquivo.
  int posicao;

  // Verifica se o arquivo conseguiu ser aberto com sucesso.
  if (file != NULL)
  {
    // Realiza uma consulta para verificar se o cliente existe nos arquivos.
    if ((posicao = consultarCliente(file, cliente_antigo)) != -1)
    {
      // Movimenta o ponteiro do fseek para o tamanho de uma struct Cliente.
      fseek(file, posicao * sizeof(Cliente), SEEK_SET);

      // Realiza a leitura de uma struct Cliente inteira salva no arquivo.
      fread(&cliente_antigo, sizeof(cliente_antigo), 1, file);

      // Copia os dados contidos no registro antigo e envia para o novo
      strcpy(cliente_antigo.nome, cliente_novo.nome);
      cliente_antigo.agencia = cliente_novo.agencia;
      cliente_antigo.numDaConta = cliente_novo.numDaConta;
      strcpy(cliente_antigo.cpf, cliente_novo.cpf);
      cliente_antigo.nascimento.dia = cliente_novo.nascimento.dia;
      cliente_antigo.nascimento.mes = cliente_novo.nascimento.mes;
      cliente_antigo.nascimento.ano = cliente_novo.nascimento.ano;
      cliente_antigo.vencimento.dia = cliente_novo.vencimento.dia;
      cliente_antigo.vencimento.mes = cliente_novo.vencimento.mes;
      cliente_antigo.vencimento.ano = cliente_novo.vencimento.ano;

      strcpy(cliente_antigo.telefone, cliente_novo.telefone);
      strcpy(cliente_antigo.endereco.endereco, cliente_novo.endereco.endereco);
      strcpy(cliente_antigo.endereco.cep, cliente_novo.endereco.cep);
      strcpy(cliente_antigo.endereco.bairro, cliente_novo.endereco.bairro);
      strcpy(cliente_antigo.endereco.cidade, cliente_novo.endereco.cidade);
      strcpy(cliente_antigo.endereco.estado, cliente_novo.endereco.estado);

      cliente_antigo.saldo = cliente_novo.saldo;

      strcpy(cliente_antigo.senha, cliente_novo.senha);
      strcpy(cliente_antigo.tipoConta, cliente_novo.tipoConta);

      // Movimenta o ponteiro até as informações encontradas do cliente.
      fseek(file, posicao * sizeof(Cliente), SEEK_SET);

      // Escreve as novas informações sobre as antigas dentro do arquivo.
      fwrite(&cliente_antigo, sizeof(cliente_novo), 1, file);

      // Envia a resposta para o usuário.
      enviarTitulo();
      printf("Os dados deste cliente foram alterados com sucesso.\n\n");
      printf("Pressione qualquer tecla para finalizar.\n");
      getch();
      system("cls");

      // Fecha o arquivo e conclui o salvamento dos novos dados.
      fclose(file);

      // Envia o usuário para o menu de funcionários novamente.
      enviarMenuFuncionario();
      // Retorna 0 em caso de sucesso.
      return 0;
    }
  }
  // Retorna -1 em caso de erro.
  return -1;
}

// Função que insere os dados do funcionário em um arquivo
int inserirFuncionario(FILE *file, Funcionario funcionario)
{
  // Declaração da variável que receberá o funcionário que for lido no arquivo.
  Funcionario funcionario_lido;

  // Variável que guarda a posição onde o funcionário está dentro do arquivo.
  int posicao;

  if (file != NULL)
  {
    // Definindo o valor da posição igual a 0 (Ao início do arquivo).
    posicao = 0;

    // Procurar se a estrutura do funcionário existe no arquivo.
    if (consultarFuncionario(file, funcionario))
    {
      // Definindo o ponteiro de busca no início do arquivo.
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

      // Move o ponteiro para a posição que o funcionário se encontra.
      fseek(file, posicao * sizeof(funcionario), SEEK_SET);

      // Define o funcionário como excluido = 0 (false).
      funcionario.excluido = 0;

      // Insere os dados do funcionário dentro do arquivo.
      fwrite(&funcionario, sizeof(funcionario), 1, file);

      // Envia a resposta da inserção para o usuário.
      enviarTitulo();
      printf("O(a) funcionario(a) foi cadastrado(a) com sucesso.\n");
      printf("Pressione qualquer tecla para concluir e salvar...\n");
      getch();
      system("cls");

      // Fecha o arquivo aberto anteriormente.
      fclose(file);

      // Envia o menu de funcionários novamente para o usuário.
      enviarMenuFuncionario();

      // Retorna 0 em caso de sucesso.
      return 0;
    }
  }
  // Retorna -1 em caso de erros.
  return -1;
}

// Função que exclui um funcionário dos registros.
int excluirFuncionario(FILE *file, Funcionario funcionario)
{
  // Variável que receberá a posição do funcionário dentro dos arquivos.
  int posicao;

  // Verifica se o arquivo foi aberto e lido com sucesso.
  if (file != NULL)
  {
    // Verifica a existência do funcionário nos arquivos.
    if ((posicao = consultarFuncionario(file, funcionario)) != -1)
    {
      // Movimenta o ponteiro de busca pelo tamanho em bytes da struct Funcionário.
      fseek(file, posicao * sizeof(funcionario), SEEK_SET);

      // Define o funcionário como excluído = true.
      funcionario.excluido = 1;

      // Modifica os dados existentes do funcionário dentro do arquivo.
      fwrite(&funcionario, sizeof(funcionario), 1, file);

      // Envia a mensagem de sucesso para o usuário.
      enviarTitulo();
      printf("Voce excluiu este funcionario dos registros com sucesso.\n");
      printf("Pressione qualquer tecla para concluir e salvar...\n");
      getch();
      system("cls");

      // Fecha o arquivo aberto anteriormente.
      fclose(file);

      // Ao pressionar qualquer tecla o usuário será movido ao menu de funcionários.
      enviarMenuFuncionario();
      // Retorna 0 em caso de sucesso.
      return 0;
    }
  }
  // Retorna -1 em caso de erro.
  return -1;
}

// Função que altera os dados de um funcionário.
int alterarFuncionario(FILE *file, Funcionario funcionario_antigo, Funcionario funcionario_novo)
{
  // Variável que encontrará a posição do funcionário no arquivo.
  int posicao;

  // Verifica se o arquivo conseguiu ser aberto com sucesso.
  if (file != NULL)
  {
    // Verifica se o funcionário existe dentro dos arquivos.
    if ((posicao = consultarFuncionario(file, funcionario_antigo)) != -1)
    {
      // Movimenta o ponteiro de busca para onde o funcionário está localizado.
      fseek(file, posicao * sizeof(Funcionario), SEEK_SET);

      // Realiza a leitura dos dados da struct Funcionario salva no arquivo.
      fread(&funcionario_antigo, sizeof(funcionario_antigo), 1, file);

      // Atualiza os dados que estavam no arquivo pelos novos dados informados.
      strcpy(funcionario_antigo.nomeFuncionario, funcionario_novo.nomeFuncionario);
      strcpy(funcionario_antigo.cpf, funcionario_novo.cpf);
      funcionario_antigo.codigoFuncionario = funcionario_novo.codigoFuncionario;
      strcpy(funcionario_antigo.cargo, funcionario_novo.cargo);
      funcionario_antigo.nascimento.dia = funcionario_novo.nascimento.dia;
      funcionario_antigo.nascimento.mes = funcionario_novo.nascimento.mes;
      funcionario_antigo.nascimento.ano = funcionario_novo.nascimento.ano;
      strcpy(funcionario_antigo.telefoneContato, funcionario_novo.telefoneContato);
      strcpy(funcionario_antigo.senhaFuncionario, funcionario_novo.senhaFuncionario);
      strcpy(funcionario_antigo.endereco.endereco, funcionario_novo.endereco.endereco);
      strcpy(funcionario_antigo.endereco.cep, funcionario_novo.endereco.cep);
      strcpy(funcionario_antigo.endereco.bairro, funcionario_novo.endereco.bairro);
      strcpy(funcionario_antigo.endereco.cidade, funcionario_novo.endereco.cidade);
      strcpy(funcionario_antigo.endereco.estado, funcionario_novo.endereco.estado);

      // Movimenta o ponteiro até a localização do funcionário.
      fseek(file, posicao * sizeof(Funcionario), SEEK_SET);

      // Realiza a inseração dos novos dados sobre os dados antigos dentro do arquivo.
      fwrite(&funcionario_antigo, sizeof(funcionario_novo), 1, file);

      enviarTitulo();
      printf("Os dados deste funcionario foram alterados com sucesso.\n");
      getch();
      system("cls");

      // Fecha o arquivo e salva os dados que foram alterados.
      fclose(file);

      // Envia o menu de funcionários ao pressionar qualquer tecla.
      enviarMenuFuncionario();
      return 1;
    }
  }
  return 0;
}

// Função que realiza a consulta nos arquivos sobre um funcionário
int consultarFuncionario(FILE *file, Funcionario funcionario)
{
  // Variável que indica o funcionário lido dentro do arquivo.
  Funcionario funcionario_lido;

  // Posição do funcionário dentro do arquivo.
  int posicao;

  // Caso o arquivo tenha sido aberto e lido com sucesso
  if (file != NULL)
  {
    // Define o ponteiro de busca para o ínicio do código
    fseek(file, 0L, SEEK_SET);

    // Define a posição inicial para 0
    posicao = 0;

    // Enquanto todos os funcionários não forem lidos, o laço continuará rodando.
    while (fread(&funcionario_lido, sizeof(funcionario_lido), 1, file))
    {
      // Compara o nome do funcionário lido com o do funcionário especificado.
      if (strcmpi(funcionario_lido.nomeFuncionario, funcionario.nomeFuncionario) == 0 &&
          (funcionario_lido.excluido == 0))
        // Retorna a posição do funcionário dentro do arquivo.
        return posicao;
      // Enquanto não encontra, pula uma posição.
      posicao++;
    };
  }
  // Retorna -1 em caso de erros.
  return -1;
}

// Função para validar e liberar o acesso do cliente ao menu.
void validarSenhaCliente(FILE *file, Cliente cliente)
{
  // Variável que bloqueia ou libera o acesso do usuário.
  int acesso;

  // Variável que recebe a posição do funcionário dentro do arquivo.
  int posicao;

  // Variável que receberá a senha digitada pelo usuário.
  char password[DEFAULT_PASS_SIZE];

  do
  {
    // Bloqueando o acesso do usuário enquanto ele não valida a senha.
    acesso = 0;

    if ((posicao = consultarCliente(file, cliente)) != -1)
    {
      // Movimenta o ponteiro dentro do arquivo para a posição da struct Cliente procurada.
      fseek(file, posicao * sizeof(cliente), SEEK_SET);

      // Realiza a leitura da struct Cliente dentro do arquivo.
      fread(&cliente, sizeof(cliente), 1, file);

      // Envia a solicitação da senha para o usuário.
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

        // Envia informações do cliente e abre o menu de clientes.
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
  // Variável que bloqueia ou libera o acesso do usuário.
  int acesso;

  // Variável que determina a posição do funcionário dentro do arquivo.
  int posicao;

  // Declaração das variáveis de senhas.
  char password[DEFAULT_PASS_SIZE];
  char passwordAdm[DEFAULT_PASS_SIZE] = "adm";

  do
  {
    // Bloqueia o acesso do usuário até a validação da senha.
    acesso = 0;

    // Consulta a existência do funcionário nos arquivos.
    if ((posicao = consultarFuncionario(file, funcionario)) != -1)
    {
      // Movimenta o ponteiro de busca pelo registro de funcionário no arquivo.
      fseek(file, posicao * sizeof(funcionario), SEEK_SET);

      // Realiza a leitura da struct Funcionario especificado dentro do arquivo.
      fread(&funcionario, sizeof(funcionario), 1, file);

      enviarTitulo();
      printf("Digite a senha: \n");
      fflush(stdin);
      gets(password);
      system("cls");

      // Verifica a se a senha digitada é a mesma senha do funcionário informado.
      if (strcmp(password, funcionario.senhaFuncionario) == 0)
      {
        acesso = 1;
        enviarTitulo();
        printf("Autenticado(a) com sucesso.\n");
        system("cls");

        enviarMenuFuncionario();
        return;
      }
      // Verifica se a senha digitada é a mesma da senha de administrador.
      else if (strcmp(password, passwordAdm) == 0)
      {
        // Libera o acesso do usuário.
        acesso = 1;
        enviarTitulo();
        printf("Autenticado(a) utilizando a senha de administrador.\n");
        system("cls");

        enviarMenuFuncionario();
        return;
      }
      else
      {
        // Bloqueia o acesso do usuário.
        acesso = 0;
        enviarTitulo();
        printf("A senha digitada esta incorreta.\n");
        printf("Pressione qualquer tecla para tentar novamente...\n");
        getch();
        system("cls");

        // Ao pressionar qualquer tecla, o usuário retornará ao menu principal.
        enviarMenuPrincipal();
        return;
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

// Função que transforma a sigla em uma string do tipo de conta.
void formatarTipoConta(Cliente cliente)
{
  if (strcmp(cliente.tipoConta, "CP") == 0)
  {
    printf("Tipo de conta: Conta Poupanca\n");
  }
  else if (strcmp(cliente.tipoConta, "CC") == 0)
  {
    printf("Tipo de conta: Conta Corrente\n");
  }
  else
  {
    printf("Tipo de conta: Indefinida\n");
  }
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
  // Verifica se o arquivo foi aberto e executa o contexto.
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
      // Ver saldo
    case 1:
      // Requisita a função para visualizar o saldo da conta do cliente.
      saldo(cliente);
      break;

    // Depósitos
    case 2:
      // Requisita a função que realiza um depósito na conta do cliente.
      depositar(cliente);
      break;

    // Saques
    case 3:
      // Requisita a função que realiza um saque na conta do cliente.
      sacar(cliente);

      break;

    // Extrato
    case 4:
      // Envia a mensagem para o usuário
      enviarTitulo();
      printf("Realizando abertura do extrato da sua conta.\n");
      printf("Pressione qualquer tecla para voltar ao menu...\n");

      // Realiza a abertura do arquivo de extrato.
      system("start excel.exe extrato.xlsx");
      getch();
      system("cls");

      // Enviar o menu de clientes ao pressionar qualquer tecla.
      enviarMenuCliente(file, cliente);
      break;

      // Consultando limite
    case 5:
      enviarTitulo();

      // Verifica o tipo de conta do cliente e envia uma mensagem específica para cada caso.
      if (strcmp(cliente.tipoConta, "CC") == 0)
      {
        printf("Seu limite atual: %.2f\n\n", cliente.limiteDaConta);
        printf("Pressione qualquer tecla para retornar ao menu...\n");
      }
      else if (strcmp(cliente.tipoConta, "CP") == 0)
      {
        printf("Consultando os limites de %s\n\n", cliente.nome);
        printf("O seu tipo de conta nao possui limites.\n");
        printf("Solicite uma alteracao para conta corrente em caso de interesse.\n\n");
        printf("Pressione qualquer tecla para retornar ao menu...\n");
      }
      getch();
      system("cls");

      // Ao pressionar qualquer tecla o usuário será redirecionado ao menu de clientes.
      enviarMenuCliente(file, cliente);
      break;

    case 6:
      enviarMenuPrincipal();
      break;

    default:
      enviarTitulo();
      printf("Voce selecionou uma opcao invalida.\n");
      printf("Pressione qualquer tecla para tentar novamente...\n");
      getch();
      system("cls");

      enviarMenuCliente(file, cliente);
      break;
    }
  } while (opcao <= 0 || opcao > 6);
}

// Função que envia o menu de abertura de conta.
void enviarMenuAberturaConta()
{
  // Tenta realizar a abertura do arquivo de clientes.
  FILE *file = fopen("clientes.txt", "r+");

  // Declaração da struct de Clientes.
  Cliente cliente;

  // Verifica se o arquivo não foi aberto corretamente.
  if (file == NULL)
  {
    printf("Arquivo nao encontrado, tentando gerar um novo...\n");
    // Tenta criar o arquivo no modo de substituição, leitura e escrita.
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
        enviarTitulo();
        printf("Informe o nome do cliente: \n");
        fflush(stdin);
        gets(cliente.nome);
        system("cls");

        enviarTitulo();
        printf("Informe o numero da agencia do cliente: \n");
        scanf("%d", &cliente.agencia);
        system("cls");

        enviarTitulo();
        printf("Informe o numero da conta do cliente: \n");
        scanf("%d", &cliente.numDaConta);
        system("cls");

        enviarTitulo();
        printf("Informe o numero do CPF do cliente: \n");
        fflush(stdin);
        gets(cliente.cpf);
        system("cls");

        enviarTitulo();
        printf("Informe a data de nascimento do cliente seguindo o padrao: 30 10 2000\n");
        scanf("%d %d %d", &cliente.nascimento.dia, &cliente.nascimento.mes, &cliente.nascimento.ano);
        system("cls");

        enviarTitulo();
        printf("Informe o numero de telefone do cliente: \n");
        fflush(stdin);
        gets(cliente.telefone);
        system("cls");

        enviarTitulo();
        printf("Informe o endereco do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.endereco);
        system("cls");

        enviarTitulo();
        printf("Informe o CEP do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.cep);
        system("cls");

        enviarTitulo();
        printf("Informe o bairro do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.bairro);
        system("cls");

        enviarTitulo();
        printf("Informe a cidade do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.cidade);
        system("cls");

        enviarTitulo();
        printf("Informe o estado do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.estado);
        system("cls");

        enviarTitulo();
        printf("Informe o saldo inicial do cliente: \n");
        scanf("%f", &cliente.saldo);
        system("cls");

        enviarTitulo();
        printf("Confirme que esta criando uma conta poupanca \n");
        printf("Digite 'CP' no campo abaixo para continuar: \n");
        fflush(stdin);
        gets(cliente.tipoConta);
        system("cls");

        enviarTitulo();
        printf("Informe a senha do cliente: \n");
        fflush(stdin);
        gets(cliente.senha);
        system("cls");

        // Requisita a função que insere os dados fornecidos pelo usuário no arquivo de clientes.
        inserirCliente(file, cliente);
        break;

      case 2:
        enviarTitulo();
        printf("Informe o nome do cliente: \n");
        fflush(stdin);
        gets(cliente.nome);
        system("cls");

        enviarTitulo();
        printf("Informe o numero da agencia do cliente: \n");
        scanf("%d", &cliente.agencia);
        system("cls");

        enviarTitulo();
        printf("Informe o numero da conta do cliente: \n");
        scanf("%d", &cliente.numDaConta);
        system("cls");

        enviarTitulo();
        printf("Informe o limite da conta do cliente: \n");
        scanf("%f", &cliente.limiteDaConta);
        system("cls");

        enviarTitulo();
        printf("Informe o numero do CPF do cliente: \n");
        fflush(stdin);
        gets(cliente.cpf);
        system("cls");

        enviarTitulo();
        printf("Informe a data de nascimento do cliente seguindo o padrao: 30 10 2000\n");
        scanf("%d %d %d", &cliente.nascimento.dia, &cliente.nascimento.mes, &cliente.nascimento.ano);
        system("cls");

        enviarTitulo();
        printf("Informe a data de vencimento da conta do cliente seguindo o padrao: 30 10 2000\n");
        scanf("%d %d %d", &cliente.vencimento.dia, &cliente.vencimento.mes, &cliente.vencimento.ano);
        system("cls");

        enviarTitulo();
        printf("Informe o numero de telefone do cliente: \n");
        fflush(stdin);
        gets(cliente.telefone);
        system("cls");

        enviarTitulo();
        printf("Informe o endereco do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.endereco);
        system("cls");

        enviarTitulo();
        printf("Informe o CEP do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.cep);
        system("cls");

        enviarTitulo();
        printf("Informe o bairro do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.bairro);
        system("cls");

        enviarTitulo();
        printf("Informe a cidade do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.cidade);
        system("cls");

        enviarTitulo();
        printf("Informe o estado do cliente: \n");
        fflush(stdin);
        gets(cliente.endereco.estado);
        system("cls");

        enviarTitulo();
        printf("Informe o saldo inicial do cliente: \n");
        scanf("%f", &cliente.saldo);
        system("cls");

        enviarTitulo();
        printf("Confirme que esta criando uma conta corrente \n");
        printf("Digite 'CC' no campo abaixo para continuar: \n");
        fflush(stdin);
        gets(cliente.tipoConta);
        system("cls");

        enviarTitulo();
        printf("Informe a senha do cliente: \n");
        fflush(stdin);
        gets(cliente.senha);
        system("cls");

        // Requisita a função que insere os dados fornecidos pelo usuário no arquivo de clientes.
        inserirCliente(file, cliente);
        break;

      // Voltar
      case 3:
        // Envia o menu de funcionários para o usuário.
        enviarMenuFuncionario();
        break;

      default:
        enviarTitulo();
        printf("Voce selecionou uma opcao invalida.\n");
        printf("Pressione qualquer tecla para tentar novamente...\n");
        getch();
        system("cls");

        enviarMenuAberturaConta();
      }
      // Executa o código acima enquanto option não for (1,2 ou 3)
    } while (option <= 0 || option > 3);
  }
}

// Função que envia o menu de funcionários.
void enviarMenuFuncionario()
{
  // Declaração do arquivo para funcionarios e clientes.
  FILE *file, *fileClientes;

  // Variável que vai receber a opção desejada pelo usuário.
  Funcionario funcionario, funcionario_alterado;

  // Variável que vai receber a opção desejada pelo usuário.
  Cliente cliente, cliente_alterado;

  // Variável da opção que será digitada pelo usuário.
  int option, posicao;

  // Tenta abrir o arquivo no modo de leitura e escrita.
  file = fopen("funcionarios.txt", "r+");

  // Tenta abrir o arquivo no modo de leitura e escrita.
  fileClientes = fopen("clientes.txt", "r+");

  // Caso o arquivo não exista, o programa tentará criar um novo...
  if (file == NULL)
  {
    printf("Arquivo de funcionarios nao encontrado, recriando o arquivo...");
    // Tenta recriar o arquivo.
    file = fopen("funcionarios.txt", "w+");
  }

  // Caso o arquivo não exista, o programa tentará criar um novo...
  if (fileClientes == NULL)
  {
    printf("Arquivo de clientes nao encontrado, recriando o arquivo...");
    // Tenta criar um novo arquivo de clientes no modo de substituição, leitura e escrita.
    fileClientes = fopen("clientes.txt", "w+");
  }

  enviarTitulo();
  // Entra no laço de repetição enquanto o usuário não digitar uma opção.
  do
  {
    printf("Menu funcionario:\n\n");
    printf("1) Abertura de Conta\n"); // Falta terminar
    printf("2) Encerramento de Conta\n");
    printf("3) Consultar Dados\n"); // Consultar dados de clientes está com erro
    printf("4) Alterar Dados\n");   // Falta terminar
    printf("5) Cadastro de Funcionarios\n");
    printf("6) Sair\n\n");
    scanf("%d", &option);
    system("cls");

    // Realizar limpeza de buffer em cada arquivo.
    fflush(file);
    fflush(fileClientes);

    switch (option)
    {
    case 1:
      enviarMenuAberturaConta();
      break;

      // Encerramento de contas
    case 2:
      do
      {
        // Recebe a opção informada pelo usuário
        enviarTitulo();
        printf("Encerramento de conta\n\n");
        printf("1) Funcionario\n");
        printf("2) Cliente\n");
        printf("3) Voltar\n\n");
        scanf("%d", &option);
        system("cls");

        switch (option)
        {
          // Encerrar uma conta de funcionário
        case 1:
          enviarTitulo();
          printf("Digite o nome do funcionario que tera a conta encerrada: \n");
          fflush(stdin); // Limpa o buffer do teclado
          gets(funcionario.nomeFuncionario);
          system("cls");

          if ((posicao = consultarFuncionario(file, funcionario)) == -1)
          {
            // Envia a resposta da consulta para o usuário.
            enviarTitulo();
            printf("O funcionario informado nao existe...\n");
            printf("Pressione qualquer tecla para voltar ao menu...\n");
            getch();
            system("cls");

            // Ao pressionar qualquer tecla o usuário voltará ao menu de funcionários
            enviarMenuFuncionario();
            return;
          }

          // Requisita a função que exclui um funcionário
          excluirFuncionario(file, funcionario);
          break;

        // Encerrar uma conta de cliente
        case 2:
          enviarTitulo();
          printf("Digite o nome do cliente que tera a conta encerrada: \n");
          fflush(stdin); // Limpa o buffer do teclado
          gets(cliente.nome);
          system("cls");

          if ((posicao = consultarCliente(fileClientes, cliente)) == -1)
          {
            // Envia a resposta da consulta para o usuário.
            enviarTitulo();
            printf("O cliente informado nao existe...\n");
            printf("Pressione qualquer tecla para voltar ao menu...\n");
            getch();
            system("cls");

            // Ao pressionar qualquer tecla o usuário voltará ao menu de funcionários
            enviarMenuFuncionario();
            return;
          }

          // Requisita a função que exclui um cliente
          excluirCliente(fileClientes, cliente);
          break;

        case 3:
          // Retorna o usuário para o menu de funcionários.
          enviarMenuFuncionario();
          break;

        default:
          break;
        }
      } while (option <= 0 || option > 3);
      break;

      // Consulta de dados
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
          fflush(stdin); // Limpa o buffer do teclado.
          printf("Digite o nome do funcionario desejado: \n");
          // Recebe o nome do funcionário que foi informado.
          gets(funcionario.nomeFuncionario);
          system("cls");

          // Verifica se o funcionário existe.
          if ((posicao = consultarFuncionario(file, funcionario)) == -1)
          {
            // Envia a resposta do programa para o usuário.
            enviarTitulo();
            printf("O funcionario nao foi encontrado.\n");
            printf("Pressione qualquer tecla para voltar ao menu.\n");
            getch();
            system("cls");
            enviarMenuFuncionario();
          }

          // Caso o funcionário exista, ele irá executar o código.
          if ((posicao = consultarFuncionario(file, funcionario)) != -1)
          {
            // Movimenta o ponteiro do fseek até o local onde o funcionário está.
            fseek(file, posicao * sizeof(funcionario), SEEK_SET);

            // Realiza a leitura dos dados de onde o ponteiro que leu o funcionário está localizado.
            fread(&funcionario, sizeof(funcionario), 1, file);

            enviarTitulo();
            // Limpa o buffer do teclado.
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
            // Realiza uma verificação de qual a situação do funcionario e envia de acordo com o retorno.
            if (funcionario.excluido == 0)
            {
              printf("Status: Conta ativa\n");
            }
            else if (funcionario.excluido == 1)
            {
              printf("Status: Conta desativada.\n");
            }
            else
            {
              printf("Status: Erro.\n");
            }
            printf("Senha: %s\n", funcionario.senhaFuncionario);

            printf("\nPressione qualquer tecla para voltar ao menu...\n");
            getch();
            system("cls");

            // Envia o menu de funcionários ao pressionar qualquer tecla.
            enviarMenuFuncionario();
          }
          break;

        case 2:
          enviarTitulo();
          printf("Digite o nome do cliente desejado: \n");
          fflush(stdin); // Limpa o buffer do teclado
          gets(cliente.nome);
          system("cls");

          // Verifica se o cliente existe nos arquivos.
          if ((posicao = consultarCliente(fileClientes, cliente)) == -1)
          {
            enviarTitulo();
            printf("O cliente especificado nao foi encontrado...\n");
            printf("Pressione qualquer tecla para retornar ao menu.\n");
            getch();
            system("cls");
            // Ao pressionar qualquer tecla o usuário será redirecionado para o menu de funcionários.
            enviarMenuFuncionario();
            return;
          }

          // Caso o cliente exista ele irá executar o código.
          if ((posicao = consultarCliente(fileClientes, cliente)) != -1)
          {
            // Movimenta o ponteiro de busca até a posição onde o cliente se encontra.
            fseek(fileClientes, posicao * sizeof(cliente), SEEK_SET);

            // Realiza a leitura dos dados de onde o ponteiro de busca está localizado.
            fread(&cliente, sizeof(cliente), 1, fileClientes);

            enviarTitulo();
            printf("Mostrando informacoes do(a) cliente %s:\n\n", cliente.nome);
            // Essa função formata a sigla do tipo de conta para um texto maior.
            formatarTipoConta(cliente); // Enviar tipo de conta do cliente.
            printf("Agencia: %d\n", cliente.agencia);
            printf("Numero da Conta: %d\n", cliente.numDaConta);
            // Verifica se o tipo de conta é corrente para enviar o limite da conta
            if (strcmp(cliente.tipoConta, "CC") == 0)
            {
              printf("Limite da Conta: R$%.2f\n", cliente.limiteDaConta);
            }
            printf("CPF: %s\n", cliente.cpf);

            printf("Data de Nascimento: %d/%d/%d\n", cliente.nascimento.dia, cliente.nascimento.mes,
                   cliente.nascimento.ano);
            // Verifica se o tipo de conta é corrente para enviar o vencimento
            if (strcmp(cliente.tipoConta, "CC") == 0)
            {
              printf("Data de Vencimento: %d/%d/%d\n", cliente.nascimento.dia, cliente.nascimento.mes,
                     cliente.nascimento.ano);
            }
            printf("Telefone: %s\n", cliente.telefone);
            printf("Endereco: %s, %s, %s, %s, %s\n", cliente.endereco.endereco, cliente.endereco.cep,
                   cliente.endereco.bairro, cliente.endereco.cidade,
                   cliente.endereco.estado);
            printf("Saldo: R$%.2f\n", cliente.saldo);

            // Realiza uma verificação de qual a situação do cliente e envia de acordo com o retorno.
            if (cliente.excluido == 0)
            {
              printf("Status: Conta ativa\n");
            }
            else if (cliente.excluido == 1)
            {
              printf("Status: Conta desativada.\n");
            }
            else
            {
              printf("Status: Erro.\n");
            }
            printf("Senha: %s\n\n", cliente.senha);

            printf("Pressione qualquer tecla para retornar ao menu...\n");
            getch();
            system("cls");
            enviarMenuFuncionario();
          }
          break;

        default:
          enviarTitulo();
          printf("Voce selecionou uma opcao invalida.\n");
          printf("Pressione qualquer tecla para tentar novamente...\n");
          getch();
          system("cls");

          enviarMenuFuncionario();
          break;
        }
      } while (option <= 0 || option > 3);

      getch();
      break;

      // Alteração de dados
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

        char password[DEFAULT_PASS_SIZE];

        switch (option)
        {
        case 1:
          // Criar funcionalidade que altera o tipo de conta de um cliente.
          // Conta Poupança = CP | Conta Corrente = CC
          break;

          // Alterar dados de um funcionário.
        case 2:

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
              printf("A senha digitada esta incorreta\n\n");
              printf("Pressione qualquer tecla para retornar ao menu...\n");
              getch();
              system("cls");

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

              if ((posicao = consultarFuncionario(file, funcionario)) == -1)
              {
                enviarTitulo();
                printf("O funcionario especificado nao foi encontrado.\n");
                printf("Pressione qualquer tecla para voltar ao menu...\n");
                getch();
                system("cls");

                // Enviar o menu de funcionários.
                enviarMenuFuncionario();
                return;
              }

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
              printf("Digite a nova cidade do funcionario: \n");
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

        // Alterar dados de um cliente.
        case 3:
          do
          {
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
              printf("A senha digitada esta incorreta.\n");
              printf("Pressione qualquer tecla para tentar novamente.\n");
              getch();
              system("cls");
              enviarMenuFuncionario();
            }

            // Se a senha digitada for a senha correta, o usuario recebe o menu de funcionários.
            if (validarSenhaAdmin(password) == 0)
            {
              enviarTitulo();
              printf("Digite o nome do cliente que tera os dados alterados: \n");
              fflush(stdin);
              gets(cliente.nome);
              system("cls");

              if ((posicao = consultarCliente(fileClientes, cliente)) == -1)
              {
                enviarTitulo();
                printf("O cliente especificado nao foi encontrado.\n");
                printf("Pressione qualquer tecla para voltar ao menu...\n");
                getch();
                system("cls");

                // Enviar o menu de funcionários.
                enviarMenuFuncionario();
                return;
              }

              fseek(fileClientes, posicao * sizeof(cliente), SEEK_SET);
              fread(&cliente, sizeof(cliente), 1, fileClientes);

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

              if (strcmp(cliente.tipoConta, "CC") == 0)
              {
                enviarTitulo();
                printf("Digite o novo limite da conta: \n");
                scanf("%f", &cliente_alterado.limiteDaConta);
                system("cls");
              }

              enviarTitulo();
              printf("Digite o novo numero de CPF: \n");
              fflush(stdin);
              gets(cliente_alterado.cpf);
              system("cls");

              enviarTitulo();
              printf("Digite o dia, mes e ano da data de nascimento nova do cliente - 11 22 3333: \n");
              scanf("%d %d %d", &cliente_alterado.nascimento.dia, &cliente_alterado.nascimento.mes, &cliente_alterado.nascimento.ano);
              system("cls");

              if (strcmp(cliente.tipoConta, "CC") == 0)
              {
                enviarTitulo();
                printf("Digite o dia, mes e ano da data de vencimento nova do cliente - 11 22 3333: \n");
                scanf("%d %d %d", &cliente_alterado.vencimento.dia, &cliente_alterado.vencimento.mes, &cliente_alterado.vencimento.ano);
                system("cls");
              }

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
              printf("O cliente possui R$%.2f em sua conta, digite o mesmo valor para manter ou outro valor para alterar: \n", cliente.saldo);
              scanf("%f", &cliente_alterado.saldo);
              system("cls");

              enviarTitulo();
              printf("Digite a nova senha do cliente: \n");
              fflush(stdin);
              gets(cliente_alterado.senha);
              system("cls");

              enviarTitulo();
              printf("Digite o novo tipo de conta do cliente: \n");
              printf("Utilize as siglas: Conta Poupanca (CP) e Conta Corrente (CC): \n");
              fflush(stdin);
              gets(cliente_alterado.tipoConta);
              system("cls");

              // Envia os dados coletados para a função que altera os dados de um cliente.
              alterarCliente(fileClientes, cliente, cliente_alterado);
            }

          } while (acesso == 0);
          break;

        default:
          enviarTitulo();
          printf("Voce selecionou uma opcao invalida.\n");
          printf("Pressione qualquer tecla para tentar novamente...\n");
          getch();
          system("cls");

          enviarMenuFuncionario();
        }
      } while (option <= 0 || option > 4);

      break;

      // Cadastrar funcionário
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
        // Limpa o buffer do teclado
        fflush(stdin);
        // Recebe a senha do funcionário em string.
        gets(funcionario.senhaFuncionario);
        // Limpa últimas mensagens enviadas.
        system("cls");

        if (strlen(funcionario.senhaFuncionario) == 0)
        {
          printf("Voce deve digitar uma senha para continuar.\n");
          system("cls");
        }
        else if (strlen(funcionario.senhaFuncionario) > 16)
        {
          printf("A senha excede o limite de 16 caracteres.\n");
          printf("Pressione qualquer tecla para tentar novamente...\n");
          // Limpa o buffer do teclado.
          fflush(stdin);
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
      printf("Voce selecionou uma opcao invalida.\n");
      printf("Pressione qualquer tecla para tentar novamente...\n");
      getch();
      system("cls");

      enviarMenuFuncionario();
      break;
    }

  } while (option <= 0 || option > 7);

  fclose(file);
}

// Função para enviar o menu principal.
void enviarMenuPrincipal()
{
  // Inicialização do arquivo e do registro.
  FILE *file;
  Funcionario funcionario;

  // Inicialização do arquivo e do registro.
  FILE *fileCliente;
  Cliente cliente;

  // Variáveis de ação do programa.
  int option, posicao;

  // Tenta realizar a abertura do arquivo de funcionários e de clientes no modo leitura/escrita.
  file = fopen("funcionarios.txt", "r+");
  fileCliente = fopen("clientes.txt", "r+");

  // Verifica se o arquivo de funcionários não existe.
  if (file == NULL)
  {
    printf("Arquivo de funcionarios nao foi encontrado, recriando o arquivo...\n");
    file = fopen("funcionarios.txt", "w+");
  }

  // Verifica se o arquivo de clientes não existe.
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

      // Recebe o nome do funcionário para comparação no arquivo.
      enviarTitulo();
      printf("Digite o nome do funcionario: \n");
      // Limpa o buffer do teclado
      fflush(stdin);
      // Recebe a string digitada pelo usuário contendo o nome do funcionário.
      gets(funcionario.nomeFuncionario);
      // Limpa as últimas mensagens enviadas.
      system("cls");

      // Verifica se o nome do funcionário informado é "admin". Usuário administrativo.
      if (strcmp(funcionario.nomeFuncionario, "admin") == 0)
      {
        // Envia o menu de funcionários sem senha caso o usuário seja administrador.
        char password[DEFAULT_PASS_SIZE];

        enviarTitulo();
        printf("Digite a senha de administrador: \n");
        // Limpa o buffer do teclado.
        fflush(stdin);
        // Recebe a senha digitada pelo usuário.
        gets(password);
        system("cls");

        // Verifica se caso a senha foi digitada corretamente.
        if (validarSenhaAdmin(password) == 0)
        {
          // O acesso é liberado e o usuário será direcionado ao menu de funcionário.
          enviarMenuFuncionario();
        }
        // Verifica se caso a senha foi digitada incorretamente.
        else if (validarSenhaAdmin(password) != 0)
        {
          enviarTitulo();
          printf("A senha informada nao e valida.\n\n");
          printf("Pressione qualquer tecla para voltar ao menu...\n");
          getch();
          system("cls");

          // Ao pressionar qualquer tecla o usuário é movido para o menu principal novamente.
          enviarMenuPrincipal();
        }

        return;
      }

      // Verifica a existência do funcionário dentro do arquivo.
      if ((posicao = consultarFuncionario(file, funcionario)) == -1)
      {
        enviarTitulo();
        printf("O funcionario informado nao existe.\n");
        printf("Pressione qualquer tecla para tentar novamente...\n");
        // Realiza uma pausa até que o usuário pressione qualquer tecla.
        getch();
        system("cls");

        // Ao pressionar qualquer tecla o usuário será movido de volta ao menu principal.
        enviarMenuPrincipal();
        return;
      }

      // Requisita a função para validação da senha do funcionário.
      validarSenhaFuncionario(file, funcionario);
      break;

    case 2:
      enviarTitulo();
      printf("Digite o nome do cliente: \n");
      // Limpa o buffer do teclado
      fflush(stdin);
      // Recebe o nome do cliente em string.
      gets(cliente.nome);
      system("cls");

      // Verifica a existência do cliente nos arquivos.
      if ((posicao = consultarCliente(fileCliente, cliente)) == -1)
      {
        // Envia a resposta do programa para o usuário.
        enviarTitulo();
        printf("O cliente informado nao existe.\n");
        printf("Pressione qualquer tecla para tentar novamente...\n");
        getch();
        system("cls");

        // Ao pressionar qualquer tecla o usuário será movido de volta ao menu principal.
        enviarMenuPrincipal();
        return;
      }

      // Requisita a função para validação da senha do cliente.
      validarSenhaCliente(fileCliente, cliente);
      break;

    case 3:
      enviarTitulo();
      printf("Saindo do programa...");
      // Encerra o programa retornando 1.
      exit(1);
      system("cls");
      break;

      // Caso o usuario digite algo que não seja aceito ou que não exista
    default:
      enviarTitulo();
      printf("Voce selecionou uma opcao invalida.\n");
      printf("Pressione qualquer tecla para tentar novamente...\n");
      getch();
      system("cls");

      // Ao pressionar qualquer tecla o usuário será redirecionado ao menu principal.
      enviarMenuPrincipal();
      break;
    }

    // Fecha os arquivos que foram abertos anteriormente.
    fclose(file);
    fclose(fileCliente);
    // O bloco de código acima será executado enquanto a opção não for (1, 2 ou 3)
  } while (option <= 0 || option > 3);

  // Fecha os arquivos que foram abertos anteriormente.
  fclose(file);
  fclose(fileCliente);
}