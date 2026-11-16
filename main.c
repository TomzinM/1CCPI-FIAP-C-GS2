#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// respostas individuais de tokens
typedef struct {
    char autor[50];
    char mensagem[500];
} Resposta;

// struct principal para reclamacoes (tokens)
typedef struct Reclamacao {
    int id;
    char escola[100];
    char titulo[50];
    char descricao[250];
    char status[20];
    
    Resposta lista_respostas[100];  // cada token tem um limite de 100 respostas
    int qtd_respostas;
    struct Reclamacao* proximo; // Aponta para a PRÓXIMA reclamação do sistema
} Reclamacao;   


// cria um token novo, registra a informação, traz de volta para a main
Reclamacao* cadastrar(Reclamacao* head, int novo_id){
    Reclamacao* novo = malloc(sizeof(Reclamacao));

    if (novo == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return head;
    }

    novo->id = novo_id;
    strcpy(novo->status, "Pendente");
    novo->qtd_respostas = 0;
    memset(novo->lista_respostas, 0, sizeof(novo->lista_respostas));

    printf("Titulo do topico: ");
    fgets(novo->titulo, sizeof(novo->titulo), stdin);
    novo->titulo[strcspn(novo->titulo, "\n")] = '\0';

    if (strlen(novo->titulo) == 0) {
        printf("O titulo nao pode ficar vazio.\n");
        free(novo);
        return head;
    }

    printf("Nome da Escola que esse pedido interessa: ");
    fgets(novo->escola, sizeof(novo->escola), stdin);
    novo->escola[strcspn(novo->escola, "\n")] = '\0';

    if (strlen(novo->escola) == 0) {
        printf("O nome da escola nao pode ficar vazio.\n");
        free(novo);
        return head;
    }

    printf("Descricao do topico: ");
    fgets(novo->descricao, sizeof(novo->descricao), stdin);
    novo->descricao[strcspn(novo->descricao, "\n")] = '\0';

    if (strlen(novo->descricao) == 0) {
        printf("A descricao nao pode ficar vazia.\n");
        free(novo);
        return head;
    }

    novo->proximo = head;

    return novo;

}

// transforma statuses em numeros para reordena-los
int obter_peso(char* status){
    if (strcmp(status, "Pendente") == 0){
        return 1;
    }
    
    if (strcmp(status, "Em Andamento") == 0){
        return 2;
    }

    if (strcmp(status, "Finalizado") == 0){
        return 3;
    }
    return 0;
}

int ler_inteiro() {
    int valor;
    int ch;

    if (scanf("%d", &valor) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF);
        return -1;
    }

    while ((ch = getchar()) != '\n' && ch != EOF);

    return valor;
}

// funcao de trocar dados para reordenação, extraida para não ter que repetir 3 vezes
void trocar_dados(Reclamacao* i, Reclamacao* j) {
    int temp_id = i->id;
    i->id = j->id;
    j->id = temp_id;

    char temp_titulo[50];
    strcpy(temp_titulo, i->titulo);
    strcpy(i->titulo, j->titulo);
    strcpy(j->titulo, temp_titulo);

    char temp_escola[100];
    strcpy(temp_escola, i->escola);
    strcpy(i->escola, j->escola);
    strcpy(j->escola, temp_escola);

    char temp_descricao[250];
    strcpy(temp_descricao, i->descricao);
    strcpy(i->descricao, j->descricao);
    strcpy(j->descricao, temp_descricao);

    char temp_status[20];
    strcpy(temp_status, i->status);
    strcpy(i->status, j->status);
    strcpy(j->status, temp_status);

    for (int k = 0; k < 100; k++) {
        Resposta temp_resp = i->lista_respostas[k];
        i->lista_respostas[k] = j->lista_respostas[k];
        j->lista_respostas[k] = temp_resp;
    }

    int temp_qtd_respostas = i->qtd_respostas;
    i->qtd_respostas = j->qtd_respostas;
    j->qtd_respostas = temp_qtd_respostas;
}

