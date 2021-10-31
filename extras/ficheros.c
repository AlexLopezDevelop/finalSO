//
// Created by Alex Lopez on 22/10/21.
//

#include "ficheros.h"
#include <stdio.h>
#include "funciones.h"

void leerFichero(char *pathFile, ConfiguracionConfig *config) {

    int fd;

    fd = open(pathFile, O_RDONLY);

    errorAbrir(fd, pathFile);

    while (!checkEOF(fd)) {
        config->tiempoLimpieza = atoi(readLineFile(fd, '\n'));
        strcpy(config->ip, readLineFile(fd, '\n'));
        config->puerto = atoi(readLineFile(fd, '\n'));
        strcpy(config->directorio, readLineFile(fd, '\n'));

    }

}