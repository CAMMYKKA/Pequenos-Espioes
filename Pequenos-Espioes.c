#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct agentes{
    char nome[50];
    char especie[50];
    char idade[50];
    int id;
};

struct contratante{
    char nome[50];
    char cargo[30];
    char cpf[12];
};

struct missoes{
    int id_missao;
    int id_agente;
    char cpf_contratante[15];
    char motivo[100];
    char resultado_da_missao[100];
    float tempo_de_missao;
};

void cad_contratante(struct contratante contratantes[], int *qtdcontratantes, char cpf[], char nome[], char cargo[], int *capacidade_contratante){
    if (*qtdcontratantes >= *capacidade_contratante) {
        *capacidade_contratante *= 2; 
        struct contratante *novo = realloc(contratantes, *capacidade_contratante * sizeof(struct contratante));
        if (!novo) {
            printf("Erro ao realocar agentes!\n");
            free(contratantes);
            return;
        }
    }
    strcpy(contratantes[*qtdcontratantes].cpf, cpf);
    strcpy(contratantes[*qtdcontratantes].nome, nome);
    strcpy(contratantes[*qtdcontratantes].cargo, cargo);
    (*qtdcontratantes)++;
}

void cad_agentes(struct agentes agentes[], int *qtdagentes, int id, char nomeagente[], char especie[], char idade[], int * capacidade_agentes){
    if (*qtdagentes >= *capacidade_agentes) {
        *capacidade_agentes *= 2; 
        struct agentes *novo = realloc(agentes, *capacidade_agentes * sizeof(struct agentes));
        if (!novo) {
            printf("Erro ao realocar agentes!\n");
            free(agentes);
            return;
        }
        agentes = novo;
    }
    
    strcpy(agentes[*qtdagentes].nome, nomeagente);
    strcpy(agentes[*qtdagentes].especie, especie);
    strcpy(agentes[*qtdagentes].idade, idade);
    agentes[*qtdagentes].id =  id;
    (*qtdagentes)++;
}

void cad_missoes(struct missoes missoes[], int *qtdmissoes, int idmissao, int escolha, char escolha1[], int resultado, float tempomissao, char motivo[], int *capacidade_missoes){
    if (*qtdmissoes >= *capacidade_missoes) {
        *capacidade_missoes *= 2; 
        struct missoes *novo = realloc(missoes, *capacidade_missoes * sizeof(struct missoes));
        if (!novo) {
            printf("Erro ao realocar agentes!\n");
            free(missoes);
            return;
        }
        missoes = novo;
    }
    
    strcpy(missoes[*qtdmissoes].cpf_contratante, escolha1);
    strcpy(missoes[*qtdmissoes].motivo, motivo);
    missoes[*qtdmissoes].tempo_de_missao = tempomissao;
    missoes[*qtdmissoes].id_agente =  escolha;
    missoes[*qtdmissoes].id_missao = idmissao;

    if(resultado == 1){
        strcpy(missoes[*qtdmissoes].resultado_da_missao, "Bem Sucedida!");
    }else if(resultado == 2){
        strcpy(missoes[*qtdmissoes].resultado_da_missao, "Mal Sucedida!");
    }

    (*qtdmissoes)++;
}

void salvar_agentes(struct agentes agentes[], int qtdagentes) {
    FILE *arquivo = fopen("agentes.bin", "wb");
    if (arquivo == NULL) {
        printf("\nErro ao salvar agentes.");
        return;
    }

    fwrite(&qtdagentes, sizeof(int), 1, arquivo);
    fwrite(agentes, sizeof(struct agentes), qtdagentes, arquivo);
    fclose(arquivo);
}

void salvar_contratantes(struct contratante contratantes[], int qtdcontratante) {
    FILE *arquivo = fopen("contratantes.bin", "wb");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo de contratantes!");
        return;
    }

    fwrite(&qtdcontratante, sizeof(int), 1, arquivo); 
    fwrite(contratantes, sizeof(struct contratante), qtdcontratante, arquivo); 
    fclose(arquivo);
}

void salvar_missoes(struct missoes missoes[], int qtdmissoes) {
    FILE *arquivo = fopen("missoes.bin", "wb");
    if (arquivo == NULL) {
        printf("\nErro ao abrir o arquivo de missoes!");
        return;
    }

    fwrite(&qtdmissoes, sizeof(int), 1, arquivo); 
    fwrite(missoes, sizeof(struct missoes), qtdmissoes, arquivo); 
    fclose(arquivo);
}

void carregar_agentes(struct agentes agentes[], int *qtdagentes) {
    FILE *arquivo = fopen("agentes.bin", "rb");
    if (arquivo == NULL) {
        printf("\nNenhum arquivo de agentes encontrado, iniciando vazio.");
        *qtdagentes = 0;
        return;
    }
    
    fread(qtdagentes, sizeof(int), 1, arquivo); 
    fread(agentes, sizeof(struct agentes), *qtdagentes, arquivo);
    fclose(arquivo);
}

void carregar_contratantes(struct contratante contratantes[], int *qtdcontratante) {
    FILE *arquivo = fopen("contratantes.bin", "rb");
    if (arquivo == NULL) {
        printf("\nNenhum arquivo de contratantes encontrado, iniciando vazio.");
        *qtdcontratante = 0;
        return;
    }

    fread(qtdcontratante, sizeof(int), 1, arquivo); 
    fread(contratantes, sizeof(struct contratante), *qtdcontratante, arquivo); 
    fclose(arquivo);
}

