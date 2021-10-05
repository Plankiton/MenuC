#include <stdio.h>
#include "almoxerifado.h"

char cmd = '0';
char (*action)(void);
int main(int argc, char *argv[])
{
  setup();
  action = &menu;
  while (cmd != 'q') {
    cmd = action();
    switch (cmd) {
      case '0':
        action = &menu;
        break;
      case '1':
        action = &cadastro;
        break;
      case '2':
        action = &ver;
        break;
      case '3':
        action = &editar;
        break;
      case '4':
        action = &excluir;
        break;
      case '5':
        action = &listar;
        break;
      case '6':
        action = &listar;
        break;
    }
  }
  return 0;
}
