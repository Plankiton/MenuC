#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "almoxerifado.h"

const char default_file [] = ".session";
char file[50];
FILE * cache;

void setup() {
  memcpy(file, default_file, sizeof(default_file));
}

char menu(void) {
  system("clear");
  printf(
      " Almoxerifado\n"
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

char cadastro(void) {
  cache = fopen(file, "ab");

  Item i;
  memset(&i, 0, sizeof (Item));

  getchar();
  printf(" Digite o nome do item: ");
  scanf("%14[0-9a-zA-Z ]", i.nome);
  getchar();

  printf(" Deseja cadastrar descrição? [s/N] ");
  if (getchar() == 's') {
    getchar();
    printf(" Digite a descrição do item: ");
    scanf("%29[0-9a-zA-Z ]", i.desc);
    getchar();
  }

  printf(" Digite a quantidade de itens: ");
  scanf("%i", &i.qtd);
  getchar();

  fseek(cache, 0, SEEK_END);
  int len = ftell(cache);

  if (len >= sizeof(Item))
    i.cod = (len/sizeof(Item));
  else i.cod = 0;

  fwrite(&i, 1, sizeof (Item), cache);

  puts(" O item escolhido ficará assim: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.desc) >= 1)
    printf("  > %s\n", i.desc);
  getchar();

  fclose(cache);
  return '0';
}

char ver(void) {
  Item i;
  memset(&i, 0, sizeof (Item));

  fflush(stdout);
  fflush(stdin);
  printf(" Digite o codigo do item que quer consultar: ");
  scanf("%i", &i.cod);
  getchar();

  cache = fopen(file, "rb");
  fseek(cache, 0, SEEK_END);
  int len = ftell(cache);
  int curr_cod = i.cod;
  int real_cod = i.cod*sizeof(Item);
  if (real_cod >= len) {
    puts(" Item não existe");
    getchar();
    return '0';
  }

  fseek(cache, real_cod, SEEK_SET);
  fread(&i, 1, sizeof (Item), cache);
  i.cod = curr_cod;
  fclose(cache);

  puts(" O item escolhido foi: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.desc) >= 1)
    printf("  > %s\n", i.desc);

  getchar();
  return '0';
}

char editar(void) {
  Item i;
  memset(&i, 0, sizeof (Item));

  fflush(stdout);
  fflush(stdin);
  printf(" Digite o codigo do item que quer editar: ");
  scanf("%i", &i.cod);
  getchar();

  cache = fopen(file, "rb");
  fseek(cache, 0, SEEK_END);
  int len = ftell(cache);
  int curr_cod = i.cod;
  int real_cod = i.cod*sizeof(Item);
  if (real_cod >= len) {
    puts(" Item não existe");
    getchar();
    return '0';
  }

  fseek(cache, real_cod, SEEK_SET);
  fread(&i, 1, sizeof (Item), cache);
  i.cod = curr_cod;
  fclose(cache);

  puts(" O item escolhido foi: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.desc) >= 1)
    printf("  > %s\n", i.desc);

  printf(
      " Deseja editar qual dado?\n"
      " 1 - Nome\n"
      " 2 - Quantidade\n"
      " 3 - Descrição\n"
      "\n:"
      );
  switch (getchar()) {
    case '1':
      getchar();
      printf(" Digite o nome do item: ");
      scanf("%14[0-9a-zA-Z ]", i.nome);
      getchar();
      break;
    case '2':
      getchar();
      printf(" Digite a quantidade de itens: ");
      scanf("%i", &i.qtd);
      getchar();
      break;

    case '3':
      getchar();
      printf(" Digite a descrição do item: ");
      scanf("%29[0-9a-zA-Z ]", i.desc);
      getchar();
      break;
  }

  puts(" O item escolhido ficará assim: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.desc) >= 1)
    printf("  > %s\n", i.desc);

  printf(" Deseja salvar os dados? [S/n] ");
  if (getchar() != 'n') {
       // Abre arquivo e pega o tamanho final
    cache = fopen(file, "rb");
    fseek(cache, 0, SEEK_END);
    int cache_size = ftell(cache);
    fseek(cache, 0, SEEK_SET);

    FILE *tmp = fopen(TMP_FILE, "wb");
    while (ftell(cache) < cache_size) {
      // Carrega item do cache em variavel
      Item item_content;
      memset(&item_content, 0, sizeof (Item));
      fread(&item_content, 1, sizeof (Item), cache);

      // Modifica item escolhido
      // no arquivo temporario
      if (item_content.cod == curr_cod) {
        memcpy(&item_content, &i, sizeof (Item));
      }
      fwrite(&item_content, 1, sizeof (Item), tmp);
    }

    fclose(tmp);
    fclose(cache);

    // Abre arquivo e pega o tamanho final
    tmp = fopen(TMP_FILE, "rb");
    fseek(tmp, 0, SEEK_END);
    int tmp_size = ftell(tmp);
    fseek(tmp, 0, SEEK_SET);

    int counter = 0;
    cache = fopen(file, "wb");
    fseek(cache, 0, SEEK_SET);
    while (ftell(tmp) < tmp_size) {
      // Carrega item do arquivo temporario em variavel
      Item item_content;
      memset(&item_content, 0, sizeof (Item));
      fread(&item_content, 1, sizeof (Item), tmp);
      item_content.cod = counter++;

      // Movendo do arquivo temporario para o cache
      fwrite(&item_content, 1, sizeof (Item), cache);
    }
    fclose(cache);
    fclose(tmp);

    cache = fopen(file, "rb");
    fclose(cache);

    puts(" Salvo com sucesso!");
    getchar();
  }

  return '0';
}

char excluir(void) {
  Item i;
  memset(&i, 0, sizeof (Item));

  fflush(stdout);
  fflush(stdin);
  printf(" Digite o codigo do item que quer editar: ");
  scanf("%i", &i.cod);
  getchar();

  cache = fopen(file, "rb");
  fseek(cache, 0, SEEK_END);
  int len = ftell(cache);
  int curr_cod = i.cod;
  int real_cod = i.cod*sizeof(Item);
  if (real_cod >= len) {
    puts(" Item não existe");
    getchar();
    return '0';
  }

  fseek(cache, real_cod, SEEK_SET);
  fread(&i, 1, sizeof (Item), cache);
  i.cod = curr_cod;
  fclose(cache);

  puts(" O item escolhido foi: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.desc) >= 1)
    printf("  > %s\n", i.desc);

  printf(" Deseja mesmo apagar o item? [S/n] ");
  if (getchar() != 'n') {
    // Abre arquivo e pega o tamanho final
    cache = fopen(file, "rb");
    fseek(cache, 0, SEEK_END);
    int cache_size = ftell(cache);
    fseek(cache, 0, SEEK_SET);

    FILE *tmp = fopen(TMP_FILE, "wb");
    while (ftell(cache) < cache_size - 1) {
      // Carrega item do cache em variavel
      Item item_content;
      memset(&item_content, 0, sizeof (Item));
      fread(&item_content, 1, sizeof (Item), cache);

      // Modifica item escolhido e salva items não excluidos
      // no arquivo temporario
      if (item_content.cod != curr_cod) {
        fwrite(&item_content, 1, sizeof (Item), tmp);
      }
    }

    fclose(tmp);
    fclose(cache);

    // Abre arquivo e pega o tamanho final
    tmp = fopen(TMP_FILE, "rb");
    fseek(tmp, 0, SEEK_END);
    int tmp_size = ftell(tmp);
    fseek(tmp, 0, SEEK_SET);

    int counter = 0;
    cache = fopen(file, "wb");
    fseek(cache, 0, SEEK_SET);
    while (ftell(tmp) < tmp_size) {
      // Carrega item do arquivo temporario em variavel
      Item item_content;
      memset(&item_content, 0, sizeof (Item));
      fread(&item_content, 1, sizeof (Item), tmp);
      item_content.cod = counter++;

      // Movendo do arquivo temporario para o cache
      fwrite(&item_content, 1, sizeof (Item), cache);
    }
    fclose(cache);
    fclose(tmp);

    cache = fopen(file, "rb");
    fclose(cache);

    puts(" Apagado com sucesso!");
    getchar();
  }
  return '0';
}

char listar(void) {
  Item i;

  // Abre arquivo e pega o tamanho final
  cache = fopen(file, "rb");
  if (!cache) {
    puts(" Você ainda não registrou nada!");
    getchar();
    getchar();
    return '0';
  }

  fseek(cache, 0, SEEK_END);
  int cache_size = ftell(cache);
  fseek(cache, 0, SEEK_SET);
  int counter = 0;
  while (ftell(cache) < cache_size - 1) {
    memset(&i, 0, sizeof (Item));
    fread(&i, 1, sizeof (Item), cache);
    i.cod = counter++;

    // Exibindo item
    printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
    if (strlen((char *)i.desc) >= 1)
      printf("  > %s\n", i.desc);
    puts("");
  }

  if (counter == 0) {
    puts(" Você ainda não registrou nada!");
    getchar();
    getchar();
    return '0';
  }

  fclose(cache);
  getchar();
  getchar();
  return '0';
}

char abrir(void) {
  getchar();
  printf(" Digite o nome do arquivo: ");
  scanf("%14[0-9a-zA-Z\\.\\_ ]", (char *)file);
  getchar();

  printf(
      " Deseja fazer o que?\n"
      " 1 - Salvar (vai sobrescrever se o arquivo existir)\n"
      " 2 - Abrir\n"
      "\n:"
      );

  if (getchar() == '1') {
      FILE * tmp = fopen(default_file, "rb");
      fseek(tmp, 0, SEEK_END);
      int tmp_size = ftell(tmp);
      fseek(tmp, 0, SEEK_SET);

      cache = fopen(file, "wb");
      fseek(cache, 0, SEEK_SET);
      while (ftell(tmp) < tmp_size) {
        // Carrega item do arquivo temporario em variavel
        Item item_content;
        memset(&item_content, 0, sizeof (Item));
        fread(&item_content, 1, sizeof (Item), tmp);

        // Movendo do arquivo temporario para o cache
        fwrite(&item_content, 1, sizeof (Item), cache);
      }
      fclose(cache);
      fclose(tmp);

      cache = fopen(file, "rb");
      fclose(cache);

      puts(" Salvo e carregado com sucesso!");
      getchar();
      getchar();
  } else {
      cache = fopen(file, "rb");
      if (!cache)
        cache = fopen(file, "wb");
      fclose(cache);

      puts(" Arquivo carregado com sucesso!");
      getchar();
  }

  return '0';
}
