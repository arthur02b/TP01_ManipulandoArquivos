#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_ASSUNTOS 10
#define MAX_CLASSES 10
#define MAX_LINHA 500
#define DATA_LEN 20

typedef struct {
    int id;
    char numero[50];
    char data_ajuizamento[DATA_LEN];
    int classes[MAX_CLASSES];
    int num_classes;
    int assuntos[MAX_ASSUNTOS];
    int num_assuntos;
    int ano_eleicao;
} Processo;

typedef struct {
    Processo* processos;
    int tamanho;
    int capacidade;
} VetorProcessos;

VetorProcessos* criar_vetor_processos(int capacidade_inicial);
void adicionar_processo(VetorProcessos* vetor, Processo processo);
void liberar_vetor_processos(VetorProcessos* vetor);
void lerProcessos(VetorProcessos* vetor, const char* filename);
void ordenarPorId(VetorProcessos* vetor);
void ordenarPorData(VetorProcessos* vetor);
int contarPorClasse(VetorProcessos* vetor, int id_classe);
int contarAssuntosUnicos(VetorProcessos* vetor);
void listarProcessosMultiplosAssuntos(VetorProcessos* vetor, int verbose);
int calcularDiasTramitacao(const char* data_ajuizamento);
void salvarProcessosCSV(const char* filename, VetorProcessos* vetor);

#endif
