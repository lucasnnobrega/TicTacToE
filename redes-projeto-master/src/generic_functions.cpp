/**
 * Aqui serão definidas as funções de uso coletivo do servidor e do cliente
**/
#include "../include/generic_functions.h"
/**
 *
**/

void startBoard(char board[3][3])
{

	//Coloquei dessa forma e não em um for pq tava dando uns erros na hora da exibição dos caracteres

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			board[i][j] = '*';
		}
	}
	// board[1][1] = '*';
	// board[1][2] = '*';
	// board[1][3] = '*';
	// board[2][1] = '*';
	// board[2][2] = '*';
	// board[2][3] = '*';
	// board[3][1] = '*';
	// board[3][2] = '*';
	// board[3][3] = '*';
}

void printBoard(char board[3][3])
{

	printf("\n   %d   %d   %d\n\n", 1, 2, 3);
	printf("%d  %c   %c   %c\n\n", 1, board[0][0], board[0][1], board[0][2]);
	printf("%d  %c   %c   %c\n\n", 2, board[1][0], board[1][1], board[1][2]);
	printf("%d  %c   %c   %c\n\n", 3, board[2][0], board[2][1], board[2][2]);
}

int validateMove(char board[3][3], int row, int column)
{
	if ((board[row - 1][column - 1] != '*') || (row < 1) || (row > 3) || (column < 1) || (column > 3))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void readInput(char board[3][3], int move[2])
{

	int row = 0;
	int column = 0;

	printBoard(board);

	printf("Your turn to play\n\n");

	// Vai ficar tentando obter os dados corretos do usuário.
	do
	{
		printf("Select a row (1-3):  ");
		scanf("%d", &row);
		printf("Select a column (1-3):  ");
		scanf("%d", &column);

		if (validateMove(board, row, column))
			break;
		else
			printf("\n%sThe inserted value is not a valid position. Please try again.%s\n\n", RED, RESET);
	} while (1);

	move[0] = row - 1;
	move[1] = column - 1;
}

int verifyGameState(char board[3][3], int turn_counter, char tipo)
{
	//Verifica as rows
	for (int i = 0; i < 3; i++)
	{
		if (board[i][0] == tipo && board[i][0] == board[i][1] && board[i][0] == board[i][2])
		{
			return VICTORY;
		}
	}

	//Verifica as columns
	for (int j = 0; j < 3; j++)
	{
		if (board[0][j] == tipo && board[0][j] == board[1][j] && board[0][j] == board[2][j])
		{
			return VICTORY;
		}
	}

	//Verifica a diagonal principal
	if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] == tipo)
	{
		return VICTORY;
	}

	//Verifica a diagonal secundária
	if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] == tipo)
	{
		return VICTORY;
	}

	//Retorna empate se estiver no nono turno e não tiver acontecido uma vitória
	if (turn_counter == 9)
	{
		return TIE;
	}

	return CONTINUE;
}
