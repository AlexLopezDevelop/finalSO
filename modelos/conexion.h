//
// Created by Alex Lopez on 4/1/22.
//

#ifndef FINALSO_CONEXION_H
#define FINALSO_CONEXION_H

#include "configuracion.h"

typedef struct {
    char origen[TRAMA_ORIGEN_SIZE];
    char tipo;
    char datos[TRAMA_DATA_SIZE];
} ConexionData;


typedef struct {
    char * nombre;
    int size;
    int sizeTrama;
    int totalTramas;
    char * md5sum;
    char **tramas;
} FotoData;

#endif //FINALSO_CONEXION_H
