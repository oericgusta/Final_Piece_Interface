#include <stdio.h>
#include <stdlib.h>
#include "album.h"

// Função dedicada exclusivamente a salvar as figurinhas no arquivo CSV
void salvar_csv(Album *al, const char *nome_arquivo) {
    
    // 1. Abre o arquivo em modo de escrita ("w" de write). 
    // Isso vai sobrescrever o arquivo antigo com os dados atualizados.
    FILE *arq = fopen(nome_arquivo, "w");
    
    if (arq == NULL) {
        printf("[!] Erro critico: Nao foi possivel abrir o arquivo %s para salvar.\n", nome_arquivo);
        return;
    }

    // 2. Escreve o cabeçalho do CSV na primeira linha
    fprintf(arq, "codigo,titulo,secao,grupo,tipo,colada,repetidas\n");

    // 3. Percorre o vetor na memória RAM e escreve linha por linha no arquivo
    for (int i = 0; i < al->total; i++) {
        fprintf(arq, "%s,%s,%s,%s,%s,%d,%d\n",
                al->figurinhas[i].codigo,
                al->figurinhas[i].titulo,
                al->figurinhas[i].secao,
                al->figurinhas[i].grupo,
                al->figurinhas[i].tipo,
                al->figurinhas[i].colada,
                al->figurinhas[i].repetidas);
    }

    // 4. Fecha o arquivo de forma segura
    fclose(arq);
    printf("[+] Guardiao dos Dados: Progresso salvo com sucesso em %s!\n", nome_arquivo);
}