# Citizen Education Complaint

## Participantes

* Eric dos Santos Mendes da Silva - 569528 -  1CCPI 
* Tom Stringasci Albuquerque Coelho de Morais - 568844 - 1CCPI 
* Rafael Côrtes Lins - 570588 - 1CCPI  
* Instituição - FIAP 
* Curso - Ciência da Computação 
* Disciplina - Data Structures and Algorithms
* Professor - Erick Yamamoto

## Overview
O **Citizen Education Complaint** (CEC) é um protótipo funcional de uma plataforma de *Citizen Tech* focada no gerenciamento de ouvidorias e demandas públicas. Para esta implementação, foi escolhido o recorte de Educação e Qualificação, simulando um fórum de reclamações e solicitações para escolas municipais e estaduais.

O cidadão pode abrir chamados públicos (Tokens) relatando problemas ou sugestões. Os administradores do sistema e gestores escolares podem interagir diretamente em cada caso, criando uma *thread* (fio de discussão) com mensagens de ida e volta pelas usuários e administradores, permitindo o acompanhamento em tempo real da resolução dos problemas até o encerramento do protocolo. Além disso, cada um desses threads são gerenciadas de forma publica, deixando com que pais e empregados possam pesquisar suas próprias escolas e saber sobre problemas que já teve, como esses problemas foram resolvidos, etc.

---

## Funcionalidades Centrais
*   Cadastro Dinâmico: Inserção de novos chamados com geração automática de IDs sequenciais exclusivos, evitando duplicidades.
*   Armazenamento em Structs: Organização de dados baseada em uma Lista Encadeada Dinâmica de structs desenvolvida do zero em C.
*   Histórico de Mensagens Integrado: Cada nó da lista encadeada gerencia de forma limpa um vetor estático de subestruturas para armazenar o histórico de mensagens trocadas sobre o acasos.
*   Filtro Parcial Avançado: Sistema de busca por nome de escola que ignora letras maiúsculas e minúsculas, deixando com que pessoas possam pesquisar uma palavra do nome da escola e encontrar todos threads relacionados a ela.
*   Painel de Estatísticas: Permite visualizar o fluxo completo de mensagens em todos threads, o total de tokens no sistema, além de quantos estão classificados em cada um dos três statuses (*Pendente*, *Em Andamento*, *Finalizado*).
*   Ordenação Customizada: Implementação do algoritmo *Selection Sort* adaptado para listas encadeadas, capaz de organizar a base de dados por ID, por atividade (mensagens) ou por prioridade de Status.
*   Deleção de Struct: Exclusão de tokens inapropriados com costura de ponteiros vizinhos.

---

## Estrutura de Struct 

```c
typedef struct Resposta {
    char autor[50];
    char mensagem[500];
} Resposta;

typedef struct Reclamacao {
    int id;
    char escola[100];
    char titulo[50];
    char descricao[250];
    char status[20];
    
    Resposta lista_respostas[100];  
    int qtd_respostas;
    struct Reclamacao* proximo;     
} Reclamacao;
```

---

## Análise de Complexidade de Algoritmos (Big-O e T(n))

Abaixo está o detalhamento matemático das equações de custo de tempo T(n) das operações principais, considerando o pior caso (quando a lista principal possui n elementos cadastrados).

### Tabela Resumida

| Operação | Função de Custo Exato | Classificação Assintótica (Big-O) |
| :--- | :--- | :--- |
| **Cadastro de Chamado** | T(n) = 15 | **O(1)** - Tempo Constante |
| **Listagem Geral (Fórum)** | T(n) = 8n + 11 | **O(n)** - Tempo Linear |
| **Exclusão de Registro** | T(n) = 4n + 7 | **O(n)** - Tempo Linear |
| **Pesquisa Filtrada por Escola** | T(n) = 3mn + 4n + 5 | **O(n)** - Tempo Linear |
| **Ordenação (Selection Sort)** | T(n) = 209n^2 - 209n + 4 | **O(n^2)** - Tempo Quadrático |

*Nota: "m" representa o comprimento da string digitada na filtragem (limitada a 100 caracteres pela struct, atuando como constante no crescimento).*

### Justificativa Detalhada 

#### Cadastro (`cadastrar`)
Por realizar a inserção de novos nós sempre na "cabeça" da lista encadeada, o algoritmo não precisa fazer varreduras, independente do tamanho atual da base de dados.
*   Passos fixos: Alocação dinâmica via `malloc` + atribuições iniciais de controle (`id`, `status`, `qtd_respostas`) = *4 operações*.
*   Leitura de dados: 3 blocos isolados de `printf` + `fgets` + `strcspn` (limitados pelo tamanho fixo dos buffers) = *9 operações*.
*   Encadeamento: Ajuste do ponteiro `proximo` para apontar para a cabeça antiga + retorno do novo endereço = *2 operações*.
*   Equação: `T(n) = 15`, que simplificado resulta em *O(1)*.

