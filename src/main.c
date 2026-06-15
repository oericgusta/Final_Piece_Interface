#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "album.h"
#include "interface.h"

int main(void) {
    int selecaoAtual = 0;
    int paginaAtual = 0;
    int grupoSelecionado = 0;

    const char* selecoes[] = { 
        "FIFA World Cup 2026", 
        "Mexico", "Africa do Sul", "Coreia do Sul", "Republica Tcheca", 
        "Canada", "Bosnia", "Qatar", "Suica", 
        "Brasil", "Marrocos", "Haiti", "Escocia", 
        "Estados Unidos", "Paraguai", "Australia", "Turquia", 
        "Alemanha", "Curacao", "Costa do Marfim", "Equador", 
        "Holanda", "Japao", "Suecia", "Tunisia", 
        "Belgica", "Egito", "Ira", "Nova Zelandia", 
        "Espanha", "Cabo Verde", "Arabia Saudita", "Uruguai", 
        "Franca", "Senegal", "Iraque", "Noruega", 
        "Argentina", "Argelia", "Austria", "Jordania", 
        "Portugal", "RD Congo", "Uzbequistao", "Colombia", 
        "Inglaterra", "Croacia", "Gana", "Panama"  
    };

    const char* nomesGrupos[] = { "ESPECIAL FWC", "GRUPO A", "GRUPO B", "GRUPO C", "GRUPO D", "GRUPO E", "GRUPO F", "GRUPO G", "GRUPO H", "GRUPO I", "GRUPO J", "GRUPO K", "GRUPO L" };
    int totalGrupos = 13;
    
    InitWindow(1920, 1080, "Álbum de Figurinhas FIFA 2026"); 
    InitAudioDevice();

    Album meu_album;
    inicializar_album(&meu_album); 
    carregar_csv(&meu_album, "assets/figurinhas2026.csv");
    carregar_texturas_album(&meu_album);

    SetExitKey(KEY_NULL);

    bool somAtivo = true;
    float volumeAtual = 0.0f; 
    Rectangle btnSom = { 1810, 40, 70, 70 }; 
    Rectangle btnVoltar = { 40, 40, 70, 70 };
    Rectangle btnSair = { 40, 970, 70, 70 };

    Music musica = LoadMusicStream("assets/music/Shakira-DaiDai.mp3"); 
    PlayMusicStream(musica); 
    musica.looping = true;
    SetMusicVolume(musica, volumeAtual);

    Texture2D iconeSomLigado = LoadTexture("assets/icons/SoundOn_Button.png");
    Texture2D iconeSomMutado = LoadTexture("assets/icons/SoundOff_Button.png");
    Texture2D iconeVoltar = LoadTexture("assets/icons/Back_Button.png");
    Texture2D iconeSair = LoadTexture("assets/icons/Exit_Button.png");
    Texture2D texturaMoldura = LoadTexture("assets/icons/moldura.png");
    Texture2D texturaPagina = LoadTexture("assets/icons/fundo_pagina.png");
    Texture2D logoInicio = LoadTexture("assets/icons/FWC_Inicio.png");
    Texture2D iconeAlbumMenu = LoadTexture("assets/icons/Album_Button.png");
    Texture2D iconePacoteMenu = LoadTexture("assets/icons/Sticker_Button.png");
    Texture2D iconeListarMenu = LoadTexture("assets/icons/Listar_Button.png");
    Texture2D fundoSelecaoGrupos = LoadTexture("assets/icons/EscolhaGrupos.png");
    
    Texture2D animFrames[30]; 
    carregar_background_frames(animFrames);

    EstadoTela tela = MENU;

    // =========================================================================
    // === VARIÁVEIS DE CONFIGURAÇÃO DE TAMANHO E POSIÇÃO (ALTERE AQUI) ========
    // =========================================================================
    // --- Configuração dos Botões do Menu Principal ---
    int larguraBtnMenu = 240;  
    int alturaBtnMenu = 80;    
    int espacamentoBtns = 40;  
    int posYBotoes = 670;      

    // --- Configuração do Fundo da Tela de Grupos ---
    int larguraFundoGrupos = 760; 
    int alturaFundoGrupos = 960;  

    // --- Configurações de posicionamento vertical para o Menu de Seleção de Grupos ---
    int menuGruposOrigemY = 180;   // Posição Y (vertical) onde começa o título principal
    int espacamentoItensY = 50;   // Distância vertical entre cada grupo da lista
    // =========================================================================

    // Cálculo automático para centralizar perfeitamente o bloco horizontal na tela de 1920
    int larguraTotalBloco = (3 * larguraBtnMenu) + (2 * espacamentoBtns);
    int iniciarX = (1920 / 2) - (larguraTotalBloco / 2);
    
    // Criação dos Retângulos perfeitos de colisão e desenho usando as variáveis
    Rectangle btnAlbum    = { iniciarX, posYBotoes, larguraBtnMenu, alturaBtnMenu };
    Rectangle btnPacote   = { iniciarX + larguraBtnMenu + espacamentoBtns, posYBotoes, larguraBtnMenu, alturaBtnMenu };
    Rectangle btnListagem = { iniciarX + (2 * (larguraBtnMenu + espacamentoBtns)), posYBotoes, larguraBtnMenu, alturaBtnMenu };

    int frame = 0, timer = 0;   

    SetTargetFPS(60); 

    while (!WindowShouldClose()) {
        UpdateMusicStream(musica);

        if (somAtivo && volumeAtual < 1.0f) {
            volumeAtual += 0.005f; 
            SetMusicVolume(musica, volumeAtual);
        }

        timer++;
        if (timer >= 10) { timer = 0; frame = (frame + 1) % 30; }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), btnSair)) {
                break;
            }

            // --- CLIQUES DO MOUSE NO MENU PRINCIPAL ---
            if (tela == MENU) {
                if (CheckCollisionPointRec(GetMousePosition(), btnAlbum)) {
                    tela = SELECAO_GRUPOS; 
                }
                if (CheckCollisionPointRec(GetMousePosition(), btnPacote)) {
                    // Próximas atualizações de tela de pacotes entram aqui!
                }
                if (CheckCollisionPointRec(GetMousePosition(), btnListagem)) {
                    // Próximas atualizações de listagem entram aqui!
                }
            }

            if (tela == SELECAO_GRUPOS && CheckCollisionPointRec(GetMousePosition(), btnVoltar)) {
                tela = MENU;
            }

            if (tela == LISTAGEM && CheckCollisionPointRec(GetMousePosition(), btnVoltar)) {
                tela = SELECAO_GRUPOS;
            }

            if (CheckCollisionPointRec(GetMousePosition(), btnSom)) {
                somAtivo = !somAtivo; 
                if (somAtivo) { volumeAtual = 0.0f; SetMasterVolume(1.0f); } 
                else { SetMasterVolume(0.0f); }
            }
        }

        if (tela == SELECAO_GRUPOS) {
            if (IsKeyPressed(KEY_DOWN)) {
                grupoSelecionado = (grupoSelecionado + 1) % totalGrupos;
            }
            if (IsKeyPressed(KEY_UP)) {
                grupoSelecionado = (grupoSelecionado - 1 + totalGrupos) % totalGrupos;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (grupoSelecionado == 0) selecaoAtual = 0; 
                else selecaoAtual = 1 + (grupoSelecionado - 1) * 4; 
                
                paginaAtual = 0;
                tela = LISTAGEM; 
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                tela = MENU;
            }
        }

        if (tela == LISTAGEM) {
            if (IsKeyPressed(KEY_RIGHT) && paginaAtual < 2) {
                paginaAtual++;
            }
            if (IsKeyPressed(KEY_LEFT) && paginaAtual > 0) {
                paginaAtual--;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                if (grupoSelecionado == 0) { } 
                else {
                    int baseGrupo = 1 + (grupoSelecionado - 1) * 4;
                    selecaoAtual = baseGrupo + ((selecaoAtual - baseGrupo + 1) % 4);
                }
                paginaAtual = 0; 
            }
            if (IsKeyPressed(KEY_UP)) {
                if (grupoSelecionado == 0) { } 
                else {
                    int baseGrupo = 1 + (grupoSelecionado - 1) * 4;
                    selecaoAtual = baseGrupo + ((selecaoAtual - baseGrupo - 1 + 4) % 4);
                }
                paginaAtual = 0; 
            }
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
                tela = SELECAO_GRUPOS; 
            }
        }

        BeginDrawing();
            desenhar_background(animFrames, frame);
            
            // --- RENDERIZAÇÃO DO MENU PRINCIPAL ---
            if (tela == MENU) {
                /* 1. Desenha o logotipo centralizado na metade superior */
                int larguraDesejadaLogo = 750; 
                int alturaDesejadaLogo = 525;  
                
                int logoX = (1920 / 2) - (larguraDesejadaLogo / 2);
                int logoY = 80; 

                Rectangle origemLogo = { 0.0f, 0.0f, (float)logoInicio.width, (float)logoInicio.height };
                Rectangle destinoLogo = { (float)logoX, (float)logoY, (float)larguraDesejadaLogo, (float)alturaDesejadaLogo };
                DrawTexturePro(logoInicio, origemLogo, destinoLogo, (Vector2){ 0, 0 }, 0.0f, WHITE);


                /* 2. Desenha os três botões com as imagens redimensionadas */

                /* === BOTÃO 1: ÁLBUM === */
                DrawTexturePro(iconeAlbumMenu, 
                               (Rectangle){ 0, 0, (float)iconeAlbumMenu.width, (float)iconeAlbumMenu.height }, 
                               btnAlbum, (Vector2){ 0, 0 }, 0.0f, WHITE);

                /* === BOTÃO 2: ABRIR PACOTE === */
                DrawTexturePro(iconePacoteMenu, 
                               (Rectangle){ 0, 0, (float)iconePacoteMenu.width, (float)iconePacoteMenu.height }, 
                               btnPacote, (Vector2){ 0, 0 }, 0.0f, WHITE);

                /* === BOTÃO 3: LISTAGEM GERAL === */
                DrawTexturePro(iconeListarMenu, 
                               (Rectangle){ 0, 0, (float)iconeListarMenu.width, (float)iconeListarMenu.height }, 
                               btnListagem, (Vector2){ 0, 0 }, 0.0f, WHITE);
            } 
            else if (tela == SELECAO_GRUPOS) {
                // Desenha a imagem de fundo centralizada usando as variáveis de tamanho
                int fundoX = (1920 / 2) - (larguraFundoGrupos / 2);
                int fundoY = (1080 / 2) - (alturaFundoGrupos / 2);

                Rectangle origemFundo = { 0.0f, 0.0f, (float)fundoSelecaoGrupos.width, (float)fundoSelecaoGrupos.height };
                Rectangle destinoFundo = { (float)fundoX, (float)fundoY, (float)larguraFundoGrupos, (float)alturaFundoGrupos }; 
                DrawTexturePro(fundoSelecaoGrupos, origemFundo, destinoFundo, (Vector2){ 0, 0 }, 0.0f, WHITE);

                // === ATUALIZADO: Título e subtítulo calculados dinamicamente para o centro exato ===
                int tituloX = (1920 / 2) - (MeasureText("SELECIONE O GRUPO DA COPA", 40) / 2);
                DrawText("SELECIONE O GRUPO DA COPA", tituloX, menuGruposOrigemY, 40, BLACK);

                int subtituloX = (1920 / 2) - (MeasureText("Use [ CIMA / BAIXO ] para navegar e [ ENTER ] para confirmar", 20) / 2);
                DrawText("Use [ CIMA / BAIXO ] para navegar e [ ENTER ] para confirmar", subtituloX, menuGruposOrigemY + 60, 20, GRAY);

                DrawTexturePro(iconeVoltar, (Rectangle){ 0, 0, iconeVoltar.width, iconeVoltar.height }, btnVoltar, (Vector2){ 0, 0 }, 0.0f, WHITE);

                // O início da lista de grupos começará 130 pixels abaixo do título principal
                int inicioListaY = menuGruposOrigemY + 130;

                // === Laço for calcula a largura de cada texto para manter tudo centralizado ===
                for (int i = 0; i < totalGrupos; i++) {

                int posY = inicioListaY + (i * espacamentoItensY);
                
                if (i == grupoSelecionado) {
                    
                    const char* textoFormatado = TextFormat("->  %s  <-", nomesGrupos[i]);
                    
                    int itemX = (1920 / 2) - (MeasureText(textoFormatado, 35) / 2); 
                    DrawText(textoFormatado, itemX, posY, 35, GREEN);
                } else {
                    
                    int itemX = (1920 / 2) - (MeasureText(nomesGrupos[i], 30) / 2); 
                    DrawText(nomesGrupos[i], itemX, posY, 30, BLACK);
                }
}
            }
            else if (tela == LISTAGEM) {
                DesenharAlbumPorSelecao(&meu_album, selecoes[selecaoAtual], paginaAtual, texturaMoldura, texturaPagina);
                DrawTexturePro(iconeVoltar, (Rectangle){ 0, 0, iconeVoltar.width, iconeVoltar.height }, btnVoltar, (Vector2){ 0, 0 }, 0.0f, WHITE);
            }

            DrawTexturePro(iconeSair, (Rectangle){ 0, 0, iconeSair.width, iconeSair.height }, btnSair, (Vector2){ 0, 0 }, 0.0f, WHITE);
            desenhar_botao_som(somAtivo, btnSom, iconeSomLigado, iconeSomMutado);
        EndDrawing();
    }

    UnloadTexture(iconeVoltar);
    UnloadTexture(iconeSair);
    UnloadTexture(texturaMoldura);
    UnloadTexture(texturaPagina);
    UnloadTexture(logoInicio);
    UnloadTexture(iconeAlbumMenu);
    UnloadTexture(iconePacoteMenu);
    UnloadTexture(iconeListarMenu);
    UnloadTexture(fundoSelecaoGrupos);

    encerrar_jogo(&meu_album, animFrames, iconeSomLigado, iconeSomMutado, musica);
    return 0;
}