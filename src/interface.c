#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "interface.h"
#include "album.h"

/* ================================================================
 *  FUNÇÕES AUXILIARES INTERNAS (não expostas no .h)
 * ================================================================ */

/**
 * @brief Converte nomes de seleções em versões abreviadas para a UI.
 * @param nomeOriginal Nome completo da seleção.
 * @return Ponteiro para a string abreviada (ou o original se não houver mapeamento).
 */
static const char* ObterNomeAbreviado(const char *nomeOriginal) {
    if (strcmp(nomeOriginal, "FIFA World Cup 2026") == 0) return "FWC 2026";
    if (strcmp(nomeOriginal, "Mexico") == 0) return "Mexico";
    if (strcmp(nomeOriginal, "Africa do Sul") == 0) return "Africa do S.";
    if (strcmp(nomeOriginal, "Coreia do Sul") == 0) return "Coreia do S.";
    if (strcmp(nomeOriginal, "Republica Tcheca") == 0) return "Rep. Tcheca";
    if (strcmp(nomeOriginal, "Canada") == 0) return "Canada";
    if (strcmp(nomeOriginal, "Bosnia") == 0) return "Bosnia";
    if (strcmp(nomeOriginal, "Suica") == 0) return "Suica";
    if (strcmp(nomeOriginal, "Escocia") == 0) return "Escocia";
    if (strcmp(nomeOriginal, "Estados Unidos") == 0) return "EUA";
    if (strcmp(nomeOriginal, "Australia") == 0) return "Australia";
    if (strcmp(nomeOriginal, "Curacao") == 0) return "Curacao";
    if (strcmp(nomeOriginal, "Costa do Marfim") == 0) return "Costa do Mar.";
    if (strcmp(nomeOriginal, "Japao") == 0) return "Japao";
    if (strcmp(nomeOriginal, "Suecia") == 0) return "Suecia";
    if (strcmp(nomeOriginal, "Tunisia") == 0) return "Tunisia";
    if (strcmp(nomeOriginal, "Belgica") == 0) return "Belgica";
    if (strcmp(nomeOriginal, "Ira") == 0) return "Ira";
    if (strcmp(nomeOriginal, "Nova Zelandia") == 0) return "N. Zelandia";
    if (strcmp(nomeOriginal, "Arabia Saudita") == 0) return "Arab. Saudita";
    if (strcmp(nomeOriginal, "Franca") == 0) return "Franca";
    if (strcmp(nomeOriginal, "Argelia") == 0) return "Argelia";
    if (strcmp(nomeOriginal, "Jordania") == 0) return "Jordania";
    if (strcmp(nomeOriginal, "Uzbequistao") == 0) return "Uzbequistao";
    if (strcmp(nomeOriginal, "Colombia") == 0) return "Colombia";
    if (strcmp(nomeOriginal, "Croacia") == 0) return "Croacia";
    if (strcmp(nomeOriginal, "Panama") == 0) return "Panama";
    return nomeOriginal;
}

/**
 * @brief Encurta títulos de figurinhas muito longos para caber na UI.
 * @param tituloOriginal Título completo.
 * @return Ponteiro para a versão curta (ou o original).
 */
static const char* ObterTituloAbreviado(const char *tituloOriginal) {
    if (strcmp(tituloOriginal, "Escudo dos EUA (Pais-Sede)") == 0)       return "Escudo (EUA)";
    if (strcmp(tituloOriginal, "Escudo do Mexico (Pais-Sede)") == 0)     return "Escudo (MEX)";
    if (strcmp(tituloOriginal, "Escudo do Canada (Pais-Sede)") == 0)     return "Escudo (CAN)";
    if (strcmp(tituloOriginal, "Logotipo da Panini") == 0)               return "Logo Panini";
    if (strcmp(tituloOriginal, "Emblema Oficial (Parte 1)") == 0)        return "Emblema Oficial 1";
    if (strcmp(tituloOriginal, "Emblema Oficial (Parte 2)") == 0)        return "Emblema Oficial 2";
    if (strcmp(tituloOriginal, "Henry Heroki Mochizuki") == 0)           return "Henry H. Mochizuki";
    if (strcmp(tituloOriginal, "Victor Nilsson Lindelof") == 0)          return "Victor N. Lindelof";
    if (strcmp(tituloOriginal, "Mohamed Ali Ben Romdhane") == 0)         return "Mohamed A. Romdhane";
    if (strcmp(tituloOriginal, "Kristoffer Vassbakk Ajer") == 0)         return "Kristoffer V. Ajer";
    if (strcmp(tituloOriginal, "Marcus Holmgren Pedersen") == 0)         return "Marcus H. Pedersen";
    if (strcmp(tituloOriginal, "Mohamed Amine Tougai") == 0)             return "Mohamed A. Tougai";
    if (strcmp(tituloOriginal, "Mohammad Abu Hashish") == 0)             return "Mohammad A. Hashish";
    if (strcmp(tituloOriginal, "Mohammad Abu Zrayq") == 0)               return "Mohammad A. Zrayq";
    if (strcmp(tituloOriginal, "Juan Fernando Quintero") == 0)           return "Juan F. Quintero";
    if (strcmp(tituloOriginal, "Abdul Issahaku Fatawu") == 0)            return "Abdul I. Fatawu";
    if (strcmp(tituloOriginal, "Trent Alexander-Arnold") == 0)           return "Trent A-Arnold";
    return tituloOriginal;
}

