#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>
#include "processo.h"

VetorProcessos* criar_vetor_processos(int capacidade_inicial) {
    VetorProcessos* vetor = malloc(sizeof(VetorProcessos));
    vetor->processos = malloc(capacidade_inicial * sizeof(Processo));
    vetor->tamanho = 0;
    vetor->capacidade = capacidade_inicial;
    return vetor;
}

void adicionar_processo(VetorProcessos* vetor, Processo processo) {
    if (vetor->tamanho >= vetor->capacidade) {
        vetor->capacidade *= 2;
        vetor->processos = realloc(vetor->processos, vetor->capacidade * sizeof(Processo));
    }
    vetor->processos[vetor->tamanho++] = processo;
}

void liberar_vetor_processos(VetorProcessos* vetor) {
    free(vetor->processos);
    free(vetor);
}

void lerProcessos(VetorProcessos* vetor, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return;

    char linha[MAX_LINHA];
    fgets(linha, sizeof(linha), file); // Skip header

    while (fgets(linha, sizeof(linha), file)) {
        Processo p = {0};
        char *token = strtok(linha, ",");
        
        // Parse basic fields
        p.id = atoi(token);
        token = strtok(NULL, ",");
        strncpy(p.numero, token, sizeof(p.numero)-1);
        token = strtok(NULL, ",");
        strncpy(p.data_ajuizamento, token, sizeof(p.data_ajuizamento)-1);
        
        // Parse classes array (format: {class1;class2})
        token = strtok(NULL, ",");
        if (token && token[0] == '{') {
            char *class_token = strtok(token+1, ";}");
            while (class_token && p.num_classes < MAX_CLASSES) {
                p.classes[p.num_classes++] = atoi(class_token);
                class_token = strtok(NULL, ";}");
            }
        }
        
        // Parse assuntos array (format: {assunto1;assunto2})
        token = strtok(NULL, ",");
        if (token && token[0] == '{') {
            char *assunto_token = strtok(token+1, ";}");
            while (assunto_token && p.num_assuntos < MAX_ASSUNTOS) {
                p.assuntos[p.num_assuntos++] = atoi(assunto_token);
                assunto_token = strtok(NULL, ";}");
            }
        }
        
        // Parse ano_eleicao
        token = strtok(NULL, "\n");
        p.ano_eleicao = token ? atoi(token) : 0;
        
        adicionar_processo(vetor, p);
    }
    fclose(file);
}

void ordenarPorId(VetorProcessos* vetor) {
    // Bubble sort implementation
    for (int i = 0; i < vetor->tamanho-1; i++) {
        for (int j = 0; j < vetor->tamanho-i-1; j++) {
            if (vetor->processos[j].id > vetor->processos[j+1].id) {
                // Swap
                Processo temp = vetor->processos[j];
                vetor->processos[j] = vetor->processos[j+1];
                vetor->processos[j+1] = temp;
            }
        }
    }
}

void ordenarPorData(VetorProcessos* vetor) {
    // Bubble sort implementation for dates (descending order)
    for (int i = 0; i < vetor->tamanho-1; i++) {
        for (int j = 0; j < vetor->tamanho-i-1; j++) {
            if (strcmp(vetor->processos[j].data_ajuizamento, vetor->processos[j+1].data_ajuizamento) < 0) {
                // Swap
                Processo temp = vetor->processos[j];
                vetor->processos[j] = vetor->processos[j+1];
                vetor->processos[j+1] = temp;
            }
        }
    }
}

int contarPorClasse(VetorProcessos* vetor, int id_classe) {
    int count = 0;
    for (int i = 0; i < vetor->tamanho; i++) {
        for (int j = 0; j < vetor->processos[i].num_classes; j++) {
            if (vetor->processos[i].classes[j] == id_classe) {
                count++;
                break;
            }
        }
    }
    return count;
}

