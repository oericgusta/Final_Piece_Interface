    // Conceito do Include Guard
#ifndef ALBUM_H
#define ALBUM_H

#include "raylib.h"

#define TAM_CODIGO  15
#define TAM_TITULO  100
#define TAM_SECAO   100
#define TAM_GRUPO   15
#define TAM_TIPO    30

    /**
     * @brief Representa uma única figurinha do álbum.
     *
     * Guarda tanto os dados "de papel" (código, título, seção, grupo, tipo)
     * quanto o estado de preenchimento do usuário (colada / repetidas) e a
     * textura carregada em VRAM para ser desenhada pela interface gráfica.
     */
typedef struct
{
    char codigo[TAM_CODIGO];  /**< Ex: "PAN20" (chave única da figurinha) */
    char titulo[TAM_TITULO];  /**< Ex: "Alberto Quintero" (nome do jogador/estádio) */
    char secao[TAM_SECAO];    /**< Ex: "Panama" ou "FIFA World Cup 2026" */
    char grupo[TAM_GRUPO];    /**< Ex: "A", "B", "ESPECIAL" */
    char tipo[TAM_TIPO];      /**< Ex: "Comum", "Especial" */
    int colada;                /**< 0 = não colada, 1 = colada no álbum */
    int repetidas;              /**< Quantidade de cópias repetidas disponíveis para troca */
    Texture2D textura;          /**< Textura carregada em VRAM (id = 0 se não carregada) */
} Figurinha;

    /**
     * @brief Estrutura que gerencia o vetor dinâmico de figurinhas (o álbum em si).
     */
typedef struct
{
    Figurinha *figurinhas; /**< Ponteiro para o vetor dinâmico de figurinhas na memória RAM */
    int total;              /**< Quantidade atual de figurinhas carregadas */
    int capacidade;          /**< Capacidade máxima atual alocada na memória RAM */
    Texture2D texturaVerso;  /**< Textura única do verso padrão da figurinha */
} Album;

    // ===================== Inicialização e Memória =====================

    /**
     * @brief Aloca o vetor dinâmico inicial do álbum.
     * @param al Ponteiro para o álbum a ser inicializado.
     */
void inicializar_album(Album *al);

    /**
     * @brief Libera toda a memória RAM alocada pelo álbum (vetor de figurinhas).
     * @param al Ponteiro para o álbum.
     */
void liberar_album(Album *al);

    /**
     * @brief Remove espaços em branco do início e do final de uma string.
     * @param str String a ser limpa (alterada em memória, "in place").
     */
void trim_spaces(char *str);

    // ============================== CRUD ================================

    /**
     * @brief Cadastra uma nova figurinha no álbum via entrada de teclado (console).
     *
     * Expande o vetor dinâmico automaticamente se necessário e impede o
     * cadastro de códigos duplicados.
     * @param al Ponteiro para o álbum.
     */
void inserir_figurinha(Album *al);

    /**
     * @brief Lista no console todas as figurinhas cadastradas no álbum.
     * @param al Ponteiro (somente leitura) para o álbum.
     */
void listar_figurinhas(const Album *al);

    /**
     * @brief Busca o índice de uma figurinha no vetor a partir do código.
     * @param al Ponteiro (somente leitura) para o álbum.
     * @param codigo Código a ser procurado.
     * @return Índice da figurinha encontrada ou -1 caso não exista.
     */
int buscar_indice_por_codigo(const Album *al, const char *codigo);

    /**
     * @brief Pesquisa e exibe no console os dados de uma figurinha pelo código.
     * @param al Ponteiro (somente leitura) para o álbum.
     */
void pesquisar_figurinha(const Album *al);

    /**
     * @brief Permite alterar campos de uma figurinha já cadastrada (console).
     * @param al Ponteiro para o álbum.
     */
void alterar_figurinha(Album *al);

    /**
     * @brief Exclui uma figurinha do álbum, reduzindo o vetor dinamicamente.
     * @param al Ponteiro para o álbum.
     */
void excluir_figurinha(Album *al);

    // ============================ Pacotinho ==============================

    /**
     * @brief Abre um pacote com 7 figurinhas aleatórias no modo texto (console).
     * @param al Ponteiro para o álbum.
     */
void abrir_pacotinho(Album *al);

    // ============================ Texturas ================================

    /**
     * @brief Carrega em VRAM o verso padrão e a foto de cada figurinha do álbum.
     * @param al Ponteiro para o álbum.
     */
void carregar_texturas_album(Album *al);

    /**
     * @brief Descarrega da VRAM todas as texturas do álbum (verso + figurinhas).
     * @param al Ponteiro para o álbum.
     */
void descarregar_texturas_album(Album *al);

#endif