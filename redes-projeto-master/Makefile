#CC := gcc
CC := g++
SRCDIR := src
BUILDDIR := build
BIN := bin

all:
	$(CC) $(SRCDIR)/generic_functions.cpp $(SRCDIR)/server.cpp -o $(BIN)/server
	$(CC) $(SRCDIR)/generic_functions.cpp $(SRCDIR)/client.cpp -o $(BIN)/client

clean:
	rm ./bin/server
	rm ./bin/client