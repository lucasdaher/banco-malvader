// Requisitando a biblioteca contendo as funcionalidades do banco.
#include "malvader.h"

int main()
{
  // Definindo linguagem padrão do programa para o mesmo do sistema operacional.
  setlocale(LC_ALL, "");

  // Envia o menu principal assim que o programa roda.
  enviarMenuPrincipal();

  // Retorna 0 por padrão.
  return 0;
}