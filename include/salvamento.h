#ifndef SALVAMENTO_H
#define SALVAMENTO_H

#include "album.h"

/* ============================================================
 *  ARQUIVO CSV  –  leitura inicial e exportação
 * ============================================================ */

/**
 * @brief Lê as figurinhas de um arquivo CSV (texto) e carrega na memória RAM.
 *
 * Usado apenas na primeira execução do programa, quando ainda não existe
 * um arquivo binário de progresso salvo.
 *
 * @param al           Ponteiro para o álbum (já inicializado).
 * @param nome_arquivo Caminho do arquivo CSV de entrada.
 */
void carregar_csv(Album *al, const char *nome_arquivo);

/**
 * @brief Exporta os dados atuais do álbum para um arquivo de texto CSV.
 *
 * Atende ao requisito de exportação do projeto; não é a persistência
 * principal (essa é feita pelo arquivo binário).
 *
 * @param al           Ponteiro para o álbum.
 * @param nome_arquivo Caminho do arquivo CSV de saída.
 */
void salvar_csv(Album *al, const char *nome_arquivo);

/* ============================================================
 *  ARQUIVO BINÁRIO  –  persistência principal entre execuções
 * ============================================================ */

/**
 * @brief Salva o progresso do álbum (todas as figurinhas) em arquivo binário.
 * @param al           Ponteiro para o álbum.
 * @param nome_arquivo Caminho do arquivo binário de saída.
 */
void salvar_binario(Album *al, const char *nome_arquivo);

/**
 * @brief Carrega o progresso do álbum a partir de um arquivo binário.
 * @param al           Ponteiro para o álbum (já inicializado).
 * @param nome_arquivo Caminho do arquivo binário de entrada.
 * @return 1 em caso de sucesso; 0 se o arquivo não existir (1ª execução).
 */
int carregar_binario(Album *al, const char *nome_arquivo);

#endif