#### Listagem de Forúm (`listar`)
Para listar os dados, o programa usa um ponteiro auxiliar e realiza uma passada completa de um em um até atingir o valor de parada (`NULL`).
*   Antes do laço: Verificações de segurança e inicialização do ponteiro auxiliar = *6 operações*.
*   Dentro do laço (Executa n vezes): Validação da condição + 6 instruções de impressão de dados na tela (`printf`) + avanço do ponteiro auxiliar (`atual = atual->proximo`) = *8 operações por nó* (resultando em 8n).
*   Menu de Opções: Impressão de comandos do submenu, leitura de opção com `scanf`, limpeza de buffer do teclado e estrutura de desvio `switch` = *5 operações*.
*   Equação: `T(n) = 6 + 8n + 5`, que simplificado resulta em `T(n) = 8n + 11`, mantendo a classificação *O(n)*.

#### Remoção de Chamado (`excluir`)
No pior cenário (o ID buscado está na última posição da lista ou não existe), o algoritmo percorre toda a cadeia guardando o nó `anterior` para não perder a referência.
*   Antes do laço: Teste de lista vazia + teste de exclusão do primeiro nó + inicialização dos ponteiros auxiliares = *5 operações*.
*   Dentro do laço (Executa n vezes):** Validação do `while` + teste de igualdade do ID (`if`) + atualização dos ponteiros de caminhada (`anterior` e `atual`) = *4 operações por iteração* (resultando em 4n).
*   Depois do laço: Exibição da mensagem de erro e retorno da cabeça original intacta = *2 operações*.
*   Equação: `T(n) = 5 + 4n + 2`, que simplificado resulta em `T(n) = 4n + 7`, mantendo a classificação *O(n)*.

#### Pesquisa por Escola (`listar_escola`)
O laço principal caminha linearmente pelos n nós. Dentro dele, existe um laço que formata a string do nó para minúsculo antes de acionar a busca parcial.
*   Entrada de dados: Leitura de teclado com `fgets`, remoção de quebra de linha e conversão da palavra digitada para caracteres minúsculos via `tolower` = *5 operações*.
*   Laço principal (Executa n vezes): Validação do nó atual + cópia caractere por caractere da escola da struct para minúsculo (tamanho m) executando 3 passos (3m) + checagem de substring com `strstr` + avanço do ponteiro = *4 + 3m operações por nó*.
*   Equação: `T(n) = 5 + n(4 + 3m) = 3mn + 4n + 5`. Como o tamanho da string "m" é delimitado estaticamente a no máximo 100 posições pela struct, ele é considerado uma constante no crescimento do algoritmo, mantendo a classificação assintótica final estável como *O(n)*.

#### Algoritmo de Ordenação (`ordenar` + `trocar_dados`)
Por utilizar a estrutura clássica de loops aninhados do *Selection Sort*, o laço de fora fixa um elemento i e o laço de dentro varre todos os elementos j adiante fazendo comparações combinatórias, executando exatamente `[n * (n - 1)] / 2` iterações.
*   Custo fixo da Função Auxiliar (`trocar_dados`): Troca de variáveis de controle simples = 6 passos. Cópia segura de 3 strings com `strcpy` = 9 passos. Laço `for` estático que move as 100 mensagens do histórico de respostas (100 iterações fazendo 4 atribuições de structs por rodada) = 400 passos. *Custo total fixo da função: 415 passos por chamada*.
*   Custo interno do laço duplo: Cada teste comparativo dos laços aninhados consome 3 operações de controle. No pior caso (onde a base está totalmente invertida e exige trocas em todas as passadas), o custo por iteração salta para `3 + 415 = 418` passos.
*   Multiplicação dos laços: `418 * [n * (n - 1)] / 2 = 209n^2 - 209n`.
*   Interações externas: Exibição do menu de seleção do tipo de ordenação e leitura da opção = *4 operações*.
*   Equação: `T(n) = 4 + (209n^2 - 209n) = 209n^2 - 209n + 4`. Como o termo de maior grau domina o crescimento do algoritmo no infinito, a classificação final é definida como *O(n^2)*.

---

## 05 / Como Executar o Projeto

1.  Certifique-se de ter um compilador para a linguagem C instalado (como o `gcc`).
2.  Abra o terminal na pasta onde o arquivo `main.c` está salvo.
3.  Compile o código executando o comando:
    ```bash
    gcc main.c -o citizen_assist
    ```
4.  Execute o binário gerado:
    *   No Linux/Mac: `./citizen_assist`
    *   No Windows: `citizen_assist.exe`
