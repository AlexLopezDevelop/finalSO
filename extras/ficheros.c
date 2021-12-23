//
// Created by Alex Lopez on 22/10/21.
//

#include "ficheros.h"
#include <stdio.h>
#include "funciones.h"

int leerFichero(char *pathFile, Configuracion *config) {

    int fd;
    char lineaFile[250];

    fd = open(pathFile, O_RDONLY);

    if (errorAbrir(fd, pathFile)) {
        return 1;
    }

    while (!checkEOF(fd)) {
        config->tiempoLimpieza = atoi(readLineFile(fd, '\n'));
        strcpy(lineaFile, readLineFile(fd, '\n'));
        config->ip = malloc(sizeof(char) * strlen(lineaFile) + 1);
        strcpy(config->ip, lineaFile);
        config->puerto = atoi(readLineFile(fd, '\n'));
        strcpy(lineaFile, readLineFile(fd, '\n'));
        config->directorio = malloc(sizeof(char) * strlen(lineaFile) + 1);
        strcpy(config->directorio, lineaFile);

    }

    close(fd);

    return 0;
}