// ordena todos threads de tres formas diferentes 
void ordenar(Reclamacao* head) {
    if (head == NULL || head->proximo == NULL) {
        printf("Nao ha chamados suficientes para ordenar.\n");
        return;
    }

    int modo;
    int ch;

    printf("1 - Ordenacao por ID\n");
    printf("2 - Ordenacao por Atividade\n");
    printf("3 - Ordenacao por Status\n");

    modo = ler_inteiro();

    if (modo == -1) {
        printf("Entrada invalida. Digite um numero.\n");
        return;
    }

    if (modo < 1 || modo > 3) {
        printf("Opcao invalida.\n");
        return;
    }

    Reclamacao* i = head;

    // Selection Sort
    while (i != NULL) {

        Reclamacao* menor = i;
        Reclamacao* j = i->proximo;

        // Procura o menor elemento do restante da lista
        while (j != NULL) {

            int trocar = 0;

            switch (modo) {
                case 1:
                    trocar = j->id < menor->id;
                    break;

                case 2:
                    trocar = j->qtd_respostas < menor->qtd_respostas;
                    break;

                case 3:
                    trocar = obter_peso(j->status) <
                             obter_peso(menor->status);
                    break;
            }

            if (trocar) {
                menor = j;
            }

            j = j->proximo;
        }

        // Faz somente uma troca ao terminar a busca
        if (menor != i) {
            trocar_dados(i, menor);
        }

        i = i->proximo;
    }

    printf("Ordenacao realizada com sucesso!\n");
}


// pode mandar mensagens dentro de um thread se estiver em visualizar_detalhes()
void mandar_mensagem(Reclamacao* head, int id_busca){
    Reclamacao* atual = head;

    while (atual != NULL){
        if (atual->id == id_busca) {
            if (atual->qtd_respostas >= 100) {
                printf("Essa thread atingiu o limite de respostas.\n");
                return;
            }

            printf("Autor: ");
            fgets(atual->lista_respostas[atual->qtd_respostas].autor, sizeof(atual->lista_respostas[atual->qtd_respostas].autor), stdin);
            atual->lista_respostas[atual->qtd_respostas].autor[strcspn(atual->lista_respostas[atual->qtd_respostas].autor, "\n")] = '\0';

            printf("Mensagem: ");
            fgets(atual->lista_respostas[atual->qtd_respostas].mensagem, sizeof(atual->lista_respostas[atual->qtd_respostas].mensagem), stdin);
            atual->lista_respostas[atual->qtd_respostas].mensagem[strcspn(atual->lista_respostas[atual->qtd_respostas].mensagem, "\n")] = '\0';
            
            strcpy(atual->status, "Em Andamento");
            atual->qtd_respostas++;

            return;

        }
        atual = atual->proximo;   
    }
    printf("O Id %d nao foi encontrado.\n", id_busca);
}

// veja todas mensagens dentro de um thread
void visualizar_detalhes(Reclamacao* head, int id_busca){
    Reclamacao* atual = head;

    while (atual != NULL){
        if (atual->id == id_busca) {
            
            int respondendo = 1; 
            while (respondendo == 1) {

            for (int i = 0; i < atual->qtd_respostas; i++){
                printf("----------------------------------------\n");
                printf("@%s\n", atual->lista_respostas[i].autor);
                printf("   %s\n", atual->lista_respostas[i].mensagem);
                printf("----------------------------------------\n");
            }
            if (atual->qtd_respostas == 0){
                printf("(Ninguem respondeu a esse Token ainda)\n");
            }

            int resposta;
            int ch;

            printf("1 - Mandar Mensagem\n2 - Finalizar Thread (Problema Resolvido)\n3 - Sair\n");
            resposta = ler_inteiro();

            if (resposta == -1) {
                printf("Entrada invalida. Digite um numero.\n");
                continue;
            }

            switch(resposta){
                case 1:{
                    if (strcmp(atual->status, "Finalizado") == 0) {
                        printf("Nao e possivel mandar mensagens em um thread finalizado.\n");
                        break;
                    }

                    mandar_mensagem(head, id_busca);
                    break;
                }
                case 2:{
                    strcpy(atual->status, "Finalizado");
                    respondendo = 0;
                    break;
                }
                case 3:{
                    respondendo = 0;
                    return;
                }
                    default:{
                        printf("Opcao invalida. Tente novamente.\n");
                        break;
                }
            }
        }
            return;
        }
        atual = atual->proximo;
    }


    printf("O Id %d nao foi encontrado.\n", id_busca);

}


