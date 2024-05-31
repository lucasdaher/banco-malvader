# Banco Malvader

Neste repositório você irá encontrar o código fonte do projeto do banco malvader. Desenvolvido para a matéria de lógica
de programação.

> [!TIP]
> A documentação do projeto ainda está sendo atualizada, certifique-se de seguir todas as informações que estão nela
> para garantir um bom funcionamento do código e do projeto.

## Documentação

A documentação do projeto irá explicar como utilizar algumas funções criadas para o sistema.

### Solicitando senha dos funcionários

```c
// Requisita a função passando o parâmetro sendo 1.
solicitarSenhaFuncionario(1);
// O valor 1 como parâmetro significa que você está requisitando o envio do menu de funcionários.
```

### Validando a senha de administrador

```c
// Crie uma variável para receber a senha digitada pelo usuário.
char senhaDigitada[16]; // o tamanho máximo da senha é 16 no sistema do banco.

// Adicione o método para receber a senha digitada pelo usuário
printf("Digite a senha: \n");
fflush(stdin); // Realiza a limpeza do buffer do teclado.
gets(senhaDigitada); // Recebe a string digitada pelo usuário e armazena na variável.

// Faz uma verificação se a senha for ou não validada utilizando a função.
if (validarSenhaAdmin(senhaDigitada, "Coloque aqui a senha que deve ser aceita") != 0) {
// Você pode utilizar uma variável ou um arquivo no lugar da senha que deve ser aceita
printf("A senha digitada está incorreta.\n");
// Retorna o código e para a sua execução.
return;
}

// Adicione o código para caso a senha seja autenticada deste local para baixo.
```

### Validando a senha de clientes

```c
// Requisita a função passando o parâmetro sendo a variável que 
// receberá a senha que foi digitada pelo usuário.
char senhaDigitada;

// Requisita a função para validar a senha digitada.
validarSenhaCliente(&senhaDigitada);
// O parâmetro deve receber um endereço de memória por se tratar de um ponteiro.
```

### Solicitando a senha para o cliente
```c
int acesso = 0;

  do
  {
    acesso = 0;

    // Verifica se a senha do usuário não for validada.
    if (validarSenhaCliente(&password) == 1)
    {
      return;
    }

    acesso = 1;
    enviarMenuCliente();

// Enquanto o acesso não for autorizado, o usuário terá que
// digitar a senha da sua conta.
  } while (acesso == 0);
```

### Solicitando senha dos funcionários

```c
// Requisita a função passando o parâmetro sendo 1.
solicitarSenhaFuncionario(1);
// O valor 1 como parâmetro significa que você está requisitando o envio do menu de funcionários.
```

> [!WARNING]
> Qualquer problema encontrado no projeto deve ser reportado na aba **Issues**!