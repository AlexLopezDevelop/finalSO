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

void leerFicheroArtreides(char *pathFile, ConfiguracionArtreides *configArtreides) {

    int fd;

    fd = open(pathFile, O_RDONLY);

    errorAbrir(fd, pathFile);

    while (!checkEOF(fd)) {
        strcpy(configArtreides->ip, readLineFile(fd, '\n'));
        configArtreides->puerto = atoi(readLineFile(fd, '\n'));
        strcpy(configArtreides->directorio, readLineFile(fd, '\n'));

    }

}