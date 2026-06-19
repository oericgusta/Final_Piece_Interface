#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "album.h"
#include "raylib.h"

    // =========== Inicialização e Memória ===========

void inicializar_album(Album *al) {
    al->total = 0;
    al->capacidade = 50; // Começa alocando espaço para 50, vai aumentar se precisar
    al->figurinhas = (Figurinha *) malloc(al->capacidade * sizeof(Figurinha));
    if (al->figurinhas == NULL) {
        printf("[!] Erro critico de memoria!\n");
        exit(1);
    }
    al->texturaVerso.id = 0;
}

void trim_spaces(char *str) {
    if (str == NULL) return;

    // Remove espaços do final
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[len - 1] = '\0';
        len--;
    }

    // Remove espaços do início
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t') {
        start++;
    }
    if (start > 0) {
        memmove(str, str + start, len - start + 1);
    }
}

    /**
     * @brief Garante que o vetor dinâmico tem espaço para mais uma figurinha,
     *        expandindo (realloc) a capacidade em blocos de 50 quando necessário.
     */
static void garantir_capacidade(Album *al) {
    if (al->total >= al->capacidade) {
        al->capacidade += 50;
        Figurinha *novo = (Figurinha *) realloc(al->figurinhas, al->capacidade * sizeof(Figurinha));
        if (novo == NULL) {
            printf("[!] Erro critico de memoria ao expandir o album!\n");
            exit(1);
        }
        al->figurinhas = novo;
    }
}

void liberar_album(Album *al) {
    free(al->figurinhas);
    al->figurinhas = NULL;
    al->total = 0;
    al->capacidade = 0;
}

    // ================= CRUD =================

