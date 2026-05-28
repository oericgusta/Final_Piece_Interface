#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "album.h"
#include "interface.h"

int main(void) {
    Album meu_album;    // Variavel do tipo album criada

    inicializar_album(&meu_album); 

    carregar_csv(&meu_album, "assets/figurinhas2026.csv");

    InitWindow(1920, 1080, "Album 2026");   // Abre a Janela
    
    // Carregar os 30 frames da pasta background
    Texture2D animFrames[30];
    char path[100];
    
    for(int i = 0; i < 30; i++) {
        sprintf(path, "assets/background/frame%02d.png", i);
        animFrames[i] = LoadTexture(path);
    }

    EstadoTela tela = MENU;
    Rectangle btnListar = { 362, 300, 300, 60 };    // Posição do Retangulo na Janela

    int frame = 0, timer = 0;   // Varaveis pra controlar a logica

    SetTargetFPS(60); // Limita o FPs

    while (!WindowShouldClose()) {

        // Lógica
        timer++;
        if (timer >= 10) { timer = 0; frame = (frame + 1) % 30; }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), btnListar)) tela = LISTAGEM;
        }

        // Desenho
        BeginDrawing();
            desenhar_background(animFrames, frame);
            if (tela == MENU) {
                desenhar_menu(btnListar);
            } 
            else if (tela == LISTAGEM) {
                desenhar_listagem(&meu_album);
            }
        EndDrawing();
    }

    for(int i = 0; i < 30; i++){
        UnloadTexture(animFrames[i]);
    } 

    salvar_csv(&meu_album, "assets/figurinhas2026.csv");

    CloseWindow();  // Fecha a Janela

    return 0;
}