#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "album.h"

int main() {
    // Semente para os números aleatórios do pacotinho funcionarem
    srand(time(NULL));

    Album meu_album;
    inicializar_album(&meu_album);

    // Carrega os dados automaticamente do seu CSV ao iniciar
    carregar_csv(&meu_album, "assets/figurinhas2026.csv");

    int opcao = 0;
    while (opcao != 5) {
        printf("\n======= ALBUM COPA 2026 =======");
        printf("\n1. Listar todas as figurinhas");
        printf("\n2. Pesquisar por codigo");
        printf("\n3. Abrir pacote (7 figurinhas)");
        printf("\n4. Adicionar/Remover Figurinhas (CRUD)"); // Espaço para expandir
        printf("\n5. Sair do Programa");
        printf("\n===============================");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listar_figurinhas(&meu_album);
                break;
            case 2:
                pesquisar_figurinha(&meu_album);
                break;
            case 3:
                abrir_pacotinho(&meu_album);
                break;
            case 4:
                printf("Ainda nao fiz ksks\n");
                break;
            case 5:
                printf("Encerrando o sistema. Ate logo!\n");
                break;
            default:
                printf("[!] Opcao invalida!\n");
        }
    }

    // Salva antes de sair e limpa a memória RAM
    liberar_album(&meu_album);
    return 0;
}