// Conceito do Include Guard
#ifndef ALBUM_H
#define ALBUM_H

// Estrutura para cada figurinha
typedef struct
{
    char codigo[15];  // Ex: "PAN20" (Chave única da figurinha)
    char titulo[100]; // Ex: "Alberto Quintero" (Nome do jogador/estádio)
    char secao[100];  // Ex: "PAN" ou "Estádios"
    char grupo[15];   // Ex: "A", "B" (Grupo da seleção)
    char tipo[30];    // Ex: "Comum", "Especial"
    int colada;       // 0 se não tiver no álbum, 1 se estiver colada
    int repetidas;    // Contador de quantas figurinhas repetidas você tem dela
} Figurinha;

// Estrutura que gerencia o Álbum e a alocação dinâmica
typedef struct
{
    Figurinha *figurinhas; // Ponteiro para o vetor dinâmico de figurinhas na memória RAM
    int total;             // Quantidade atual de figurinhas carregadas
    int capacidade;        // Capacidade máxima atual alocada na memória RAM
} Album;

// Protótipos das funções
// Funções de Inicialização e Memória
void inicializar_album(Album *al);
void liberar_album(Album *al);

// Funções de Arquivo / Persistência 
void salvar_csv(Album *al, const char *nome_arquivo);
void carregar_csv(Album *al, const char *nome_arquivo);

// funcões de guardar e buscar
void listar_figurinhas(Album *al);
void pesquisar_figurinha(Album *al);

// Função do Pacotinho
void abrir_pacotinho(Album *al);

#endif