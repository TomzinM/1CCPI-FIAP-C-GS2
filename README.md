# Citizen Education Complaint

## Participantes

- Eric dos Santos Mendes da Silva - 569528 - 1CCPI
- Tom Stringasci Albuquerque Coelho de Morais - 568844 - 1CCPI
- Rafael Côrtes Lins - 570588 - 1CCPI
- Instituição - FIAP
- Curso - Ciência da Computação
- Disciplina - Data Structures and Algorithms
- Professor - Erick Yamamoto

---

## Overview

O **Citizen Education Complaint (CEC)** é um protótipo funcional de uma plataforma de Citizen Tech focada no gerenciamento de reclamações e demandas relacionadas à educação pública. Para esta implementação, foi escolhido o recorte de Educação e Qualificação, simulando um sistema de ouvidoria para escolas municipais e estaduais.

O cidadão pode cadastrar reclamações relatando problemas ou sugestões relacionados a uma escola. Cada registro recebe um identificador único e pode ser acompanhado por meio de um histórico de mensagens entre o cidadão e os responsáveis pelo atendimento. O sistema permite consultar, pesquisar, atualizar, ordenar e excluir reclamações, além de acompanhar seus respectivos status.

A aplicação funciona por meio de um terminal e utiliza uma estrutura de dados dinâmica para armazenar os registros durante a execução. Os dados são mantidos em memória e não são preservados após o encerramento do programa.

---

## Funcionalidades Centrais

- **Cadastro Dinâmico:** Inserção de novas reclamações com geração automática de IDs exclusivos, evitando duplicidades.
- **Armazenamento em Structs:** Organização dos dados baseada em uma Lista Encadeada Dinâmica de structs desenvolvida do zero em C.
- **Histórico de Mensagens Integrado:** Cada nó da lista encadeada gerencia um vetor estático de subestruturas para armazenar o histórico de mensagens trocadas sobre a reclamação.
- **Consulta de Reclamações:** Exibição dos registros cadastrados, permitindo selecionar uma reclamação para visualizar seus detalhes e histórico de mensagens.
- **Pesquisa por ID:** Localização de reclamações por meio de seus identificadores.
- **Filtro Parcial por Escola:** Sistema de busca por nome de escola que ignora letras maiúsculas e minúsculas, permitindo pesquisar parte do nome e encontrar as reclamações relacionadas.
- **Atualização de Dados:** Possibilidade de alterar o título, o nome da escola, a descrição e o status de uma reclamação.
- **Painel de Estatísticas:** Permite visualizar o total de reclamações no sistema, quantas estão classificadas em cada um dos três status (Pendente, Em Andamento e Finalizado), além da quantidade total de mensagens registradas.
- **Ordenação Customizada:** Implementação do algoritmo Selection Sort adaptado para listas encadeadas, capaz de organizar os registros por ID, quantidade de mensagens ou prioridade de status.
- **Exclusão de Registros:** Remoção de reclamações da lista encadeada por meio da reorganização dos ponteiros, liberando a memória alocada.
- **Validação de Entradas:** Verificação de opções inválidas nos menus e de campos obrigatórios vazios, evitando cadastros incompletos.

---

## Estrutura de Struct

O projeto utiliza duas estruturas principais para organizar os dados.

A primeira é a struct `Resposta`, responsável por armazenar o autor e o conteúdo de cada mensagem registrada em uma reclamação.

```c
typedef struct {
    char autor[50];
    char mensagem[500];
} Resposta;
```

A segunda é a struct `Reclamacao`, responsável por armazenar os dados de cada registro e conectar os elementos da lista encadeada.

