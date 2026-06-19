# ===========================================================================
# Makefile – Álbum de Figurinhas FIFA 2026
# Uso:
#   make        -> compila o projeto
#   make clean  -> remove objetos e executável
#   make run    -> compila se necessário e executa
# ===========================================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -I.
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCS   = main.c album.c interface.c salvamento.c
OBJS   = $(SRCS:.c=.o)
TARGET = album2026

.PHONY: all clean run

# ---- Alvo padrão ----
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---- Limpeza ----
clean:
	rm -f $(OBJS) $(TARGET)

# ---- Compilar e executar ----
run: all
	./$(TARGET)