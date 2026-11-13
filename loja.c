/*
 * ATIVIDADE AVALIATIVA 04 - Cadastro de Loja
 * Une: arquivos em C (aula 8), ponteiros (aula 9),
 *      ordenacao (aula 11) e switch (aula 12).
 *
 * Os dados ficam no arquivo texto "produtos.txt", uma linha por produto:
 * ID|Produto|Categoria|Preco|Quantidade|Fornecedor
 */

#include <stdio.h>   // printf, fopen, fgets, fprintf, remove, rename
#include <stdlib.h>  // atoi
#include <string.h>  // strcpy, strlen, strcspn
#include <ctype.h>   // tolower

#define ARQUIVO "produtos.txt"      // nome do arquivo principal
#define TEMP    "temp.txt"          // arquivo temporario (alterar/excluir)
#define CSV     "produtos.csv"      // arquivo exportado
#define MAX     200                 // maximo de produtos na memoria

// Estrutura que representa um produto da loja
typedef struct {
    int   id;               // codigo gerado automaticamente
    char  produto[50];      // nome do produto (aumente se precisar)
    char  categoria[30];    // categoria do produto
    float preco;            // preco em reais
    int   quantidade;       // quantidade em estoque
    char  fornecedor[30];   // nome do fornecedor
} Produto;

/* ---------- FUNCOES AUXILIARES ---------- */

// Le uma linha do teclado (aceita espacos) e remove o "\n" do final
void ler_texto(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);                 // le a linha digitada
    destino[strcspn(destino, "\n")] = '\0';         // troca o \n por fim de string
}

// Le um numero real (aceita virgula ou ponto)
float ler_float(void) {
    char buf[30];                                   // guarda o texto digitado
    ler_texto(buf, sizeof(buf));                    // le o texto
    for (char *p = buf; *p != '\0'; p++)            // percorre com ponteiro
        if (*p == ',') *p = '.';                    // troca virgula por ponto
    return (float)atof(buf);                        // converte para float
}

// Le um numero inteiro
int ler_int(void) {
    char buf[20];                                   // guarda o texto digitado
    ler_texto(buf, sizeof(buf));                    // le o texto
    return atoi(buf);                               // converte para int
}

// Compara duas strings ignorando maiusculas/minusculas
// Retorna 0 se iguais, <0 se a<b, >0 se a>b
int comparar(const char *a, const char *b) {
    while (*a && *b) {                              // enquanto nenhuma acabou
        int ca = tolower((unsigned char)*a);        // letra de a em minuscula
        int cb = tolower((unsigned char)*b);        // letra de b em minuscula
        if (ca != cb) return ca - cb;               // diferentes: devolve a diferenca
        a++; b++;                                   // avanca os ponteiros
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b); // fim das strings
}

// Verifica se "texto" contem "busca" (ignorando maiusculas). 1 = contem
int contem(const char *texto, const char *busca) {
    int n = strlen(busca);                          // tamanho do que se busca
    int t = strlen(texto);                          // tamanho do texto
    for (int i = 0; i + n <= t; i++) {              // testa cada posicao possivel
        int ok = 1;                                 // assume que achou
        for (int j = 0; j < n; j++)                 // compara letra por letra
            if (tolower((unsigned char)texto[i + j]) !=
                tolower((unsigned char)busca[j])) { ok = 0; break; }
        if (ok) return 1;                           // achou
    }
    return 0;                                       // nao achou
}

/* ---------- ARQUIVO ---------- */

// Cria o arquivo com os 4 produtos de exemplo (se ja existir, recria)
void criar_arquivo(void) {
    FILE *f = fopen(ARQUIVO, "w");                  // abre para escrita (apaga o antigo)
    if (f == NULL) { printf("Erro ao criar o arquivo!\n"); return; }
    fprintf(f, "1|Camiseta Basica|Vestuario|49.90|30|Moda Brasil\n");
    fprintf(f, "2|Mouse Optico USB|Informatica|59.99|15|TecnoShop\n");
    fprintf(f, "3|Shampoo 300ml|Higiene Pessoal|22.50|40|Beleza Pura\n");
    fprintf(f, "4|Cafe 500g|Alimentos|18.00|25|Graos do Sul\n");
    fclose(f);                                      // fecha o arquivo
    printf("Arquivo criado com 4 produtos de exemplo.\n");
}

