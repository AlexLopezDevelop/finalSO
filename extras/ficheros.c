//
// Created by Alex Lopez on 22/10/21.
//

#include "ficheros.h"
#include <stdio.h>
#include "funciones.h"

int leerFichero(char *pathFile, Configuracion *config) {

    int fd;

    fd = open(pathFile, O_RDONLY);

    if (errorAbrir(fd, pathFile)) {
        return 1;
    }

    while (!checkEOF(fd)) {
        config->tiempoLimpieza = atoi(readLineFile(fd, '\n'));
        config->ip = strdup(readLineFile(fd, '\n'));
        config->puerto = atoi(readLineFile(fd, '\n'));
        config->directorio = strdup(readLineFile(fd, '\n'));
    }

    close(fd);

    return 0;
}


/*
 *
 *char *readLineFile(int fd, char hasta) {
    int i = 0, size;
    char c = '\0';
    char *string = (char *) malloc(sizeof(char));

    while (1) {
        size = read(fd, &c, sizeof(char));

        if (c != hasta && size > 0) {
            string = (char *) realloc(string, sizeof(char) * (i + 2));
            string[i++] = c;
        } else {
            break;
        }

    }

    string[i] = '\0';

    return string;

}
 *
 */