/* ================================================================
 *  BACKGROUND ANIMADO
 * ================================================================ */

void carregar_background_frames(Texture2D frames[]) {
    char path[100];
    for (int i = 0; i < 30; i++) {
        sprintf(path, "assets/background/frame%02d.png", i);
        frames[i] = LoadTexture(path);
    }
}

void desenhar_background(Texture2D *frames, int frameAtual) {
    DrawTexture(frames[frameAtual], 0, 0, WHITE);
}

/* ================================================================
 *  BOTÃO DE SOM
 * ================================================================ */

void desenhar_botao_som(bool somAtivo, Rectangle btnSom, Texture2D iconeOn, Texture2D iconeOff) {
    Texture2D icone = somAtivo ? iconeOn : iconeOff;
    DrawTexturePro(icone,
                   (Rectangle){ 0, 0, (float)icone.width, (float)icone.height },
                   btnSom, (Vector2){ 0, 0 }, 0.0f, WHITE);
}

/* ================================================================
 *  TELA: LISTAGEM DO ÁLBUM POR SELEÇÃO
 * ================================================================ */

void DesenharAlbumPorSelecao(Album *al, const char *secaoAtiva, int paginaAtiva,
                              Texture2D moldura, Texture2D fundoPagina) {
    /* Configurações da grade de figurinhas */
    int colunas     = 4;
    int linhas      = 2;
    int larguraCard = 210;
    int alturaCard  = 280;
    int espacamentoX = 85;
    int espacamentoY = 90;

    int larguraTotalBloco = (colunas * larguraCard) + ((colunas - 1) * espacamentoX);
    int alturaTotalBloco  = (linhas  * alturaCard)  + ((linhas  - 1) * espacamentoY);

    int margemEsquerda = (GetScreenWidth()  - larguraTotalBloco) / 2;
    int margemTopo     = ((GetScreenHeight() - alturaTotalBloco)  / 2) + 40;

    int cardsPorPagina = colunas * linhas;

    /* Filtragem das figurinhas da seleção ativa */
    Figurinha *filtradas[50];
    int totalFiltradas   = 0;
    int coladasNaSelecao = 0;

    for (int i = 0; i < al->total; i++) {
        if (strcmp(al->figurinhas[i].secao, secaoAtiva) == 0) {
            filtradas[totalFiltradas] = &al->figurinhas[i];
            if (al->figurinhas[i].colada == 1) coladasNaSelecao++;
            totalFiltradas++;
        }
    }

    int indiceInicial = paginaAtiva * cardsPorPagina;
    int indiceFinal   = indiceInicial + cardsPorPagina;
    if (indiceFinal > totalFiltradas) indiceFinal = totalFiltradas;

    /* Fundo da página e cabeçalho */
    if (totalFiltradas > 0) {
        int folgaHorizontal = 80;
        int folgaVertical   = 170;

        int centroBlocoX = margemEsquerda + (larguraTotalBloco / 2);
        int centroBlocoY = margemTopo     + (alturaTotalBloco  / 2);

        int novaLarguraFolha = larguraTotalBloco + (folgaHorizontal * 2);
        int novaAlturaFolha  = alturaTotalBloco  + (folgaVertical   * 2);

        Rectangle rectDestinoFolha = {
            (float)(centroBlocoX - (novaLarguraFolha / 2)),
            (float)(centroBlocoY - (novaAlturaFolha  / 2) - 45),
            (float)novaLarguraFolha,
            (float)novaAlturaFolha
        };

        int textoSelecaoX = (int)rectDestinoFolha.x + 75;
        int textoSelecaoY = (int)rectDestinoFolha.y + 93;

        DrawTexturePro(fundoPagina,
                       (Rectangle){ 0, 0, (float)fundoPagina.width, (float)fundoPagina.height },
                       rectDestinoFolha, (Vector2){ 0, 0 }, 0.0f, WHITE);

        const char *nomeExibicao = ObterNomeAbreviado(secaoAtiva);

        DrawText(nomeExibicao, textoSelecaoX,     textoSelecaoY - 1, 38, BLACK);
        DrawText(nomeExibicao, textoSelecaoX + 1, textoSelecaoY - 1, 38, BLACK);

        char txtProgresso[30];
        sprintf(txtProgresso, "%d/%d coladas", coladasNaSelecao, totalFiltradas);

        int larguraTxtProgresso = MeasureText(txtProgresso, 32);
        int progressoX = (int)(rectDestinoFolha.x + rectDestinoFolha.width) - 75 - larguraTxtProgresso;
        int progressoY = (int)rectDestinoFolha.y + 97;

        DrawText(txtProgresso, progressoX,     progressoY, 32, BLACK);
        DrawText(txtProgresso, progressoX + 1, progressoY, 32, BLACK);

        char txtPagina[10];
        sprintf(txtPagina, "%02d", paginaAtiva + 1);
        int larguraTxtPagina = MeasureText(txtPagina, 45);
        int paginaX = (int)(rectDestinoFolha.x + rectDestinoFolha.width / 2) - (larguraTxtPagina / 2);
        int paginaY = (int)(rectDestinoFolha.y + rectDestinoFolha.height) - 65;

        DrawText(txtPagina, paginaX, paginaY, 45, BLACK);
    }

    /* Renderização das figurinhas */
    int contadorPosicao = 0;

    for (int i = indiceInicial; i < indiceFinal; i++) {
        int colunaAtual = contadorPosicao % colunas;
        int iLinha      = contadorPosicao / colunas;

        int posX = margemEsquerda + (colunaAtual * (larguraCard + espacamentoX));
        int posY = margemTopo     + (iLinha      * (alturaCard  + espacamentoY));
        Rectangle rectDestino = { (float)posX, (float)posY, (float)larguraCard, (float)alturaCard };

        char codigoLimpo[TAM_CODIGO];
        sscanf(filtradas[i]->codigo, "%s", codigoLimpo);

        if (filtradas[i]->colada == 1) {
            if (filtradas[i]->textura.id > 0) {
                DrawTexturePro(filtradas[i]->textura,
                               (Rectangle){ 0, 0, (float)filtradas[i]->textura.width,
                                                   (float)filtradas[i]->textura.height },
                               rectDestino, (Vector2){ 0, 0 }, 0.0f, WHITE);
            } else {
                DrawRectangleRec(rectDestino, LIGHTGRAY);
                DrawText("Imagem\nNao Encontrada", posX + 15, posY + 80, 16, DARKGRAY);
            }
        } else {
            if (al->texturaVerso.id > 0) {
                DrawTexturePro(al->texturaVerso,
                               (Rectangle){ 0, 0, (float)al->texturaVerso.width,
                                                   (float)al->texturaVerso.height },
                               rectDestino, (Vector2){ 0, 0 }, 0.0f, WHITE);
            } else {
                DrawRectangleRec(rectDestino, ColorAlpha(BLACK, 0.4f));
                DrawRectangleLinesEx(rectDestino, 2, GRAY);
            }

            int larguraTextoCodigo = MeasureText(codigoLimpo, 24);
            int paddingX = 14, paddingY = 8;
            Rectangle rectCaixaCodigo = {
                (float)(posX + (larguraCard / 2) - (larguraTextoCodigo / 2) - paddingX),
                (float)(posY + (alturaCard  / 2) - 12 - paddingY),
                (float)(larguraTextoCodigo + paddingX * 2),
                (float)(24 + paddingY * 2)
            };

            DrawRectangleRec(rectCaixaCodigo, ColorAlpha(BLACK, 0.45f));
            DrawRectangleLinesEx(rectCaixaCodigo, 2, LIGHTGRAY);
            DrawText(codigoLimpo, posX + (larguraCard / 2) - (larguraTextoCodigo / 2) + 2,
                     posY + (alturaCard / 2) - 10, 24, BLACK);
            DrawText(codigoLimpo, posX + (larguraCard / 2) - (larguraTextoCodigo / 2),
                     posY + (alturaCard / 2) - 12, 24, WHITE);
        }

        /* Moldura */
        int margemMoldura = 8;
        Rectangle rectMoldura = {
            (float)(posX - margemMoldura),
            (float)(posY - margemMoldura),
            (float)(larguraCard + margemMoldura * 2),
            (float)(alturaCard  + margemMoldura * 2)
        };
        DrawTexturePro(moldura,
                       (Rectangle){ 0, 0, (float)moldura.width, (float)moldura.height },
                       rectMoldura, (Vector2){ 0, 0 }, 0.0f, WHITE);

        /* Título do jogador */
        const char *tituloExibicao = ObterTituloAbreviado(filtradas[i]->titulo);
        int tamanhoTexto = MeasureText(tituloExibicao, 24);
        int textoX = posX + (larguraCard / 2) - (tamanhoTexto / 2);
        int textoY = posY + alturaCard + 15;

        DrawText(tituloExibicao, textoX,     textoY, 24, BLACK);
        DrawText(tituloExibicao, textoX + 1, textoY, 24, BLACK);

        /* Badge de repetidas */
        if (filtradas[i]->repetidas > 0) {
            char txtRepetidas[10];
            sprintf(txtRepetidas, "+%d", filtradas[i]->repetidas);
            int cx = posX + larguraCard - 5;
            int cy = posY + 2;
            int lw = MeasureText(txtRepetidas, 22);
            DrawCircle(cx, cy, 20, GOLD);
            DrawText(txtRepetidas, cx - lw / 2, cy - 11, 22, BLACK);
        }

        contadorPosicao++;
    }

    if (totalFiltradas == 0) {
        int avX = (GetScreenWidth()  - MeasureText("Nenhuma figurinha encontrada para esta selecao.", 22)) / 2;
        int avY = GetScreenHeight() / 2;
        DrawText("Nenhuma figurinha encontrada para esta selecao.", avX, avY, 22, RED);
    }
}

