#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "album.h"
#include "interface.h"

    // Protótipos de funções de arquivo (definidas em salvamento.c)
void carregar_csv(Album *al, const char *nome_arquivo);
void salvar_csv(Album *al, const char *nome_arquivo);
void salvar_binario(Album *al, const char *nome_arquivo);
int  carregar_binario(Album *al, const char *nome_arquivo);

/* Caminhos dos arquivos de dados */
#define ARQUIVO_BINARIO "assets/album2026.bin"
#define ARQUIVO_CSV     "assets/figurinhas2026.csv"

/* Protótipos das funções deste arquivo */
static void exibir_menu_principal(void);
static void exibir_submenu_cadastro(void);
static void menu_cadastro(Album *al);
static void executar_interface_grafica(Album *meu_album);

/* ========================================================================
 *  MAIN  –  Menu de console (obrigatório pelo enunciado) + chamada da GUI
 * ======================================================================== */
int main(void) {
    srand((unsigned int) time(NULL));

    Album meu_album;
    inicializar_album(&meu_album);

    // Tenta carregar o progresso salvo em binário; se não existir
    // (1ª execução), carrega os dados iniciais a partir do CSV.
    if (!carregar_binario(&meu_album, ARQUIVO_BINARIO)) {
        printf("[i] Nenhum progresso salvo encontrado. Carregando dados iniciais do CSV...\n");
        carregar_csv(&meu_album, ARQUIVO_CSV);
    }

    int opcao;
    do {
        exibir_menu_principal();

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // limpa o buffer em caso de entrada inválida
            printf("\a[!] Entrada invalida! Digite um numero.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1: menu_cadastro(&meu_album); break;
            case 2: executar_interface_grafica(&meu_album); break;
            case 3: abrir_pacotinho(&meu_album); break;
            case 4: salvar_csv(&meu_album, ARQUIVO_CSV); break;
            case 0: printf("\nSalvando e encerrando...\n"); break;
            default: printf("\a[!] Opcao invalida!\n");
        }
    } while (opcao != 0);

    // Persistência final: dados salvos em arquivo binário no HD
    salvar_binario(&meu_album, ARQUIVO_BINARIO);
    liberar_album(&meu_album);

    printf("Ate a proxima Copa!\n");
    return 0;
}

    // ======= MENUS DE CONSOLE =======
static void exibir_menu_principal(void) {
    printf("\n");
    printf("==================================================\n");
    printf("   ALBUM DE FIGURINHAS - COPA DO MUNDO 2026\n");
    printf("==================================================\n");
    printf(" 1 - Cadastro de Figurinhas\n");
    printf(" 2 - Acessar o Album (Interface Grafica)\n");
    printf(" 3 - Abrir Pacotinho (modo texto)\n");
    printf(" 4 - Exportar dados para CSV\n");
    printf(" 0 - Salvar e Sair\n");
    printf("==================================================\n");
    printf("Escolha uma opcao: ");
}

