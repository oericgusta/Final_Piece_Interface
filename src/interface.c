#include "raylib.h"
#include "interface.h"
#include "album.h"

void desenhar_background(Texture2D *frames, int frameAtual) {
    DrawTexture(frames[frameAtual], 0, 0, WHITE);
}

void desenhar_menu(Rectangle btn) {
    DrawText("MENU PRINCIPAL", 380, 200, 30, DARKGRAY);
    DrawRectangleRec(btn, RED);
    DrawText("Listar Figurinhas", btn.x + 60, btn.y + 20, 20, WHITE);
}

void desenhar_listagem(Album *a) {
    DrawText("TELA DE LISTAGEM", 100, 100, 30, DARKGRAY);
}