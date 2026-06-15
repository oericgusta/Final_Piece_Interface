#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "interface.h"
#include "album.h"

void desenhar_background(Texture2D *frames, int frameAtual) {
    DrawTexture(frames[frameAtual], 0, 0, WHITE);
}

void desenhar_listagem(Album *a) {
    DrawText("TELA DE LISTAGEM", 100, 100, 30, DARKGRAY);
}

void carregar_background_frames(Texture2D frames[]) {
    char path[100];
    for(int i = 0; i < 30; i++) {
        sprintf(path, "assets/background/frame%02d.png", i);
        frames[i] = LoadTexture(path);
    }
}

void desenhar_botao_som(bool somAtivo, Rectangle btnSom, Texture2D iconeOn, Texture2D iconeOff) {
    if (somAtivo) {
        DrawTexturePro(iconeOn, (Rectangle){ 0, 0, iconeOn.width, iconeOn.height }, btnSom, (Vector2){ 0, 0 }, 0.0f, WHITE);
    } else {
        DrawTexturePro(iconeOff, (Rectangle){ 0, 0, iconeOff.width, iconeOff.height }, btnSom, (Vector2){ 0, 0 }, 0.0f, WHITE);
    }
}

void encerrar_jogo(Album *album, Texture2D frames[], Texture2D iconeOn, Texture2D iconeOff, Music musica) {
    // Descarrega os frames do fundo animado
    for(int i = 0; i < 30; i++){
        UnloadTexture(frames[i]);
    } 

    UnloadTexture(iconeOn);
    UnloadTexture(iconeOff);

    // === DESCARREGA TODAS AS TEXTURAS DAS FIGURINHAS ===
    UnloadTexture(album->texturaVerso); // Limpa o verso
    for (int i = 0; i < album->total; i++) {
        if (album->figurinhas[i].textura.id > 0) {
            UnloadTexture(album->figurinhas[i].textura);
        }
    }

    salvar_csv(album, "assets/figurinhas2026.csv");
    liberar_album(album); 

    UnloadMusicStream(musica);   
    CloseAudioDevice();
    CloseWindow();  
}

/* Função para converter nomes longos em abreviações personalizadas */
const char* ObterNomeAbreviado(const char *nomeOriginal) {
    if (strcmp(nomeOriginal, "FIFA World Cup 2026") == 0) return "FWC 2026";
    if (strcmp(nomeOriginal, "Mexico") == 0) return "México";
    if (strcmp(nomeOriginal, "Africa do Sul") == 0) return "Africa do S.";
    if (strcmp(nomeOriginal, "Coreia do Sul") == 0) return "Coreia do S.";
    if (strcmp(nomeOriginal, "Republica Tcheca") == 0) return "Rep. Tcheca";
    if (strcmp(nomeOriginal, "Canada") == 0) return "Canadá";
    if (strcmp(nomeOriginal, "Bosnia") == 0) return "Bósnia";
    if (strcmp(nomeOriginal, "Suica") == 0) return "Suiça";
    if (strcmp(nomeOriginal, "Escocia") == 0) return "Escócia";
    if (strcmp(nomeOriginal, "Estados Unidos") == 0) return "EUA";
    if (strcmp(nomeOriginal, "Australia") == 0) return "Austrália";
    if (strcmp(nomeOriginal, "Curacao") == 0) return "Curaçao";
    if (strcmp(nomeOriginal, "Costa do Marfim") == 0) return "Costa do Mar.";
    if (strcmp(nomeOriginal, "Japao") == 0) return "Japão";
    if (strcmp(nomeOriginal, "Suecia") == 0) return "Suécia";
    if (strcmp(nomeOriginal, "Tunisia") == 0) return "Tunísia";
    if (strcmp(nomeOriginal, "Belgica") == 0) return "Bélgica";
    if (strcmp(nomeOriginal, "Ira") == 0) return "Irã";
    if (strcmp(nomeOriginal, "Nova Zelandia") == 0) return "N. Zelândia";
    if (strcmp(nomeOriginal, "Arabia Saudita") == 0) return "Aráb. Saudita";
    if (strcmp(nomeOriginal, "Franca") == 0) return "França";
    if (strcmp(nomeOriginal, "Argelia") == 0) return "Argélia";
    if (strcmp(nomeOriginal, "Jordania") == 0) return "Jordânia";
    if (strcmp(nomeOriginal, "Uzbequistao") == 0) return "Uzbequistão";
    if (strcmp(nomeOriginal, "Colombia") == 0) return "Colômbia";
    if (strcmp(nomeOriginal, "Croacia") == 0) return "Croácia";
    if (strcmp(nomeOriginal, "Panama") == 0) return "Panamá";

    /* Se não precisar de abreviação, retorna o nome original */
    return nomeOriginal;
}