```c
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

Cada reclamação possui um vetor estático com capacidade para até 100 respostas, além de um contador que indica quantas mensagens foram registradas. O ponteiro `proximo` conecta cada nó ao próximo elemento da lista.

A lista é dinâmica: cada nova reclamação é alocada utilizando `malloc`, e os registros removidos ou a lista inteira são liberados utilizando `free`.

---

## Análise de Complexidade de Algoritmos (Big-O)

A análise abaixo considera o pior caso das operações principais, utilizando **n** para representar a quantidade de reclamações cadastradas na lista encadeada.

Como os campos de texto possuem tamanhos máximos definidos na struct e cada reclamação comporta no máximo 100 respostas, essas operações sobre os campos e os históricos possuem custo limitado. Portanto, são consideradas constantes em relação ao crescimento de **n**.

### Tabela Resumida

| Operação | Complexidade de Tempo (Big-O) | Justificativa |
|---|---|---|
| Cadastro de Reclamação | O(1) | A inserção acontece diretamente no início da lista, sem percorrer os elementos existentes. |
| Consulta / Listagem Geral | O(n) | É necessário percorrer a lista para exibir todos os registros. |
| Pesquisa por ID | O(n) | No pior caso, todos os nós precisam ser percorridos até encontrar o identificador ou atingir o final da lista. |
| Pesquisa por Escola | O(n) | A lista inteira pode precisar ser percorrida para encontrar todas as correspondências. |
| Atualização de Reclamação | O(n) | É necessário localizar o registro antes de modificar seus dados. |
| Exclusão de Reclamação | O(n) | No pior caso, é necessário percorrer a lista até encontrar o registro a ser removido. |
| Adição de Mensagem | O(n) | A localização da reclamação pelo ID exige uma busca linear; a inclusão da mensagem após encontrá-la é O(1). |
| Painel de Estatísticas | O(n) | Uma única passagem pela lista é suficiente para contabilizar os registros e suas mensagens. |
| Ordenação (Selection Sort) | O(n²) | O algoritmo compara os elementos em passagens sucessivas pela lista. |
| Liberação da Lista | O(n) | Cada nó precisa ser visitado e liberado individualmente. |

### Justificativa Detalhada

#### Cadastro de Reclamação (`cadastrar`)

A inserção de novos registros acontece sempre na cabeça da lista encadeada. Dessa forma, o algoritmo não precisa percorrer os elementos já cadastrados para encontrar a posição de inserção.

- **Alocação:** Criação de um novo nó utilizando `malloc`.
- **Inicialização:** Preenchimento do identificador, status inicial e contador de respostas.
- **Leitura de dados:** Recebimento das informações de título, escola e descrição.
- **Encadeamento:** O ponteiro `proximo` do novo nó recebe o endereço da antiga cabeça da lista.
- **Retorno:** O novo nó passa a ser a cabeça da lista.

Como nenhuma operação depende da quantidade de nós já existentes, a complexidade de tempo é **O(1)**.

#### Consulta e Listagem Geral (`listar`)

Para apresentar todos os registros, o programa utiliza um ponteiro auxiliar que percorre a lista encadeada até encontrar `NULL`.

- **Inicialização:** O ponteiro auxiliar recebe o endereço da cabeça da lista.
- **Percurso:** Cada nó é visitado uma única vez.
- **Exibição:** Os dados de cada reclamação são apresentados na tela.
- **Avanço:** O ponteiro recebe o endereço do próximo nó.

Como cada um dos **n** elementos precisa ser visitado uma vez, a complexidade de tempo é **O(n)**.

#### Pesquisa por ID

A pesquisa por identificador utiliza uma varredura sequencial da lista encadeada.

- **Inicialização:** O ponteiro auxiliar começa na cabeça da lista.
- **Comparação:** O identificador procurado é comparado com o ID de cada nó.
- **Avanço:** Caso não haja correspondência, o ponteiro passa para o próximo elemento.
- **Resultado:** A busca termina quando o registro é encontrado ou quando o ponteiro chega a `NULL`.

No pior caso, o registro está no último nó ou não existe. Assim, todos os elementos podem precisar ser percorridos, resultando em **O(n)**.

#### Pesquisa por Escola (`listar_escola`)

A pesquisa por escola permite localizar reclamações por meio de uma correspondência parcial do nome, ignorando diferenças entre letras maiúsculas e minúsculas.

- **Preparação da entrada:** O texto digitado é convertido para letras minúsculas.
- **Percurso:** O programa percorre todos os nós da lista.
- **Normalização:** O nome da escola armazenado em cada registro também é convertido para letras minúsculas.
- **Comparação:** A função `strstr` verifica se o texto pesquisado aparece dentro do nome da escola.
- **Exibição:** Todas as reclamações correspondentes são apresentadas.

O percurso principal visita os **n** nós. Como os nomes das escolas possuem tamanho máximo fixo, o processamento das strings possui custo limitado em relação a **n**.

Portanto, a complexidade de tempo em função da quantidade de reclamações é **O(n)**.

#### Atualização de Reclamação (`atualizar`)

A atualização exige primeiro localizar o registro por meio do seu identificador.

- **Busca:** O programa percorre a lista até encontrar o ID informado.
- **Seleção:** O usuário escolhe qual informação deseja alterar.
- **Modificação:** O campo selecionado recebe o novo valor após a validação da entrada.

A localização do registro pode exigir a visita a todos os **n** nós. A alteração do campo, por sua vez, possui custo constante devido aos tamanhos fixos das estruturas.

Assim, a complexidade total é **O(n)**.

#### Exclusão de Reclamação (`excluir`)

A remoção utiliza ponteiros auxiliares para localizar o registro e reorganizar as conexões da lista.

- **Lista vazia:** O programa verifica se existem elementos cadastrados.
- **Primeiro nó:** Caso o registro esteja na cabeça, o ponteiro principal passa a apontar para o próximo elemento.
- **Demais nós:** O ponteiro do elemento anterior passa a apontar para o próximo elemento do nó removido.
- **Liberação:** A memória do registro excluído é liberada com `free`.

No pior caso, o registro está no final da lista ou não existe, exigindo uma busca por todos os elementos.

A reorganização dos ponteiros e a liberação da memória possuem custo constante após a localização. Portanto, a complexidade total é **O(n)**.

#### Adição de Mensagem (`mandar_mensagem`)

A inclusão de uma mensagem começa com a localização da reclamação por seu identificador.

- **Busca:** O programa percorre a lista até encontrar o registro desejado.
- **Validação:** É verificado se o histórico ainda possui espaço disponível.
- **Inclusão:** A nova mensagem é armazenada na próxima posição disponível do vetor de respostas.
- **Atualização:** O contador de respostas é incrementado e o status da reclamação passa para Em Andamento.

A busca pelo registro possui complexidade **O(n)**. Após sua localização, a inclusão no vetor é **O(1)**, pois o índice da próxima posição é obtido diretamente pelo contador.

Dessa forma, a complexidade total é **O(n)**.

#### Painel de Estatísticas (`painel`)

O painel realiza uma única passagem pela lista encadeada para calcular os indicadores do sistema.

Durante o percurso, são contabilizados:

- Total de reclamações cadastradas.
- Quantidade de reclamações pendentes.
- Quantidade de reclamações em andamento.
- Quantidade de reclamações finalizadas.
- Total de mensagens registradas.

Cada nó é visitado uma única vez e os contadores são atualizados com operações de custo constante.

Assim, a complexidade de tempo é **O(n)**.

#### Algoritmo de Ordenação (`ordenar` + `trocar_dados`)

O projeto utiliza o algoritmo **Selection Sort**, adaptado para trabalhar com uma lista encadeada.

O algoritmo percorre os elementos da lista em passagens sucessivas. Em cada passagem, procura o menor elemento da parte ainda não ordenada e realiza a troca dos dados quando necessário.

- **Laço externo:** Define o elemento atual que será organizado.
- **Laço interno:** Percorre os elementos seguintes para encontrar o menor valor.
- **Comparações:** Os registros são comparados conforme o critério escolhido pelo usuário: ID, quantidade de mensagens ou status.
- **Troca:** A função `trocar_dados` realiza a troca dos dados entre os nós selecionados, sem modificar os ponteiros que conectam a lista.

O número de comparações do Selection Sort é:

\[
\frac{n(n-1)}{2}
\]

Isso significa que o algoritmo realiza uma quantidade quadrática de comparações conforme o número de elementos aumenta.

A função `trocar_dados` também movimenta os campos de texto e o vetor de até 100 respostas. Como esses campos possuem tamanho máximo fixo, o custo de cada troca é constante em relação a **n**.

Portanto, a complexidade de tempo da ordenação é **O(n²)**.

A ordenação utiliza apenas variáveis auxiliares e não cria uma segunda estrutura proporcional à quantidade de registros. Assim, sua complexidade de espaço auxiliar é **O(1)**.

### Complexidade de Espaço

Além do tempo de execução, é importante considerar a memória utilizada pelas estruturas.

- **Lista Encadeada:** Cada reclamação exige um nó alocado dinamicamente. Portanto, o espaço ocupado pela lista cresce proporcionalmente à quantidade de registros, resultando em **O(n)**.
- **Histórico de Respostas:** Cada nó possui espaço reservado para até 100 respostas. Como esse limite é fixo, o espaço de cada nó é constante em relação a **n**.
- **Ordenação:** O Selection Sort utiliza variáveis auxiliares para comparar e trocar os dados, sem criar uma segunda lista. Seu espaço auxiliar é **O(1)**.

Considerando toda a estrutura de dados, o espaço total utilizado pelo sistema é **O(n)**.

---

## Testes e Validação

O sistema foi desenvolvido considerando diferentes cenários de utilização, incluindo operações comuns, entradas inválidas e situações em que não existem registros cadastrados.

Os testes devem contemplar os seguintes cenários:

| Teste | Cenário | Resultado Esperado |
|---|---|---|
| Cadastro válido | Inserir uma reclamação com todos os campos preenchidos. | Reclamação cadastrada com sucesso e ID gerado automaticamente. |
| Cadastro incompleto | Tentar cadastrar uma reclamação deixando um campo obrigatório vazio. | Cadastro rejeitado e memória alocada liberada. |
| Consulta de lista vazia | Acessar a listagem sem reclamações cadastradas. | Mensagem informando que não existem registros. |
| Pesquisa por ID existente | Informar o ID de uma reclamação cadastrada. | Exibição dos detalhes da reclamação correspondente. |
| Pesquisa por ID inexistente | Informar um ID que não está cadastrado. | Mensagem informando que o registro não foi encontrado. |
| Pesquisa por escola | Pesquisar utilizando o nome completo ou parcial de uma escola. | Exibição das reclamações correspondentes, ignorando diferenças entre letras maiúsculas e minúsculas. |
| Pesquisa sem resultados | Informar um nome de escola sem correspondências. | Mensagem informando que nenhuma reclamação foi encontrada. |
| Atualização | Alterar o título, a escola, a descrição ou o status de uma reclamação. | Dados atualizados corretamente. |
| Exclusão | Excluir uma reclamação existente. | Registro removido e memória liberada corretamente. |
| Ordenação | Ordenar os registros por ID, quantidade de mensagens e status. | Reclamações organizadas conforme o critério selecionado. |
| Histórico de mensagens | Adicionar mensagens a uma reclamação. | Mensagens armazenadas e contador atualizado. |
| Limite de mensagens | Tentar adicionar uma mensagem quando o histórico já possui 100 respostas. | Operação impedida, sem ultrapassar a capacidade do vetor. |
| Entrada inválida | Informar letras ou opções fora do intervalo esperado nos menus. | Entrada inválida identificada e usuário orientado a tentar novamente. |
| Encerramento | Encerrar o programa após cadastrar reclamações. | Memória ocupada pela lista encadeada liberada corretamente. |

Os testes permitem verificar o funcionamento das operações principais, a integridade da lista encadeada, a validação das entradas e o gerenciamento da memória dinâmica.

---

## Como Executar o Projeto

1. Certifique-se de ter um compilador para a linguagem C instalado, como o `gcc`.
2. Baixe ou clone o repositório e abra o terminal na pasta onde o arquivo `main.c` está salvo.
3. Compile o código executando o comando:

   ```bash
   gcc main.c -o citizen_assist
   ```

4. Execute o binário gerado:

   - **Linux/Mac:**
     ```bash
     ./citizen_assist
     ```

   - **Windows:**
     ```bash
     citizen_assist.exe
     ```

---

## Observações

- O sistema utiliza armazenamento em memória, portanto os registros são perdidos quando o programa é encerrado.
- Cada reclamação possui capacidade para armazenar até 100 mensagens.
- O projeto foi desenvolvido em linguagem C, utilizando alocação dinâmica, structs, ponteiros e lista encadeada.
- A ordenação é realizada por meio de uma implementação própria do algoritmo Selection Sort.

