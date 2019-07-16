/**
 * Neste arquivo serão contidas as funções relacionadas ao cliente
 *
**/
#include "../include/generic_functions.h"
/**
 *
**/

int fazMove(int socket_servidor, char board[3][3], int eu_sou)
{

    int verificador_de_erro, // variavel que vai ser usada para verificar se houve erro na função
        row,               // Variável que armazena o valor da row
        column;              // Variável que armazena o valor da column

    int move[2]; //Utilizada no retorno do le Input. O move[0] vai conter a row e o move[1] a column

    // Vai ficar tentando obter os dados corretos do usuário. readInput vai tratar qualquer dado ruim
    readInput(board, move);

    row = move[0];
    column = move[1];
    board[row][column] = eu_sou;

    // Quando receber os dados corretos põe no formato de envio e envia os 2
    row = htons(row);
    column = htons(column);
    verificador_de_erro = write(socket_servidor, &row, sizeof(row));
    // Caso o resultado seja -1 há um erro de escrita
    if (verificador_de_erro < 0)
    {
        //perror("Houve um erro no envio do dado de row para o servidor, o programa será encerrado.");
        std::cerr << "Error sending the row data for the server, program will be stopped. Error: " << strerror(errno) << std::endl; 
        exit(1);
    }
    verificador_de_erro = write(socket_servidor, &column, sizeof(column));
    // Caso o resultado seja -1 há um erro de escrita
    if (verificador_de_erro < 0)
    {
        //perror("Houve um erro no envio do dado de column para o servidor, o programa será encerrado.");
        std::cerr << "Error sending the row data for the server, program will be stopped. Error: " << strerror(errno) << std::endl;
        exit(1);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    //Verifica argumentos
    if (argc != 3)
    {
        fprintf(stderr, "Error: number of arguments invalid\n");
        fprintf(stderr, "cliente need 3 arguments e.g:\n");
        fprintf(stderr, "./client ip port \n");
        exit(1);
    }

    int port = atoi(argv[2]),            // Salva o valor da port.
        socket_servidor,                  // Aqui vai ser mantido a conexão com o servidor
        verifica_erro,                    // Utilizado para fazer as verificações de erro da leitura
        row,                            // registra a row enviada pelo servidor
        column;                           // registra a column enviada pelo servidor
    char board[3][3];                 // board do cliente
    struct hostent *servidor;             // Aqui vai manter o nome do servidor
    struct sockaddr_in endereco_servidor; // Aqui será mantido alguns dados do servidor para conexão apenas
    struct timeval tv;                    // Tempo limite de resposta
    servidor = gethostbyname(argv[1]);    // tenta achar o servidor pelo ip. Lembrando que localhost é 127.0.0.1

    // tempo de timeout
    tv.tv_sec = 30;
    tv.tv_usec = 0;

    startBoard(board);

    // Se não conseguir achar o host é um problema
    if (servidor == NULL)
    {
        //perror("Servidor não encontrado. O programa será encerrado");
        std::cerr << "Error: " << strerror(errno) << std::endl; 
        exit(1);
    }
    // Tenta abrir conexão
    printf("Server found\n");
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

    // adiciona timeout no socket
    setsockopt(socket_servidor, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
    setsockopt(socket_servidor, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof tv);

    // Se não conseguir abrir conexão é um problema
    if (socket_servidor < 0)
    {
        //perror("Houve um ao acessar o servidor. O programa será encerrado");
        std::cerr << "Error in access the server. the program will be stopped. Error: " << strerror(errno) << std::endl; 
        exit(1);
    }
    printf("Open socket\n");
    // Limpa a estrutura de dados
    memset((struct sockaddr_in *)&endereco_servidor, 0, sizeof(endereco_servidor));

    // põe os dados relacionados a conexão
    endereco_servidor.sin_family = AF_INET;

    // Pega alguns dados vindos da conexão por IP
    bcopy((char *)servidor->h_addr, (char *)&endereco_servidor.sin_addr.s_addr, servidor->h_length);
    // Define a port da conexão
    endereco_servidor.sin_port = htons(port);
    printf("Estabilish struture\n");
    // Tenta conectar ao servidor, mas se não conseguir é um problema
    if (connect(socket_servidor, (const struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor)) < 0)
    {
        //perror("Houve um ao se conectar o servidor. O programa será fechado");
        std::cerr << "Error: " << strerror(errno) << std::endl; 
        exit(1);
    }
    printf("Conected to server\n");
    // Assim que conectar ele vai receber uma mensagem dizendo de quem é a vez
    int estado = 0;
    verifica_erro = read(socket_servidor, &estado, sizeof(estado));
    // Caso o resultado seja -1 há um erro de leitura
    if (verifica_erro < 0)
    {
        //perror("Houve um erro na leitura do dado de estado, o programa será encerrado.");
        std::cerr << "Error while read state data, the program will be stopped. Error: " << strerror(errno) << std::endl; 
        exit(1);
    }
    estado = ntohs(estado);
    int eu_sou = '-';
    int estado_do_jogo = 0;
    if (estado == FIRST_TO_PLAY)
    {
        eu_sou = 'X';
        while (1)
        {
            fazMove(socket_servidor, board, eu_sou);
            printBoard(board);

            printf("Waiting to the other player make a move\n");
            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                std::cerr << "Error: " << strerror(errno) << std::endl; 
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VICTORY)
            {
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DEFEAT)
            {
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == TIE)
            {
                printf("%s\nTIE... :|%s\n\n", YELLOW, RESET);
                close(socket_servidor);
                exit(0);
            }

            verifica_erro = read(socket_servidor, &row, sizeof(row));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de row, o programa será encerrado.");
                std::cerr << ERROR_LINE  <<"Error: " << strerror(errno) << std::endl; 
                exit(1);
            }
            verifica_erro = read(socket_servidor, &column, sizeof(column));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de column, o programa será encerrado.");
                std::cerr << ERROR_COLUMN << "Error whlie reading the column, the program will be stopped. Error: " << strerror(errno) <<std::endl; 

                exit(1);
            }
            row = ntohs(row);
            column = ntohs(column);
            board[row][column] = 'O';
            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                std::cerr << "Error whlie reading the game state, the program will be stopped. Error: " << strerror(errno) << std::endl;  
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VICTORY)
            {
                printBoard(board);
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DEFEAT)
            {
                printBoard(board);
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == TIE)
            {
                printBoard(board);
                printf("%s\nTIE... :|%s\n\n", YELLOW, RESET);
                close(socket_servidor);
                exit(0);
            }
        }
    }
    else
    {
        eu_sou = 'O';
        while (1)
        {
            printf("Waiting to the other player make a move\n");
            verifica_erro = read(socket_servidor, &row, sizeof(row));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de row, o programa será encerrado.");
                std::cerr << ERROR_LINE << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            verifica_erro = read(socket_servidor, &column, sizeof(column));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de column, o programa será encerrado.");
                //perror("Error whlie reading the row, the program will be stopped.");
                std::cerr << ERROR_COLUMN << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }

            row = ntohs(row);
            column = ntohs(column);
            board[row][column] = 'X';

            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                //perror("Error whlie reading the game state, the program will be stopped.");
                std::cerr << ERROR_GAME_STATE << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VICTORY)
            {
                printBoard(board);
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DEFEAT)
            {
                printBoard(board);
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == TIE)
            {
                printBoard(board);
                printf("\n%sTIE... :|%s\n\n", YELLOW, RESET);
                close(socket_servidor);
                exit(0);
            }

            fazMove(socket_servidor, board, eu_sou);
            printBoard(board);

            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                std::cerr << "Error whlie reading the game state, the program will be stopped. Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VICTORY)
            {
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DEFEAT)
            {
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == TIE)
            {
                printf("%s\nTIE... :|%s\n\n", YELLOW, RESET);
                close(socket_servidor);
                exit(0);
            }
        }
    }
    return 1;
}