/* Função para encurtar nomes muito longos de figurinhas específicas */
const char* ObterTituloAbreviado(const char *tituloOriginal) {

    /* Abreviações para os Escudos e nomes*/
    if (strcmp(tituloOriginal, "Escudo dos EUA (Pais-Sede)") == 0) return "Escudo (EUA)";
    if (strcmp(tituloOriginal, "Escudo do Mexico (Pais-Sede)") == 0) return "Escudo (MEX)";
    if (strcmp(tituloOriginal, "Escudo do Canada (Pais-Sede)") == 0) return "Escudo (CAN)";
    if (strcmp(tituloOriginal, "Logotipo da Panini") == 0) return "Logotipo Panini";
    if (strcmp(tituloOriginal, "Emblema Oficial (Parte 1)") == 0) return "Emblema Oficial (Pt 1)";
    if (strcmp(tituloOriginal, "Emblema Oficial (Parte 2)") == 0) return "Emblema Oficial (Pt 2)";
    if (strcmp(tituloOriginal, "Henry Heroki Mochizuki") == 0) return "Henry H. Mochizuki";
    if (strcmp(tituloOriginal, "Victor Nilsson Lindelof") == 0) return "Victor N. Lindelof";
    if (strcmp(tituloOriginal, "Mohamed Ali Ben Romdhane") == 0) return "Mohamed A. Romdhane";
    if (strcmp(tituloOriginal, "Kristoffer Vassbakk Ajer") == 0) return "Kristoffer V. Ajer";
    if (strcmp(tituloOriginal, "Marcus Holmgren Pedersen") == 0) return "Marcus H. Pedersen";
    if (strcmp(tituloOriginal, "Mohamed Amine Tougai") == 0) return "Mohamed A. Tougai";
    if (strcmp(tituloOriginal, "Mohammad Abu Hashish") == 0) return "Mohammad A. Hashish";
    if (strcmp(tituloOriginal, "Mohammad Abu Zrayq") == 0) return " Mohammad A. Zrayq";
    if (strcmp(tituloOriginal, "Juan Fernando Quintero") == 0) return "Juan F. Quintero";
    if (strcmp(tituloOriginal, "Abdul Issahaku Fatawu") == 0) return "Abdul I. Fatawu";
    if (strcmp(tituloOriginal, "Trent Alexander-Arnold") == 0) return "Trent A-Arnold";

    /* Se o nome do jogador ou figurinha já for bom, retorna o original */
    return tituloOriginal;
}

