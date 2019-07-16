#ifndef GENERIC_FUNCTIONS_H
#define GENERIC_FUNCTIONS_H

#include <unistd.h>
#include <netinet/in.h>
#include <locale.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

// CPP Libs
#include <iostream>

// Personal Libs
#include "constants.h"


#define FIRST_TO_PLAY 42
#define SECOND_TO_PLAY 24
#define TIE 55
#define VICTORY 81
#define DEFEAT 17
#define CONTINUE 32 //Continua o jogo pois não aconteceu vitória nem empate

extern void startBoard(char board[3][3]);
extern void printBoard(char board[3][3]);
extern int validateMove(char board[3][3], int row, int column);
extern void readInput(char board[3][3], int move[2]);
extern int verifyGameState(char board[3][3], int turn_counter, char tipo);

#endif //GENERIC_FUNCTIONS_H