void carregar_missoes(struct missoes missoes[], int *qtdmissoes) {
    FILE *arquivo = fopen("missoes.bin", "rb");
    if (arquivo == NULL) {
        printf("\nNenhum arquivo de missoes encontrado, iniciando vazio.");
        *qtdmissoes = 0;
        return;
    }

    fread(qtdmissoes, sizeof(int), 1, arquivo); 
    fread(missoes, sizeof(struct missoes), *qtdmissoes, arquivo); 
    fclose(arquivo);
}

void lerArquivosAgente(struct agentes **agentes, int *quantidadeAgentes) {
    FILE *arquivoAgentes = fopen("agentes.txt", "r");
    if (arquivoAgentes == NULL) {
        printf("Erro ao abrir o arquivo de agentes.\n");
        *quantidadeAgentes = 0;
        return;
    }

    int capacidadeAgentes = 10;
    *quantidadeAgentes = 0;
    *agentes = malloc(capacidadeAgentes * sizeof(struct agentes));
    
    if (*agentes == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivoAgentes);
        *quantidadeAgentes = 0;
        return;
    }

    char linha[256]; 

    while (fgets(linha, sizeof(linha), arquivoAgentes) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        if (*quantidadeAgentes >= capacidadeAgentes) {
            capacidadeAgentes *= 2;  
            struct agentes *temp = realloc(*agentes, capacidadeAgentes * sizeof(struct agentes));
            if (temp == NULL) {
                printf("Erro ao realocar memória.\n");
                break;
            }
            *agentes = temp;
        }

        
        if (sscanf(linha, "%d|%[^|]|%[^|]|%[^|]",
                 &(*agentes)[*quantidadeAgentes].id,
                 (*agentes)[*quantidadeAgentes].nome,
                 (*agentes)[*quantidadeAgentes].especie,
                 (*agentes)[*quantidadeAgentes].idade) == 4) {

            printf("Lido agente %d: ID: %d, Nome: %s, Especie: %s, Idade: %s\n",
                   *quantidadeAgentes + 1,
                   (*agentes)[*quantidadeAgentes].id,
                   (*agentes)[*quantidadeAgentes].nome,
                   (*agentes)[*quantidadeAgentes].especie,
                   (*agentes)[*quantidadeAgentes].idade);

            (*quantidadeAgentes)++;
        } else {
            printf("Formato inválido na linha: %s\n", linha);
        }
    }

    fclose(arquivoAgentes);
    printf("Total de agentes lidos: %d\n", *quantidadeAgentes);
}

void LerArquivosCont(struct contratante **contratantes, int *quantidadeContratantes) {
    FILE *arquivoContratantes = fopen("contratantes.txt", "r");
    if (arquivoContratantes == NULL) {
        printf("Erro ao abrir o arquivo de contratantes.\n");
        *quantidadeContratantes = 0;
        return;
    }

    int capacidadeContratantes = 10;
    *quantidadeContratantes = 0;
    *contratantes = malloc(capacidadeContratantes * sizeof(struct contratante));
    
    if (*contratantes == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivoContratantes);
        *quantidadeContratantes = 0;
        return;
    }

    char linha[256];

    while (fgets(linha, sizeof(linha), arquivoContratantes) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        if (*quantidadeContratantes >= capacidadeContratantes) {
            capacidadeContratantes *= 2;
            struct contratante *temp = realloc(*contratantes, capacidadeContratantes * sizeof(struct contratante));
            if (temp == NULL) {
                printf("Erro ao realocar memória.\n");
                break;
            }
            *contratantes = temp;
        }

        if (sscanf(linha, "%[^|]|%[^|]|%[^|]",
                 (*contratantes)[*quantidadeContratantes].nome,
                 (*contratantes)[*quantidadeContratantes].cargo,
                 (*contratantes)[*quantidadeContratantes].cpf) == 3) {
            
            printf("Lido contratante %d: Nome: %s, Cargo:  %s, CPF:  %s\n", 
                   *quantidadeContratantes + 1,
                   (*contratantes)[*quantidadeContratantes].nome,
                   (*contratantes)[*quantidadeContratantes].cargo,
                   (*contratantes)[*quantidadeContratantes].cpf);

            (*quantidadeContratantes)++;
        } else {
            printf("Formato inválido na linha: %s\n", linha);
        }
    }

    fclose(arquivoContratantes);
    printf("Total de contratantes lidos: %d\n", *quantidadeContratantes);
}

void LerArquivosMissoes(struct missoes **missoes, int *quantidadeMissoes) {
    FILE *arquivoMissoes = fopen("missoes.txt", "r");
    if (arquivoMissoes == NULL) {
        printf("Erro ao abrir o arquivo de missões.\n");
        return;
    }

    int capacidadeMissoes = 10; 
    *missoes = malloc(capacidadeMissoes * sizeof(struct missoes));
    
    if (*missoes == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivoMissoes);
        *quantidadeMissoes = 0;
        return;
    }

    char linha[256];  

    while (fgets(linha, sizeof(linha), arquivoMissoes) != NULL) {
        
        linha[strcspn(linha, "\n")] = '\0';

        
        if (*quantidadeMissoes >= capacidadeMissoes) {
            capacidadeMissoes *= 2;  
            struct missoes *temp = realloc(*missoes, capacidadeMissoes * sizeof(struct missoes));
            if (temp == NULL) {
                printf("Erro ao realocar memória.\n");
                break;
            }
            *missoes = temp;
        }

        
        if (sscanf(linha, "%d|%d|%[^|]|%[^|]|%[^|]|%f",
                 &(*missoes)[*quantidadeMissoes].id_missao,
                 &(*missoes)[*quantidadeMissoes].id_agente,
                 (*missoes)[*quantidadeMissoes].cpf_contratante,
                 (*missoes)[*quantidadeMissoes].motivo,
                 (*missoes)[*quantidadeMissoes].resultado_da_missao,
                 &(*missoes)[*quantidadeMissoes].tempo_de_missao) == 6) {

            printf("Lido missao %d: ID: %d, ID do Agente: %d, CPF do Contratante: %s, Motivo: %s, Resultado: %s, Tempo: %.2f\n",
                   *quantidadeMissoes + 1,
                   (*missoes)[*quantidadeMissoes].id_missao,
                   (*missoes)[*quantidadeMissoes].id_agente,
                   (*missoes)[*quantidadeMissoes].cpf_contratante,
                   (*missoes)[*quantidadeMissoes].motivo,
                   (*missoes)[*quantidadeMissoes].resultado_da_missao,
                   (*missoes)[*quantidadeMissoes].tempo_de_missao);

            (*quantidadeMissoes)++;
        } else {
            printf("Formato inválido na linha: %s\n", linha);
        }
    }

    fclose(arquivoMissoes);
    printf("Total de missoes lidas: %d\n", *quantidadeMissoes);
}

