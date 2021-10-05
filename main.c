#include <stdio.h>
#include "almoxerifado.h"

char cmd = '0', last;
char (*action)(char);
int main(int argc, char *argv[])
{
  action = &menu;
  while (cmd != 'q') {
    last = cmd;
    cmd = action(last);
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
    }
  }
  return 0;
}
