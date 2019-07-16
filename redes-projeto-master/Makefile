#CC := gcc
CC := g++
SRCDIR := src
BUILDDIR := build

all:
	$(CC) $(SRCDIR)/jogo_da_velha_redes_funcoes_genericas.cpp $(SRCDIR)/jogo_da_velha_redes_servidor.cpp -o server
	$(CC) $(SRCDIR)/jogo_da_velha_redes_funcoes_genericas.cpp $(SRCDIR)/jogo_da_velha_redes_cliente.cpp -o client

clean:
	rm server
	rm client