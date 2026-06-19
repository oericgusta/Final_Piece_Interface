#ifndef INTERFACE_H
#define INTERFACE_H

#include "raylib.h"
#include "album.h"

    /**
     * @brief Estados possíveis da tela principal do aplicativo.
     *
     * O fluxo de navegação é:
     *   MENU <-> SELECAO_GRUPOS <-> LISTAGEM
     *   MENU <-> PACOTE
     *   MENU <-> ESTATISTICAS
     */
typedef enum {
    MENU,
    SELECAO_GRUPOS,
    LISTAGEM,
    PACOTE,
    ESTATISTICAS
} EstadoTela;

    // ====================== Background animado =========================

    /**
     * @brief Carrega os 30 frames do background animado da pasta assets/bg.
     * @param frames Vetor de 30 texturas a ser preenchido.
     */
void carregar_background_frames(Texture2D frames[]);

    /**
     * @brief Desenha o frame atual do background animado cobrindo a tela inteira.
     * @param frames     Vetor com os 30 frames.
     * @param frameAtual Índice do frame a desenhar (0–29).
     */
void desenhar_background(Texture2D *frames, int frameAtual);

    // ====================== Botão de som ===============================

    /**
     * @brief Desenha o botão de mute/unmute no canto superior direito.
     * @param somAtivo Se true desenha o ícone de som ligado; senão, mutado.
     * @param btnSom   Retângulo de posição e tamanho do botão.
     * @param iconeOn  Textura do ícone "som ligado".
     * @param iconeOff Textura do ícone "som mutado".
     */
void desenhar_botao_som(bool somAtivo, Rectangle btnSom, Texture2D iconeOn, Texture2D iconeOff);

    // ====================== Telas ======================================

    /**
     * @brief Desenha o álbum filtrado por seção e página na tela LISTAGEM.
     * @param al         Ponteiro para o álbum.
     * @param secaoAtiva Nome da seção ativa (ex: "Brasil").
     * @param paginaAtiva Índice da página (0, 1 ou 2).
     * @param moldura    Textura de moldura individual da figurinha.
     * @param fundoPagina Textura de fundo da página do álbum.
     */
void DesenharAlbumPorSelecao(Album *al, const char *secaoAtiva, int paginaAtiva,
                              Texture2D moldura, Texture2D fundoPagina);

    /**
     * @brief Desenha a tela de abrir pacotinho com animação de revelar carta.
     *
     * @param al              Ponteiro para o álbum.
     * @param indicesPacote   Vetor com os 7 índices sorteados.
     * @param cartaAtual      Carta sendo exibida no momento (0–6).
     * @param cartaVirada     true = frente visível; false = verso visível.
     * @param pacoteFinalizado true = todos os 7 processados, mostra resumo.
     * @param novasObtidas    Contagem de figurinhas novas coladas neste pacote.
     * @param moldura         Textura de moldura da figurinha.
     */
void DesenharPacote(Album *al, int *indicesPacote, int cartaAtual,
                    bool cartaVirada, bool pacoteFinalizado,
                    int novasObtidas, Texture2D moldura);

    /**
     * @brief Desenha a tela de estatísticas de preenchimento do álbum.
     *
     * Mostra o progresso geral (barra) e, por grupo da Copa (A–L + Especial FWC),
     * a porcentagem de figurinhas já coladas.
     *
     * @param al Ponteiro (somente leitura) para o álbum.
     */
void DesenharEstatisticas(const Album *al);

// ====================== Encerramento ===============================

    /**
     * @brief Fecha a janela gráfica: descarrega texturas, música e áudio.
     *
     * Importante: esta função NÃO libera a memória RAM do álbum
     * (album->figurinhas), pois o programa retorna ao menu de console e o
     * usuário pode continuar cadastrando/pesquisando figurinhas. Quem libera
     * a memória de fato é o main(), através de liberar_album(), apenas quando
     * o programa inteiro é encerrado (opção "Salvar e Sair").
     *
     * @param album    Ponteiro para o álbum (apenas as texturas são descarregadas).
     * @param frames   Vetor com os 30 frames do background.
     * @param iconeOn  Textura do ícone de som ligado.
     * @param iconeOff Textura do ícone de som mutado.
     * @param musica   Stream de música a ser encerrado.
     */
void encerrar_jogo(Album *album, Texture2D frames[],
                   Texture2D iconeOn, Texture2D iconeOff, Music musica);

#endif