/**
 * Neste arquivo serão contidas as funções relacionadas ao cliente
 *
**/
#include "../include/generic_functions.h"
/**
 *
**/

int fazJogada(int socket_servidor, char tabuleiro[3][3], int eu_sou)
{

    int verificador_de_erro, // variavel que vai ser usada para verificar se houve erro na função
        linha,               // Variável que armazena o valor da linha
        coluna;              // Variável que armazena o valor da coluna

    int jogada[2]; //Utilizada no retorno do le Input. O jogada[0] vai conter a linha e o jogada[1] a coluna

    // Vai ficar tentando obter os dados corretos do usuário. leInput vai tratar qualquer dado ruim
    leInput(tabuleiro, jogada);

    linha = jogada[0];
    coluna = jogada[1];
    tabuleiro[linha][coluna] = eu_sou;

    // Quando receber os dados corretos põe no formato de envio e envia os 2
    linha = htons(linha);
    coluna = htons(coluna);
    verificador_de_erro = write(socket_servidor, &linha, sizeof(linha));
    // Caso o resultado seja -1 há um erro de escrita
    if (verificador_de_erro < 0)
    {
        //perror("Houve um erro no envio do dado de linha para o servidor, o programa será encerrado.");
        std::cerr << "Error sending the line data for the server, program will be stopped. Error: " << strerror(errno) << std::endl; 
        exit(1);
    }
    verificador_de_erro = write(socket_servidor, &coluna, sizeof(coluna));
    // Caso o resultado seja -1 há um erro de escrita
    if (verificador_de_erro < 0)
    {
        //perror("Houve um erro no envio do dado de coluna para o servidor, o programa será encerrado.");
        std::cerr << "Error sending the line data for the server, program will be stopped. Error: " << strerror(errno) << std::endl;
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
        fprintf(stderr, "./client ip porta \n");
        exit(1);
    }

    int porta = atoi(argv[2]),            // Salva o valor da porta.
        socket_servidor,                  // Aqui vai ser mantido a conexão com o servidor
        verifica_erro,                    // Utilizado para fazer as verificações de erro da leitura
        linha,                            // registra a linha enviada pelo servidor
        coluna;                           // registra a coluna enviada pelo servidor
    char tabuleiro[3][3];                 // tabuleiro do cliente
    struct hostent *servidor;             // Aqui vai manter o nome do servidor
    struct sockaddr_in endereco_servidor; // Aqui será mantido alguns dados do servidor para conexão apenas
    struct timeval tv;                    // Tempo limite de resposta
    servidor = gethostbyname(argv[1]);    // tenta achar o servidor pelo ip. Lembrando que localhost é 127.0.0.1

    // tempo de timeout
    tv.tv_sec = 30;
    tv.tv_usec = 0;

    iniciaTabuleiro(tabuleiro);

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
    // Define a porta da conexão
    endereco_servidor.sin_port = htons(porta);
    printf("Estabilish struture\n");
    // Tenta conectar ao servidor, mas se não conseugir é um problema
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
        std::cerr << "Error: " << strerror(errno) << std::endl; 
        exit(1);
    }
    estado = ntohs(estado);
    int eu_sou = '-';
    int estado_do_jogo = 0;
    if (estado == PRIMEIRO_A_JOGAR)
    {
        eu_sou = 'X';
        while (1)
        {
            fazJogada(socket_servidor, tabuleiro, eu_sou);
            printTabuleiro(tabuleiro);

            //printf("Waiting to the other player make a move\n");
            std::cout << "Waiting to the other player make a move\n" << std::endl;
            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                std::cerr << "Error: " << strerror(errno) << std::endl; 
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VITORIA)
            {
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DERROTA)
            {
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == EMPATE)
            {
                printf("%s\nEMPATE... :|%s\n\n", YELLOW, RESET);
                close(socket_servidor);
                exit(0);
            }

            verifica_erro = read(socket_servidor, &linha, sizeof(linha));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de linha, o programa será encerrado.");
                std::cerr << ERROR_LINE  <<"Error: " << strerror(errno) << std::endl; 
                exit(1);
            }
            verifica_erro = read(socket_servidor, &coluna, sizeof(coluna));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de coluna, o programa será encerrado.");
                std::cerr << ERROR_COLUMN << "Error whlie reading the column, the program will be stopped. Error: " << strerror(errno) <<std::endl; 

                exit(1);
            }
            linha = ntohs(linha);
            coluna = ntohs(coluna);
            tabuleiro[linha][coluna] = 'O';
            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                std::cerr << "Error whlie reading the game state, the program will be stopped. Error: " << strerror(errno) << std::endl;  
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VITORIA)
            {
                printTabuleiro(tabuleiro);
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DERROTA)
            {
                printTabuleiro(tabuleiro);
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == EMPATE)
            {
                printTabuleiro(tabuleiro);
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
            verifica_erro = read(socket_servidor, &linha, sizeof(linha));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de linha, o programa será encerrado.");
                std::cerr << ERROR_LINE << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            verifica_erro = read(socket_servidor, &coluna, sizeof(coluna));

            // Caso o resultado seja -1 há um erro de leitura
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do dado de coluna, o programa será encerrado.");
                //perror("Error whlie reading the line, the program will be stopped.");
                std::cerr << ERROR_COLUMN << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }

            linha = ntohs(linha);
            coluna = ntohs(coluna);
            tabuleiro[linha][coluna] = 'X';

            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                //perror("Error whlie reading the game state, the program will be stopped.");
                std::cerr << ERROR_GAME_STATE << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VITORIA)
            {
                printTabuleiro(tabuleiro);
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DERROTA)
            {
                printTabuleiro(tabuleiro);
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == EMPATE)
            {
                printTabuleiro(tabuleiro);
                printf("%s\nEMPATE... :|%s\n\n", YELLOW, RESET);
                close(socket_servidor);
                exit(0);
            }

            fazJogada(socket_servidor, tabuleiro, eu_sou);
            printTabuleiro(tabuleiro);

            verifica_erro = read(socket_servidor, &estado_do_jogo, sizeof(estado_do_jogo));
            if (verifica_erro < 0)
            {
                //perror("Houve um erro na leitura do estado do jogo, o programa será encerrado.");
                std::cerr << "Error whlie reading the game state, the program will be stopped. Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            estado_do_jogo = ntohs(estado_do_jogo);
            if (estado_do_jogo == VITORIA)
            {
                printf("\n%sVICTORY!!! :D %s\n\n", GREEN, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == DERROTA)
            {
                printf("\n%sDEFEAT... :(%s\n\n", RED, RESET);
                close(socket_servidor);
                exit(0);
            }
            else if (estado_do_jogo == EMPATE)
            {
                printf("%s\nTIE... :|%s\n\n", YELLOW, RESET);
                close(socket_servidor);
                exit(0);
            }
        }
    }
    return 1;
}