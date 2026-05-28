@echo off

gcc src/*.c -o programa.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -o programa.exe

if %errorlevel% == 0 (
    echo Compilacao feita com sucesso!
    programa.exe
) else (
    echo --- ERRO NA COMPILACAO ---
    pause
)