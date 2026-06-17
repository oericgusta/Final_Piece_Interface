#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "album.h"
#include "raylib.h"
#include <ctype.h>

// Inicializa o álbum com uma capacidade inicial na memória RAM
void inicializar_album(Album *al) {
    al->total = 0;
    al->capacidade = 50; // Começa alocando espaço para 50, vai aumentar se precisar
    al->figurinhas = (Figurinha *) malloc(al->capacidade * sizeof(Figurinha));
    if (al->figurinhas == NULL) {
        printf("[!] Erro crítico de memória!\n");
        exit(1);
    }
}

// Função auxiliar para remover espaços em branco das pontas das strings
void trim_spaces(char *str) {
    if (str == NULL) return;

    // Remove espaços do final
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        len--;
    }

    // Remove espaços do início
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t') {
        start++;
    }
    if (start > 0) {
        memmove(str, str + start, len - start + 1);
    }
}

// Lê as figurinhas do arquivo e joga na memória RAM
void carregar_csv(Album *al, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL) {
        printf("[!] Erro: Arquivo %s nao encontrado.\n", nome_arquivo);
        return;
    }

    char linha[1024];

    if (fgets(linha, sizeof(linha), arq) == NULL) {
        fclose(arq);
        return;
    }

    al->total = 0;

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        
        if (al->total >= al->capacidade) {
            al->capacidade += 50;
            al->figurinhas = (Figurinha *) realloc(al->figurinhas, al->capacidade * sizeof(Figurinha));
        }

        linha[strcspn(linha, "\r\n")] = '\0';

        if (strlen(linha) == 0) continue;

        Figurinha f;
        char *token;

        // Código
        token = strtok(linha, ",");
        if (!token) continue;
        trim_spaces(token); // <--- Limpa espaços extras
        snprintf(f.codigo, sizeof(f.codigo), "%s", token);

        // Título
        token = strtok(NULL, ",");
        if (!token) continue;
        trim_spaces(token); // <--- Limpa espaços extras
        snprintf(f.titulo, sizeof(f.titulo), "%s", token);

        // Seção (Ex: Mexico)
        token = strtok(NULL, ",");
        if (!token) continue;
        trim_spaces(token); // <--- Remove os espaços que estavam quebrando o filtro!
        snprintf(f.secao, sizeof(f.secao), "%s", token);

        // Grupo
        token = strtok(NULL, ",");
        if (!token) continue;
        trim_spaces(token); // <--- Limpa espaços extras
        snprintf(f.grupo, sizeof(f.grupo), "%s", token);

        // Tipo
        token = strtok(NULL, ",");
        if (!token) continue;
        trim_spaces(token); // <--- Limpa espaços extras
        snprintf(f.tipo, sizeof(f.tipo), "%s", token);

        // Colada
        token = strtok(NULL, ",");
        f.colada = token ? atoi(token) : 0;

        // Repetidas
        token = strtok(NULL, ",");
        f.repetidas = token ? atoi(token) : 0;

        // --- TRAPAÇA TEMPORÁRIA PARA TESTE ---
        f.colada = 0;    
        f.repetidas = 0;
        // -------------------------------------

        al->figurinhas[al->total] = f;
        al->total++;
    }
    fclose(arq);
    printf("Sucesso: %d figurinhas carregadas na memoria!\n", al->total);
}

// Lista todas as figurinhas salvas na RAM
void listar_figurinhas(Album *al) {
    if (al->total == 0) {
        printf("Nenhuma figurinha no album.\n");
        return;
    }
    printf("\n=== LISTA DE FIGURINHAS ===\n");
    for (int i = 0; i < al->total; i++) {
        printf("[%s] %-25s | Status: %s | Repetidas: %d\n", 
               al->figurinhas[i].codigo, 
               al->figurinhas[i].titulo,
               al->figurinhas[i].colada ? "Colada " : "Faltando", 
               al->figurinhas[i].repetidas);
    }
}

// Pesquisa uma figurinha pelo código ex: FWC00
void pesquisar_figurinha(Album *al) {
    char busca[15];
    printf("Digite o codigo da figurinha: ");
    scanf(" %s", busca);

    for (int i = 0; i < al->total; i++) {
        if (strcmp(al->figurinhas[i].codigo, busca) == 0) {
            printf("\nEncontrada:\nNome: %s\nSecao: %s\nTipo: %s\nStatus: %s\n",
                   al->figurinhas[i].titulo, al->figurinhas[i].secao, al->figurinhas[i].tipo,
                   al->figurinhas[i].colada ? "Colada" : "Nao colada");
            return;
        }
    }
    printf("[!] Figurinha com codigo %s nao encontrada.\n", busca);
}

// Sistema de abrir pacote com 7 figurinhas aleatórias
void abrir_pacotinho(Album *al) {
    if (al->total == 0) {
        printf("Erro: Carregue o arquivo de figurinhas antes.\n");
        return;
    }

    printf("\n--- ABRINDO PACOTINHO (7 Figurinhas) ---\n");
    for (int i = 0; i < 7; i++) {
        int indice_aleatorio = rand() % al->total;
        Figurinha *f = &al->figurinhas[indice_aleatorio];

        printf("\n[%d/7] Voce tirou: %s (%s)\n", i+1, f->titulo, f->codigo);
        
        char opcao;
        printf("Deseja colar no album? (s/n): ");
        scanf(" %c", &opcao);

        if (opcao == 's' || opcao == 'S') {
            if (f->colada == 0) {
                f->colada = 1;
                printf("Boa! Nova figurinha colada!\n");
            } else {
                f->repetidas++;
                printf("Voce ja tinha essa. Adicionada as repetidas para troca!\n");
            }
        } else {
            printf("Figurinha guardada na gaveta.\n");
        }
    }
}

// Libera a memória alocada antes de fechar o programa
void liberar_album(Album *al) {
    free(al->figurinhas);
}

void carregar_texturas_album(Album *al) {
    char caminho[150];
    char codigoLimpo[15];

    // 1. Carrega a imagem do Verso Padrão uma única vez
    const char *caminhoVerso = "assets/icons/Verso_Fig.png";
    if (FileExists(caminhoVerso)) {
        al->texturaVerso = LoadTexture(caminhoVerso);
    } else {
        printf("[!] Aviso: Imagem do verso nao encontrada em %s\n", caminhoVerso);
    }

    // 2. Carrega a foto de cada jogador do álbum
    for (int i = 0; i < al->total; i++) {
        sscanf(al->figurinhas[i].codigo, "%s", codigoLimpo);
        sprintf(caminho, "assets/figurinhas/%s.png", codigoLimpo);

        if (FileExists(caminho)) {
            al->figurinhas[i].textura = LoadTexture(caminho);
        } else {
            // Se não achar a foto do jogador, deixa o ID como 0 para segurança
            al->figurinhas[i].textura.id = 0;

            printf("[!] Erro VRAM: Nao achei a foto do codigo %s em: %s\n", codigoLimpo, caminho);
        }
    }
    printf("[+] Total de %d texturas processadas.\n", al->total);
}
