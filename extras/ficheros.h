//
// Created by Alex Lopez on 22/10/21.
//

#ifndef PRACTICAFINALSO_FICHEROS_H
#define PRACTICAFINALSO_FICHEROS_H

#include "../modelos/configuracion.h"
#include "../modelos/conexion.h"

int ficheros_leer_fichero(char *pathFile, Configuracion *config);

ConexionData *ficheros_guardar_trama(const char *trama);

FotoData *ficheros_destruct_data_imagen(char *datos);

#endif //PRACTICAFINALSO_FICHEROS_H
