#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "album.h"
#include "interface.h"

int main(void) {

    Album meu_album;    
    inicializar_album(&meu_album); 
    carregar_csv(&meu_album, "assets/figurinhas2026.csv");

    InitWindow(1920, 1080, "Album 2026"); 
    InitAudioDevice();

    bool somAtivo = true;
    float volumeAtual = 0.0f; 
    Rectangle btnSom = { 1810, 40, 70, 70 }; 

    Music musica = LoadMusicStream("assets/music/Shakira-DaiDai.mp3"); 
    PlayMusicStream(musica); 
    musica.looping = true;
    SetMusicVolume(musica, volumeAtual);

    Texture2D iconeSomLigado = LoadTexture("assets/icons/SoundOn_Button.png");
    Texture2D iconeSomMutado = LoadTexture("assets/icons/SoundOff_Button.png");
    
    Texture2D animFrames[30]; 
    carregar_background_frames(animFrames);

    EstadoTela tela = MENU;
    Rectangle btnListar = { 362, 300, 300, 60 };
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
            if (CheckCollisionPointRec(GetMousePosition(), btnListar)) tela = LISTAGEM;

            if (CheckCollisionPointRec(GetMousePosition(), btnSom)) {
                somAtivo = !somAtivo; 
                if (somAtivo) {
                    volumeAtual = 0.0f;
                    SetMasterVolume(1.0f); 
                } else {
                    SetMasterVolume(0.0f); 
                }
            }
        }

        BeginDrawing();
            desenhar_background(animFrames, frame);
            
            if (tela == MENU) {
                desenhar_menu(btnListar);
            } 
            else if (tela == LISTAGEM) {
                desenhar_listagem(&meu_album);
            }

            desenhar_botao_som(somAtivo, btnSom, iconeSomLigado, iconeSomMutado);
            
        EndDrawing();
    }

    encerrar_jogo(&meu_album, animFrames, iconeSomLigado, iconeSomMutado, musica);

    return 0;
}