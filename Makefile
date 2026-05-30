# ==========================================
# Projeto: Álbum de Figurinhas da Copa 2026
# ==========================================

# Verifica o sistema operacional automaticamente
ifdef OS
  OS := $(strip $(OS))
else
  OS := $(strip $(shell uname))
endif

# Nome do executável que será gerado (baseado na sua foto anterior)
BINNAME = programa

# ==========================================
# Configurações Específicas por Sistema
# ==========================================
# ==========================================
# Configurações Específicas por Sistema
# ==========================================
ifeq ($(OS),Windows_NT)
	# Configurações para Windows (Mantém o -L./lib pois no Windows precisa)
	INCLUDE = -I./include/ -L./lib
	EXTRA_FLAGS = -Wall -Wextra -std=c99 -Wno-missing-braces -lraylib -lopengl32 -lgdi32 -lwinmm -lm
	BIN = $(BINNAME).exe
	RM = del /Q /F
	RUN_CMD = .\$(BIN)
else
	# Configurações para Linux (Removemos o -L./lib)
	INCLUDE = -I./include/
	EXTRA_FLAGS = -Wall -Wextra -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	BIN = $(BINNAME)
	RM = rm -f
	RUN_CMD = ./$(BIN)
endif

# ==========================================
# Arquivos Fonte (Aponta para a pasta src!)
# ==========================================
SRC = src/*.c

# ==========================================
# Regras de Compilação
# ==========================================

# A regra padrão compila o projeto
all:
	gcc $(SRC) $(INCLUDE) -o $(BIN) $(EXTRA_FLAGS) -g

# Compila (se precisar) e já roda o programa
run: all
	$(RUN_CMD)

# Compila e roda no modo debug
debug: all
	gdb $(BIN)

# Limpa o executável gerado
clean:
	$(RM) $(BIN)

# Analisador de memória (Normalmente só usado no Linux)
valgrind: all
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BIN)