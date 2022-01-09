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
#include <ctype.h>


void comandos_comparar_md5sum(const Usuario *usuario, char *trama, FotoData *fotoData);

int comandos_establecer_conexion() {

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

int comandos_extraer_id_trama(char *tramaRespuesta) {
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

    funciones_liberar_memoria(aux);
    return idUSer;
}

char *comandos_crear_trama(char *origen, char tipo, char *data) {
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
    return comandos_crear_trama("FREMEN", tipo, data);
}


ListadoUsuarios *comandos_destruct_trama_respuesta(char *tramaRespuesta, int socketFD) {
    ListadoUsuarios *listadoUsuarios = malloc(sizeof(ListadoUsuarios));
    char *dataTrama;
    int dataIndex = 0;
    char *lineaFile;
    int totalUsuarios = 0;

    dataTrama = malloc(sizeof(char) * TRAMA_DATA_SIZE);
    for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; i++) {
        dataTrama[dataIndex] = tramaRespuesta[i];
        dataIndex++;
    }
    //total
    lineaFile = strdup((funciones_read_string_to(dataTrama, "*")));
    int tramaIndex = strlen(lineaFile)+1;
    listadoUsuarios->total = atoi(lineaFile);
    funciones_liberar_memoria(lineaFile);
    //reservamos memoria con el total
    listadoUsuarios->usuarios = malloc(sizeof(Usuario) * listadoUsuarios->total);

    // init
    char *auxString = malloc(sizeof(char));
    strcpy(auxString, "");
    int auxIndex = 0;

    int sizeTrama = strlen(dataTrama) + 1;
    bool loopName = true;
    bool loopId = true;

    // usuarios
    for (int i = 0; i < listadoUsuarios->total; i++) {

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

        funciones_liberar_memoria(auxString);
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
        totalUsuarios++;

        funciones_liberar_memoria(auxString);
        auxString = malloc(sizeof(char));
        auxIndex = 0;
        tramaIndex++;
        loopId = true;

        if (listadoUsuarios->total != totalUsuarios) {
            if (dataTrama[tramaIndex-1] == '\0') { // quedan mas tramas por llegar
                memset(tramaRespuesta, 0, MAX_TRAMA_SIZE);
                read(socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

                if (tramaRespuesta[15] == 'L') {

                    dataIndex = 0;
                    dataTrama = malloc(sizeof(char) * TRAMA_DATA_SIZE);
                    for (int j = TRAMA_ORIGEN_SIZE + 1; j < MAX_TRAMA_SIZE; j++) {
                        dataTrama[dataIndex] = tramaRespuesta[j];
                        dataIndex++;
                    }

                    tramaIndex = 0;
                }
            }
        }
    }

    funciones_liberar_memoria(auxString);
    funciones_liberar_memoria(dataTrama);

    return listadoUsuarios;
}

int comandos_propios(char **instruccion, int totalParams, int socketFD, Usuario *usuario) {
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
        if (totalParams == 2 && strcmp(instruccion[1], "") != 0 && strcmp(instruccion[2], "") != 0) {

            if (isdigit(*instruccion[2])) {
                socketFD = comandos_establecer_conexion();
                char *data;
                asprintf(&data, "%s*%s", instruccion[1], instruccion[2]);
                char *trama = comandos_obtener_trama('C', data);
                write(socketFD, trama, MAX_TRAMA_SIZE);

                funciones_liberar_memoria(data);
                funciones_liberar_memoria(trama);

                char tramaRespuesta[MAX_TRAMA_SIZE];
                read(socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

                if (tramaRespuesta[15] == 'O') {
                    //idUser
                    usuario->id = comandos_extraer_id_trama(tramaRespuesta);
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
                funciones_display("El segundo parametro ha de ser un numero\n");
            }
        } else {
            funciones_display("Comanda KO. Falta paràmetres\n");
        }
    } else if (strcmp("PHOTO", comando) == 0) {
        if (totalParams == 1 && strcmp(instruccion[1], "") != 0) {
            if (isdigit(*instruccion[1])) {
                char *data;
                asprintf(&data, "%s", instruccion[1]);
                char *trama = comandos_obtener_trama('P', data);
                write(usuario->socketFD, trama, MAX_TRAMA_SIZE);

                //funciones_liberar_memoria(data);
                //funciones_liberar_memoria(trama);

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
                    memset(conexionData, 0, sizeof(ConexionData));
                    read(usuario->socketFD, tramaImagen, MAX_TRAMA_SIZE);

                    int fd;

                    fd = open(fotoData->nombre, O_WRONLY | O_CREAT | O_APPEND, 00666);

                    if (funciones_error_abrir(fd)) {
                        funciones_display("Error al guardar la imagen\n");
                    }

                    conexionData = ficheros_guardar_trama(tramaImagen);

                    if (fotoData->size % TRAMA_DATA_SIZE != 0 && (fotoData->totalTramas + 4) == i) {
                        write(fd, conexionData->datos, sizeof(char) * (fotoData->size % TRAMA_DATA_SIZE));
                        i = 0;
                        descargandoImagen = false;
                        comandos_comparar_md5sum(usuario, trama, fotoData);
                    } else {
                        if ((fotoData->totalTramas + 4) == i) {
                            descargandoImagen = false;
                            comandos_comparar_md5sum(usuario, trama, fotoData);
                        }
                        write(fd, conexionData->datos, sizeof(char) * TRAMA_DATA_SIZE);
                        i++;
                    }

                    close(fd);
                }

                funciones_display("Foto descargada :)\n");

               // funciones_liberar_memoria(conexionData);
                //funciones_liberar_memoria(fotoData->nombre);
                //funciones_liberar_memoria(fotoData->md5sum);
                //funciones_liberar_memoria(fotoData);

            } else {
                funciones_display("El primer parametro ha de ser un numero\n");
            }
        } else {
            funciones_display("Comanda KO. error paràmetres\n");
        }
    } else if ((strcmp("SEARCH", comando) == 0)) {
        if (totalParams == 1 && strcmp(instruccion[1], "") != 0) {
            if (isdigit(*instruccion[1])) {
                if (usuario->socketFD > 0) {
                    char idString[20];
                    sprintf(idString, "%d", usuario->id);

                    char *data;
                    asprintf(&data, "%s*%s*%s", usuario->nombre, idString, instruccion[1]);

                    char *trama = comandos_obtener_trama('S', data);
                    write(usuario->socketFD, trama, MAX_TRAMA_SIZE);

                    funciones_liberar_memoria(data);
                    funciones_liberar_memoria(trama);

                    char tramaRespuesta[MAX_TRAMA_SIZE];
                    read(usuario->socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

                    if (tramaRespuesta[15] == 'L') {
                        char auxid[30];

                        ListadoUsuarios *listadoUsuarios = comandos_destruct_trama_respuesta(tramaRespuesta, usuario->socketFD);

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

                        funciones_liberar_memoria(listadoUsuarios->usuarios);
                        funciones_liberar_memoria(listadoUsuarios);

                    } else if (tramaRespuesta[15] == 'K') {
                        funciones_display("No hay ningun usuario con este codigo postal\n");
                    }
                } else {
                    funciones_display("Has de realizar el login primero\n");
                }

            } else {
                funciones_display("El primer parametro ha de ser un numero\n");
            }
        } else {
            funciones_display("Comanda KO. error paràmetres\n");
        }
    } else if (strcmp("SEND", comando) == 0) {
        if (totalParams == 1 && strcmp(instruccion[1], "") != 0) {
            char sizeFileString[100];

            int fd = open(instruccion[1], O_RDONLY);
            if (fd > 0) {
                close(fd);
                int sizeFile = funciones_get_file_size(instruccion[1]);
                sprintf(sizeFileString, "%d", sizeFile);

                char *md5File = funciones_generate_md5sum(instruccion[1]);

                char *data;
                asprintf(&data, "%s*%s*%s", instruccion[1], sizeFileString, md5File);

                char *trama = comandos_obtener_trama('F', data);
                write(usuario->socketFD, trama, MAX_TRAMA_SIZE);

                funciones_liberar_memoria(md5File);
                funciones_liberar_memoria(data);
                funciones_liberar_memoria(trama);

                int totalTramas = sizeFile / TRAMA_DATA_SIZE;
                if (sizeFile % TRAMA_DATA_SIZE != 0) {
                    totalTramas++;
                }

                funciones_send_image(usuario->socketFD, instruccion[1], totalTramas);

                char tramaRespuesta[MAX_TRAMA_SIZE];
                read(usuario->socketFD, tramaRespuesta, MAX_TRAMA_SIZE);
                if (tramaRespuesta[15] == 'I') {
                    funciones_display("Foto enviada amb èxit a Atreides.\n\n");
                } else if (tramaRespuesta[15] == 'R') {
                    funciones_display("Error foto no enviada a Atreides.\n\n");
                }
            } else {
                funciones_display("Error. No existe la imagen\n\n");
            }


        } else {
            funciones_display("Comanda KO. error paràmetres\n");
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

                funciones_liberar_memoria(data);
                funciones_liberar_memoria(trama);
                funciones_liberar_memoria(usuario->nombre);
                funciones_liberar_memoria(usuario->codigoPostal);

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

    funciones_liberar_memoria(comando);
    return 0;
}

void comandos_comparar_md5sum(const Usuario *usuario, char *trama, FotoData *fotoData) {
    char *md5File = funciones_generate_md5sum(fotoData->nombre);
    if (strcmp(fotoData->md5sum, md5File) == 0) {
        trama = comandos_obtener_trama('I', "IMAGE OK");
        write(usuario->socketFD, trama, MAX_TRAMA_SIZE);
    }
}

_Noreturn void comandos_pedir_instruccion() {
    int socketFD = -1;
    Usuario *usuario = malloc(sizeof(Usuario));
    while (1) {

        // TODO: quitar statico
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

        if (comandos_propios(paramList, totalParams - 1, socketFD, usuario)) {

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
                    break;
            }
        }
    }
}

char* read_until(int fd, char end) {
    int i = 0, size;
    char c = '\0';
    char* string = (char*)malloc(sizeof(char));

    while (1) {
        size = read(fd, &c, sizeof(char));

        if (c != end && size > 0) {
            string = (char*)realloc(string, sizeof(char) * (i + 2));
            string[i++] = c;
        } else {
            break;
        }
    }

    string[i] = '\0';
    return string;
}