/* ================================================================
 *  TELA: PACOTINHO (revelação de 7 figurinhas)
 * ================================================================ */

void DesenharPacote(Album *al, int *indicesPacote, int cartaAtual,
                    bool cartaVirada, bool pacoteFinalizado,
                    int novasObtidas, Texture2D moldura) {

    /* Painel central semi-transparente */
    int painelW = 700, painelH = 760;
    int painelX = (GetScreenWidth()  - painelW) / 2;
    int painelY = (GetScreenHeight() - painelH) / 2;
    DrawRectangle(painelX, painelY, painelW, painelH, ColorAlpha(BLACK, 0.72f));
    DrawRectangleLinesEx((Rectangle){ (float)painelX, (float)painelY,
                                      (float)painelW,  (float)painelH }, 3, GOLD);

    /* ---- Tela de RESUMO (quando os 7 já foram processados) ---- */
    if (pacoteFinalizado) {
        const char *titulo = "PACOTE CONCLUIDO!";
        int tw = MeasureText(titulo, 42);
        DrawText(titulo, (GetScreenWidth() - tw) / 2, painelY + 60, 42, GOLD);

        char linha1[60];
        sprintf(linha1, "%d figurinha(s) NOVA(S) colada(s)!", novasObtidas);
        int l1w = MeasureText(linha1, 30);
        DrawText(linha1, (GetScreenWidth() - l1w) / 2, painelY + 160, 30,
                 novasObtidas > 0 ? GREEN : LIGHTGRAY);

        int repNoPacote = 7 - novasObtidas;
        char linha2[60];
        sprintf(linha2, "%d figurinha(s) ficaram nas repetidas.", repNoPacote);
        int l2w = MeasureText(linha2, 26);
        DrawText(linha2, (GetScreenWidth() - l2w) / 2, painelY + 210, 26, GRAY);

        /* Lista das 7 figurinhas do pacote */
        for (int i = 0; i < 7; i++) {
            Figurinha *f = &al->figurinhas[indicesPacote[i]];
            char item[80];
            sprintf(item, "[%s] %s", f->codigo, ObterTituloAbreviado(f->titulo));
            int itemW = MeasureText(item, 22);
            DrawText(item, (GetScreenWidth() - itemW) / 2,
                     painelY + 290 + i * 40, 22,
                     f->colada ? SKYBLUE : GRAY);
        }

        const char *instrucao = "Pressione ENTER para voltar";
        int iw = MeasureText(instrucao, 24);
        DrawText(instrucao, (GetScreenWidth() - iw) / 2, painelY + painelH - 60, 24, WHITE);
        return;
    }

    /* ---- Tela de REVELAÇÃO da carta atual ---- */

    /* Título e contador */
    const char *tituloPacote = "ABRINDO PACOTINHO";
    int tpw = MeasureText(tituloPacote, 36);
    DrawText(tituloPacote, (GetScreenWidth() - tpw) / 2, painelY + 30, 36, GOLD);

    char contador[20];
    sprintf(contador, "Figurinha  %d / 7", cartaAtual + 1);
    int cw = MeasureText(contador, 26);
    DrawText(contador, (GetScreenWidth() - cw) / 2, painelY + 78, 26, LIGHTGRAY);

    /* Área da carta */
    int cardW = 260, cardH = 360;
    int cardX = (GetScreenWidth()  - cardW) / 2;
    int cardY = painelY + 130;
    Rectangle rectCard = { (float)cardX, (float)cardY, (float)cardW, (float)cardH };

    Figurinha *f = &al->figurinhas[indicesPacote[cartaAtual]];

    if (!cartaVirada) {
        /* ---- VERSO ---- */
        if (al->texturaVerso.id > 0) {
            DrawTexturePro(al->texturaVerso,
                           (Rectangle){ 0, 0, (float)al->texturaVerso.width,
                                               (float)al->texturaVerso.height },
                           rectCard, (Vector2){ 0, 0 }, 0.0f, WHITE);
        } else {
            DrawRectangleRec(rectCard, DARKBLUE);
            DrawRectangleLinesEx(rectCard, 3, BLUE);
            int qw = MeasureText("?", 80);
            DrawText("?", cardX + cardW / 2 - qw / 2, cardY + cardH / 2 - 45, 80, WHITE);
        }

        /* Moldura sobre o verso */
        int mm = 8;
        DrawTexturePro(moldura,
                       (Rectangle){ 0, 0, (float)moldura.width, (float)moldura.height },
                       (Rectangle){ (float)(cardX - mm), (float)(cardY - mm),
                                    (float)(cardW + mm * 2), (float)(cardH + mm * 2) },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);

        const char *dica = "[ENTER] ou [ESPACO] para revelar";
        int dw = MeasureText(dica, 22);
        DrawText(dica, (GetScreenWidth() - dw) / 2, cardY + cardH + 30, 22, YELLOW);

    } else {
        /* ---- FRENTE ---- */
        if (f->textura.id > 0) {
            DrawTexturePro(f->textura,
                           (Rectangle){ 0, 0, (float)f->textura.width,
                                               (float)f->textura.height },
                           rectCard, (Vector2){ 0, 0 }, 0.0f, WHITE);
        } else {
            DrawRectangleRec(rectCard, DARKGREEN);
            DrawRectangleLinesEx(rectCard, 3, GREEN);
            int tw = MeasureText(f->codigo, 32);
            DrawText(f->codigo, cardX + cardW / 2 - tw / 2, cardY + cardH / 2 - 20, 32, WHITE);
        }

        /* Moldura sobre a frente */
        int mm = 8;
        DrawTexturePro(moldura,
                       (Rectangle){ 0, 0, (float)moldura.width, (float)moldura.height },
                       (Rectangle){ (float)(cardX - mm), (float)(cardY - mm),
                                    (float)(cardW + mm * 2), (float)(cardH + mm * 2) },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);

        /* Nome do jogador */
        const char *nomeExib = ObterTituloAbreviado(f->titulo);
        int nw = MeasureText(nomeExib, 26);
        DrawText(nomeExib, (GetScreenWidth() - nw) / 2, cardY + cardH + 18, 26, WHITE);

        /* Tipo e grupo */
        char infoTipo[60];
        sprintf(infoTipo, "%s  |  Grupo %s", f->tipo, f->grupo[0] ? f->grupo : "FWC");
        int iw = MeasureText(infoTipo, 20);
        DrawText(infoTipo, (GetScreenWidth() - iw) / 2, cardY + cardH + 52, 20, LIGHTGRAY);

        /* Aviso se já tinha */
        if (f->colada) {
            const char *aviso = "Voce ja tem essa! Ira para as repetidas.";
            int aw = MeasureText(aviso, 20);
            DrawRectangle((GetScreenWidth() - aw) / 2 - 12, cardY + cardH + 82, aw + 24, 30,
                          ColorAlpha(ORANGE, 0.85f));
            DrawText(aviso, (GetScreenWidth() - aw) / 2, cardY + cardH + 86, 20, BLACK);
        } else {
            const char *aviso = "Figurinha NOVA!";
            int aw = MeasureText(aviso, 22);
            DrawRectangle((GetScreenWidth() - aw) / 2 - 12, cardY + cardH + 82, aw + 24, 30,
                          ColorAlpha(GREEN, 0.85f));
            DrawText(aviso, (GetScreenWidth() - aw) / 2, cardY + cardH + 85, 22, BLACK);
        }

        /* Instruções de ação */
        const char *acoes = "[S] Colar no album     [N] Guardar para troca";
        int aaw = MeasureText(acoes, 22);
        DrawText(acoes, (GetScreenWidth() - aaw) / 2, painelY + painelH - 55, 22, YELLOW);
    }
}