void ver_cadastros(struct agentes agentes[], struct contratante contratantes[], int qtdagentes, int qtdcontratante, int op1){
    if (op1 == 1 && qtdagentes > 0){
        printf("\nAgentes cadastrados:");
        for (int i = 0; i < qtdagentes; i++)
        {
            printf("\n%d. %s [id: %d]", i + 1, agentes[i].nome, agentes[i].id);
        }
    }
    if (op1 == 2 && qtdcontratante > 0){
        printf("\nContratantes Cadastrados:");
        for (int i = 0; i < qtdcontratante; i++)
        {
            printf("\n%d. %s [cpf: %s]", i + 1, contratantes[i].nome, contratantes[i].cpf);
        }
    }
}

int excluir_agente(struct agentes agentes[], int qtdagentes, int escolha, int num){
    if (num == 1){
        for (int i = (escolha - 1); i < qtdagentes; i++)
        {
            agentes[i] = agentes[i + 1];
        }
        return qtdagentes - 1;
    }
    if (num == 2) {
        printf("Voltando ao menu...");
    }
    return qtdagentes;
}

int ver_missoes(struct missoes missoes[], struct contratante contratante[], struct agentes agentes[], int *qtdmissoes, int qtdagentes, int qtdcontratante, int escolha, int op1) {
    int tem_sucesso = 0;
    int tem_falha = 0;

    if (op1 == 1) { 
        printf("\nMissoes bem sucedidas:");

        for (int i = 0; i < *qtdmissoes; i++) {
            if (strcmp(missoes[i].resultado_da_missao, "Bem Sucedida!") == 0) {
                tem_sucesso = 1;
                printf("\n\n%d. [ID da missao: %d]", i + 1, missoes[i].id_missao);
                
                for (int j = 0; j < qtdcontratante; j++) {
                    if (strcmp(contratante[j].cpf, missoes[i].cpf_contratante) == 0) {
                        printf("\nContratante: %s", contratante[j].nome);
                    }
                }
                
                for (int j = 0; j < qtdagentes; j++) {
                    if (agentes[j].id == missoes[i].id_agente) {
                        printf("\nAgente: %s", agentes[j].nome);
                    }
                }
                
                printf("\nMotivo: %s", missoes[i].motivo);
                printf("\nTempo de execucao: %.2f", missoes[i].tempo_de_missao);
                printf("\nResultado da missao: %s", missoes[i].resultado_da_missao);
                printf("\n\n__");
            }
        }

        if (!tem_sucesso) {
            printf("\nNenhuma missao bem sucedida por enquanto!");
        }
    }else if (op1 == 2){
    
        printf("\nMissoes mal sucedidas:");

        for (int i = 0; i < *qtdmissoes; i++) {
            if (strcmp(missoes[i].resultado_da_missao, "Mal Sucedida!") == 0) {
                tem_falha = 1;
                printf("\n\n%d. [ID da missao: %d]", i + 1, missoes[i].id_missao);
        
                for (int j = 0; j < qtdcontratante; j++) {
                    if (strcmp(contratante[j].cpf, missoes[i].cpf_contratante) == 0) {
                        printf("\nContratante: %s", contratante[j].nome);
                    }
                }
        
                for (int j = 0; j < qtdagentes; j++) {
                    if (agentes[j].id == missoes[i].id_agente) {
                        printf("\nAgente: %s", agentes[j].nome);
                
                    }
                }
        
                printf("\nMotivo: %s", missoes[i].motivo);
                printf("\nTempo de execucao: %.2f", missoes[i].tempo_de_missao);
                printf("\nResultado da missao: %s", missoes[i].resultado_da_missao);
                printf("\n\n__");
            }
        }

        if (!tem_falha) {
            printf("\nNenhuma missao mal sucedida por enquanto!");
        }
    }else if (op1 == 3) {
        int tem_rapida = 0;
        printf("\nMissoes terminadas em 1 dia:");

        for (int i = 0; i < *qtdmissoes; i++) {
            if (missoes[i].tempo_de_missao <= 1.0) {
                tem_rapida = 1;
                printf("\n\n%d. [ID da missao: %d]", i + 1, missoes[i].id_missao);
        
                for (int j = 0; j < qtdcontratante; j++) {
                    if (strcmp(contratante[j].cpf, missoes[i].cpf_contratante) == 0) {
                        printf("\nContratante: %s", contratante[j].nome);
                 
                    }
                }
        
                for (int j = 0; j < qtdagentes; j++) {
                    if (agentes[j].id == missoes[i].id_agente) {
                        printf("\nAgente: %s", agentes[j].nome); 
                    }
                }
        
                printf("\nTempo de execucao: %.2f", missoes[i].tempo_de_missao);
                printf("\nResultado da missao: %s", missoes[i].resultado_da_missao);
                printf("\n\n__");
                printf("\nMotivo: %s", missoes[i].motivo);
            }
        }

        if (!tem_rapida) {
            printf("\nNenhuma missao finalizada em 1 dia ainda!");
        }
    }else if (op1 == 4) {
        printf("\nTodas as missoes cadastradas:");
        for (int i = 0; i < *qtdmissoes; i++) {
            printf("\n\n%d. [ID da missao: %d]", i + 1, missoes[i].id_missao);

            for (int j = 0; j < qtdcontratante; j++) {
                if (strcmp(contratante[j].cpf, missoes[i].cpf_contratante) == 0) {
                    printf("\nContratante: %s", contratante[j].nome);
            
                }
            }

            for (int j = 0; j < qtdagentes; j++) {
                if (agentes[j].id == missoes[i].id_agente) {
                    printf("\nAgente: %s", agentes[j].nome);  
                }
            }

            printf("\nMotivo: %s", missoes[i].motivo);
            printf("\nTempo de execucao: %.2f", missoes[i].tempo_de_missao);
            printf("\nResultado da missao: %s", missoes[i].resultado_da_missao);
            printf("\n\n__");
        }
    }else if (op1 == 5) {
        if (qtdagentes == 0) {
            printf("\nNenhum agente cadastrado!");
        }else{
            for (int i = 0; i < qtdagentes; i++) {
                printf("\n%d. %s [id: %d]", i + 1, agentes[i].nome, agentes[i].id);
            }
            
            printf("\nSelecione o agente para ver missoes associadas: ");
            scanf("%d", &escolha);
            setbuf(stdin, NULL);
        
            if (escolha < 1 || escolha > qtdagentes) {
                printf("\nOpcao invalida!");
            }
            else {
                int id_agente = agentes[escolha-1].id;
                int tem_missoes = 0;
                
                for (int i = 0; i < *qtdmissoes; i++) {
                    if (missoes[i].id_agente == id_agente) {
                        tem_missoes = 1;
                        printf("\n\n%d. [ID da missao: %d]", i + 1, missoes[i].id_missao);
                    
                        for (int j = 0; j < qtdcontratante; j++) {
                            if (strcmp(contratante[j].cpf, missoes[i].cpf_contratante) == 0) {
                                printf("\nContratante: %s", contratante[j].nome);
                            }
                        }
                    
                        printf("\nAgente: %s", agentes[escolha-1].nome);
                        printf("\nMotivo: %s", missoes[i].motivo);
                        printf("\nTempo de execucao: %.2f", missoes[i].tempo_de_missao);
                        printf("\nResultado da missao: %s", missoes[i].resultado_da_missao);
                        printf("\n\n__");
                    }
                }
            
                if (!tem_missoes) {
                    printf("\nNenhuma missao encontrada para este agente!");
                    printf("\nPressione qualquer tecla para voltar...");
                    getchar();
                }
            }
            
        }

    } else if (op1 == 6) { 
        if (qtdcontratante == 0) printf("\nNenhum contratante cadastrado!");
        else {
            for (int i = 0; i < qtdcontratante; i++) {
                printf("\n%d. %s [cpf: %s]", i + 1, contratante[i].nome, contratante[i].cpf);
            }
        
            printf("\nSelecione o contratante para ver missoes associadas: ");
            scanf("%d", &escolha);
            setbuf(stdin, NULL);
        
            if (escolha < 1 || escolha > qtdcontratante) printf("\nOpcao invalida!");
            else {
                char cpf[15];
                strcpy(cpf, contratante[escolha-1].cpf);
                int tem_missoes = 0;
            
                for (int i = 0; i < *qtdmissoes; i++) {
                    if (strcmp(missoes[i].cpf_contratante, cpf) == 0) {
                        tem_missoes = 1;
                        printf("\n\n%d. [ID da missao: %d]", i + 1, missoes[i].id_missao);
                        printf("\nContratante: %s", contratante[escolha-1].nome);
                        
                        for (int j = 0; j < qtdagentes; j++) {
                            if (agentes[j].id == missoes[i].id_agente) {
                                printf("\nAgente: %s", agentes[j].nome);
                                break;
                            }
                        }
                        
                        printf("\nMotivo: %s", missoes[i].motivo);
                        printf("\nTempo de execucao: %.2f", missoes[i].tempo_de_missao);
                        printf("\nResultado da missao: %s", missoes[i].resultado_da_missao);
                        printf("\n\n__");
                    }
                }

                if (!tem_missoes) {
                    printf("\nNenhuma missao encontrada para este contratante!");
                    printf("\nPressione qualquer tecla para voltar...");
                    getchar();
                } else {
                    printf("\nOpcao invalida!");
                }
            }
        }
        return 0;
    }
}

