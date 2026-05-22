@echo off
gcc src/main.c src/album.c -Iinclude -o programa.exe
if %errorlevel% == 0 (
    echo Compilado com sucesso! Abrindo o Album da Copa...
    programa.exe
) else (
    echo Erro na compilacao. Verifique o codigo.
    pause
)