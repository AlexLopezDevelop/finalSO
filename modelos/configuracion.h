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
    char * ip;
    int puerto;
    char * directorio;
} Configuracion;

typedef struct {
    char * nombre;
    char * codigoPostal;
    int id;
    int socketFD;
}Usuario;

typedef struct {
    Usuario * usuarios;
    int total;
} ListadoUsuarios;

#endif //PRACTICAFINALSO_CONFIGURACION_H
