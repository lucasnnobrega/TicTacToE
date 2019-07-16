/**
 * Neste arquivo serão contidas as funções relacionadas ao servidor
 *
**/
#include "../include/generic_functions.h"
/**
 *
**/

int leMove(int socket_sender, int *socket_jogadores, char board[3][3], int turno, int turn_counter)
{

    int verificador_de_erro, // Variável que vai ser usada para verificar se houve erro na função
        row,               // Variável que armazena o valor da row
        column;              // Variável que armazena o valor da column

    char tipo; // Vai ser 'X' ou 'O'

    int estadoDoJogo = 0; //Vai guardar o estado do jogo

    // Aqui eu estou lendo, do socket_sender, o valor da row.
    // sizeof(row) é usado para dizer o tamanho do dado sendo lido.
    // Uma vez que este pode variar
    verificador_de_erro = read(socket_sender, &row, sizeof(row));

    // Caso o resultado seja -1 há um erro de leitura
    if (verificador_de_erro < 0)
    {
        //perror("Houve um erro na leitura do dado de row, o programa será encerrado.");
        std::cerr << "Error reading the row data, the program will be stopped. Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Aqui eu estou lendo, do socket_sender, o valor da column.
    // sizeof(column) é usado para dizer o tamanho do dado sendo lido.
    // Uma vez que este pode variar
    verificador_de_erro = read(socket_sender, &column, sizeof(column));

    // Caso o resultado seja -1 há um erro de leitura
    if (verificador_de_erro < 0)
    {
        //perror("Houve um erro na leitura do dado de column, o programa será encerrado.");
        std::cerr << "Error read column data. The program will be stopped. Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Como o servidor é central precisamos avisar ao outro jogador qual foi a move
    // Dessa forma, basicamente, vamos repassar essa mensagem para o outro jogador
    // Os dados ainda estão no formato de network nesse momento dessa forma basta
    // repassar eles
    // Mas primeiro precisamos saber para qual jogador mandar.
    // O socket_sender indica qual o jogador que mandou a mensagem
    if (socket_sender == socket_jogadores[0])
    {
        verificador_de_erro = write(socket_jogadores[1], &row, sizeof(row));
        // Caso o resultado seja -1 há um erro de escrita
        if (verificador_de_erro < 0)
        {
            //perror("Houve um erro no reeenvio do dado de row para o cliente, o programa será encerrado.");
            std::cerr << ERROR_RESEND_LIN_CLIENT <<"Error: " << strerror(errno) << std::endl;
            exit(1);
        }
        verificador_de_erro = write(socket_jogadores[1], &column, sizeof(column));
        // Caso o resultado seja -1 há um erro de escrita
        if (verificador_de_erro < 0)
        {
            //perror("Houve um erro no reeenvio do dado de column para o cliente, o programa será encerrado.");
            std::cerr << ERROR_RESEND_COL_CLIENT << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }
    }
    else
    {
        verificador_de_erro = write(socket_jogadores[0], &row, sizeof(row));
        // Caso o resultado seja -1 há um erro de escrita
        if (verificador_de_erro < 0)
        {
            //perror("Houve um erro no reeenvio do dado de row para o cliente, o programa será encerrado.");
            std::cerr << ERROR_RESEND_LIN_CLIENT << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }
        verificador_de_erro = write(socket_jogadores[0], &column, sizeof(column));
        // Caso o resultado seja -1 há um erro de escrita
        if (verificador_de_erro < 0)
        {
            //perror("Houve um erro no reeenvio do dado de column para o cliente, o programa será encerrado.");
            std::cerr << ERROR_RESEND_COL_CLIENT << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }
    }
    // O valor foi convertido para um formato de envio e dessa forma
    // necessita ser convertido de volta para um valor trabalhavel
    row = ntohs(row);
    column = ntohs(column);

    // Agora precisamoe saber se é o jogador "X" ou o jogador "O", para isto utilizamos
    // A variável turno, a qual é verdadeira para o jogador "X" e falta para o
    // jogador "O".
    board[row][column] = turno ? 'O' : 'X';

    //Agora irá atualizar o board
    printBoard(board);

    if (turno)
    {
        tipo = 'O';
    }
    else
    {
        tipo = 'X';
    }

    //Depois que foi realizado a move, verifica se o cliente em questão ganhou o jogo (VICTORY), se ocorreu um empate (TIE) ou se não aconteceu nada e o jogo deve continuar (CONTINUE)
    estadoDoJogo = verifyGameState(board, turn_counter, tipo);
    if (estadoDoJogo == CONTINUE || estadoDoJogo == TIE)
    {
        int temp = estadoDoJogo;
        estadoDoJogo = htons(estadoDoJogo);
        verificador_de_erro = write(socket_jogadores[1], &estadoDoJogo, sizeof(estadoDoJogo));
        // Caso o resultado seja -1 há um erro de escrita
        if (verificador_de_erro < 0)
        {
            //perror("Houve um erro no reenvio do dado de column para o cliente, o programa será encerrado.");
            std::cerr << ERROR_SEND_COLUMN_CLIENT << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }
        estadoDoJogo = temp;
        estadoDoJogo = htons(estadoDoJogo);
        verificador_de_erro = write(socket_jogadores[0], &estadoDoJogo, sizeof(estadoDoJogo));
        // Caso o resultado seja -1 há um erro de escrita
        if (verificador_de_erro < 0)
        {
            //perror("Houve um erro no reeenvio do dado de column para o cliente, o programa será encerrado.");
            std::cerr << ERROR_SEND_COLUMN_CLIENT << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }
    }
    else if (estadoDoJogo == VICTORY)
    {
        if (socket_sender == socket_jogadores[0])
        {
            estadoDoJogo = htons(estadoDoJogo);
            verificador_de_erro = write(socket_jogadores[0], &estadoDoJogo, sizeof(estadoDoJogo));
            // Caso o resultado seja -1 há um erro de escrita
            if (verificador_de_erro < 0)
            {
                //perror("Houve um erro no reeenvio do dado de column para o cliente, o programa será encerrado.");
                std::cerr << ERROR_RESEND_COL_CLIENT << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            estadoDoJogo = DEFEAT;
            estadoDoJogo = htons(estadoDoJogo);
            verificador_de_erro = write(socket_jogadores[1], &estadoDoJogo, sizeof(estadoDoJogo));
            // Caso o resultado seja -1 há um erro de escrita
            if (verificador_de_erro < 0)
            {
                //perror("Houve um erro no reeenvio do dado de column para o cliente, o programa será encerrado.");
                std::cerr << ERROR_RESEND_COL_CLIENT << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
        }
        else
        {
            estadoDoJogo = htons(estadoDoJogo);
            verificador_de_erro = write(socket_jogadores[1], &estadoDoJogo, sizeof(estadoDoJogo));
            // Caso o resultado seja -1 há um erro de escrita
            if (verificador_de_erro < 0)
            {
                //perror("Houve um erro no reeenvio do dado de column para o cliente, o programa será encerrado.");
                std::cerr << ERROR_RESEND_COL_CLIENT << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
            estadoDoJogo = DEFEAT;
            estadoDoJogo = htons(estadoDoJogo);
            verificador_de_erro = write(socket_jogadores[0], &estadoDoJogo, sizeof(estadoDoJogo));
            // Caso o resultado seja -1 há um erro de escrita
            if (verificador_de_erro < 0)
            {
                //perror("Houve um erro no reeenvio do dado de column para o cliente, o programa será encerrado.");
                std::cerr << ERROR_RESEND_COL_CLIENT << "Error: " << strerror(errno) << std::endl;
                exit(1);
            }
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    //Verifica argumentos
    if (argc != 2)
    {
        fprintf(stderr, "Error: number of arguments invalid\n");
        fprintf(stderr, "server program need 2 arguments e.g:\n");
        fprintf(stderr, "./server port\n");
        exit(1);
    }

    int socket_jogadores[2], // Armazena o socket do jogador 1 e 2
        socket_servidor,     // Armazena o socket do servidor
        turno,               // Define de quem é o turno
        turn_counter;     // Conta quantos turnos se passaram

    char board[3][3];                                      // array que controla o board
    turn_counter = 0;                                       // Vai contar os turnos do jogador
    turno = 0;                                                 // vai dizer quem é o primeiro a jogar
    int verificador_de_erro;                                   // verifica se operações deram erro
    setlocale(LC_ALL, "Portuguese");                           // Essa estrutura é definida pelo socket
    struct sockaddr_in endereco_servidor, endereco_cliente[2]; // As variáveis irão manipular, respectivamente, os endereços do servidor e do cliente.
    struct timeval tv;                                         // Tempo limite de resposta

    // tempo de timeout
    tv.tv_sec = 30;
    tv.tv_usec = 0;

    startBoard(board);

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    // Caso o resultado seja -1 há um erro de leitura
    if (socket_servidor < 0)
    {
        //perror("Houve um erro ao criar o servidor.");
        std::cerr << "Error while create server. Error: " << strerror(errno) << std::endl;
        exit(1);
    }
    // Define o valor start para a port como 1025
    int port = atoi(argv[1]);

    // Seta todos os bytes para 0
    memset((struct sockaddr_in *)&endereco_servidor, 0, sizeof(endereco_servidor));

    // Define a estrutura do endereço do servidor.
    //
    endereco_servidor.sin_family = AF_INET;         // endereço da família
    endereco_servidor.sin_addr.s_addr = INADDR_ANY; // define que vai aceitar qualquer endereço da internet

    do
    {
        endereco_servidor.sin_port = htons(port); // define the port used by the server
        // Não queremos selecionar a port então usamos esse código para fazer o computador
        // procurar por uma port livre para usar. A partir da 1025 não há proteção do sistema sobre
        // a port
        if (bind(socket_servidor, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor)) < 0){
            port++;
        }
        else{
            // sai ao achar uma port
            break; 
        }
    } while (1);

    // É necessário saber o valor da port do servidor por isso ela será printada.
    printf("The port number is: %d\n", port);
    // Põe o servidor para esperar por clientes, o numero inteiro indica quantos clientes podem conectar
    // listen(socket_servidor, 2);

    // conta quantos clientes conectaram até o momento
    int clientes_conectados = 0;
    // É preciso fazer isto para a função de accept, basicamente é o tipo size_t do socket
    // É um unsigned int no fim de tudo.
    // Endereco_cliente foi mudado para um array e como ambos são do mesmo tipo,
    // Foi melhor usar endereco_servidor
    socklen_t tamanho_endereco = sizeof(endereco_servidor);

    printf("Waiting players conections\n");
    while (clientes_conectados < 2)
    {
        listen(socket_servidor, 2);
        // Aqui vamos receber uma solicitação de cliente
        // Se tiver vagas ela vai ser aceita.
        // o casting do segundo parâmetro é algo necessário para converter sockaddr_in para sockaddr
        socket_jogadores[clientes_conectados] = accept(socket_servidor, (struct sockaddr *)&endereco_cliente[clientes_conectados], &tamanho_endereco);
        if (socket_jogadores[clientes_conectados] < 0)
        {
            //perror("Houve um erro na conexão do cliente com o servidor.");
            std::cerr << "Error in conection between client and server. Error: " << strerror(errno) << std::endl;
        }
        else
        {
            // adiciona timeout no socket
            setsockopt(socket_jogadores[clientes_conectados], SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
            setsockopt(socket_jogadores[clientes_conectados], SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof tv);
            clientes_conectados++;
        }

        printf("%sConected %d\n%s", GREEN, clientes_conectados, RESET);

        if (clientes_conectados == 1)
        {
            int define_ordem = htons(FIRST_TO_PLAY); // variável temporária pq precisa ser um ponteiro no write
            // Avisa ao primeiro a se conectar que ele é o primeiro a jogar
            verificador_de_erro = write(socket_jogadores[clientes_conectados - 1], &define_ordem, sizeof(FIRST_TO_PLAY));
            // Caso o resultado seja -1 há um erro de escrita
            if (verificador_de_erro < 0)
            {
                //perror("Houve um erro no envio da info do primeiro a jogar, o programa será encerrado.");
                std::cerr << "Error in sending the info of the first player, the program will be stopped. Error: " << strerror(errno) << std::endl;
                exit(1);
            }
        }
        else
        {
            // Avisa ao segundo a se conectar que ele é o segundo a jogar
            int define_ordem = htons(SECOND_TO_PLAY);
            verificador_de_erro = write(socket_jogadores[clientes_conectados - 1], &define_ordem, sizeof(SECOND_TO_PLAY));
            // Caso o resultado seja -1 há um erro de escrita
            if (verificador_de_erro < 0)
            {
                //perror("Houve um erro no envio da info do primeiro a jogar, o programa será encerrado.");
                std::cerr << "Error in sending the info of the first player, the program will be stopped. Error: " << strerror(errno) << std::endl;
                exit(1);
            }
        }
    }
    // Não precisa mais ficar esperando jogadores
    close(socket_servidor);
    turn_counter = 0;
    // Servidor começa a ouvir as respostas e esperar
    while (1)
    {
        turn_counter++; // passa o turno
        // Fica esperando move
        leMove(socket_jogadores[turno], socket_jogadores, board, turno, turn_counter);
        turno = !turno; // Passa o turno. O valor ocila entre 1 e 0.
    }
}