static void exibir_submenu_cadastro(void) {
    printf("\n");
    printf("------------- CADASTRO DE FIGURINHAS ------------\n");
    printf(" 1 - Inserir nova figurinha\n");
    printf(" 2 - Listar todas as figurinhas\n");
    printf(" 3 - Pesquisar figurinha por codigo\n");
    printf(" 4 - Alterar figurinha\n");
    printf(" 5 - Excluir figurinha\n");
    printf(" 0 - Voltar ao menu principal\n");
    printf("---------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

    /**
     * @brief Submenu responsável pelo CRUD completo de figurinhas (console).
     * @param al Ponteiro para o álbum.
     */
static void menu_cadastro(Album *al) {
    int opcao;
    do {
        exibir_submenu_cadastro();

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf("\a[!] Entrada invalida!\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1: inserir_figurinha(al);   break;
            case 2: listar_figurinhas(al);   break;
            case 3: pesquisar_figurinha(al); break;
            case 4: alterar_figurinha(al);   break;
            case 5: excluir_figurinha(al);   break;
            case 0: break;
            default: printf("\a[!] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

    // ======== INTERFACE GRÁFICA (Raylib) ========

    /**
     * @brief Abre a janela gráfica do álbum: visualização, abertura de pacotes
     *        e estatísticas. Ao fechar a janela, retorna ao menu de console
     *        sem perder os dados (o álbum continua na memória RAM).
     * @param meu_album Ponteiro para o álbum já carregado em memória.
     */
static void executar_interface_grafica(Album *meu_album) {

    // ------- Dados de navegação do álbum -------
    int selecaoAtual    = 0;
    int paginaAtual     = 0;
    int grupoSelecionado = 0;

    const char *selecoes[] = {
        "FIFA World Cup 2026",
        "Mexico",         "Africa do Sul",    "Coreia do Sul",   "Republica Tcheca",
        "Canada",         "Bosnia",           "Qatar",           "Suica",
        "Brasil",         "Marrocos",         "Haiti",           "Escocia",
        "Estados Unidos", "Paraguai",         "Australia",       "Turquia",
        "Alemanha",       "Curacao",          "Costa do Marfim", "Equador",
        "Holanda",        "Japao",            "Suecia",          "Tunisia",
        "Belgica",        "Egito",            "Ira",             "Nova Zelandia",
        "Espanha",        "Cabo Verde",       "Arabia Saudita",  "Uruguai",
        "Franca",         "Senegal",          "Iraque",          "Noruega",
        "Argentina",      "Argelia",          "Austria",         "Jordania",
        "Portugal",       "RD Congo",         "Uzbequistao",     "Colombia",
        "Inglaterra",     "Croacia",          "Gana",            "Panama"
    };

    const char *nomesGrupos[] = {
        "ESPECIAL FWC",
        "GRUPO A", "GRUPO B", "GRUPO C", "GRUPO D", "GRUPO E", "GRUPO F",
        "GRUPO G", "GRUPO H", "GRUPO I", "GRUPO J", "GRUPO K", "GRUPO L"
    };
    int totalGrupos = 13;

    // ------- Estado da tela do pacotinho -------
    int  indicesPacote[7] = { 0 };
    int  cartaAtualPacote  = 0;
    bool cartaViradaPacote = false;
    bool pacoteFinalizado  = false;
    int  novasObtidas      = 0;

    // ------- Inicialização da janela e do áudio -------
    InitWindow(1920, 1080, "Album de Figurinhas FIFA 2026");
    InitAudioDevice();
    SetExitKey(KEY_NULL);

    // Carrega as texturas das figurinhas (precisa da janela já aberta)
    carregar_texturas_album(meu_album);

    // ------- Configurações de volume e música -------
    bool  somAtivo    = true;
    float volumeAtual = 0.0f;

    Music musica = LoadMusicStream("assets/music/Shakira-DaiDai.mp3");
    musica.looping = true;
    PlayMusicStream(musica);
    SetMusicVolume(musica, volumeAtual);

    // ------- Carregamento de texturas da interface -------
    Texture2D iconeSomLigado  = LoadTexture("assets/icons/SoundOn_Button.png");
    Texture2D iconeSomMutado  = LoadTexture("assets/icons/SoundOff_Button.png");
    Texture2D iconeVoltar     = LoadTexture("assets/icons/Back_Button.png");
    Texture2D iconeSair       = LoadTexture("assets/icons/Exit_Button.png");
    Texture2D texturaMoldura  = LoadTexture("assets/icons/moldura.png");
    Texture2D texturaPagina   = LoadTexture("assets/icons/fundo_pagina.png");
    Texture2D logoInicio      = LoadTexture("assets/icons/FWC_Inicio.png");
    Texture2D iconeAlbumMenu  = LoadTexture("assets/icons/Album_Button.png");
    Texture2D iconePacoteMenu = LoadTexture("assets/icons/Sticker_Button.png");
    Texture2D iconeStatsMenu  = LoadTexture("assets/icons/Listar_Button.png");
    Texture2D fundoSelGrupos  = LoadTexture("assets/icons/EscolhaGrupos.png");

    Texture2D animFrames[30];
    carregar_background_frames(animFrames);

    // ------- Botões persistentes (som, voltar, sair) -------
    Rectangle btnSom    = { 1810, 40,  70, 70 };
    Rectangle btnVoltar = {   40, 40,  70, 70 };
    Rectangle btnSair   = {   40, 970, 70, 70 };

    // ------- Layout dos botões do menu principal -------
    int larguraBtnMenu   = 240;
    int alturaBtnMenu    = 80;
    int espacamentoBtns  = 40;
    int posYBotoes       = 670;

    int larguraTotalBloco = (3 * larguraBtnMenu) + (2 * espacamentoBtns);
    int iniciarX          = (1920 / 2) - (larguraTotalBloco / 2);

    Rectangle btnAlbum    = { (float)iniciarX,                                          (float)posYBotoes, (float)larguraBtnMenu, (float)alturaBtnMenu };
    Rectangle btnPacote   = { (float)(iniciarX + larguraBtnMenu + espacamentoBtns),     (float)posYBotoes, (float)larguraBtnMenu, (float)alturaBtnMenu };
    Rectangle btnStats    = { (float)(iniciarX + 2 * (larguraBtnMenu + espacamentoBtns)), (float)posYBotoes, (float)larguraBtnMenu, (float)alturaBtnMenu };

    // Layout do fundo da tela de seleção de grupos
    int larguraFundoGrupos = 760;
    int alturaFundoGrupos  = 960;
    int menuGruposOrigemY  = 180;
    int espacamentoItensY  = 50;

    // --------- Animação e estado da tela ---------
    int frame = 0, timer = 0;
    EstadoTela tela = MENU;

    SetTargetFPS(60);

    // ======== LOOP DA INTERFACE GRÁFICA ========
    while (!WindowShouldClose()) {

        UpdateMusicStream(musica);

        // Fade-in do volume
        if (somAtivo && volumeAtual < 1.0f) {
            volumeAtual += 0.005f;
            SetMusicVolume(musica, volumeAtual);
        }

        // Animação do background
        if (++timer >= 10) { timer = 0; frame = (frame + 1) % 30; }

        // --------- TRATAMENTO DE ENTRADA ---------

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            // Botão Sair: fecha a janela e volta para o menu de console
            if (CheckCollisionPointRec(GetMousePosition(), btnSair)) {
                break;
            }

            // Botão Som (sempre visível)
            if (CheckCollisionPointRec(GetMousePosition(), btnSom)) {
                somAtivo = !somAtivo;
                SetMasterVolume(somAtivo ? 1.0f : 0.0f);
                if (somAtivo) volumeAtual = 0.0f;
            }

            // ---- MENU PRINCIPAL ----
            if (tela == MENU) {
                if (CheckCollisionPointRec(GetMousePosition(), btnAlbum)) {
                    tela = SELECAO_GRUPOS;
                }
                if (CheckCollisionPointRec(GetMousePosition(), btnPacote) && meu_album->total > 0) {
                    // Gera 7 índices aleatórios e inicializa o estado do pacote
                    for (int i = 0; i < 7; i++) {
                        indicesPacote[i] = rand() % meu_album->total;
                    }
                    cartaAtualPacote  = 0;
                    cartaViradaPacote = false;
                    pacoteFinalizado  = false;
                    novasObtidas      = 0;
                    tela = PACOTE;
                }
                if (CheckCollisionPointRec(GetMousePosition(), btnStats)) {
                    tela = ESTATISTICAS;
                }
            }

            // ---- VOLTAR ----
            if (tela == SELECAO_GRUPOS && CheckCollisionPointRec(GetMousePosition(), btnVoltar)) {
                tela = MENU;
            }
            if (tela == LISTAGEM && CheckCollisionPointRec(GetMousePosition(), btnVoltar)) {
                tela = SELECAO_GRUPOS;
            }
            if (tela == PACOTE && CheckCollisionPointRec(GetMousePosition(), btnVoltar)) {
                tela = MENU;
            }
            if (tela == ESTATISTICAS && CheckCollisionPointRec(GetMousePosition(), btnVoltar)) {
                tela = MENU;
            }
        }

        // ---- TECLADO: SELEÇÃO DE GRUPOS ----
        if (tela == SELECAO_GRUPOS) {
            if (IsKeyPressed(KEY_DOWN))
                grupoSelecionado = (grupoSelecionado + 1) % totalGrupos;
            if (IsKeyPressed(KEY_UP))
                grupoSelecionado = (grupoSelecionado - 1 + totalGrupos) % totalGrupos;
            if (IsKeyPressed(KEY_ENTER)) {
                selecaoAtual = (grupoSelecionado == 0) ? 0 : 1 + (grupoSelecionado - 1) * 4;
                paginaAtual  = 0;
                tela         = LISTAGEM;
            }
            if (IsKeyPressed(KEY_ESCAPE))
                tela = MENU;
        }

        // ---- TECLADO: LISTAGEM ----
        if (tela == LISTAGEM) {
            if (IsKeyPressed(KEY_RIGHT) && paginaAtual < 2) paginaAtual++;
            if (IsKeyPressed(KEY_LEFT)  && paginaAtual > 0) paginaAtual--;

            if (IsKeyPressed(KEY_DOWN) && grupoSelecionado != 0) {
                int base = 1 + (grupoSelecionado - 1) * 4;
                selecaoAtual = base + ((selecaoAtual - base + 1) % 4);
                paginaAtual = 0;
            }
            if (IsKeyPressed(KEY_UP) && grupoSelecionado != 0) {
                int base = 1 + (grupoSelecionado - 1) * 4;
                selecaoAtual = base + ((selecaoAtual - base - 1 + 4) % 4);
                paginaAtual = 0;
            }
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
                tela = SELECAO_GRUPOS;
        }

        // ---- TECLADO: PACOTINHO ----
        if (tela == PACOTE) {
            if (!pacoteFinalizado) {
                if (!cartaViradaPacote) {
                    // Revela a carta atual
                    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                        cartaViradaPacote = true;
                } else {
                    // Decide o que fazer com a carta revelada
                    if (IsKeyPressed(KEY_S)) {
                        Figurinha *f = &meu_album->figurinhas[indicesPacote[cartaAtualPacote]];
                        if (f->colada == 0) {
                            f->colada = 1;
                            novasObtidas++;
                        } else {
                            f->repetidas++;
                        }
                        cartaAtualPacote++;
                        cartaViradaPacote = false;
                        if (cartaAtualPacote >= 7) pacoteFinalizado = true;
                    }
                    if (IsKeyPressed(KEY_N)) {
                        cartaAtualPacote++;
                        cartaViradaPacote = false;
                        if (cartaAtualPacote >= 7) pacoteFinalizado = true;
                    }
                }
            } else {
                if (IsKeyPressed(KEY_ENTER)) tela = MENU;
            }
            if (IsKeyPressed(KEY_ESCAPE)) tela = MENU;
        }

        // ---- TECLADO: ESTATÍSTICAS ----
        if (tela == ESTATISTICAS) {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
                tela = MENU;
        }

        // ======= RENDERIZAÇÃO ======= 
        BeginDrawing();

            desenhar_background(animFrames, frame);

            // ---- MENU PRINCIPAL ----
            if (tela == MENU) {
                int larguraDesejadaLogo = 750;
                int alturaDesejadaLogo  = 525;
                int logoX = (1920 / 2) - (larguraDesejadaLogo / 2);
                int logoY = 80;

                DrawTexturePro(logoInicio,
                               (Rectangle){ 0, 0, (float)logoInicio.width, (float)logoInicio.height },
                               (Rectangle){ (float)logoX, (float)logoY, (float)larguraDesejadaLogo, (float)alturaDesejadaLogo },
                               (Vector2){ 0, 0 }, 0.0f, WHITE);

                DrawTexturePro(iconeAlbumMenu,
                               (Rectangle){ 0, 0, (float)iconeAlbumMenu.width,  (float)iconeAlbumMenu.height },
                               btnAlbum, (Vector2){ 0, 0 }, 0.0f, WHITE);

                DrawTexturePro(iconePacoteMenu,
                               (Rectangle){ 0, 0, (float)iconePacoteMenu.width, (float)iconePacoteMenu.height },
                               btnPacote, (Vector2){ 0, 0 }, 0.0f, WHITE);

                DrawTexturePro(iconeStatsMenu,
                               (Rectangle){ 0, 0, (float)iconeStatsMenu.width,  (float)iconeStatsMenu.height },
                               btnStats, (Vector2){ 0, 0 }, 0.0f, WHITE);
            }

            // ---- SELEÇÃO DE GRUPOS ----
            else if (tela == SELECAO_GRUPOS) {
                int fundoX = (1920 / 2) - (larguraFundoGrupos / 2);
                int fundoY = (1080 / 2) - (alturaFundoGrupos / 2);

                DrawTexturePro(fundoSelGrupos,
                               (Rectangle){ 0, 0, (float)fundoSelGrupos.width, (float)fundoSelGrupos.height },
                               (Rectangle){ (float)fundoX, (float)fundoY, (float)larguraFundoGrupos, (float)alturaFundoGrupos },
                               (Vector2){ 0, 0 }, 0.0f, WHITE);

                int tituloX = (1920 / 2) - (MeasureText("SELECIONE O GRUPO DA COPA", 40) / 2);
                DrawText("SELECIONE O GRUPO DA COPA", tituloX, menuGruposOrigemY, 40, BLACK);

                int subtituloX = (1920 / 2) - (MeasureText("Use [ CIMA / BAIXO ] para navegar e [ ENTER ] para confirmar", 20) / 2);
                DrawText("Use [ CIMA / BAIXO ] para navegar e [ ENTER ] para confirmar",
                         subtituloX, menuGruposOrigemY + 60, 20, GRAY);

                DrawTexturePro(iconeVoltar,
                               (Rectangle){ 0, 0, (float)iconeVoltar.width, (float)iconeVoltar.height },
                               btnVoltar, (Vector2){ 0, 0 }, 0.0f, WHITE);

                int inicioListaY = menuGruposOrigemY + 130;

                for (int i = 0; i < totalGrupos; i++) {
                    int posY = inicioListaY + (i * espacamentoItensY);
                    if (i == grupoSelecionado) {
                        const char *textoSel = TextFormat("->  %s  <-", nomesGrupos[i]);
                        int itemX = (1920 / 2) - (MeasureText(textoSel, 35) / 2);
                        DrawText(textoSel, itemX, posY, 35, GREEN);
                    } else {
                        int itemX = (1920 / 2) - (MeasureText(nomesGrupos[i], 30) / 2);
                        DrawText(nomesGrupos[i], itemX, posY, 30, BLACK);
                    }
                }
            }

            // ---- LISTAGEM ----
            else if (tela == LISTAGEM) {
                DesenharAlbumPorSelecao(meu_album, selecoes[selecaoAtual], paginaAtual,
                                        texturaMoldura, texturaPagina);
                DrawTexturePro(iconeVoltar,
                               (Rectangle){ 0, 0, (float)iconeVoltar.width, (float)iconeVoltar.height },
                               btnVoltar, (Vector2){ 0, 0 }, 0.0f, WHITE);
            }

            // ---- PACOTINHO ----
            else if (tela == PACOTE) {
                DesenharPacote(meu_album, indicesPacote, cartaAtualPacote,
                               cartaViradaPacote, pacoteFinalizado,
                               novasObtidas, texturaMoldura);
                DrawTexturePro(iconeVoltar,
                               (Rectangle){ 0, 0, (float)iconeVoltar.width, (float)iconeVoltar.height },
                               btnVoltar, (Vector2){ 0, 0 }, 0.0f, WHITE);
            }

            // ---- ESTATÍSTICAS ----
            else if (tela == ESTATISTICAS) {
                DesenharEstatisticas(meu_album);
                DrawTexturePro(iconeVoltar,
                               (Rectangle){ 0, 0, (float)iconeVoltar.width, (float)iconeVoltar.height },
                               btnVoltar, (Vector2){ 0, 0 }, 0.0f, WHITE);
            }

            // Botões sempre visíveis
            DrawTexturePro(iconeSair,
                           (Rectangle){ 0, 0, (float)iconeSair.width, (float)iconeSair.height },
                           btnSair, (Vector2){ 0, 0 }, 0.0f, WHITE);
            desenhar_botao_som(somAtivo, btnSom, iconeSomLigado, iconeSomMutado);

        EndDrawing();
    }

    // ==================================================================
    //  Salva o progresso como rede de segurança ao fechar a janela      
    //  (o álbum continua vivo na RAM para o menu de console)            
    // ==================================================================
    salvar_binario(meu_album, ARQUIVO_BINARIO);

    // Descarrega ícones de UI (o resto fica por conta de encerrar_jogo)
    UnloadTexture(texturaMoldura);
    UnloadTexture(texturaPagina);
    UnloadTexture(logoInicio);
    UnloadTexture(iconeAlbumMenu);
    UnloadTexture(iconePacoteMenu);
    UnloadTexture(iconeStatsMenu);
    UnloadTexture(fundoSelGrupos);
    UnloadTexture(iconeVoltar);
    UnloadTexture(iconeSair);

    // Fecha janela/áudio e descarrega texturas do álbum (NÃO libera a RAM)
    encerrar_jogo(meu_album, animFrames, iconeSomLigado, iconeSomMutado, musica);
}