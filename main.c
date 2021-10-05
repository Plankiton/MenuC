#include <stdio.h>
#define TMP_arquivo ".arquivo_atual"

char menu(void);
char cadastro(void);
char ver(void);
char editar(void);
char listar(void);

char resposta = '0';
int main(int argc, char *argv[])
{
  while (resposta != 'q') {
    switch (resposta) {
      case '0':
        resposta = menu();
        break;
      case '1':
        resposta = cadastro();
        break;
      case '2':
        resposta = ver();
        break;
      case '3':
        resposta = editar();
        break;
      case '5':
        resposta = listar();
        break;
    }
  }
  return 0;
}

const char arquivo_padrao [] = ".bancodedados";
char arquivo[50];
FILE * arquivo_atual;

char menu(void) {
  system("clear");
  printf(
      "| Almoxerifado                   |\n"
      "+--------------------------------+\n"
      "| 1 - Cadastrar item             |\n"
      "| 2 - Visualizar item            |\n"
      "| 3 - Editar item                |\n"
      "| 5 - Visualizar todos os items  |\n"
      "\nResposta:"
      );

  char c;
  if (strchr("123456", (c = getchar())))
    return c;
  return 'q';
}

typedef struct {
  unsigned int cod;
  unsigned char nome[15];
  unsigned char info[30];
  unsigned int qtd;
} Item;

char cadastro(void) {
  arquivo_atual = fopen(arquivo, "ab");

  Item i;
  memset(&i, 0, sizeof (Item));

  getchar();
  printf(" Digite o nome do item: ");
  scanf("%14[0-9a-zA-Z ]", i.nome);
  getchar();

  printf(" Deseja cadastrar informação? [s/N] ");
  if (getchar() == 's') {
    getchar();
    printf(" Digite a informação do item: ");
    scanf("%29[0-9a-zA-Z ]", i.info);
    getchar();
  }

  printf(" Digite a quantidade de itens: ");
  scanf("%i", &i.qtd);
  getchar();

  fseek(arquivo_atual, 0, SEEK_END);
  int len = ftell(arquivo_atual);

  if (len >= sizeof(Item))
    i.cod = (len/sizeof(Item));
  else i.cod = 0;

  fwrite(&i, 1, sizeof (Item), arquivo_atual);

  puts(" O item escolhido ficará assim: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.info) >= 1)
    printf("  > %s\n", i.info);
  getchar();

  fclose(arquivo_atual);
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

  arquivo_atual = fopen(arquivo, "rb");
  fseek(arquivo_atual, 0, SEEK_END);
  int len = ftell(arquivo_atual);
  int atual_cod = i.cod;
  int real_cod = i.cod*sizeof(Item);
  if (real_cod >= len) {
    puts(" Item não existe");
    getchar();
    return '0';
  }

  fseek(arquivo_atual, real_cod, SEEK_SET);
  fread(&i, 1, sizeof (Item), arquivo_atual);
  i.cod = atual_cod;
  fclose(arquivo_atual);

  puts(" O item escolhido foi: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.info) >= 1)
    printf("  > %s\n", i.info);

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

  arquivo_atual = fopen(arquivo, "rb");
  fseek(arquivo_atual, 0, SEEK_END);
  int len = ftell(arquivo_atual);
  int atual_cod = i.cod;
  int real_cod = i.cod*sizeof(Item);
  if (real_cod >= len) {
    puts(" Item não existe");
    getchar();
    return '0';
  }

  fseek(arquivo_atual, real_cod, SEEK_SET);
  fread(&i, 1, sizeof (Item), arquivo_atual);
  i.cod = atual_cod;
  fclose(arquivo_atual);

  puts(" O item escolhido foi: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.info) >= 1)
    printf("  > %s\n", i.info);

  printf(
      " Deseja editar qual dado?\n"
      " 1 - Nome\n"
      " 2 - Quantidade\n"
      " 3 - informação\n"
      "\nResposta:"
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
      printf(" Digite a informação do item: ");
      scanf("%29[0-9a-zA-Z ]", i.info);
      getchar();
      break;
  }

  puts(" O item escolhido ficará assim: ");
  printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
  if (strlen((char *)i.info) >= 1)
    printf("  > %s\n", i.info);

  printf(" Deseja salvar os dados? [S/n] ");
  if (getchar() != 'n') {
    // Abre arquivo e pega o tamanho final
    arquivo_atual = fopen(arquivo, "rb");
    fseek(arquivo_atual, 0, SEEK_END);
    int arquivo_atual_size = ftell(arquivo_atual);

    // Carrega arquivo em um buffer temporario
    Item arquivo_atual_content[arquivo_atual_size / sizeof (Item) + 1];
    memset(arquivo_atual_content, 0, arquivo_atual_size);
    fread(arquivo_atual_content, 1, arquivo_atual_size, arquivo_atual);
    fclose(arquivo_atual);

    // Modifica item escolhido e salva
    arquivo_atual = fopen(arquivo, "wb");
    memcpy(&arquivo_atual_content[i.cod], &i, sizeof (Item));
    fwrite(arquivo_atual_content, 1, arquivo_atual_size, arquivo_atual);
    fclose(arquivo_atual);

    arquivo_atual = fopen(arquivo, "rb");
    fclose(arquivo_atual);

    puts(" Salvo com sucesso!");
  }

  getchar();
  return '0';
}

char listar(void) {
  Item i;

  // Abre arquivo e pega o tamanho final
  arquivo_atual = fopen(arquivo, "rb");
  if (!arquivo_atual) {
    puts(" Você ainda não registrou nada!");
    getchar();
    getchar();
    return '0';
  }

  fseek(arquivo_atual, 0, SEEK_END);
  int arquivo_atual_size = ftell(arquivo_atual);
  fseek(arquivo_atual, 0, SEEK_SET);
  int contador = 0;
  while (ftell(arquivo_atual) < arquivo_atual_size - 1) {
    memset(&i, 0, sizeof (Item));
    fread(&i, 1, sizeof (Item), arquivo_atual);
    i.cod = contador++;

    // Exibindo item
    printf(" %i - %s, %i items\n", i.cod, i.nome, i.qtd);
    if (strlen((char *)i.info) >= 1)
      printf("  > %s\n", i.info);
    puts("");
  }

  if (contador == 0) {
    puts(" Você ainda não registrou nada!");
    getchar();
    getchar();
    return '0';
  }

  fclose(arquivo_atual);
  getchar();
  getchar();
  return '0';
}
