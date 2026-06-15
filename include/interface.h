#ifndef INTERFACE_H
#define INTERFACE_H

#include "raylib.h"
#include "album.h"

typedef enum { MENU, LISTAGEM, PACOTE, SELECAO_GRUPOS } EstadoTela;

void desenhar_background(Texture2D *frames, int frameAtual);
//void desenhar_menu(Rectangle btn);
void desenhar_listagem(Album *a);
void carregar_background_frames(Texture2D frames[]);
void desenhar_botao_som(bool somAtivo, Rectangle btnSom, Texture2D iconeOn, Texture2D iconeOff);
void encerrar_jogo(Album *album, Texture2D frames[], Texture2D iconeOn, Texture2D iconeOff, Music musica);
void DesenharAlbumPorSelecao(Album *al, const char *secaoAtiva, int paginaAtiva, Texture2D moldura, Texture2D fundoPagina);

#endif