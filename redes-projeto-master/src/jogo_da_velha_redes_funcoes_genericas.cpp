/**
 * Aqui serão definidas as funções de uso coletivo do servidor e do cliente
**/
#include "../include/jogo_da_velha_redes_funcoes_genericas.h"
/**
 *
**/

void iniciaTabuleiro(char tabuleiro[3][3])
{

	//Coloquei dessa forma e não em um for pq tava dando uns erros na hora da exibição dos caracteres

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			tabuleiro[i][j] = '*';
		}
	}
	// tabuleiro[1][1] = '*';
	// tabuleiro[1][2] = '*';
	// tabuleiro[1][3] = '*';
	// tabuleiro[2][1] = '*';
	// tabuleiro[2][2] = '*';
	// tabuleiro[2][3] = '*';
	// tabuleiro[3][1] = '*';
	// tabuleiro[3][2] = '*';
	// tabuleiro[3][3] = '*';
}

void printTabuleiro(char tabuleiro[3][3])
{

	printf("\n   %d   %d   %d\n\n", 1, 2, 3);
	printf("%d  %c   %c   %c\n\n", 1, tabuleiro[0][0], tabuleiro[0][1], tabuleiro[0][2]);
	printf("%d  %c   %c   %c\n\n", 2, tabuleiro[1][0], tabuleiro[1][1], tabuleiro[1][2]);
	printf("%d  %c   %c   %c\n\n", 3, tabuleiro[2][0], tabuleiro[2][1], tabuleiro[2][2]);
}

int validaJogada(char tabuleiro[3][3], int linha, int coluna)
{
	if ((tabuleiro[linha - 1][coluna - 1] != '*') || (linha < 1) || (linha > 3) || (coluna < 1) || (coluna > 3))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void leInput(char tabuleiro[3][3], int jogada[2])
{

	int linha = 0;
	int coluna = 0;

	printTabuleiro(tabuleiro);

	printf("Your turn to play\n\n");

	// Vai ficar tentando obter os dados corretos do usuário.
	do
	{
		printf("Select a line (1-3):  ");
		scanf("%d", &linha);
		printf("Select a column (1-3):  ");
		scanf("%d", &coluna);

		if (validaJogada(tabuleiro, linha, coluna))
			break;
		else
			printf("\n%sThe inserted value is not a valid position. Please try again.%s\n\n", RED, RESET);
	} while (1);

	jogada[0] = linha - 1;
	jogada[1] = coluna - 1;
}

int verificaEstadoDoJogo(char tabuleiro[3][3], int contador_turnos, char tipo)
{
	//Verifica as linhas
	for (int i = 0; i < 3; i++)
	{
		if (tabuleiro[i][0] == tipo && tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][0] == tabuleiro[i][2])
		{
			return VITORIA;
		}
	}

	//Verifica as colunas
	for (int j = 0; j < 3; j++)
	{
		if (tabuleiro[0][j] == tipo && tabuleiro[0][j] == tabuleiro[1][j] && tabuleiro[0][j] == tabuleiro[2][j])
		{
			return VITORIA;
		}
	}

	//Verifica a diagonal principal
	if (tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[0][0] == tabuleiro[2][2] && tabuleiro[0][0] == tipo)
	{
		return VITORIA;
	}

	//Verifica a diagonal secundária
	if (tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[0][2] == tabuleiro[2][0] && tabuleiro[0][2] == tipo)
	{
		return VITORIA;
	}

	//Retorna empate se estiver no nono turno e não tiver acontecido uma vitória
	if (contador_turnos == 9)
	{
		return EMPATE;
	}

	return CONTINUA;
}