/* ================================================================
 *  TELA: ESTATÍSTICAS DE PREENCHIMENTO
 * ================================================================ */

void DesenharEstatisticas(const Album *al) {
    /* Nomes e letras de cada grupo para filtragem */
    const char *nomesGrupos[13] = {
        "ESPECIAL FWC", "GRUPO A", "GRUPO B", "GRUPO C", "GRUPO D",
        "GRUPO E",      "GRUPO F", "GRUPO G", "GRUPO H", "GRUPO I",
        "GRUPO J",      "GRUPO K", "GRUPO L"
    };
    const char *letrasGrupos[13] = {
        "FWC", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"
    };

    /* Contagem geral e por grupo */
    int totalColadas   = 0;
    int totalRepetidas = 0;
    int statTotal  [13]; /* total de figurinhas no grupo */
    int statColadas[13]; /* quantas já coladas           */

    for (int g = 0; g < 13; g++) {
        statTotal[g]   = 0;
        statColadas[g] = 0;
    }

    for (int i = 0; i < al->total; i++) {
        if (al->figurinhas[i].colada)   totalColadas++;
        totalRepetidas += al->figurinhas[i].repetidas;

        for (int g = 0; g < 13; g++) {
            int pertence = 0;
            if (g == 0) {
                /* Especial FWC: stickers cuja secao é "FIFA World Cup 2026"
                   OU cujo grupo não é uma letra simples */
                pertence = (strcmp(al->figurinhas[i].secao, "FIFA World Cup 2026") == 0);
            } else {
                pertence = (strcmp(al->figurinhas[i].grupo, letrasGrupos[g]) == 0);
            }

            if (pertence) {
                statTotal[g]++;
                if (al->figurinhas[i].colada) statColadas[g]++;
                break;
            }
        }
    }

    /* ---- Layout ---- */
    int painelW = 1360, painelH = 950;
    int painelX = (GetScreenWidth()  - painelW) / 2;
    int painelY = (GetScreenHeight() - painelH) / 2;

    DrawRectangle(painelX, painelY, painelW, painelH, ColorAlpha(BLACK, 0.80f));
    DrawRectangleLinesEx((Rectangle){ (float)painelX, (float)painelY,
                                      (float)painelW,  (float)painelH }, 3, GOLD);

    /* Título */
    const char *titulo = "ESTATISTICAS DO ALBUM FIFA 2026";
    int tw = MeasureText(titulo, 38);
    DrawText(titulo, (GetScreenWidth() - tw) / 2, painelY + 28, 38, GOLD);

    /* Barra de progresso geral */
    float pctGeral = al->total > 0 ? (totalColadas * 100.0f) / al->total : 0.0f;
    char txtGeral[60];
    sprintf(txtGeral, "Total Geral:  %d / %d figurinhas  (%.1f%%)", totalColadas, al->total, pctGeral);
    int tgw = MeasureText(txtGeral, 26);
    DrawText(txtGeral, (GetScreenWidth() - tgw) / 2, painelY + 88, 26, WHITE);

    int barGeralX = painelX + 60;
    int barGeralY = painelY + 128;
    int barGeralW = painelW - 120;
    int barGeralH = 32;
    DrawRectangle(barGeralX, barGeralY, barGeralW, barGeralH, DARKGRAY);
    DrawRectangle(barGeralX, barGeralY, (int)(barGeralW * pctGeral / 100.0f), barGeralH,
                  pctGeral >= 80 ? GREEN : pctGeral >= 40 ? YELLOW : ORANGE);
    DrawRectangleLinesEx((Rectangle){ (float)barGeralX, (float)barGeralY,
                                      (float)barGeralW,  (float)barGeralH }, 2, LIGHTGRAY);

    char txtRepetidas[40];
    sprintf(txtRepetidas, "Figurinhas para troca: %d", totalRepetidas);
    int trw = MeasureText(txtRepetidas, 22);
    DrawText(txtRepetidas, (GetScreenWidth() - trw) / 2, painelY + 172, 22, SKYBLUE);

    /* ---- Grade de grupos: 2 colunas ---- */
    /* Coluna 1: grupos  0–6  (7 grupos) */
    /* Coluna 2: grupos  7–12 (6 grupos) */
    int col1X = painelX + 50;
    int col2X = painelX + painelW / 2 + 20;
    int inicioY = painelY + 215;
    int alturaLinha = 70;
    int barW = 580;
    int barH = 20;

    for (int g = 0; g < 13; g++) {
        int colX = (g <= 6) ? col1X : col2X;
        int rowIdx = (g <= 6) ? g : (g - 7);
        int posY = inicioY + rowIdx * alturaLinha;

        float pct = statTotal[g] > 0 ? (statColadas[g] * 100.0f) / statTotal[g] : 0.0f;
        Color corBarra = pct >= 80 ? GREEN : pct >= 40 ? YELLOW : ORANGE;

        /* Nome do grupo */
        DrawText(nomesGrupos[g], colX, posY, 22, WHITE);

        /* Fração X/Y */
        char frac[20];
        sprintf(frac, "%d/%d", statColadas[g], statTotal[g]);
        int fw = MeasureText(frac, 20);
        DrawText(frac, colX + barW - fw, posY, 20, LIGHTGRAY);

        /* Barra */
        DrawRectangle(colX, posY + 28, barW, barH, DARKGRAY);
        DrawRectangle(colX, posY + 28, (int)(barW * pct / 100.0f), barH, corBarra);
        DrawRectangleLinesEx((Rectangle){ (float)colX, (float)(posY + 28),
                                          (float)barW,   (float)barH }, 1, GRAY);

        /* Porcentagem */
        char pctTxt[10];
        sprintf(pctTxt, "%.0f%%", pct);
        DrawText(pctTxt, colX + barW + 8, posY + 28, 18, corBarra);
    }

    /* Instrução de voltar */
    const char *instrucao = "[ESC] ou [BACKSPACE] para voltar";
    int iw = MeasureText(instrucao, 22);
    DrawText(instrucao, (GetScreenWidth() - iw) / 2, painelY + painelH - 48, 22, GRAY);
}

