//
// Created by Alex Lopez on 22/10/21.
//

#include "ficheros.h"
#include "funciones.h"

int ficheros_leer_fichero(char *pathFile, Configuracion *config) {

    int fd;

    fd = open(pathFile, O_RDONLY);

    if (funciones_error_abrir(fd)) {
        return 1;
    }

    while (!funciones_check_eof(fd)) {
        config->tiempoLimpieza = atoi(funciones_read_line_file(fd, '\n'));
        config->ip = strdup(funciones_read_line_file(fd, '\n'));
        config->puerto = atoi(funciones_read_line_file(fd, '\n'));
        config->directorio = strdup(funciones_read_line_file(fd, '\n'));
    }

    close(fd);

    return 0;
}

ConexionData *ficheros_guardar_trama(const char *trama) {
    ConexionData *conexionData;
    conexionData = malloc(sizeof(ConexionData));

    // obtener origen
    for (int i = 0; i < TRAMA_ORIGEN_SIZE; ++i) {
        conexionData->origen[i] = trama[i];
        if (trama[i] == '\0') {
            break;
        }
    }

    // obtener tipo
    conexionData->tipo = trama[TRAMA_ORIGEN_SIZE];

    // obtener data
    int dataIndex = 0;

    for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
        conexionData->datos[dataIndex] = trama[i];
        dataIndex++;
    }

    return conexionData;
}

FotoData *ficheros_destruct_data_imagen(char *datos) {
    FotoData *fotoData = malloc(sizeof(FotoData));
    char delim[] = "*";
    char *ptr = strtok(datos, delim);

    while (ptr != NULL) {
        fotoData->nombre = strdup(ptr);
        ptr = strtok(NULL, delim);
        fotoData->size = atoi(ptr);
        ptr = strtok(NULL, delim);
        fotoData->md5sum = strdup(ptr);
        ptr = strtok(NULL, delim);
    }

    fotoData->sizeTrama = fotoData->size / TRAMA_DATA_SIZE;

    funciones_liberar_memoria(ptr);

    return fotoData;
}