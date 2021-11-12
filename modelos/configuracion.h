//
// Created by Alex Lopez on 22/10/21.
//

#ifndef PRACTICAFINALSO_CONFIGURACION_H
#define PRACTICAFINALSO_CONFIGURACION_H

typedef struct {
    int tiempoLimpieza;
    char ip[50];
    int puerto;
    char directorio[100];
} ConfiguracionConfig;

typedef struct {
    char ip[50];
    int puerto;
    char directorio[100];
} ConfiguracionArtreides;

#endif //PRACTICAFINALSO_CONFIGURACION_H