// Carrega o arquivo para o vetor "lista". Retorna quantos produtos leu
int carregar(Produto *lista) {
    FILE *f = fopen(ARQUIVO, "r");                  // abre para leitura
    if (f == NULL) return 0;                        // arquivo nao existe: 0 produtos
    char linha[200];                                // guarda cada linha lida
    int n = 0;                                      // contador de produtos
    while (n < MAX && fgets(linha, sizeof(linha), f) != NULL) {
        Produto *p = &lista[n];                     // ponteiro para a posicao atual
        // separa os campos usando o caractere | como divisor
        int lidos = sscanf(linha, "%d|%49[^|]|%29[^|]|%f|%d|%29[^\n]",
                           &p->id, p->produto, p->categoria,
                           &p->preco, &p->quantidade, p->fornecedor);
        if (lidos == 6) n++;                        // so conta se leu os 6 campos
    }
    fclose(f);                                      // fecha o arquivo
    return n;                                       // devolve a quantidade
}

// Grava o vetor inteiro usando arquivo temporario e depois troca pelo original
// (evita corromper os dados se der erro no meio da gravacao)
void salvar(Produto *lista, int n) {
    FILE *f = fopen(TEMP, "w");                     // abre o temporario
    if (f == NULL) { printf("Erro ao salvar!\n"); return; }
    for (int i = 0; i < n; i++)                     // grava cada produto
        fprintf(f, "%d|%s|%s|%.2f|%d|%s\n", lista[i].id, lista[i].produto,
                lista[i].categoria, lista[i].preco, lista[i].quantidade,
                lista[i].fornecedor);
    fclose(f);                                      // fecha o temporario
    remove(ARQUIVO);                                // apaga o original
    rename(TEMP, ARQUIVO);                          // temporario vira o original
}

// Gera o proximo ID: maior ID existente + 1
int gerar_id(Produto *lista, int n) {
    int maior = 0;                                  // comeca em zero
    for (int i = 0; i < n; i++)                     // procura o maior ID
        if (lista[i].id > maior) maior = lista[i].id;
    return maior + 1;                               // proximo ID
}

// Procura um produto pelo nome exato (sem diferenciar maiusculas)
// Retorna a posicao no vetor ou -1 se nao achar
int achar_nome(Produto *lista, int n, const char *nome) {
    for (int i = 0; i < n; i++)
        if (comparar(lista[i].produto, nome) == 0) return i;
    return -1;
}

// Procura um produto pelo ID. Retorna a posicao ou -1
int achar_id(Produto *lista, int n, int id) {
    for (int i = 0; i < n; i++)
        if (lista[i].id == id) return i;
    return -1;
}

/* ---------- EXIBICAO ---------- */

// Mostra uma lista de produtos em forma de tabela (nao altera o arquivo)
void mostrar_tabela(Produto *lista, int n) {
    printf("\n-------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-20s | %-16s | %10s | %5s | %-14s |\n",
           "ID", "Produto", "Categoria", "Preco (R$)", "Qtd", "Fornecedor");
    printf("-------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        Produto *p = &lista[i];                     // ponteiro para o produto atual
        printf("| %-3d | %-20s | %-16s | %10.2f | %5d | %-14s |\n",
               p->id, p->produto, p->categoria, p->preco, p->quantidade, p->fornecedor);
    }
    printf("-------------------------------------------------------------------------------------\n");
    printf("Total: %d produto(s)\n", n);
}

/* ---------- OPERACOES ---------- */

// LISTAR: carrega do arquivo e mostra
void listar(void) {
    Produto lista[MAX];                             // vetor de estruturas
    int n = carregar(lista);                        // le o arquivo
    if (n == 0) { printf("Nenhum produto cadastrado.\n"); return; }
    mostrar_tabela(lista, n);                       // exibe em tabela
}