int excluir_contratante(struct contratante contratantes[], int qtdcontratante, int escolha, int num){
    if (num == 1){
        for (int i = (escolha - 1); i < qtdcontratante; i++){
            contratantes[i] = contratantes[i + 1];
        }
        return qtdcontratante - 1;
    }
    if (num == 2){
        printf("Voltando ao menu...");
        return qtdcontratante;
    }
    return 0;
}

int excluir_missoes(struct missoes missoes[], int *qtdmissoes, int escolha, int num){
    if (num == 1)
    {
        for (int i = (escolha - 1); i < *qtdmissoes; i++)
        {
            missoes[i] = missoes[i + 1];
        }
        printf("\nMissao excluida com sucesso!\n");
        return (*qtdmissoes)--;
    }
    else
    {
        printf("Operacao cancelada.\n");
        return *qtdmissoes;
    }
}

void buscar_por_nome(struct agentes agentes[], int qtdagentes, struct contratante contratantes[], int qtdcontratantes, char termo[]) {
    int encontrados = 0;
    printf("\n=== RESULTADOS ENCONTRADOS ===\n");

    for(int i = 0; i < qtdagentes; i++) {
        if(strcmp(agentes[i].nome, termo) == 0) {
            printf("\n[AGENTE] ID: %d | Nome: %s | Especie: %s",
            agentes[i].id, agentes[i].nome, agentes[i].especie);
            encontrados++;
        }
    }

    
    for(int i = 0; i < qtdcontratantes; i++) {
        if(strcmp(contratantes[i].nome, termo) == 0) {
            printf("\n[CONTRATANTE] CPF: %s | Nome: %s | Cargo: %s",
            contratantes[i].cpf, contratantes[i].nome, contratantes[i].cargo);
            encontrados++;
        }
    }

    if(encontrados == 0) {
        printf("\nNenhum registro encontrado com \"%s\"", termo);
    }
}

