//
// Created by Alex Lopez on 22/10/21.
//

#include "comandos.h"
#include "funciones.h"
#include "../modelos/configuracion.h"
#include "../modelos/conexion.h"
#include "ficheros.h"

#include <string.h>
#include <stdbool.h>

int establecerConexion() {

    int socketFD;
    struct sockaddr_in servidor;


    if ((socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        funciones_display("Error creant el socket\n");
    } else {
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8755);

    if (inet_pton(AF_INET, "127.0.0.1", &servidor.sin_addr) < 0) {
        funciones_display("Error configurant IP\n");
    } else {
    }

    if (connect(socketFD, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
        funciones_display("Error fent el connect\n");
    } else {
    }

    return socketFD;

}

int extraerIdTrama(char *tramaRespuesta) {
    int idUSer = 0;
    int dataIndex = 0;
    char *aux;
    aux = malloc(sizeof(char) * TRAMA_DATA_SIZE);

    for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
        aux[dataIndex] = tramaRespuesta[i];
        dataIndex++;
        if (tramaRespuesta[i] == '\0') {
            idUSer = atoi(aux);
            break;
        }
    }
    return idUSer;
}

char *crearTrama(char *origen, char tipo, char *data) {
    char *trama = NULL;
    trama = malloc(sizeof(char) * MAX_TRAMA_SIZE);

    int tipoSize = 1;
    int origenSize = strlen(origen);

    // origen
    for (int i = 0; i < TRAMA_ORIGEN_SIZE; i++) {
        if (i < origenSize) {
            trama[i] = origen[i];
        } else {
            trama[i] = '\0';
        }
    }

    // tipo
    trama[TRAMA_ORIGEN_SIZE] = tipo;

    // data
    int dataIndex = 0;

    if (tipo == 'D') { // es binario
        for (int i = TRAMA_ORIGEN_SIZE + tipoSize; i < MAX_TRAMA_SIZE; i++) {
            trama[i] = data[dataIndex];
            dataIndex++;
        }
    } else {
        int dataSize = strlen(data);

        for (int i = TRAMA_ORIGEN_SIZE + tipoSize; i < MAX_TRAMA_SIZE; i++) {
            if (dataIndex < dataSize) {
                trama[i] = data[dataIndex];
                dataIndex++;
            } else {
                trama[i] = '\0';
            }
        }
    }


    return trama;
}

char *comandos_obtener_trama(char tipo, char *data) {
    return crearTrama("FREMEN", tipo, data);
}


ListadoUsuarios *destructTramaRespuesta(char *tramaRespuesta) {
    ListadoUsuarios *listadoUsuarios = malloc(sizeof(ListadoUsuarios));
    char *dataTrama;
    int dataIndex = 0;
    char *lineaFile;

    dataTrama = malloc(sizeof(char) * TRAMA_DATA_SIZE);
    for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
        dataTrama[dataIndex] = tramaRespuesta[i];
        dataIndex++;
    }

    //total
    lineaFile = strdup((funciones_read_string_to(dataTrama, '*')));
    listadoUsuarios->total = atoi(lineaFile);
    free(lineaFile);
    //reservamos memoria con el total
    listadoUsuarios->usuarios = malloc(sizeof(Usuario) * listadoUsuarios->total);

    // init
    char *auxString = malloc(sizeof(char));
    strcpy(auxString, "");
    int tramaIndex = listadoUsuarios->total;
    int auxIndex = 0;

    int sizeTrama = strlen(dataTrama) + 1;
    bool loopName = true;
    bool loopId = true;

    // usuarios
    for (int i = 0; i < listadoUsuarios->total; ++i) {

        // nombre
        while (loopName) {
            auxString = realloc(auxString, sizeof(char) * (auxIndex + 1));
            auxString[auxIndex] = dataTrama[tramaIndex];
            auxIndex++;
            tramaIndex++;


            if (dataTrama[tramaIndex] == '*' || dataTrama[tramaIndex] == '\0' || tramaIndex >= sizeTrama) {
                loopName = false;
                auxString = realloc(auxString, sizeof(char) * (auxIndex + 1));
                auxString[auxIndex] = '\0';
            }
        }

        listadoUsuarios->usuarios[i].nombre = strdup(auxString);

        free(auxString);
        auxString = malloc(sizeof(char));
        auxIndex = 0;
        tramaIndex++;
        loopName = true;

        // id
        while (loopId) {
            auxString = realloc(auxString, sizeof(char) * (auxIndex + 1));
            auxString[auxIndex] = dataTrama[tramaIndex];
            auxIndex++;
            tramaIndex++;
            if (dataTrama[tramaIndex] == '*' || dataTrama[tramaIndex] == '\0' || tramaIndex >= sizeTrama) {
                loopId = false;
                auxString = realloc(auxString, sizeof(char) * (auxIndex + 1));
                auxString[auxIndex] = '\0';
            }
        }

        listadoUsuarios->usuarios[i].id = atoi(auxString);

        free(auxString);
        auxString = malloc(sizeof(char));
        auxIndex = 0;
        tramaIndex++;
        loopId = true;
    }

    return listadoUsuarios;
}