// INSERIR: pede os dados, valida e grava no final do arquivo
void inserir(void) {
    Produto lista[MAX];
    int n = carregar(lista);                        // carrega os existentes
    if (n >= MAX) { printf("Limite de produtos atingido!\n"); return; }

    Produto novo;                                   // produto que sera cadastrado
    printf("Nome do produto: ");
    ler_texto(novo.produto, sizeof(novo.produto));
    if (strlen(novo.produto) == 0) { printf("Nome vazio!\n"); return; }

    // verificacao de duplicata (mesmo nome, sem diferenciar maiusculas)
    if (achar_nome(lista, n, novo.produto) != -1) {
        printf("Ja existe um produto com esse nome!\n");
        return;
    }

    printf("Categoria: ");
    ler_texto(novo.categoria, sizeof(novo.categoria));
    printf("Preco (R$): ");
    novo.preco = ler_float();
    printf("Quantidade: ");
    novo.quantidade = ler_int();
    printf("Fornecedor: ");
    ler_texto(novo.fornecedor, sizeof(novo.fornecedor));

    novo.id = gerar_id(lista, n);                   // ID automatico (nunca repete)

    FILE *f = fopen(ARQUIVO, "a");                  // "a" = adiciona no final
    if (f == NULL) { printf("Erro ao abrir o arquivo!\n"); return; }
    fprintf(f, "%d|%s|%s|%.2f|%d|%s\n", novo.id, novo.produto, novo.categoria,
            novo.preco, novo.quantidade, novo.fornecedor);
    fclose(f);
    printf("Produto cadastrado com ID %d.\n", novo.id);
}

// ALTERAR: muda os dados de um produto (pelo ID)
void alterar(void) {
    Produto lista[MAX];
    int n = carregar(lista);
    printf("ID do produto a alterar: ");
    int id = ler_int();
    int pos = achar_id(lista, n, id);               // posicao no vetor
    if (pos == -1) { printf("Produto nao encontrado.\n"); return; }

    Produto *p = &lista[pos];                       // ponteiro para o produto escolhido
    char buf[50];                                   // texto temporario
    printf("Deixe em branco para manter o valor atual.\n");

    printf("Nome [%s]: ", p->produto);
    ler_texto(buf, sizeof(buf));
    if (strlen(buf) > 0) {                          // so altera se digitou algo
        int outro = achar_nome(lista, n, buf);      // checa duplicata
        if (outro != -1 && outro != pos) {
            printf("Ja existe outro produto com esse nome! Alteracao cancelada.\n");
            return;
        }
        strcpy(p->produto, buf);
    }
    printf("Categoria [%s]: ", p->categoria);
    ler_texto(buf, sizeof(buf));
    if (strlen(buf) > 0) strncpy(p->categoria, buf, sizeof(p->categoria) - 1);

    printf("Preco [%.2f]: ", p->preco);
    ler_texto(buf, sizeof(buf));
    if (strlen(buf) > 0) {
        for (char *c = buf; *c; c++) if (*c == ',') *c = '.';
        p->preco = (float)atof(buf);
    }
    printf("Quantidade [%d]: ", p->quantidade);
    ler_texto(buf, sizeof(buf));
    if (strlen(buf) > 0) p->quantidade = atoi(buf);

    printf("Fornecedor [%s]: ", p->fornecedor);
    ler_texto(buf, sizeof(buf));
    if (strlen(buf) > 0) strncpy(p->fornecedor, buf, sizeof(p->fornecedor) - 1);

    salvar(lista, n);                               // regrava via arquivo temporario
    printf("Produto alterado.\n");
}

// EXCLUIR: remove um produto (pelo ID)
void excluir(void) {
    Produto lista[MAX];
    int n = carregar(lista);
    printf("ID do produto a excluir: ");
    int id = ler_int();
    int pos = achar_id(lista, n, id);
    if (pos == -1) { printf("Produto nao encontrado.\n"); return; }

    for (int i = pos; i < n - 1; i++)               // "puxa" os proximos para tras
        lista[i] = lista[i + 1];
    n--;                                            // um produto a menos
    salvar(lista, n);                               // regrava via arquivo temporario
    printf("Produto excluido.\n");
}