int contarAssuntosUnicos(VetorProcessos* vetor) {
    int max_assunto = 0;
    
    // First find maximum assunto value
    for (int i = 0; i < vetor->tamanho; i++) {
        for (int j = 0; j < vetor->processos[i].num_assuntos; j++) {
            if (vetor->processos[i].assuntos[j] > max_assunto) {
                max_assunto = vetor->processos[i].assuntos[j];
            }
        }
    }
    
    // Allocate and initialize counting array
    int* assuntos = calloc(max_assunto + 1, sizeof(int));
    if (!assuntos) {
        return 0;
    }
    
    // Count unique assuntos
    int count = 0;
    for (int i = 0; i < vetor->tamanho; i++) {
        for (int j = 0; j < vetor->processos[i].num_assuntos; j++) {
            int assunto = vetor->processos[i].assuntos[j];
            if (assunto > 0 && assuntos[assunto] == 0) {
                assuntos[assunto] = 1;
                count++;
            }
        }
    }
    
    free(assuntos);
    return count;
}

void listarProcessosMultiplosAssuntos(VetorProcessos* vetor, int verbose) {
    printf("\n=== Processos com múltiplos assuntos ===\n");
    printf("Quantidade encontrada: ");
    
    // First count how many we have
    int count = 0;
    for (int i = 0; i < vetor->tamanho; i++) {
        if (vetor->processos[i].num_assuntos > 1) {
            count++;
        }
    }
    printf("%d\n\n", count);

    // Now print details
    for (int i = 0; i < vetor->tamanho; i++) {
        if (vetor->processos[i].num_assuntos > 1) {
            printf("Processo ID: %d\n", vetor->processos[i].id);
            printf("Numero: %s\n", vetor->processos[i].numero);
            printf("Data ajuizamento: %s\n", vetor->processos[i].data_ajuizamento);
            printf("Assuntos (%d): ", vetor->processos[i].num_assuntos);
            
            for (int j = 0; j < vetor->processos[i].num_assuntos; j++) {
                printf("%d", vetor->processos[i].assuntos[j]);
                if (j < vetor->processos[i].num_assuntos - 1) printf(", ");
            }
            printf("\n");
            
            if (verbose) {
                printf("Classes (%d): ", vetor->processos[i].num_classes);
                for (int j = 0; j < vetor->processos[i].num_classes; j++) {
                    printf("%d", vetor->processos[i].classes[j]);
                    if (j < vetor->processos[i].num_classes - 1) printf(", ");
                }
                printf("\n");
                printf("Ano eleicao: %d\n", vetor->processos[i].ano_eleicao);
            }
            printf("--------------------------------\n");
        }
    }
}

int calcularDiasTramitacao(const char* data_ajuizamento) {
    struct tm tm = {0};
    time_t now;
    time(&now);
    
    // Parse date in format "YYYY-MM-DD"
    if (sscanf(data_ajuizamento, "%d-%d-%d", 
              &tm.tm_year, &tm.tm_mon, &tm.tm_mday) != 3) {
        return 0;
    }
    
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_hour = 12;
    
    time_t ajuizamento = mktime(&tm);
    if (ajuizamento == -1) return 0;
    
    double diff = difftime(now, ajuizamento);
    return (int)(diff / (60 * 60 * 24));
}

void salvarProcessosCSV(const char* filename, VetorProcessos* vetor) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    fprintf(file, "id,numero,data_ajuizamento,classes,assuntos,ano_eleicao\n");
    for (int i = 0; i < vetor->tamanho; i++) {
        Processo p = vetor->processos[i];
        
        // Format classes as comma-separated list
        fprintf(file, "%d,%s,%s,", p.id, p.numero, p.data_ajuizamento);
        for (int j = 0; j < p.num_classes; j++) {
            fprintf(file, "%d", p.classes[j]);
            if (j < p.num_classes-1) fprintf(file, ",");
        }
        
        fprintf(file, ",");
        
        // Format assuntos as comma-separated list
        for (int j = 0; j < p.num_assuntos; j++) {
            fprintf(file, "%d", p.assuntos[j]);
            if (j < p.num_assuntos-1) fprintf(file, ",");
        }
        
        fprintf(file, ",%d\n", p.ano_eleicao);
    }
    fclose(file);
}
