#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "album.h"

    // ============================================================
    //  ARQUIVO CSV  –  leitura inicial e exportação
    // ============================================================

    /**
     * @brief Lê as figurinhas de um arquivo CSV e carrega na memória RAM.
     *
     * Formato esperado: codigo,titulo,secao,grupo,tipo,colada,repetidas
     * A primeira linha (cabeçalho) é ignorada automaticamente.
     * Na primeira execução o progresso vem zerado; nas demais usa o binário.
     *
     * @param al         Ponteiro para o álbum.
     * @param nome_arquivo Caminho do arquivo CSV.
     */
void carregar_csv(Album *al, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL) {
        printf("[!] Erro: Arquivo %s nao encontrado.\n", nome_arquivo);
        return;
    }

    char linha[1024];

    // Pula o cabeçalho
    if (fgets(linha, sizeof(linha), arq) == NULL) {
        fclose(arq);
        return;
    }

    al->total = 0;

    while (fgets(linha, sizeof(linha), arq) != NULL) {

        // Expande o vetor se necessário
        if (al->total >= al->capacidade) {
            al->capacidade += 50;
            Figurinha *novo = (Figurinha *) realloc(al->figurinhas, al->capacidade * sizeof(Figurinha));
            if (novo == NULL) {
                printf("[!] Erro critico de memoria durante leitura do CSV!\n");
                fclose(arq);
                return;
            }
            al->figurinhas = novo;
        }

        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) == 0) continue;

        Figurinha f;
        memset(&f, 0, sizeof(Figurinha));
        char *token;

        token = strtok(linha, ","); if (!token) continue;
        trim_spaces(token);
        snprintf(f.codigo, sizeof(f.codigo), "%s", token);

        token = strtok(NULL, ","); if (!token) continue;
        trim_spaces(token);
        snprintf(f.titulo, sizeof(f.titulo), "%s", token);

        token = strtok(NULL, ","); if (!token) continue;
        trim_spaces(token);
        snprintf(f.secao, sizeof(f.secao), "%s", token);

        token = strtok(NULL, ","); if (!token) continue;
        trim_spaces(token);
        snprintf(f.grupo, sizeof(f.grupo), "%s", token);

        token = strtok(NULL, ","); if (!token) continue;
        trim_spaces(token);
        snprintf(f.tipo, sizeof(f.tipo), "%s", token);

        // Campos de progresso: lê do CSV mas podem ser sobrescritos pelo binário
        token = strtok(NULL, ",");
        f.colada = token ? atoi(token) : 0;

        token = strtok(NULL, ",");
        f.repetidas = token ? atoi(token) : 0;

        f.textura.id = 0; // textura é carregada à parte

        al->figurinhas[al->total] = f;
        al->total++;
    }
    fclose(arq);
    printf("[+] CSV: %d figurinhas carregadas na memoria.\n", al->total);
}

    /**
     * @brief Exporta os dados atuais do álbum em formato CSV.
     *
     * Sobrescreve o arquivo destino com o estado atual de colada/repetidas.
     * Útil como backup e para atender ao requisito de exportação do projeto.
     *
     * @param al         Ponteiro para o álbum.
     * @param nome_arquivo Caminho do arquivo de saída.
     */
void salvar_csv(Album *al, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "w");
    if (arq == NULL) {
        printf("[!] Erro: Nao foi possivel abrir %s para exportar.\n", nome_arquivo);
        return;
    }

    fprintf(arq, "codigo,titulo,secao,grupo,tipo,colada,repetidas\n");

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

    fclose(arq);
    printf("[+] Exportacao CSV concluida em: %s\n", nome_arquivo);
}

    // ============================================================
    //   ARQUIVO BINÁRIO  –  persistência principal entre sessões
    // ============================================================

    /**
     * @brief Salva o progresso do álbum em arquivo binário (persistência principal).
     *
     * A estrutura Texture2D (ponteiros de VRAM) é ignorada intencionalmente,
     * salvando apenas os dados de texto e os campos colada/repetidas.
     *
     * @param al         Ponteiro para o álbum.
     * @param nome_arquivo Caminho do arquivo binário (.bin).
     */
void salvar_binario(Album *al, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "wb");
    if (arq == NULL) {
        printf("[!] Erro critico: Nao foi possivel salvar em %s\n", nome_arquivo);
        return;
    }

    // Salva a quantidade total de figurinhas
    fwrite(&al->total, sizeof(int), 1, arq);

    // Salva cada figurinha campo a campo (sem textura, que é volátil em VRAM)
    for (int i = 0; i < al->total; i++) {
        fwrite(al->figurinhas[i].codigo,     sizeof(al->figurinhas[i].codigo),   1, arq);
        fwrite(al->figurinhas[i].titulo,     sizeof(al->figurinhas[i].titulo),   1, arq);
        fwrite(al->figurinhas[i].secao,      sizeof(al->figurinhas[i].secao),    1, arq);
        fwrite(al->figurinhas[i].grupo,      sizeof(al->figurinhas[i].grupo),    1, arq);
        fwrite(al->figurinhas[i].tipo,       sizeof(al->figurinhas[i].tipo),     1, arq);
        fwrite(&al->figurinhas[i].colada,    sizeof(int), 1, arq);
        fwrite(&al->figurinhas[i].repetidas, sizeof(int), 1, arq);
    }

    fclose(arq);
    printf("[+] Progresso salvo no arquivo binario: %s\n", nome_arquivo);
}

    /**
     * @brief Tenta carregar o progresso do álbum a partir de um arquivo binário.
     *
     * Retorna 0 se o arquivo não existir (primeira execução), 1 se for lido
     * com sucesso. Neste caso o chamador NÃO precisa chamar carregar_csv.
     *
     * @param al         Ponteiro para o álbum (já inicializado).
     * @param nome_arquivo Caminho do arquivo binário (.bin).
     * @return           1 em caso de sucesso; 0 se o arquivo não existir ou der erro.
     */
int carregar_binario(Album *al, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "rb");
    if (arq == NULL) {
        return 0; // Primeira execução — arquivo ainda não existe
    }

    int total = 0;
    if (fread(&total, sizeof(int), 1, arq) != 1 || total < 0) {
        fclose(arq);
        return 0;
    }

    // Garante capacidade suficiente
    if (total > al->capacidade) {
        Figurinha *novo = (Figurinha *) realloc(al->figurinhas, total * sizeof(Figurinha));
        if (novo == NULL) {
            printf("[!] Erro critico de memoria ao carregar binario!\n");
            fclose(arq);
            return 0;
        }
        al->figurinhas = novo;
        al->capacidade = total;
    }

    // Lê cada figurinha campo a campo
    for (int i = 0; i < total; i++) {
        fread(al->figurinhas[i].codigo,     sizeof(al->figurinhas[i].codigo),   1, arq);
        fread(al->figurinhas[i].titulo,     sizeof(al->figurinhas[i].titulo),   1, arq);
        fread(al->figurinhas[i].secao,      sizeof(al->figurinhas[i].secao),    1, arq);
        fread(al->figurinhas[i].grupo,      sizeof(al->figurinhas[i].grupo),    1, arq);
        fread(al->figurinhas[i].tipo,       sizeof(al->figurinhas[i].tipo),     1, arq);
        fread(&al->figurinhas[i].colada,    sizeof(int), 1, arq);
        fread(&al->figurinhas[i].repetidas, sizeof(int), 1, arq);
        al->figurinhas[i].textura.id = 0; // textura será carregada separadamente
    }

    al->total = total;
    fclose(arq);
    printf("[+] Progresso carregado do binario: %d figurinhas.\n", al->total);
    return 1;
}