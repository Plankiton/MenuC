#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "almoxerifado.h"

FILE * cache;

char menu(char prev) {
  system("clear");
  printf(
      "---------------------------------\n"
      " 1 - Cadastrar item\n"
      " 2 - Visualizar item\n"
      " 3 - Editar item\n"
      " 4 - Excluir item\n"
      " 5 - Visualizar todos os items\n"
      " 6 - Abrir/Salvar arquivo\n"
      "\n:"
      );

  char c;
  if (strchr("123456", (c = getchar())))
    return c;
  return 'q';
}

typedef struct {
  unsigned int cod;
  unsigned char nome[15];
  unsigned char desc[30];
  unsigned int qtd;
} Item;

char cadastro(char prev) {
  cache = fopen(".cache", "ab");

  Item i;
  memset(&i, 0, sizeof (Item));

  printf("Digite o nome do item: ");
  scanf("%s", i.nome);
  getchar();

  printf("Deseja cadastrar descrição? [s/N] ");
  if (getchar() == 's') {
    printf("Digite a descrição do item: ");
    scanf("%s", i.desc);
    getchar();
  }

  printf("Digite a quantidade de itens: ");
  scanf("%i", &i.qtd);
  getchar();


  fseek(cache, 0, SEEK_END);
  int len = ftell(cache);
  if (len % sizeof(Item) != 0) {
    fclose(cache);
    cache = fopen(".cache", "wb");
  }

  if (len >= sizeof(Item))
    i.cod = (len/sizeof(Item));
  else i.cod = 0;

  fwrite(&i, 1, sizeof (Item), cache);

  fclose(cache);
  return '0';
}

char ver(char prev) {
  cache = fopen(".cache", "rb");

  Item i;
  memset(&i, 0, sizeof (Item));

  fflush(stdout);
  fflush(stdin);
  printf("Digite o codigo do item que quer consultar: ");
  scanf("%i", &i.cod);
  getchar();

  fseek(cache, i.cod*sizeof(Item), SEEK_SET);
  fread(&i, 1, sizeof (Item), cache);
  fclose(cache);

  printf("%i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (i.desc[0])
    printf("  > %s\n", i.desc);

  getchar();
  return '0';
}
