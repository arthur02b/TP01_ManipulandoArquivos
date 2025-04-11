#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "processo.h"

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    VetorProcessos* vetor = criar_vetor_processos(1000);
    
    lerProcessos(vetor, "processo_043_202409032338 (1).csv");
    printf("Total de processos lidos: %d\n", vetor->tamanho);
    
    ordenarPorId(vetor);
    salvarProcessosCSV("ordenado_por_id.csv", vetor);
    printf("Processos classificados por ID (crescente) salvos em 'ordenado_por_id.csv'\n");
    
    ordenarPorData(vetor);
    salvarProcessosCSV("ordenado_por_data.csv", vetor);
    printf("Processos classificados por data (decrescente) salvos em 'ordenado_por_data.csv'\n");
    
    int id_classe = 12553;
    int count = contarPorClasse(vetor, id_classe);
    printf("Processos associados à classe %d: %d\n", id_classe, count);
    
    int assuntos_distintos = contarAssuntosUnicos(vetor);
    printf("Total de assuntos distintos: %d\n", assuntos_distintos);
    
    listarProcessosMultiplosAssuntos(vetor, 0); // 0 shows only essential info
    
    if (vetor->tamanho > 0) {
        int dias = calcularDiasTramitacao(vetor->processos[0].data_ajuizamento);
    printf("Processo %d está tramitando há %d dias\n", vetor->processos[0].id, dias);
    }
    
    liberar_vetor_processos(vetor);
    return 0;
}