// lista de todos threads com todas informações relacionadas
void listar(Reclamacao* head){
    if (head == NULL){
        printf("\nNao ha reclamacoes cadastradas.\n");
        return;
    }

    int id;
    int ch;
    int resposta;
    int forum = 1;
    while (forum == 1){
        Reclamacao* atual = head;
        while (atual != NULL){
            printf("\nId: %d\n", atual->id);
            printf("'%s'\n", atual->titulo);
            printf("Escola: %s\n", atual->escola);
            printf("Descricao: %s\n", atual->descricao);
            printf("Status: %s\n", atual->status);
            printf("Numero de mensagens desse topico: %d\n\n", atual->qtd_respostas);
            atual = atual->proximo;
        }
        printf("1 - Entrar em Thread\n2 - Ordenar Tickets\n3 - Sair\n\n");
        resposta = ler_inteiro();

        if (resposta == -1) {
            printf("Entrada invalida. Digite um numero.\n");
            continue;
        }

        switch(resposta){
            case 1:{
                printf("Qual ID do Thread que quer entrar? ");
                id = ler_inteiro();
                if (id == -1) {
                    printf("ID invalido. Digite um numero.\n");
                    break;
                }

                visualizar_detalhes(head, id);
                break;
            }
            
            case 2:{
                ordenar(head);
                break;
            }
            case 3:{
                forum = 0;
                break;
            }
            default:{
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
        }

    }
    

}

// painel mostra se tem reclamacoes faltando, quantas estao ocorrendo, quantas ja terminaram, etc
void painel(Reclamacao* head){
    int total = 0;
    int pendentes = 0;
    int andamento = 0;
    int finalizados = 0;
    int mensagens = 0;

    Reclamacao* atual = head;

    while (atual != NULL){
        if (strcmp(atual->status, "Pendente") == 0){
            pendentes++;
        }
        if (strcmp(atual->status, "Em Andamento") == 0){
            andamento++;
        }
        if (strcmp(atual->status, "Finalizado") == 0){
            finalizados++;
        }
        total++;
        mensagens += atual->qtd_respostas;
        atual = atual->proximo;
    }

    printf("=================================\n");
    printf("    Citizen Education Complaint Report\n   \n");
    printf("=================================\n");
    printf("Total de Casos Registrados: %d\nReclamacoes Pendentes: %d\nEm Atendimento: %d\nCasos Finalizados: %d\nTotal de Mensagens Enviadas: %d\n", total, pendentes, andamento, finalizados, mensagens);
    printf("=================================\n\n");


}

// remove threads especificos, ja que ja existe a função de finalizar isso tecnicamente so seria feito em casos extremos
Reclamacao* excluir(Reclamacao* head, int id_busca) {
    if (head == NULL) {
        printf("\nNao ha chamados cadastrados para excluir.\n");
        return NULL;
    }

    // testa se o ID ta no head primeiro
    if (head->id == id_busca) {
        Reclamacao* temp = head;
        head = head->proximo;
        free(temp);
        printf("\nId %d excluido com sucesso!\n", id_busca);
        return head;
    }

    // caminha pela lista inteira se nao
    Reclamacao* atual = head;
    Reclamacao* anterior = NULL;

    while (atual != NULL) {
        if (atual->id == id_busca) {
            anterior->proximo = atual->proximo; // pula o atual
            free(atual);
            printf("\nId %d excluido com sucesso!\n", id_busca);
            return head; 
        }
        
        // o anterior sempre fica um a tras para poder fazer o ato de pular por cima do atual
        anterior = atual;
        atual = atual->proximo;
    }

    printf("\nO ID %d nao foi encontrado para exclusao.\n", id_busca);
    return head;
}

// mesma coisa de listar porem faz uso the tolower e strstr para encontra apenas especificamente reclamacoes relacionadas a escolas especificas
void listar_escola(Reclamacao* head){

    if (head == NULL){
        printf("\nNao ha reclamacoes cadastradas.\n");
        return;
    }

    int id;
    int ch;
    int resposta;
    int forum = 1;
    char escola[50];

    printf("Nome da escola para filtrar: ");
    fgets(escola, sizeof(escola), stdin);
    escola[strcspn(escola, "\n")] = '\0';

    // impede pesquisa vazia
    if (strlen(escola) == 0) {
        printf("O nome da escola nao pode ficar vazio.\n");
        return;
    }

    // converte a pesquisa para letras minusculas
    for (int k = 0; escola[k]; k++) {
        escola[k] = tolower((unsigned char)escola[k]);
    }

    while (forum == 1){

        Reclamacao* atual = head;
        int encontrou = 0;

        while (atual != NULL){

            char escola_no_minusculo[100];
            int k = 0;

            // converte o nome da escola cadastrada para minusculo
            for (k = 0; atual->escola[k]; k++) {
                escola_no_minusculo[k] =
                    tolower((unsigned char)atual->escola[k]);
            }

            escola_no_minusculo[k] = '\0';

            // verifica se a escola corresponde a pesquisa
            if (strstr(escola_no_minusculo, escola) != NULL){

                encontrou = 1;

                printf("\nId: %d\n", atual->id);
                printf("'%s'\n", atual->titulo);
                printf("Escola: %s\n", atual->escola);
                printf("Descricao: %s\n", atual->descricao);
                printf("Status: %s\n", atual->status);
                printf("Numero de mensagens desse topico: %d\n\n",
                       atual->qtd_respostas);
            }

            atual = atual->proximo;
        }

        // informa quando nenhuma reclamacao corresponde a pesquisa
        if (encontrou == 0) {
            printf("Nenhuma reclamacao encontrada para essa escola.\n");
            return;
        }

        printf("1 - Entrar em Thread\n");
        printf("2 - Ordenar Tickets\n");
        printf("3 - Sair\n\n");

        resposta = ler_inteiro();
        
        if (resposta == -1) {
            printf("Entrada invalida. Digite um numero.\n");
            continue;
        }

        switch(resposta){

            case 1:{
                printf("Qual ID do Thread que quer entrar? ");
                id = ler_inteiro();
                if (id == -1) {
                    printf("ID invalido. Digite um numero.\n");
                    break;
                }

                visualizar_detalhes(head, id);
                break;
            }

            case 2:{
                ordenar(head);
                break;
            }

            case 3:{
                forum = 0;
                break;
            }

            default:{
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
        }
    }
}

Reclamacao* atualizar(Reclamacao* head, int id_atualizacao){

    Reclamacao* atual = head;

    while (atual != NULL){
        if (atual->id == id_atualizacao) {
            
            int atualizando = 1;
            while (atualizando == 1) {
                int escolha;

                printf("\nId: %d\n", atual->id);
                printf("'%s'\n", atual->titulo);
                printf("Escola: %s\n", atual->escola);
                printf("Descricao: %s\n", atual->descricao);
                printf("Status: %s\n", atual->status);
                printf("Numero de mensagens desse topico: %d\n\n", atual->qtd_respostas);
                
                printf("O que deseja atualizar?\n1 - Titulo\n2 - Escola\n3 - Descricao\n4 - Status\n5 - Sair\n");
                escolha = ler_inteiro();

                if (escolha == -1) {
                    printf("Entrada invalida. Digite um numero.\n");
                    continue;
                }

                switch(escolha){
                    case 1:{
                        do {
                            printf("Novo Titulo do topico: ");
                            fgets(atual->titulo, sizeof(atual->titulo), stdin);
                            atual->titulo[strcspn(atual->titulo, "\n")] = '\0';

                            if (strlen(atual->titulo) == 0) {
                                printf("O titulo nao pode ficar vazio.\n");
                            }
                        } while (strlen(atual->titulo) == 0);

                        break;
                    }
                    case 2:{
                        do {
                            printf("Nova Escola que esse pedido interessa: ");
                            fgets(atual->escola, sizeof(atual->escola), stdin);
                            atual->escola[strcspn(atual->escola, "\n")] = '\0';

                            if (strlen(atual->escola) == 0) {
                                printf("O nome da escola nao pode ficar vazio.\n");
                            }
                        } while (strlen(atual->escola) == 0);

                        break;
                    }
                    case 3:{
                        do {
                            printf("Nova Descricao do topico: ");
                            fgets(atual->descricao, sizeof(atual->descricao), stdin);
                            atual->descricao[strcspn(atual->descricao, "\n")] = '\0';

                            if (strlen(atual->descricao) == 0) {
                                printf("A descricao nao pode ficar vazia.\n");
                            }
                        } while (strlen(atual->descricao) == 0);
                        
                        
                        break;
                    }
                    case 4:{
                        int status_opcao;

                        do {
                            printf("Novo Status do topico (1 - Pendente/2 - Em Andamento/3 - Finalizado): ");
                            status_opcao = ler_inteiro();

                            switch(status_opcao){
                                case 1:
                                    strcpy(atual->status, "Pendente");
                                    break;
                                case 2:
                                    strcpy(atual->status, "Em Andamento");
                                    break;
                                case 3:
                                    strcpy(atual->status, "Finalizado");
                                    break;
                                default:
                                    printf("Opcao invalida. Tente novamente.\n");
                                    break;
                            }

                        } while (status_opcao < 1 || status_opcao > 3);

                        break;
                    }
                    case 5:{
                        atualizando = 0;
                        break;
                    }
                    default:{
                        printf("Opcao invalida. Tente novamente.\n");
                        break;
                    }
                }
                
            }
            
            return head;
        }
        atual = atual->proximo;
    }

    printf("O Id %d nao foi encontrado para atualizacao.\n", id_atualizacao);
    return head;
}

void liberar_lista(Reclamacao* head) {
    Reclamacao* atual = head;

    while (atual != NULL) {
        Reclamacao* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}




// função principal
int main(){
    Reclamacao* lista_educacao = NULL;
    int escolha;
    int sistema = 1;
    int id = 0;
    int ch;
    while(sistema == 1){
        printf("--- Citizen Education Complaint ---\n");
        printf("1 - Criar Token\n2 - Acessar Forum\n3 - Acessar Forum da Sua Escola\n4 - Dashboard\n5 - Remocao de Token (ADMIN)\n6 - Update Token (ADMIN)\n7 - Sair\n\n");
        escolha = ler_inteiro();

        if (escolha == -1) {
            printf("Entrada invalida. Digite um numero.\n");
            continue;
        }


        switch(escolha){
            case 1:{
                lista_educacao = cadastrar(lista_educacao, id);
                id++;
                break;
            }
            case 2:{
                listar(lista_educacao);
                break;
            }
            case 3:{
                listar_escola(lista_educacao);
                break;
            }
            case 4:{
                painel(lista_educacao);
                break;
            }
            case 5:{
                char continuar;
                int id_delecao;
                printf("Apenas remova um Thread se e de alguma forma malicioso. Quer continuar? (S/N) ");
                scanf("%c", &continuar);
                while ((ch = getchar()) != '\n' && ch != EOF);

                if (continuar == 's' || continuar == 'S'){
                    printf("Qual id do Thread sendo removido? ");
                    id_delecao = ler_inteiro();

                    if (id_delecao == -1) {
                        printf("ID invalido. Digite um numero.\n");
                        break;
                    }

                    lista_educacao = excluir(lista_educacao, id_delecao);
                }
                break;
            }
            case 6:{
                char continuar;
                int id_atualizacao;
                printf("Deseja atualizar um Token? (S/N) ");
                scanf("%c", &continuar);
                while ((ch = getchar()) != '\n' && ch != EOF);

                if (continuar == 's' || continuar == 'S'){
                    printf("Qual id do Token sendo atualizado? ");
                    id_atualizacao = ler_inteiro();

                    if (id_atualizacao == -1) {
                        printf("ID invalido. Digite um numero.\n");
                        break;
                    }

                    lista_educacao = atualizar(lista_educacao, id_atualizacao);
                }
                break;
            }
            case 7:{
                liberar_lista(lista_educacao);
                lista_educacao = NULL;
                sistema = 0;
                break;
            }
            default: {
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
        }
    }

    return 0;
}
