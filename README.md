# Banco Malvader

Neste repositório você irá encontrar o código fonte do projeto do banco malvader. Desenvolvido para a matéria de lógica
de programação.

> [!TIP]
> A documentação do projeto ainda está sendo atualizada, certifique-se de seguir todas as informações que estão nela
> para garantir um bom funcionamento do código e do projeto.

## Documentação

A documentação do projeto irá explicar como utilizar algumas funções criadas para o sistema.

### Como utilizar a função solicitarSenhaFuncionario(int tipoDeMenu):

```js
// Requisita a função passando o parâmetro sendo 1.
solicitarSenhaFuncionario(1);
// O valor 1 como parâmetro significa que você está requisitando o envio do menu de funcionários.
```

### Como utilizar a função validarSenhaAdmin(char *senhaDigitada):

Também será mostrado como captar a senha digitada pelo usuário e armazená-la em uma variável.

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

- A documentação ainda está sendo desenvolvida...

> [!WARNING]
> Qualquer problema encontrado no projeto deve ser reportado na aba **Issues**!