//
// Created by Alex Lopez on 22/10/21.
//

#include "ficheros.h"
#include <stdio.h>
#include "funciones.h"

int leerFichero(char *pathFile, Configuracion *config) {

    int fd;
    char *lineaFile;

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