#ifndef JOGO_DA_VELHA_REDES_FUNCOES_GENERICAS_H
#define JOGO_DA_VELHA_REDES_FUNCOES_GENERICAS_H

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
#include "jogo_da_velha_formatacao_print.h"

#include <iostream>


#define PRIMEIRO_A_JOGAR 42
#define SEGUNDO_A_JOGAR 24
#define EMPATE 55
#define VITORIA 81
#define DERROTA 17
#define CONTINUA 32 //Continua o jogo pois não aconteceu vitória nem empate

extern void iniciaTabuleiro(char tabuleiro[3][3]);
extern void printTabuleiro(char tabuleiro[3][3]);
extern int validaJogada(char tabuleiro[3][3], int linha, int coluna);
extern void leInput(char tabuleiro[3][3], int jogada[2]);
extern int verificaEstadoDoJogo(char tabuleiro[3][3], int contador_turnos, char tipo);

#endif //JOGO_DA_VELHA_REDES_FUNCOES_GENERICAS_H