int contar_missoes_por_id_agente(struct missoes missoes[], int qtdmissoes, int id_alvo) {
    int contador = 0;

    for(int i = 0; i < qtdmissoes; i++) {
        if(missoes[i].id_agente == id_alvo) {
            contador++;
        }
    }

    return contador;
}

int contar_missoes_por_cpf(struct missoes missoes[], int qtdmissoes, char cpf_alvo[]) {
    int contador = 0;

    for(int i = 0; i < qtdmissoes; i++) {
        if(strcmp(missoes[i].cpf_contratante, cpf_alvo) == 0) {
            contador++;
        }
    }

    return contador;
}

void buscar_por_numeros(struct missoes missoes[], int qtdmissoes, int opcao, int id, char cpf[]) {
    if(opcao == 1) {
        int total = contar_missoes_por_id_agente(missoes, qtdmissoes, id);
        printf("\nO agente ID %d participou de %d missoes", id, total);
    }
    else if(opcao == 2){
        int total = contar_missoes_por_cpf(missoes, qtdmissoes, cpf);
        printf("\nO contratante CPF %s requisitou %d missoes", cpf, total);
    }
}

void ordenar_agentes_id_shell(struct agentes agentes[], int qtdagentes) {
    int intervalo, i, j;
    struct agentes temp;
    
    for (intervalo = qtdagentes/2; intervalo > 0; intervalo /= 2) {
        for (i = intervalo; i < qtdagentes; i++) {
            temp = agentes[i];
            for (j = i; j >= intervalo && agentes[j-intervalo].id > temp.id; j -= intervalo) {
                agentes[j] = agentes[j-intervalo];
            }
            agentes[j] = temp;
        }
    }
}

void ordenar_contratantes_cpf_shell(struct contratante contratantes[], int qtdcontratantes) {
    int intervalo, i, j;
    struct contratante temp;
    
    for (intervalo = qtdcontratantes/2; intervalo > 0; intervalo /= 2) {
        for (i = intervalo; i < qtdcontratantes; i++) {
            temp = contratantes[i];
            for (j = i; j >= intervalo && strcmp(contratantes[j-intervalo].cpf, temp.cpf) > 0; j -= intervalo) {
                contratantes[j] = contratantes[j-intervalo];
            }
            contratantes[j] = temp;
        }
    }
}