void DesenharAlbumPorSelecao(Album *al, const char *secaoAtiva, int paginaAtiva, Texture2D moldura, Texture2D fundoPagina) {
    /* Configurações da grade de figurinhas (dimensões e espaçamentos) */
    int colunas = 4;
    int linhas = 2;
    int larguraCard = 210;
    int alturaCard = 280;
    int espacamentoX = 85;
    int espacamentoY = 90;

    /* Cálculo das dimensões totais ocupadas pelo bloco de figurinhas */
    int larguraTotalBloco = (colunas * larguraCard) + ((colunas - 1) * espacamentoX);
    int alturaTotalBloco = (linhas * alturaCard) + ((linhas - 1) * espacamentoY);

    /* Definição das margens para centralizar o bloco de figurinhas na tela */
    int margemEsquerda = (GetScreenWidth() - larguraTotalBloco) / 2;
    int margemTopo = ((GetScreenHeight() - alturaTotalBloco) / 2) + 40;

    /* Quantidade máxima de cartas exibidas por página */
    int cardsPorPagina = colunas * linhas;

    /* Vetor temporário para armazenar apenas as figurinhas da seleção atual */
    Figurinha *filtradas[50]; 
    int totalFiltradas = 0;
    int coladasNaSelecao = 0;

    /* Filtragem das figurinhas com base na seção ativa e contagem de coladas reais */
    for (int i = 0; i < al->total; i++) {
        if (strcmp(al->figurinhas[i].secao, secaoAtiva) == 0) {
            filtradas[totalFiltradas] = &al->figurinhas[i];
            
            /* Incrementa o contador apenas se a figurinha estiver realmente colada */
            if (al->figurinhas[i].colada == 1) {
                coladasNaSelecao++;
            }
            
            totalFiltradas++;
        }
    }

    /* Determinação dos índices inicial e final para o controle da paginação */
    int indiceInicial = paginaAtiva * cardsPorPagina;
    int indiceFinal = indiceInicial + cardsPorPagina;

    /* Ajuste do índice final para não ultrapassar o total de figurinhas filtradas */
    if (indiceFinal > totalFiltradas) {
        indiceFinal = totalFiltradas;
    }

    /* Renderização dos elements de fundo caso existam figurinhas filtradas */
    if (totalFiltradas > 0) {
        /* Definição das folgas laterais e verticais para o dimensionamento do fundo */
        int folgaHorizontal = 80; 
        int folgaVertical = 170; 

        /* Coordenadas centrais do bloco de figurinhas */
        int centroBlocoX = margemEsquerda + (larguraTotalBloco / 2);
        int centroBlocoY = margemTopo + (alturaTotalBloco / 2);

        /* Cálculo da nova largura e altura para o fundo da página */
        int novaLarguraFolha = larguraTotalBloco + (folgaHorizontal * 2);
        int novaAlturaFolha = alturaTotalBloco + (folgaVertical * 2);

        /* Definição do retângulo de destino para o fundo da página com deslocamento vertical */
        Rectangle rectDestinoFolha = { 
            centroBlocoX - (novaLarguraFolha / 2), 
            centroBlocoY - (novaAlturaFolha / 2) - 45, 
            novaLarguraFolha, 
            novaAlturaFolha 
        };

        /* Coordenadas para o posicionamento do nome da seleção na faixa clara superior esquerda */
        int textoSelecaoX = rectDestinoFolha.x + 75;
        int textoSelecaoY = rectDestinoFolha.y + 93;

        /* Desenho da textura de fundo dimensionada e centralizada */
        DrawTexturePro(
            fundoPagina,
            (Rectangle){ 0, 0, fundoPagina.width, fundoPagina.height },
            rectDestinoFolha,
            (Vector2){ 0, 0 }, 0.0f, WHITE
        );

        /* Obtém o nome tratado (abreviado se necessário) para exibição */
        const char *nomeExibicao = ObterNomeAbreviado(secaoAtiva);

        /* Exibição do nome da seleção na parte superior esquerda com efeito de negrito */
        DrawText(nomeExibicao, textoSelecaoX, textoSelecaoY, 40, BLACK);
        DrawText(nomeExibicao, textoSelecaoX + 1, textoSelecaoY, 40, BLACK);

        /* Formatação do texto do contador de progresso real (ex: "8/20 coladas") */
        char txtProgresso[30];
        sprintf(txtProgresso, "%d/%d coladas", coladasNaSelecao, totalFiltradas);

        /* Cálculo do tamanho do texto para alinhar à direita na faixa clara superior direita */
        int larguraTxtProgresso = MeasureText(txtProgresso, 32);
        int progressoX = rectDestinoFolha.x + rectDestinoFolha.width - 75 - larguraTxtProgresso;
        int progressoY = rectDestinoFolha.y + 97; 

        /* Exibição do progresso de coladas com efeito de negrito */
        DrawText(txtProgresso, progressoX, progressoY, 32, BLACK);
        DrawText(txtProgresso, progressoX + 1, progressoY, 32, BLACK);

        /* Formatação e cálculo para centralizar o número da página na aba inferior */
        char txtPagina[10];
        sprintf(txtPagina, "%d", paginaAtiva + 1);
        int larguraTxtPagina = MeasureText(txtPagina, 24);
        int paginaX = rectDestinoFolha.x + (rectDestinoFolha.width / 2) - (larguraTxtPagina / 2);
        int paginaY = rectDestinoFolha.y + rectDestinoFolha.height - 45;

        /* Exibição do número da página ativa */
        DrawText(txtPagina, paginaX, paginaY, 24, DARKGRAY);
    }

    /* Contador para gerenciar a posição de cada carta na grade (colunas e linhas) */
    int contadorPosicao = 0;

    /* Laço para renderizar as figurinhas da página atual */
    for (int i = indiceInicial; i < indiceFinal; i++) {
        /* Cálculo da coluna e linha atuais com base no contador de posição */
        int colunaAtual = contadorPosicao % colunas;
        int iLinha = contadorPosicao / colunas;

        /* Definição das coordenadas X e Y para a figurinha atual */
        int posX = margemEsquerda + (colunaAtual * (larguraCard + espacamentoX));
        int posY = margemTopo + (iLinha * (alturaCard + espacamentoY));
        Rectangle rectDestino = { posX, posY, larguraCard, alturaCard };

        /* Limpeza de resíduos de formatação no código da figurinha */
        char codigoLimpo[15];
        sscanf(filtradas[i]->codigo, "%s", codigoLimpo);

        /* Fluxo de renderização baseado estritamente no estado real da figurinha */
        if (filtradas[i]->colada == 1) {
            /* Se a figurinha possui textura válida carregada, desenha a foto */
            if (filtradas[i]->textura.id > 0) {
                DrawTexturePro(
                    filtradas[i]->textura,
                    (Rectangle){ 0, 0, filtradas[i]->textura.width, filtradas[i]->textura.height },
                    rectDestino,
                    (Vector2){ 0, 0 }, 0.0f, WHITE
                );
            } 
            /* Fallback caso a marcação esteja como colada mas a textura falhe */
            else {
                DrawRectangleRec(rectDestino, LIGHTGRAY);
                DrawText("Imagem\nNão Encontrada", posX + 15, posY + 80, 16, DARKGRAY);
            }
        } 
        /* Fluxo de renderização normal para figurinhas não coladas (exibe o verso) */
        else {
            /* Desenha o fundo padrão do verso (textura ou bloco cinza/escuro) */
            if (al->texturaVerso.id > 0) {
                DrawTexturePro(
                    al->texturaVerso,
                    (Rectangle){ 0, 0, al->texturaVerso.width, al->texturaVerso.height },
                    rectDestino,
                    (Vector2){ 0, 0 }, 0.0f, WHITE
                );
            } else {
                DrawRectangleRec(rectDestino, ColorAlpha(BLACK, 0.4f));
                DrawRectangleLinesEx(rectDestino, 2, GRAY);
            }

            /* RETÂNGULO EM VOLTA DO CÓDIGO */
            int larguraTextoCodigo = MeasureText(codigoLimpo, 24);
            int alturaTextoCodigo = 24;

            /* Configuração do tamanho da caixinha interna (padding = folga interna) */
            int paddingX = 14;
            int paddingY = 8;
            Rectangle rectCaixaCodigo = {
                posX + (larguraCard / 2) - (larguraTextoCodigo / 2) - paddingX,
                posY + (alturaCard / 2) - (alturaTextoCodigo / 2) - paddingY,
                larguraTextoCodigo + (paddingX * 2),
                alturaTextoCodigo + (paddingY * 2)
            };

            /* Desenha o retângulo de fundo menor (preto semi-transparente para dar contraste) */
            DrawRectangleRec(rectCaixaCodigo, ColorAlpha(BLACK, 0.45f));
            
            /* Desenha uma borda fina ao redor do retângulo menor */
            DrawRectangleLinesEx(rectCaixaCodigo, 2, LIGHTGRAY);

            /* Desenha o texto do código por cima, perfeitamente centralizado */
            DrawText(codigoLimpo, posX + (larguraCard / 2) - (larguraTextoCodigo / 2) + 2, posY + (alturaCard / 2) - 10, 24, BLACK);
            DrawText(codigoLimpo, posX + (larguraCard / 2) - (larguraTextoCodigo / 2), posY + (alturaCard / 2) - 12, 24, WHITE);
        }

        /* Configuração do retângulo da moldura para cobrir ligeiramente as bordas externas */
        int margemMoldura = 8; 
        Rectangle rectMoldura = {
            posX - margemMoldura,                          
            posY - margemMoldura,                          
            larguraCard + (margemMoldura * 2),             
            alturaCard + (margemMoldura * 2)              
        };

        /* Desenho da moldura sobre a figurinha */
        DrawTexturePro(
            moldura,
            (Rectangle){ 0, 0, moldura.width, moldura.height },
            rectMoldura, 
            (Vector2){ 0, 0 }, 0.0f, WHITE
        );

        /* Cálculo e exibição do nome do jogador com efeito de negrito */
        /* Obtém o título tratado/curto para a figurinha */
        const char *tituloExibicao = ObterTituloAbreviado(filtradas[i]->titulo);

        /* Cálculo e exibição do título com efeito de negrito usando a string tratada */
        int tamanhoTexto = MeasureText(tituloExibicao, 24); 
        int textoX = posX + (larguraCard / 2) - (tamanhoTexto / 2);
        int textoY = posY + alturaCard + 15; 
        
        DrawText(tituloExibicao, textoX, textoY, 24, BLACK);
        DrawText(tituloExibicao, textoX + 1, textoY, 24, BLACK);

        /* Renderização do indicador de repetidas se a quantidade for maior que zero */
        if (filtradas[i]->repetidas > 0) {
            char txtRepetidas[10];
            sprintf(txtRepetidas, "+%d", filtradas[i]->repetidas);

            /* Definição do centro do círculo no canto superior direito */
            int centroCirculoX = posX + larguraCard - 5;
            int centroCirculoY = posY + 2;

            /* Alinhamento do texto indicador dentro do círculo dourado */
            int larguraTextoRepetidas = MeasureText(txtRepetidas, 22);
            int textoRepetidasX = centroCirculoX - (larguraTextoRepetidas / 2);
            int textoRepetidasY = centroCirculoY - (22 / 2); 

            /* Desenho do círculo e do texto da quantidade de repetidas */
            DrawCircle(centroCirculoX, centroCirculoY, 20, GOLD);
            DrawText(txtRepetidas, textoRepetidasX, textoRepetidasY, 22, BLACK);
        }

        /* Incremento para mover o posicionamento para o próximo slot da grade */
        contadorPosicao++;
    }

    /* Mensagem de aviso caso nenhuma figurinha corresponda à seção selecionada */
    if (totalFiltradas == 0) {
        DrawText("Nenhuma figurinha encontrada para esta seleção.", margemEsquerda, margemTopo, 20, RED);
    }
}