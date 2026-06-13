#include <stdio.h>
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

    for(int i = 0; i < 30; i++){
        UnloadTexture(frames[i]);
    } 

    UnloadTexture(iconeOn);
    UnloadTexture(iconeOff);

    salvar_csv(album, "assets/figurinhas2026.csv");
    liberar_album(album); 

    UnloadMusicStream(musica);   
    CloseAudioDevice();
    CloseWindow();  
}