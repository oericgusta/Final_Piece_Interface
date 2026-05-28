#ifndef INTERFACE_H
#define INTERFACE_H

#include "raylib.h"
#include "album.h"

typedef enum { MENU, LISTAGEM } EstadoTela;

void desenhar_background(Texture2D *frames, int frameAtual);
void desenhar_menu(Rectangle btn);
void desenhar_listagem(Album *a);

#endif