int buscar_indice_por_codigo(const Album *al, const char *codigo) {
    for (int i = 0; i < al->total; i++) {
        if (strcmp(al->figurinhas[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}

void inserir_figurinha(Album *al) {
    Figurinha f;
    memset(&f, 0, sizeof(Figurinha));

    printf("\n=== CADASTRAR NOVA FIGURINHA ===\n");

    printf("Codigo (ex: PAN20): ");
    scanf(" %14[^\n]", f.codigo);
    trim_spaces(f.codigo);

    if (strlen(f.codigo) == 0) {
        printf("\a[!] Codigo nao pode ser vazio. Cadastro cancelado.\n");
        return;
    }

    if (buscar_indice_por_codigo(al, f.codigo) != -1) {
        printf("\a[!] Ja existe uma figurinha com o codigo %s.\n", f.codigo);
        return;
    }

    printf("Titulo (jogador/estadio): ");
    scanf(" %99[^\n]", f.titulo);
    trim_spaces(f.titulo);

    printf("Secao (selecao/pais): ");
    scanf(" %99[^\n]", f.secao);
    trim_spaces(f.secao);

    printf("Grupo (ex: A, B, ESPECIAL FWC): ");
    scanf(" %14[^\n]", f.grupo);
    trim_spaces(f.grupo);

    printf("Tipo (ex: Comum, Especial): ");
    scanf(" %29[^\n]", f.tipo);
    trim_spaces(f.tipo);

    f.colada = 0;
    f.repetidas = 0;
    f.textura.id = 0;

    garantir_capacidade(al);
    al->figurinhas[al->total] = f;
    al->total++;

    printf("[+] Figurinha %s cadastrada com sucesso! (Total: %d)\n", f.codigo, al->total);
}

void listar_figurinhas(const Album *al) {
    if (al->total == 0) {
        printf("\nNenhuma figurinha no album.\n");
        return;
    }
    printf("\n=== LISTA DE FIGURINHAS (%d no total) ===\n", al->total);
    for (int i = 0; i < al->total; i++) {
        printf("[%s] %-25s | Secao: %-18s | Status: %-8s | Repetidas: %d\n",
               al->figurinhas[i].codigo,
               al->figurinhas[i].titulo,
               al->figurinhas[i].secao,
               al->figurinhas[i].colada ? "Colada" : "Faltando",
               al->figurinhas[i].repetidas);
    }
}

void pesquisar_figurinha(const Album *al) {
    char busca[TAM_CODIGO];
    printf("Digite o codigo da figurinha: ");
    scanf(" %14s", busca);

    int idx = buscar_indice_por_codigo(al, busca);
    if (idx == -1) {
        printf("\a[!] Figurinha com codigo %s nao encontrada.\n", busca);
        return;
    }

    const Figurinha *f = &al->figurinhas[idx];
    printf("\nEncontrada:\nCodigo: %s\nTitulo: %s\nSecao: %s\nGrupo: %s\nTipo: %s\nStatus: %s\nRepetidas: %d\n",
           f->codigo, f->titulo, f->secao, f->grupo, f->tipo,
           f->colada ? "Colada" : "Nao colada", f->repetidas);
}

void alterar_figurinha(Album *al) {
    char busca[TAM_CODIGO];
    printf("Digite o codigo da figurinha que deseja alterar: ");
    scanf(" %14s", busca);

    int idx = buscar_indice_por_codigo(al, busca);
    if (idx == -1) {
        printf("\a[!] Figurinha com codigo %s nao encontrada.\n", busca);
        return;
    }

    Figurinha *f = &al->figurinhas[idx];
    int opcao;

    do {
        printf("\n=== ALTERAR FIGURINHA [%s] ===\n", f->codigo);
        printf("1 - Titulo (atual: %s)\n", f->titulo);
        printf("2 - Secao  (atual: %s)\n", f->secao);
        printf("3 - Grupo  (atual: %s)\n", f->grupo);
        printf("4 - Tipo   (atual: %s)\n", f->tipo);
        printf("5 - Status colada (atual: %s)\n", f->colada ? "Colada" : "Faltando");
        printf("6 - Repetidas (atual: %d)\n", f->repetidas);
        printf("0 - Voltar\n");
        printf("Escolha o campo: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // limpa entrada invalida
            printf("\a[!] Entrada invalida.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Novo titulo: ");
                scanf(" %99[^\n]", f->titulo);
                trim_spaces(f->titulo);
                break;
            case 2:
                printf("Nova secao: ");
                scanf(" %99[^\n]", f->secao);
                trim_spaces(f->secao);
                break;
            case 3:
                printf("Novo grupo: ");
                scanf(" %14[^\n]", f->grupo);
                trim_spaces(f->grupo);
                break;
            case 4:
                printf("Novo tipo: ");
                scanf(" %29[^\n]", f->tipo);
                trim_spaces(f->tipo);
                break;
            case 5:
                f->colada = !f->colada;
                printf("[+] Status alterado para: %s\n", f->colada ? "Colada" : "Faltando");
                break;
            case 6:
                printf("Nova quantidade de repetidas: ");
                scanf("%d", &f->repetidas);
                if (f->repetidas < 0) f->repetidas = 0;
                break;
            case 0:
                break;
            default:
                printf("\a[!] Opcao invalida.\n");
        }
    } while (opcao != 0);

    printf("[+] Figurinha %s atualizada com sucesso!\n", f->codigo);
}

void excluir_figurinha(Album *al) {
    char busca[TAM_CODIGO];
    printf("Digite o codigo da figurinha que deseja excluir: ");
    scanf(" %14s", busca);

    int idx = buscar_indice_por_codigo(al, busca);
    if (idx == -1) {
        printf("\a[!] Figurinha com codigo %s nao encontrada.\n", busca);
        return;
    }

    char confirma;
    printf("Tem certeza que deseja excluir [%s] %s? (s/n): ",
           al->figurinhas[idx].codigo, al->figurinhas[idx].titulo);
    scanf(" %c", &confirma);

    if (confirma != 's' && confirma != 'S') {
        printf("Exclusao cancelada.\n");
        return;
    }

    if (al->figurinhas[idx].textura.id > 0) {
        UnloadTexture(al->figurinhas[idx].textura);
    }

    // Desloca os elementos seguintes uma posição para trás (vetor "encolhe")
    for (int i = idx; i < al->total - 1; i++) {
        al->figurinhas[i] = al->figurinhas[i + 1];
    }
    al->total--;

    printf("[+] Figurinha removida com sucesso! O album agora tem %d figurinhas.\n", al->total);
}

    // ================= Pacotinho =================

void abrir_pacotinho(Album *al) {
    if (al->total == 0) {
        printf("\a[!] Carregue o arquivo de figurinhas antes.\n");
        return;
    }

    printf("\n--- ABRINDO PACOTINHO (7 Figurinhas) ---\n");
    for (int i = 0; i < 7; i++) {
        int indice_aleatorio = rand() % al->total;
        Figurinha *f = &al->figurinhas[indice_aleatorio];

        printf("\n[%d/7] Voce tirou: %s (%s)\n", i + 1, f->titulo, f->codigo);

        char opcao;
        printf("Deseja colar no album? (s/n): ");
        scanf(" %c", &opcao);

        if (opcao == 's' || opcao == 'S') {
            if (f->colada == 0) {
                f->colada = 1;
                printf("[+] Boa! Nova figurinha colada!\n");
            } else {
                f->repetidas++;
                printf("Voce ja tinha essa. Adicionada as repetidas para troca!\n");
            }
        } else {
            printf("Figurinha guardada na gaveta.\n");
        }
    }
}

    // ================= Texturas ==================

void carregar_texturas_album(Album *al) {
    char caminho[150];
    char codigoLimpo[TAM_CODIGO];

    // 1. Carrega a imagem do Verso Padrão uma única vez
    const char *caminhoVerso = "assets/icons/Verso_Fig.png";
    if (FileExists(caminhoVerso)) {
        al->texturaVerso = LoadTexture(caminhoVerso);
    } else {
        printf("[!] Aviso: Imagem do verso nao encontrada em %s\n", caminhoVerso);
    }

    // 2. Carrega a foto de cada jogador do álbum
    for (int i = 0; i < al->total; i++) {
        sscanf(al->figurinhas[i].codigo, "%s", codigoLimpo);
        sprintf(caminho, "assets/figurinhas/%s.png", codigoLimpo);

        if (FileExists(caminho)) {
            al->figurinhas[i].textura = LoadTexture(caminho);
        } else {
            // Se não achar a foto do jogador, deixa o ID como 0 para segurança
            al->figurinhas[i].textura.id = 0;
            printf("[!] Erro VRAM: Nao achei a foto do codigo %s em: %s\n", codigoLimpo, caminho);
        }
    }
    printf("[+] Total de %d texturas processadas.\n", al->total);
}

void descarregar_texturas_album(Album *al) {
    if (al->texturaVerso.id > 0) {
        UnloadTexture(al->texturaVerso);
        al->texturaVerso.id = 0;
    }

    for (int i = 0; i < al->total; i++) {
        if (al->figurinhas[i].textura.id > 0) {
            UnloadTexture(al->figurinhas[i].textura);
            al->figurinhas[i].textura.id = 0;
        }
    }
}