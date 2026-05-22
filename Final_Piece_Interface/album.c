#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "album.h"

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

// Lê as figurinhas do arquivo e joga na memória RAM
void carregar_csv(Album *al, const char *nome_arquivo) {
    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL) {
        printf("[!] Erro: Arquivo %s nao encontrado.\n", nome_arquivo);
        return;
    }

    char linha[1024];

    // Ignora a primeira linha do CSV (o cabeçalho)
    if (fgets(linha, sizeof(linha), arq) == NULL) {
        fclose(arq);
        return;
    }

    // Lê linha por linha do arquivo de forma segura
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        
        // Se a memória RAM encher, aumenta dinamicamente
        if (al->total >= al->capacidade) {
            al->capacidade += 50;
            al->figurinhas = (Figurinha *) realloc(al->figurinhas, al->capacidade * sizeof(Figurinha));
        }

        Figurinha f;
        f.colada = 0;
        f.repetidas = 0;

        // Extrai os dados da linha usando sscanf de forma limpa
        int lidos = sscanf(linha, " %[^,], %[^,], %[^,], %[^,], %[^\n\r]", 
                           f.codigo, f.titulo, f.secao, f.grupo, f.tipo);

        // Se conseguiu ler os 5 campos da linha com sucesso, adiciona no álbum
        if (lidos == 5) {
            al->figurinhas[al->total] = f;
            al->total++;
        }
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