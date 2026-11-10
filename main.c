#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// respostas individuais de tokens
typedef struct Resposta {
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
    novo->id = novo_id;
    strcpy(novo->status, "Pendente");
    novo->qtd_respostas = 0;

    printf("Titulo do topico: ");
    fgets(novo->titulo, sizeof(novo->titulo), stdin);
    novo->titulo[strcspn(novo->titulo, "\n")] = '\0';

    printf("Nome da Escola que esse pedido interessa: ");
    fgets(novo->escola, sizeof(novo->escola), stdin);
    novo->escola[strcspn(novo->escola, "\n")] = '\0';

    printf("Descricao do topico: ");
    fgets(novo->descricao, sizeof(novo->descricao), stdin);
    novo->descricao[strcspn(novo->descricao, "\n")] = '\0';

    novo->proximo = head;

    return novo;

}

// transforma statuses em numeros para reordena-los
int obter_peso(char* status){
    if (strcmp(status, "Pendente") == 0){
        return 1;
    }
    
    if (strcmp(status, "Em andamento") == 0){
        return 2;
    }

    if (strcmp(status, "Finalizado") == 0){
        return 3;
    }
    return 0;
}

// funcao de trocar dados para reordenação, extraida para não ter que repetir 3 vezes
void trocar_dados(Reclamacao* i, Reclamacao* j) {
    int temp_id = i->id;
    i->id = j->id;
    j->id = temp_id;

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
    // se a lista estiver vazia ou tiver apenas um elemento, não precisa ordenar
    if (head == NULL || head->proximo == NULL){
        printf("Nao ha como ordernar um ticket.");
        return;    
    } 

    int modo;
    int ch;

    printf("1 - Ordenacao por ID\n2 - Ordenacao por Atividade\n3 - Ordenacao por Status\n");
    scanf("%d", &modo);
    while ((ch = getchar()) != '\n' && ch != EOF);

    switch(modo){
        case 1:
        {
            Reclamacao* i = head;
    
        // vai do primeiro até o penúltimo nó
        while (i != NULL) {
            Reclamacao* j = i->proximo; // 'j' começa sempre uma posição à frente de 'i'
            
            // vai do segundo até o ultimo
            while (j != NULL) {
                
                // criterio de troca
                if (j->id < i->id) {
                    trocar_dados(i, j); // troca em si é feita em trocar_dados()
                }
                j = j->proximo; // avança o 'j'
            }
            i = i->proximo; // avança o 'i'
        }
        break;
        }
        case 2:
        {
            Reclamacao* i = head;
    
        while (i != NULL) {
            Reclamacao* j = i->proximo; 
            
            while (j != NULL) {
                
                if (j->qtd_respostas < i->qtd_respostas) {
                    trocar_dados(i, j);
                }
                j = j->proximo; 
            }
            i = i->proximo; 
        }
        break;

        }
        
        case 3:
        {
            Reclamacao* i = head;
    

        while (i != NULL) {
            Reclamacao* j = i->proximo; 
            
            while (j != NULL) {
                
                if (obter_peso(j->status) < obter_peso(i->status)) {
                    trocar_dados(i, j);
                }
                j = j->proximo; 
            }
            i = i->proximo; 
        }
        break;
        }
        
        default:
        return;

    }

}

// pode mandar mensagens dentro de um thread se estiver em visualizar_detalhes()
void mandar_mensagem(Reclamacao* head, int id_busca){
    Reclamacao* atual = head;

    while (atual != NULL){
        if (atual->id == id_busca) {
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
            for (int i = 0; i < atual->qtd_respostas; i++){
                printf("----------------------------------------\n");
                printf("@%s\n", atual->lista_respostas[i].autor);
                printf("   %s\n", atual->lista_respostas[i].mensagem);
                printf("----------------------------------------\n");
            }
            if (atual->qtd_respostas == 0){
                printf("(Ninguem respondeu a esse Token ainda)\n");
            }

            char resposta = 'S';
            int ch;

            printf("1 - Mandar Mensagem\n2 - Finalizar Thread (Problema Resolvido)\n3 - Sair\n");
            scanf("%c", &resposta);
            while ((ch = getchar()) != '\n' && ch != EOF);

            switch(resposta){
                case 1:{
                    // abre denovo instantaneamente para pode ver as novas mensagens
                    mandar_mensagem(head, id_busca);
                    visualizar_detalhes(head, id_busca);
                    break;
                }
                case 2:{
                    strcpy(atual->status, "Finalizado");
                    break;
                }
                case 3:{
                    return;
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

    int id = 0;
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
        scanf("%d", &resposta);
        while ((ch = getchar()) != '\n' && ch != EOF);

        switch(resposta){
            case 1:{
                printf("Qual ID do Thread que quer entrar? ");
                scanf("%d", &id);
                while ((ch = getchar()) != '\n' && ch != EOF);

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
    
    int id = 0;
    int ch;
    int resposta;
    int forum = 1;
    char escola[50];

    printf("Nome da escola para filtrar: ");
    fgets(escola, sizeof(escola), stdin);
    escola[strcspn(escola, "\n")] = '\0';

    for (int k = 0; escola[k]; k++) {
        escola[k] = tolower(escola[k]);
    }


    while (forum == 1){
        Reclamacao* atual = head;
        while (atual != NULL){
            char escola_no_minusculo[100];
            int k = 0;
            for (k = 0; atual->escola[k]; k++) {
                escola_no_minusculo[k] = tolower(atual->escola[k]);
            }
            escola_no_minusculo[k] = '\0'; // Fecha a string de forma segura

            if (strstr(escola_no_minusculo, escola) != NULL){
                printf("\nId: %d\n", atual->id);
                printf("'%s'\n", atual->titulo);
                printf("Escola: %s\n", atual->escola);
                printf("Descricao: %s\n", atual->descricao);
                printf("Status: %s\n", atual->status);
                printf("Numero de mensagens desse topico: %d\n\n", atual->qtd_respostas);
            }

            atual = atual->proximo;
        }
        printf("1 - Entrar em Thread\n2 - Ordenar Tickets\n3 - Sair\n\n");
        scanf("%d", &resposta);
        while ((ch = getchar()) != '\n' && ch != EOF);

        switch(resposta){
            case 1:{
                printf("Qual ID do Thread que quer entrar? ");
                scanf("%d", &id);
                while ((ch = getchar()) != '\n' && ch != EOF);

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
        }

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
        printf("1 - Criar Token\n2 - Acessar Forum\n3 - Acessar Forum da Sua Escola\n4 - Dashboard\n5 - Remocao de Token\n6 - Sair\n\n");
        scanf("%d", &escolha);
        while ((ch = getchar()) != '\n' && ch != EOF);


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
                    scanf("%d", &id_delecao);
                    while ((ch = getchar()) != '\n' && ch != EOF);

                    lista_educacao = excluir(lista_educacao, id_delecao);
                }
                break;
            }
            case 6:{
                sistema = 0;
            }
            default:{
                break;
            }
        }
    }

    return 0;
}