/* ================================================================
 *  TELA: TROCA COM JOGADOR VIRTUAL (extra 2)
 * ================================================================ */

void DesenharTroca(const Album *al, int idxRepetida, int idxFaltante, bool ofertaValida) {
    int painelW = 1000, painelH = 700;
    int painelX = (GetScreenWidth()  - painelW) / 2;
    int painelY = (GetScreenHeight() - painelH) / 2;

    DrawRectangle(painelX, painelY, painelW, painelH, ColorAlpha(BLACK, 0.78f));
    DrawRectangleLinesEx((Rectangle){ (float)painelX, (float)painelY,
                                      (float)painelW,  (float)painelH }, 3, GOLD);

    const char *titulo = "TROCA COM JOGADOR VIRTUAL";
    int tw = MeasureText(titulo, 36);
    DrawText(titulo, (GetScreenWidth() - tw) / 2, painelY + 30, 36, GOLD);

    if (!ofertaValida) {
        const char *msg1 = "Voce nao tem figurinhas repetidas disponiveis no momento.";
        int m1w = MeasureText(msg1, 24);
        DrawText(msg1, (GetScreenWidth() - m1w) / 2, painelY + 300, 24, ORANGE);

        const char *msg2 = "Abra mais pacotinhos para conseguir repetidas e tente novamente!";
        int m2w = MeasureText(msg2, 20);
        DrawText(msg2, (GetScreenWidth() - m2w) / 2, painelY + 340, 20, LIGHTGRAY);
        return;
    }

    const Figurinha *oferta = &al->figurinhas[idxRepetida];
    const Figurinha *recebe = &al->figurinhas[idxFaltante];

    int cardW = 260, cardH = 340;
    int cardY = painelY + 130;
    int cardEsqX = painelX + 80;
    int cardDirX = painelX + painelW - 80 - cardW;

    Rectangle rectEsq = { (float)cardEsqX, (float)cardY, (float)cardW, (float)cardH };
    Rectangle rectDir = { (float)cardDirX, (float)cardY, (float)cardW, (float)cardH };

    /* ---- Card da esquerda: o que o jogador oferece ---- */
    DrawText("VOCE OFERECE", cardEsqX, cardY - 35, 22, ORANGE);
    if (oferta->textura.id > 0) {
        DrawTexturePro(oferta->textura,
                       (Rectangle){ 0, 0, (float)oferta->textura.width, (float)oferta->textura.height },
                       rectEsq, (Vector2){ 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(rectEsq, DARKGRAY);
        int tw2 = MeasureText(oferta->codigo, 26);
        DrawText(oferta->codigo, cardEsqX + cardW / 2 - tw2 / 2, cardY + cardH / 2 - 13, 26, WHITE);
    }
    DrawRectangleLinesEx(rectEsq, 3, ORANGE);

    const char *nomeOferta = ObterTituloAbreviado(oferta->titulo);
    int now = MeasureText(nomeOferta, 20);
    DrawText(nomeOferta, cardEsqX + cardW / 2 - now / 2, cardY + cardH + 12, 20, WHITE);

    char txtRep[40];
    sprintf(txtRep, "(Voce tem %d repetidas)", oferta->repetidas);
    int trw = MeasureText(txtRep, 18);
    DrawText(txtRep, cardEsqX + cardW / 2 - trw / 2, cardY + cardH + 38, 18, GRAY);

    /* ---- Card da direita: o que o jogador recebe ---- */
    DrawText("VOCE RECEBE", cardDirX, cardY - 35, 22, GREEN);
    if (recebe->textura.id > 0) {
        DrawTexturePro(recebe->textura,
                       (Rectangle){ 0, 0, (float)recebe->textura.width, (float)recebe->textura.height },
                       rectDir, (Vector2){ 0, 0 }, 0.0f, WHITE);
    } else {
        DrawRectangleRec(rectDir, DARKGRAY);
        int tw3 = MeasureText(recebe->codigo, 26);
        DrawText(recebe->codigo, cardDirX + cardW / 2 - tw3 / 2, cardY + cardH / 2 - 13, 26, WHITE);
    }
    DrawRectangleLinesEx(rectDir, 3, GREEN);

    const char *nomeRecebe = ObterTituloAbreviado(recebe->titulo);
    int nrw = MeasureText(nomeRecebe, 20);
    DrawText(nomeRecebe, cardDirX + cardW / 2 - nrw / 2, cardY + cardH + 12, 20, WHITE);

    /* Seta central */
    const char *seta = "<=>";
    int sw = MeasureText(seta, 50);
    DrawText(seta, (GetScreenWidth() - sw) / 2, cardY + cardH / 2 - 25, 50, GOLD);

    const char *instrucao = "[ESC] para voltar";
    int iw2 = MeasureText(instrucao, 20);
    DrawText(instrucao, (GetScreenWidth() - iw2) / 2, painelY + painelH - 45, 20, GRAY);
}

/* ================================================================
 *  TELA: MINI-GAME ROLETA DA SORTE (extra 3)
 * ================================================================ */

void DesenharRoleta(Color cores[3], bool girando, bool venceu, bool jaGirou) {
    int painelW = 900, painelH = 600;
    int painelX = (GetScreenWidth()  - painelW) / 2;
    int painelY = (GetScreenHeight() - painelH) / 2;

    DrawRectangle(painelX, painelY, painelW, painelH, ColorAlpha(BLACK, 0.78f));
    DrawRectangleLinesEx((Rectangle){ (float)painelX, (float)painelY,
                                      (float)painelW,  (float)painelH }, 3, GOLD);

    const char *titulo = "ROLETA DA SORTE";
    int tw = MeasureText(titulo, 38);
    DrawText(titulo, (GetScreenWidth() - tw) / 2, painelY + 40, 38, GOLD);

    const char *dica = "Acerte as 3 cores e ganhe um pacotinho extra!";
    int dw = MeasureText(dica, 22);
    DrawText(dica, (GetScreenWidth() - dw) / 2, painelY + 95, 22, LIGHTGRAY);

    int espacamento  = 220;
    int centroY      = painelY + 300;
    int raio         = 80;
    int centroXBase  = (GetScreenWidth() / 2) - espacamento;

    for (int i = 0; i < 3; i++) {
        int cx = centroXBase + (i * espacamento);
        DrawCircle(cx, centroY, raio + 8, BLACK);
        DrawCircle(cx, centroY, raio, cores[i]);
        DrawCircleLines(cx, centroY, raio, WHITE);
    }

    if (girando) {
        const char *txt = "Girando...";
        int txw = MeasureText(txt, 28);
        DrawText(txt, (GetScreenWidth() - txw) / 2, centroY + 120, 28, WHITE);
    } else if (jaGirou) {
        const char *txt = venceu ? "PARABENS! Voce ganhou um pacotinho extra!"
                                  : "Nao foi dessa vez... tente novamente!";
        Color cor = venceu ? GOLD : MAROON;
        int txw = MeasureText(txt, 26);
        DrawText(txt, (GetScreenWidth() - txw) / 2, centroY + 120, 26, cor);
    }

    const char *instrucao = "[ESC] para voltar";
    int iw = MeasureText(instrucao, 20);
    DrawText(instrucao, (GetScreenWidth() - iw) / 2, painelY + painelH - 40, 20, GRAY);
}

/* ================================================================
 *  ENCERRAMENTO
 * ================================================================ */

void encerrar_jogo(Album *album, Texture2D frames[],
                   Texture2D iconeOn, Texture2D iconeOff, Music musica) {
    /* Descarrega os frames do fundo animado */
    for (int i = 0; i < 30; i++) {
        UnloadTexture(frames[i]);
    }

    UnloadTexture(iconeOn);
    UnloadTexture(iconeOff);

    /* Descarrega as texturas das figurinhas e o verso (a memoria RAM dos
       DADOS do album NAO e liberada aqui: o programa volta para o menu
       de terminal e o usuario pode continuar cadastrando/consultando.
       A liberacao final (liberar_album) so acontece quando o usuario
       escolhe "Salvar e Sair" no menu principal do main()). */
    descarregar_texturas_album(album);

    UnloadMusicStream(musica);
    CloseAudioDevice();
    CloseWindow();
}