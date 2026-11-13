# Cadastro de Loja em C

Programa em C para gerenciar o cadastro de produtos de uma loja, com os dados salvos em arquivo de texto. Desenvolvido para a **Atividade Avaliativa 04**, unindo algoritmos de ordenação, manipulação de arquivos, estruturas de dados e ponteiros.

## Funcionalidades

- **Criar** o arquivo de dados com produtos de exemplo
- **Listar** os produtos em formato de tabela
- **Inserir** produto, com:
  - ID gerado automaticamente (maior ID existente + 1)
  - verificação de duplicatas (mesmo nome não é cadastrado duas vezes)
- **Alterar** produto pelo ID (campos em branco mantêm o valor atual)
- **Excluir** produto pelo ID
- **Buscar** por nome ou por categoria (aceita parte do texto)
- **Ordenar** (exibe em tabela sem alterar o arquivo):
  - nome A–Z
  - nome Z–A
  - preço menor → maior
  - preço maior → menor
- **Exportar** os dados para CSV (separado por vírgula)

Comparações e buscas ignoram maiúsculas e minúsculas.

## Conceitos utilizados

| Conceito | Onde aparece |
|---|---|
| Arquivos em C (`fopen`, `fgets`, `fprintf`, `remove`, `rename`) | leitura, gravação e exportação |
| Estrutura (`struct Produto`) e vetor de estruturas | armazenamento em memória |
| Ponteiros | acesso aos campos (`p->preco`), passagem de vetores para funções |
| Bubble Sort | ordenação dos produtos |
| `switch` | menu principal e critérios de ordenação |

**Alterar e excluir** gravam os dados em um arquivo temporário (`temp.txt`), apagam o original e renomeiam o temporário. Essa técnica evita corromper os dados se algo falhar durante a gravação.

## Como compilar e executar

É necessário ter o compilador **GCC** instalado.

```bash
gcc loja.c -o loja
```

Linux / macOS:

```bash
./loja
```

Windows (PowerShell):

```powershell
.\loja.exe
```

Execute o programa a partir da pasta onde está o `loja.c`, pois os arquivos de dados são criados no diretório atual.

## Menu

```
===== CADASTRO DA LOJA =====
 1 - Criar arquivo (com exemplos)
 2 - Listar produtos
 3 - Inserir produto
 4 - Alterar produto
 5 - Excluir produto
 6 - Buscar por nome
 7 - Buscar por categoria
 8 - Ordenar por nome (A-Z)
 9 - Ordenar por nome (Z-A)
10 - Ordenar por preco (menor -> maior)
11 - Ordenar por preco (maior -> menor)
12 - Exportar para CSV
 0 - Sair
```

## Formato dos dados

Os produtos ficam em `produtos.txt`, uma linha por produto, com campos separados por `|`:

```
ID|Produto|Categoria|Preco|Quantidade|Fornecedor
1|Camiseta Basica|Vestuario|49.90|30|Moda Brasil
2|Mouse Optico USB|Informatica|59.99|15|TecnoShop
3|Shampoo 300ml|Higiene Pessoal|22.50|40|Beleza Pura
4|Cafe 500g|Alimentos|18.00|25|Graos do Sul
```

Exemplo de listagem no programa:

```
-------------------------------------------------------------------------------------
| ID  | Produto              | Categoria        | Preco (R$) |   Qtd | Fornecedor     |
-------------------------------------------------------------------------------------
| 1   | Camiseta Basica      | Vestuario        |      49.90 |    30 | Moda Brasil    |
| 2   | Mouse Optico USB     | Informatica      |      59.99 |    15 | TecnoShop      |
| 3   | Shampoo 300ml        | Higiene Pessoal  |      22.50 |    40 | Beleza Pura    |
| 4   | Cafe 500g            | Alimentos        |      18.00 |    25 | Graos do Sul   |
-------------------------------------------------------------------------------------
```

## Limitações

- Os campos têm tamanho fixo (`produto[50]`, `categoria[30]`, `fornecedor[30]`). Aumente os tamanhos na `struct Produto` se precisar de textos maiores.
- O limite é de 200 produtos (`MAX`).
- Os textos exibidos não usam acentos, para evitar problemas de codificação no terminal do Windows.
- O caractere `|` não deve ser usado nos campos de texto, pois é o separador do arquivo.

## Estrutura do projeto

```
.
├── loja.c          # código-fonte completo e comentado
├── README.md
├── produtos.txt    # gerado pelo programa
└── produtos.csv    # gerado ao exportar
```

## Autor

Christiano