void merge_agentes(struct agentes agentes[], int esq, int meio, int dir) {
    int i, j, k;
    int n1 = meio - esq + 1;
    int n2 = dir - meio;
  
    struct agentes L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = agentes[esq + i];
    for (j = 0; j < n2; j++)
        R[j] = agentes[meio + 1 + j];

    i = 0; j = 0; k = esq;
    while (i < n1 && j < n2) {
        if (strcmp(L[i].nome, R[j].nome) <= 0) {
            agentes[k] = L[i];
            i++;
        } else {
            agentes[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        agentes[k] = L[i];
        i++; k++;
    }
    while (j < n2) {
        agentes[k] = R[j];
        j++; k++;
    }
}


void ordenar_agentes_nome_merge(struct agentes agentes[], int esq, int dir) {
    if (esq < dir) {
        int meio = esq + (dir - esq) / 2;
        ordenar_agentes_nome_merge(agentes, esq, meio);
        ordenar_agentes_nome_merge(agentes, meio + 1, dir);
        merge_agentes(agentes, esq, meio, dir);
    }
}


void ordenar_agentes_nome(struct agentes agentes[], int qtdagentes) {
    ordenar_agentes_nome_merge(agentes, 0, qtdagentes - 1);
}

int main(){
    int qtdagentes = 0, qtdcontratantes = 0, escolha  = 0, num = 0, cont;
    int qtdmissoes = 0, opcao, opcadastro, MAX = 50, op1 = 0, op = 0;
    char cpf[12], termo[50];
    int capacidade_agentes = 1, capacidade_contratantes = 1, capacidade_missoes = 1;
    
    struct agentes *agentes = malloc(capacidade_agentes * sizeof(struct agentes));
    struct contratante *contratantes = malloc(capacidade_contratantes * sizeof(struct contratante));
    struct missoes *missoes = malloc(capacidade_missoes * sizeof(struct missoes));
    
    carregar_agentes(agentes, &qtdagentes);
    carregar_contratantes(contratantes, &qtdcontratantes);
    carregar_missoes(missoes, &qtdmissoes);
    printf("\nDados carregados com sucesso!");

    if (!agentes || !contratantes || !missoes) {
        printf("Erro na alocação inicial!\n");
        return 1;
    }else{
        printf("\nSucesso na alocacao inicial! :) ");
    }

    do{
        printf("\n\n----------Pequenos Espioes - MENU----------");
        printf("\n1. Cadastrar");
        printf("\n2. Cadastrar a partir de arquivo");
        printf("\n3. Remover");
        printf("\n4. Missoes");
        printf("\n5. Visualizar cadastros");
        printf("\n6. Salvar dados em arquivo");
        printf("\n7. Buscar");
        printf("\n8. Ordenar");
        printf("\n0. Sair");
        printf("\n\nSelecione a opcao desejada: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1: 
                printf("\n__");
                printf("\n\n1. Cadastrar contratante\n");
                printf("2. Cadastrar agente\n");
                printf("3. Cadastrar missao\n");
                printf("4. Voltar\n");
                printf("\nSelecione a opcao desejada: ");
                scanf("%d", &opcadastro);
                printf("\n__");

                if(opcadastro == 1){
                    char cpf[12], nome[50], cargo[30];

                    if (qtdcontratantes >= MAX){
                        printf("Limite de contratantes atingido!\n");
                    }else{
                        getchar();
                        printf("\n\nDigite o CPF do contratante (somente numeros): ");
                        fgets(cpf, 12, stdin);
                        cpf[strcspn(cpf,"\n")] = 0;
                        cont = 0;
                        for (int i = 0; i < qtdcontratantes; i++){
                            if (strcmp(contratantes[i].cpf, cpf) == 0)
                            {
                                printf("\nCPF do contratante ja cadastrado, tente novamente\n");
                                cont = 1;
                            }
                        }
                        if(cont != 1){
                            printf("Digite o nome do contratante: ");
                            fgets(nome, 50, stdin);
                            nome[strcspn(nome,"\n")] = 0;
        
                            
                            printf("Digite o cargo do contratante: ");
                            fgets(cargo, 30, stdin);
        
                            cad_contratante(contratantes, &qtdcontratantes, cpf, nome, cargo, &capacidade_contratantes);
                            printf("\nContratante cadastrado com sucesso!");
                            printf("\n\n__");
                            
                        }                        
                    }
                }
                else if(opcadastro == 2){
                    char nomeagente[50], especie[50], idade[50];
                    int id;

                    if (qtdagentes >= MAX){
                        printf("Limite de agentes atingido!\n");
                    }else{
                        setbuf(stdin, NULL);;
                        printf("\n\nDigite a id do agente: ");
                        scanf("%d", &id);
    
                        cont = 0;
                        for (int i = 0; i < qtdagentes; i++){
                            if (agentes[i].id == id)
                            {
                                printf("Id de agente ja cadastrado, tente novamente");
                                cont = 1;
                            }
                        }
                        if(cont != 1){
                            setbuf(stdin, NULL);;
                            printf("Digite o nome do agente: ");
                            fgets(nomeagente, 50, stdin);
                            nomeagente[strcspn(nomeagente,"\n")] = 0;
    
                            setbuf(stdin, NULL);;
                            printf("Digite a especie do agente: ");
                            fgets(especie, 50, stdin);
                            especie[strcspn(especie,"\n")] = 0;
    
                            setbuf(stdin, NULL);;
                            printf("Digite a idade do agente: ");
                            fgets(idade, 50, stdin);
    
                            cad_agentes(agentes, &qtdagentes, id, nomeagente, especie, idade, &capacidade_agentes);
                            printf("\n\nAgente cadastrado com sucesso!");
                            printf("\n\n__");
                        }
                        cont = 0;
                    }
                }
                else if(opcadastro == 3){
                    int idmissao = 0, escolha_agente, resultado, cont = 0;
                    float tempomissao;
                    char motivo[100], escolha1[12];
                
                    if(qtdagentes == 0 || qtdcontratantes == 0){
                        printf("Nenhum agente ou contratante cadastrado para a missao!\n");
                    } else {
                        printf("\n\nDigite o id da missao: ");
                        scanf("%d", &idmissao);
                
                        for(int i = 0; i < qtdmissoes; i++){
                            if(missoes[i].id_missao == idmissao){
                                printf("Id da missao ja cadastrado, tente novamente.\n");
                                cont = 1;
                                break;
                            }
                        }
                
                        if(cont != 1){
                            printf("\nDigite o id do agente para a missao:\n");
                            for(int i = 0; i < qtdagentes; i++){
                                printf("%d. %s [id: %d]\n", i + 1, agentes[i].nome, agentes[i].id);
                            }
                            scanf("%d", &escolha_agente);
                            int agente_valido = 0;
                            for(int i = 0; i < qtdagentes; i++){
                                if(escolha_agente == agentes[i].id){
                                    agente_valido = 1;
                                    break;
                                }
                            }
                            if(!agente_valido){
                                printf("Escolha invalida de agente!\n");
                                return;
                            }
                
                            
                            printf("\nDigite o CPF do contratante para a missao:\n");
                            for(int i = 0; i < qtdcontratantes; i++){
                                printf("%d. %s [CPF: %s]\n", i + 1, contratantes[i].nome, contratantes[i].cpf);
                            }
                
                            getchar();
                            fgets(escolha1, 12, stdin);
                            escolha1[strcspn(escolha1, "\n")] = 0;
                
                            int cpf_valido = 0;
                            for (int i = 0; i < qtdcontratantes; i++){
                                if (strcmp(escolha1, contratantes[i].cpf) == 0){
                                    cpf_valido = 1;
                                    break;
                                }
                            }
                
                            if (!cpf_valido){
                                printf("CPF invalido!\n");
                                return;
                            }
                
                            
                            printf("\nDescreva o motivo do contrato: ");
                            getchar(); 
                            fgets(motivo, 100, stdin);
                            motivo[strcspn(motivo, "\n")] = 0;
                
                            
                            printf("Digite em quantos dias a missao foi executada: ");
                            scanf("%f", &tempomissao);
                
                            
                            printf("\n1. Missao bem sucedida\n");
                            printf("2. Missao mal sucedida\n");
                            printf("Digite o resultado da missao: ");
                            scanf("%d", &resultado);
                
                            while(resultado != 1 && resultado != 2){
                                printf("Resultado invalido! Tente novamente: ");
                                scanf("%d", &resultado);
                            }
                
                            
                            cad_missoes(missoes, &qtdmissoes, idmissao, escolha_agente, escolha1, resultado, tempomissao, motivo, &capacidade_missoes);
                            printf("Missao cadastrada com sucesso!\n");
                        }
                    }
                }else if(opcadastro == 4){
                    printf("Voltando....\n");
                }
                break;
            case 2:
                lerArquivosAgente(&agentes, &qtdagentes);
                LerArquivosCont(&contratantes, &qtdcontratantes);
                LerArquivosMissoes(&missoes, &qtdmissoes);
                break;
            case 3:
                printf("\n1. Excluir agente");
                printf("\n2. Excluir contratante");
                printf("\n3. Excluir missao");
                printf("\n4. Voltar");
                printf("\n\nSelecione a opcao desejada: ");
                scanf("%d", &op1);

                if (op1 == 1){
                    if (qtdagentes == 0){
                        printf("Nenhum agente cadastrado!");
                    }else{
                        for (int i = 0; i < qtdagentes; i++){
                            printf("\n%d. %s [id: %d]", i + 1, agentes[i].nome, agentes[i].id);
                        }
                        printf("\nSelecione o agente a ser excluido:");
                        scanf("%d", &escolha);
    
                        if (escolha < 1 || escolha > qtdagentes){
                            printf("Escolha invalida.\n");
                        }else{
                            for(int i=0; i<qtdmissoes; i++){
                                if(agentes[(escolha - 1)].id == missoes[i].id_agente){
                                    printf("\nEsse agente esta cadastrado em uma missao, nao pode ser excluido.");
                                    cont = 1;
                                }
                            }
    
                            if(cont != 1){
                                printf("Confirmar exclusao do agente [%s], ID: [%d] , idade [%s] e especie [%s]?" , agentes[escolha - 1].nome, agentes[escolha - 1].id, agentes[escolha - 1].idade, agentes[escolha - 1].especie);
                                printf("\n1.Sim");
                                printf("\n2.Nao");
                                scanf("\n%d", &num);
        
                                if (num != 1 && num != 2){
                                    printf("Opcao invalida, digite novamente!");
                                    scanf("%d", &num);
                                }
                                qtdagentes = excluir_agente(agentes, qtdagentes, op, num);
                                salvar_agentes(agentes, qtdagentes);
                                printf("\n\nAgente excluido com sucesso!");    
                            }
                            cont = 0;
                        }
                    }
                }
                else if (op1 == 2){
                    if (qtdcontratantes == 0){
                        printf("\nNenhum contratante cadastrado!");
                    }else{
                        for (int i = 0; i < qtdcontratantes; i++){
                            printf("\n%d. %s [cpf: %s]", i + 1, contratantes[i].nome, contratantes[i].cpf);
                        }
    
                        printf("\nSelecione o contratante a ser excluido: ");
                        scanf("%d", &escolha);
                    
                        if (escolha < 1 || escolha > qtdcontratantes){
                            printf("Escolha invalida!\n");
                        }else{
                            for(int i=0; i<qtdmissoes; i++){
                                if(strcmp(contratantes[(escolha - 1)].cpf, missoes[i].cpf_contratante)==0){
                                    printf("\nEsse contratante esta cadastrado em uma missao, nao pode ser excluido.");
                                    cont = 1;
                                }
                            }
        
                            if(cont != 1){
                                printf("Confirmar exclusao do contratante [%s], CPF: [%s] e cargo [%s]?", contratantes[escolha - 1].nome, contratantes[escolha - 1].cpf, contratantes[escolha - 1].cargo);
                                printf("\n1.Sim");
                                printf("\n2.Nao");
                                scanf("\n%d", &num);
            
                                if (num != 1 && num != 2){
                                    printf("Opcao invalida, digite novamente!");
                                    scanf("%d", &num);
                                }
            
                                qtdcontratantes = excluir_contratante(contratantes, qtdcontratantes, escolha, num);
                                salvar_contratantes(contratantes, qtdcontratantes);
                                printf("\n\nContratante excluido com sucesso!");
                            }
                        }

                    }
                }
                else if (op1 == 3){
                    if (qtdmissoes == 0){
                        printf("Nenhuma missao cadastrada!\n");
                    }else{
                        for (int i = 0; i < qtdmissoes; i++){
                            printf("\n\n%d. [ID da missao: %d]", i + 1, missoes[i].id_missao);
                            printf("\nContratante: %s", contratantes[i].nome);
                            printf("\nAgente: %s", agentes[i].nome);
                            printf("\nMotivo: %s", missoes[i].motivo);
                        }
                    
                        printf("\n\nSelecione a missao a ser excluida: ");
                        scanf("%d", &escolha);
                    
                        if (escolha < 1 || escolha > qtdmissoes){
                            printf("Escolha invalida!\n");
                        }else{
                            printf("Confirmar exclusao da missao de ID: [%d] motivo [%s]?\n" , missoes[escolha - 1].id_missao, missoes[escolha - 1].motivo);
                            printf("1. Sim\n");
                            printf("2. Nao\n");
                            scanf("%d", &num);
    
                            excluir_missoes(missoes, &qtdmissoes, escolha, num);
                            salvar_missoes(missoes, qtdmissoes);
                        }
                    }
                }
                break;
            case 4: 
                if(qtdmissoes == 0){
                    printf("Nenhuma missao cadastrada!\n");
                }else{
                    printf("\n1. Ver missoes bem sucedidas.");
                    printf("\n2. Ver missoes mal sucedidas.");
                    printf("\n3. Ver missoes terminadas em 1 dia.");
                    printf("\n4. Ver todas as missoes cadastradas.");
                    printf("\n5. Ver missoes executadas por um agente especifico.");
                    printf("\n6. Ver missoes executadas por um contratante especifico.");
                    printf("\nEscolha uma opcao: ");
                    scanf("%d", &op1);

                    if (qtdmissoes == 0) {
                        printf("\nNenhuma missao cadastrada ainda!\n");
                    }else{
                        printf("\nMissoes bem sucedidas: ");
                        op = ver_missoes(missoes, contratantes, agentes, &qtdmissoes, qtdagentes, qtdcontratantes, escolha, op1);
                    }
                }
                break;
            case 5:       
                printf("\n1. Visualizar agentes cadastrados.");
                printf("\n2. Visualizar contratantes cadastrados.");
                printf("\n3. Voltar\n");
                scanf("%d", &op1);

                if(op1 == 1){
                    if(qtdagentes == 0){
                        printf("\nNenhum agente cadastrado!");
                    }
                }
                else if(op1 == 2){
                    if (qtdcontratantes == 0){
                        printf("\nNenhum contratante cadastrado!");
                    }
                }

                else if (op1 == 3){
                    printf("\nVoltando...");
                }
                else if (op1 != 1 && op1 != 2 && op1 != 3){
                    printf("Opcao invalida! Voltando ao menu...");
                }

                ver_cadastros(agentes, contratantes, qtdagentes, qtdcontratantes, op1);
                break;
            case 6:
                if(qtdmissoes > 0){
                    salvar_missoes(missoes, qtdmissoes);
                }
                if(qtdagentes > 0){
                    salvar_agentes(agentes, qtdagentes);
                }
                if(qtdcontratantes > 0){
                    salvar_contratantes(contratantes, qtdcontratantes);
                }
                break;
            case 7:
                printf("\n1. Busca por nome (sequencial)");
                printf("\n2. Busca por ID/CPF (binaria)");
                printf("\nEscolha: ");
                scanf("%d", &op);
                
                if(op == 1){
                    getchar();
                    printf("\nDigite o nome a buscar: ");
                    fgets(termo , 50, stdin);
                    termo[strcspn(termo , "\n")] = 0;
                    
                    buscar_por_nome(agentes, qtdagentes, contratantes, qtdcontratantes, termo);
                }
                else if(op == 2) {
                    printf("\n1. Buscar por ID de agente");
                    printf("\n2. Buscar por CPF de contratante");
                    printf("\nEscolha: ");
                    scanf("%d", &opcao);

                    if(opcao == 1){
                        printf("Digite o ID do agente: ");
                        scanf("%d", &op);
                    }else if(opcao == 2){
                        printf("Digite o CPF do contratante (apenas numeros): ");
                        scanf("%s", cpf);
                    }

                    buscar_por_numeros(missoes, qtdmissoes, opcao, op, cpf);
                }
                else if(op != 1 && op != 2){
                    printf("Opcao invalida!");
                }
                break;
            case 8:
                printf("\n1. Ordenar agentes por ID (Shell Sort)");
                printf("\n2. Ordenar contratantes por CPF (Shell Sort)");
                printf("\n3. Ordenar agentes por nome (Merge Sort)");
                printf("\nEscolha: ");
                scanf("%d", &opcao);

                if(opcao == 1) {
                    ordenar_agentes_id_shell(agentes, qtdagentes);
                    printf("\nAgentes ordenados por ID!");
                }
                else if(opcao == 2) {
                    ordenar_contratantes_cpf_shell(contratantes, qtdcontratantes);
                    printf("\nContratantes ordenados por CPF!");
                }
                else if(opcao == 3) {
                    ordenar_agentes_nome(agentes, qtdagentes);
                    printf("\nAgentes ordenados por nome!");
                }

                break;
            case 0:
                printf("Deseja salvar os dados em arquivo antes de sair?");
                printf("\n1. Sim, salvar.\n2. Nao, sair sem salvar.\n\n");
                scanf("%d" , &op);

                if(op == 1){
                    printf("Salvando e saindo...");
                    salvar_missoes(missoes, qtdmissoes);
                    salvar_contratantes(contratantes, qtdcontratantes);
                    salvar_agentes(agentes, qtdagentes);
                    
                }else if(op == 2){
                    printf("Saindo sem salvar...");
                }
                if(op != 1 && op != 2){
                    printf("Opcao invalida!");
                }
                break;
            default:
                printf("Opcao invalida!");
                break;
        }
    }while(opcao!=0);

    free(agentes);
    free(contratantes);
    free(missoes);
    return 0;
}
