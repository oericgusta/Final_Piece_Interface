# ==========================================
# Projeto: Álbum de Figurinhas da Copa 2026
# ==========================================
# Opção do Make pra usar em sistemas Linux 
# ==========================================

# Nome do compilador
CC = gcc

# Opções de compilação (mostra avisos importantes no código)
CFLAGS = -Wall -Wextra -g

# Nome do arquivo executável final
TARGET = programa

# Arquivos de código-fonte
SRCS = main.c album.c

# Comando padrão que roda quando você digita apenas "make"
all: $(TARGET)

# Regra para compilar o programa juntando os arquivos .c
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Regra para limpar os arquivos gerados (para rodar: make clean)
clean:
	rm -f $(TARGET)