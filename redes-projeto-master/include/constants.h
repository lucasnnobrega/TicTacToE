#ifndef CONSTANTS_H
#define CONSTANTS_H

#define RESET "\033[00m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW ""

// ERROR STRINGS
#define ERROR_LINE               "Error while reading the row, the program will be stopped."
#define ERROR_COLUMN             "Error while reading the column, the program will be stopped."
#define ERROR_GAME_STATE         "Error whlie reading the game state, the program will be stopped."
#define ERROR_SEND_COLUMN_CLIENT "Error while sending data column to client, the program will be stopped."
#define ERROR_RESEND_COL_CLIENT  "Error while resend data column to client, the program will be stopped."
#define ERROR_RESEND_LIN_CLIENT  "Error while resend data row to client, the program will be stopped."

#endif //CONSTANTS