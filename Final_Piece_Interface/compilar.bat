@echo off
echo Compilando o projeto do Album da Copa...
gcc main.c album.c -o programa
if %errorlevel% equ 0 (
    echo Compilacao feita com sucesso!
    echo Abrindo o programa automaticamente...
    echo -----------------------------------
    start programa.exe
) else (
    echo [ERRO] Algo deu errado na compilacao. Dá uma olhada no codigo!
)
pause