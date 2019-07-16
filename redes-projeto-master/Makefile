#CC := gcc
CC := g++
SRCDIR := src
BUILDDIR := build


all:
	$(CC) $(SRCDIR)/generic_functions.cpp $(SRCDIR)/server.cpp -o ./bin/server
	$(CC) $(SRCDIR)/generic_functions.cpp $(SRCDIR)/client.cpp -o ./bin/client

clean:
	rm ./bin/server
	rm ./bin/client