int comandosPropios(char **instruccion, int totalParams, int socketFD, Usuario *usuario) {
    int i = 0;
    char print[200];

    char *comando;
    comando = strdup(instruccion[0]);

    // Pasar a mayusculas
    while (comando[i] != '\0') {
        if (comando[i] >= 'a' && comando[i] <= 'z') {
            comando[i] = comando[i] - 32;
        }
        i++;
    }

    if (strcmp("LOGIN", comando) == 0) {
        if (totalParams == 2) {
            socketFD = establecerConexion();
            //char *data = funciones_concat_strings_por_asterico(instruccion[1], instruccion[2]);
            char *data;
            asprintf(&data, "%s*%s", instruccion[1], instruccion[2]);
            char *trama = comandos_obtener_trama('C', data);
            write(socketFD, trama, MAX_TRAMA_SIZE);


            char tramaRespuesta[MAX_TRAMA_SIZE];
            read(socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

            if (tramaRespuesta[15] == 'O') {
                //idUser
                usuario->id = extraerIdTrama(tramaRespuesta);
                //nombreUser
                usuario->nombre = strdup(instruccion[1]);
                //CodigoPostalUser
                usuario->codigoPostal = strdup(instruccion[2]);
                sprintf(print, "Benvingut %s. Tens ID %d.\nAra estàs connectat a Atreides.\n\n", usuario->nombre,
                        usuario->id);
                funciones_display(print);
                usuario->socketFD = socketFD;
            }

        } else {
            funciones_display("Comanda KO. Falta paràmetres\n");
        }
    } else if (strcmp("PHOTO", comando) == 0) {
        if (totalParams == 1) {
            char *data;
            asprintf(&data, "%s", instruccion[1]);
            char *trama = comandos_obtener_trama('P', data);
            write(usuario->socketFD, trama, MAX_TRAMA_SIZE);

            char tramaRespuesta[MAX_TRAMA_SIZE];
            read(usuario->socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

            ConexionData *conexionData = ficheros_guardar_trama(tramaRespuesta);
            FotoData *fotoData = ficheros_destruct_data_imagen(conexionData->datos);

            bool descargandoImagen = true;
            char tramaImagen[MAX_TRAMA_SIZE];

            fotoData->totalTramas = fotoData->size / TRAMA_DATA_SIZE;
            if (fotoData->size % TRAMA_DATA_SIZE != 0) {
                fotoData->totalTramas++;
            }

            while (descargandoImagen) {
                memset(tramaImagen, 0, TRAMA_DATA_SIZE);
                memset(conexionData, 0, sizeof (ConexionData));
                read(usuario->socketFD, tramaImagen, MAX_TRAMA_SIZE);

                int fd;

                fd = open(fotoData->nombre, O_WRONLY | O_CREAT | O_APPEND, 00666);

                if (funciones_error_abrir(fd, fotoData->nombre)) {
                    funciones_display("Error al guardar la imagen\n");
                }

                conexionData = ficheros_guardar_trama(tramaImagen);

                if (fotoData->size % TRAMA_DATA_SIZE != 0 && (fotoData->totalTramas - 1) == i) {
                    write(fd, conexionData->datos, sizeof(char) * (fotoData->size % TRAMA_DATA_SIZE));
                    i = 0;
                    descargandoImagen = false;
                } else {
                    write(fd, conexionData->datos, sizeof(char) * TRAMA_DATA_SIZE);
                    i++;
                }

                close(fd);
            }

            funciones_display("Foto descargada :)\n");

        } else {
            funciones_display("Comanda KO. Massa paràmetres\n");
        }
    } else if ((strcmp("SEARCH", comando) == 0)) {
        if (totalParams == 1) {
            if (usuario->socketFD > 0) {
                char idString[20];
                sprintf(idString, "%d", usuario->id);
                char *data;
                asprintf(&data, "%s*%s*%s", usuario->nombre, idString, instruccion[1]);
                char *trama = comandos_obtener_trama('S', data);

                write(usuario->socketFD, trama, MAX_TRAMA_SIZE);

                char tramaRespuesta[MAX_TRAMA_SIZE];
                read(usuario->socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

                if (tramaRespuesta[15] == 'L') {
                    char auxid[30];

                    ListadoUsuarios *listadoUsuarios = destructTramaRespuesta(tramaRespuesta);

                    funciones_display("\n");
                    sprintf(print, "Hi han %d persones humanes a %s\n", listadoUsuarios->total, instruccion[1]);
                    funciones_display(print);
                    funciones_display("\n");

                    for (int j = 0; j < listadoUsuarios->total; ++j) {
                        sprintf(auxid, "%d", listadoUsuarios->usuarios[j].id);
                        sprintf(print, "%s ", auxid);
                        funciones_display(print);
                        funciones_display(listadoUsuarios->usuarios[j].nombre);
                        funciones_display("\n");
                    }

                    funciones_display("\n");
                } else if (tramaRespuesta[15] == 'K') {
                    funciones_display("No hay ningun usuario con este codigo postal\n");
                }
            } else {
                funciones_display("Has de realizar el login primero\n");
            }
        } else {
            funciones_display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("SEND", comando) == 0) {
        if (totalParams == 1) {
            char sizeFileString[100];


            int sizeFile = funciones_get_file_size(instruccion[1]);
            sprintf(sizeFileString, "%d", sizeFile);

            char *md5File = funciones_generate_md5sum(instruccion[1]);

            char *data;
            asprintf(&data, "%s*%s*%s", instruccion[1], sizeFileString, md5File);

            char *trama = comandos_obtener_trama('F', data);
            write(usuario->socketFD, trama, MAX_TRAMA_SIZE);

            funciones_send_image(usuario->socketFD, instruccion[1]);

            char tramaRespuesta[MAX_TRAMA_SIZE];
            read(usuario->socketFD, tramaRespuesta, MAX_TRAMA_SIZE);
            if (tramaRespuesta[15] == 'I') {
                funciones_display("IMAGE OK\n");
            } else if (tramaRespuesta[15] == 'R') {
                funciones_display("IMAGE KO\n");
            }



        } else {
            funciones_display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("LOGOUT", comando) == 0) {
        if (strcmp("LOGOUT", comando) == 0) {
            if (usuario->socketFD > 0) {
                char userId[20];
                sprintf(userId, "%d", usuario->id);
                char *data;
                asprintf(&data, "%s*%s", usuario->nombre, userId);
                char *trama = comandos_obtener_trama('Q', data);
                write(usuario->socketFD, trama, MAX_TRAMA_SIZE);
                funciones_display("Desconectado de Atreides! Dew!\n");

                exit(0);
            } else {
                funciones_display("Has de realizar el login primero\n");
            }
        } else {
            funciones_display("Comanda KO. Massa paràmetres\n");
        }
    } else {
        return 1;
    }

    return 0;
}

_Noreturn void comandos_pedir_instruccion() {
    int socketFD = -1;
    Usuario *usuario = malloc(sizeof(Usuario));
    while (1) {

        char entradaUsuario[40];
        funciones_display("$ ");
        read(0, entradaUsuario, 300);

        entradaUsuario[strcspn(entradaUsuario, "\n") + 1] = '\0';

        int totalParams = 0;

        //  Separar instruccion y parametros
        int i = 0;
        char *aux = malloc(sizeof(char));
        char caracter = '\0';
        int lenEntradaUser = sizeof(entradaUsuario);
        char **paramList = NULL;
        paramList = (char **) malloc((sizeof(char *)));

        for (int j = 0; j < lenEntradaUser && caracter != '\n'; ++j) {
            caracter = entradaUsuario[j];
            aux[i] = caracter;
            i++;

            if (caracter == ' ' || caracter == '|' || caracter == '\n') {

                aux[i - 1] = '\0';
                paramList[totalParams] = strdup(aux);

                totalParams++;
                paramList = realloc(paramList, sizeof(char *) * (totalParams + 1));
                i = 0;
                funciones_liberar_memoria(aux);
                aux = malloc(sizeof(char));
            } else {
                aux = (char *) realloc(aux, (i + 1));
            }
        }

        funciones_liberar_memoria(aux);
        // END Separar instruccion y parametros

        // Añadir el NULL al final del params
        paramList[totalParams] = NULL;

        if (comandosPropios(paramList, totalParams - 1, socketFD, usuario)) {

            funciones_display("Comanmdo: ");
            funciones_display(paramList[0]);
            funciones_display("\n");

            // init fork
            pid_t son_pid;
            son_pid = fork();

            switch (son_pid) {
                case -1: // Error
                    funciones_display("Error en el fork!\n");
                    break;
                case 0: // Hijo
                    funciones_display("------------------------------------------------------------\n");

                    if (execvp(paramList[0], paramList) == -1) {
                        funciones_display("No se ha encontrado el comando ");
                        funciones_display(paramList[0]);
                        funciones_display("\n");
                    }

                    funciones_display("------------------------------------------------------------\n");
                    break;
                default: // Padre
                    waitpid(son_pid, NULL, 0);
                    funciones_display("------------------------------------------------------------\n");
                    for (int j = 0; j < totalParams; j++) {
                        funciones_liberar_memoria(paramList[j]);
                    }
                    funciones_liberar_memoria(paramList);
                    funciones_liberar_memoria(aux);
                    funciones_liberar_memoria(usuario->nombre);
                    funciones_liberar_memoria(usuario->codigoPostal);
                    funciones_liberar_memoria(usuario);
                    break;
            }
        }
    }
}