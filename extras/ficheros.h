//
// Created by Alex Lopez on 22/10/21.
//

#ifndef PRACTICAFINALSO_FICHEROS_H
#define PRACTICAFINALSO_FICHEROS_H

#include "../modelos/configuracion.h"
#include "../modelos/conexion.h"

int leerFichero(char *pathFile, Configuracion *config);

ConexionData *guardarTrama(const char *trama);

FotoData *destructDataImagen(char *datos);

#endif //PRACTICAFINALSO_FICHEROS_H
