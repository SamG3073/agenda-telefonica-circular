# Nome do compilador
CC = gcc

# Nome do executável final
EXEC = agenda_contatos

# Arquivos-fonte (.c)
SRCS = main.c gerenciador_app.c contato.c listalib.c

# Arquivos-objeto (.o) - gerados automaticamente a partir dos .c
OBJS = $(SRCS:.c=.o)

# Flags de compilação (avisos, debug, etc.)
CFLAGS = -Wall -g

# Flags do linker (bibliotecas)
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lm

# Regra principal: o que fazer quando 'make' é executado
all: $(EXEC)

# Regra para criar o executável a partir dos arquivos-objeto
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Regra para criar arquivos-objeto a partir dos arquivos-fonte
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean