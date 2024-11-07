#include <inputParser.h>




void findPipe(char *str, char **part1, char **part2) {
    // Encuentra el delimitador en la cadena
    char *delimiter_position = NULL;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '-') { // tomo como el pipe el - ya que es mas facil de encontrar
            delimiter_position = &str[i];
            break;
        }
    }

    // Si no se encuentra el delimitador, asigna NULL a part2 y deja part1 como la cadena completa
    if (delimiter_position == NULL) {
        *part1 = str;
        *part2 = NULL;
        return;
    }

    // Asigna la primera parte antes del delimitador
    *delimiter_position = '\0';  // Termina la primera parte con '\0'
    *part1 = str;

    // Asigna la segunda parte después del delimitador
    *part2 = delimiter_position + 1;
}


int split(char *str, char argc, char ** args){
    int readParams = 0;
    while (*str != ' ' && *str != '\0') {
        str++;
    }
    *str = '\0';
    str++;

    for (int i = 0; i < argc; ++i) {
        while (*str == ' ') {
            str++;
        }

        int j = 0;
        char * arg = args[i];
        char paramRead = 0;
        while (*str != ' ' && *str != '\0') {
            if(!paramRead){
                readParams++;
                paramRead = 1;
            }
            arg[j] = *str;
            str++;
            j++;
        }
        arg[j] = '\0';
        if(*str == '\0'){
            return readParams;
        }
        str++;
    }
    return readParams;
}