# --- Variáveis de Configuração ---

# O compilador que queremos usar (gcc)
CC = gcc

# O nome do nosso executável final
EXEC = chip8_emulator

# Encontrar automaticamente todos os arquivos .c no diretório
SRCS = $(wildcard *.c)

# Gerar nomes de arquivos .o a partir dos .c (ex: main.c -> main.o)
OBJS = $(SRCS:.c=.o)

# Flags do Compilador:
# -Wall -Wextra = Ativa todos os avisos de código (muito útil!)
# $(shell ...)  = Executa o comando e usa a saída como flag
CFLAGS = -Wall -Wextra $(shell sdl2-config --cflags)

# Bibliotecas (Libs) para o Linker:
# $(shell ...) = Obtém as flags corretas para linkar o SDL2
LIBS = $(shell sdl2-config --libs)

# --- Regras (Targets) ---

# A regra 'all' é a padrão (o que é executado quando você digita 'make')
# Ela depende do nosso executável
.PHONY: all
all: $(EXEC)

# Regra para construir o executável:
# Depende de todos os arquivos .o
# Linka todos os .o e as bibliotecas (LIBS)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LIBS)

# Regra de Padrão (Pattern Rule) para compilar .c em .o:
# Ensina o 'make' a transformar CUALQUER .c em um .o
# -c = Compilar, mas não linkar
# $< = O nome do pré-requisito (o arquivo .c)
# $@ = O nome do alvo (o arquivo .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra 'clean':
# Remove todos os arquivos gerados (objetos e o executável)
.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)