// BUSCAR: por nome (opcao 1) ou categoria (opcao 2), sem diferenciar maiusculas
void buscar(int tipo) {
    Produto lista[MAX], achados[MAX];               // achados guarda os resultados
    int n = carregar(lista), qtd = 0;
    char busca[50];
    printf(tipo == 1 ? "Nome (ou parte): " : "Categoria (ou parte): ");
    ler_texto(busca, sizeof(busca));

    for (int i = 0; i < n; i++) {
        const char *campo = (tipo == 1) ? lista[i].produto : lista[i].categoria;
        if (contem(campo, busca)) achados[qtd++] = lista[i];  // guarda o encontrado
    }
    if (qtd == 0) printf("Nenhum produto encontrado.\n");
    else mostrar_tabela(achados, qtd);
}

/* ---------- ORDENACAO (Bubble Sort) ---------- */

// Criterio: 1=nome A-Z, 2=nome Z-A, 3=preco menor->maior, 4=preco maior->menor
// Retorna 1 se "a" deve vir DEPOIS de "b" (ou seja, precisam trocar de lugar)
int deve_trocar(Produto *a, Produto *b, int criterio) {
    switch (criterio) {
        case 1: return comparar(a->produto, b->produto) > 0;
        case 2: return comparar(a->produto, b->produto) < 0;
        case 3: return a->preco > b->preco;
        default: return a->preco < b->preco;
    }
}

// Ordena o vetor em memoria (o arquivo NAO e alterado) e mostra a tabela
void ordenar(int criterio) {
    Produto lista[MAX];
    int n = carregar(lista);
    if (n == 0) { printf("Nenhum produto cadastrado.\n"); return; }

    for (int i = 0; i < n - 1; i++)                 // passadas do bubble sort
        for (int j = 0; j < n - 1 - i; j++)         // compara vizinhos
            if (deve_trocar(&lista[j], &lista[j + 1], criterio)) {
                Produto aux = lista[j];             // troca os dois produtos
                lista[j] = lista[j + 1];
                lista[j + 1] = aux;
            }
    mostrar_tabela(lista, n);                       // exibe sem mexer no arquivo
}

/* ---------- EXPORTAR CSV ---------- */

// Exporta os dados para produtos.csv separados por virgula
void exportar_csv(void) {
    Produto lista[MAX];
    int n = carregar(lista);
    FILE *f = fopen(CSV, "w");
    if (f == NULL) { printf("Erro ao criar o CSV!\n"); return; }
    fprintf(f, "ID,Produto,Categoria,Preco,Quantidade,Fornecedor\n");  // cabecalho
    for (int i = 0; i < n; i++)
        fprintf(f, "%d,%s,%s,%.2f,%d,%s\n", lista[i].id, lista[i].produto,
                lista[i].categoria, lista[i].preco, lista[i].quantidade,
                lista[i].fornecedor);
    fclose(f);
    printf("Exportado %d produto(s) para %s\n", n, CSV);
}

/* ---------- PROGRAMA PRINCIPAL ---------- */

int main(void) {
    int opcao;                                      // opcao escolhida no menu

    do {
        printf("\n===== CADASTRO DA LOJA =====\n");
        printf(" 1 - Criar arquivo (com exemplos)\n");
        printf(" 2 - Listar produtos\n");
        printf(" 3 - Inserir produto\n");
        printf(" 4 - Alterar produto\n");
        printf(" 5 - Excluir produto\n");
        printf(" 6 - Buscar por nome\n");
        printf(" 7 - Buscar por categoria\n");
        printf(" 8 - Ordenar por nome (A-Z)\n");
        printf(" 9 - Ordenar por nome (Z-A)\n");
        printf("10 - Ordenar por preco (menor -> maior)\n");
        printf("11 - Ordenar por preco (maior -> menor)\n");
        printf("12 - Exportar para CSV\n");
        printf(" 0 - Sair\n");
        printf("Opcao: ");
        opcao = ler_int();                          // le a opcao

        switch (opcao) {                            // chama a funcao escolhida
            case 1:  criar_arquivo();  break;
            case 2:  listar();         break;
            case 3:  inserir();        break;
            case 4:  alterar();        break;
            case 5:  excluir();        break;
            case 6:  buscar(1);        break;
            case 7:  buscar(2);        break;
            case 8:  ordenar(1);       break;
            case 9:  ordenar(2);       break;
            case 10: ordenar(3);       break;
            case 11: ordenar(4);       break;
            case 12: exportar_csv();   break;
            case 0:  printf("Ate logo!\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);                           // repete ate escolher sair

    return 0;
}
