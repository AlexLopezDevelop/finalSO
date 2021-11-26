//
// Created by Alex Lopez on 22/10/21.
//

#ifndef PRACTICAFINALSO_CONFIGURACION_H
#define PRACTICAFINALSO_CONFIGURACION_H

#define MAX_TRAMA_SIZE 256
#define TRAMA_ORIGEN_SIZE 15
#define TRAMA_DATA_SIZE 240

typedef struct {
    int tiempoLimpieza;
    char ip[50];
    int puerto;
    char directorio[100];
} Configuracion;

typedef struct {
    char * nombre;
    char * codigoPostal;
    int id;
}Usuario;

#endif //PRACTICAFINALSO_CONFIGURACION_H
