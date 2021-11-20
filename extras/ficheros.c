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
        strcpy(config->ip, readLineFile(fd, '\n'));
        config->puerto = atoi(readLineFile(fd, '\n'));
        strcpy(config->directorio, readLineFile(fd, '\n'));

    }

    return 0;
}