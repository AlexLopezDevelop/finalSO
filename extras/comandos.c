//
// Created by Alex Lopez on 22/10/21.
//

#include "comandos.h"
#include "funciones.h"
#include "../modelos/configuracion.h"

#include <string.h>
#include <stdbool.h>

int establecerConexion() {

    int socketFD;
    struct sockaddr_in servidor;


    if ((socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        display("Error creant el socket\n");
    } else {
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8710);

    if (inet_pton(AF_INET, "127.0.0.1", &servidor.sin_addr) < 0) {
        display("Error configurant IP\n");
    } else {
    }

    if (connect(socketFD, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
        display("Error fent el connect\n");
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

    int origenSize = strlen(origen);
    int tipoSize = 1;
    int dataSize = strlen(data);

    // origen
    for (int i = 0; i < TRAMA_ORIGEN_SIZE; ++i) {
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

    for (int i = TRAMA_ORIGEN_SIZE + tipoSize; i < TRAMA_DATA_SIZE; ++i) {
        if (dataIndex < dataSize) {
            trama[i] = data[dataIndex];
            dataIndex++;
        } else {
            trama[i] = '\0';
        }
    }


    return trama;
}

char *obtenerTrama(char tipo, char *data) {
    return crearTrama("FREMEN", tipo, data);
}


ListadoUsuarios *destructTramaRespuesta(char *tramaRespuesta) {
    ListadoUsuarios *listadoUsuarios = malloc(sizeof(ListadoUsuarios));
    char *dataTrama;
    int dataIndex = 0;
    char lineaFile[250];

    dataTrama = malloc(sizeof(char) * TRAMA_DATA_SIZE);
    for (int i = TRAMA_ORIGEN_SIZE + 1; i < MAX_TRAMA_SIZE; ++i) {
        dataTrama[dataIndex] = tramaRespuesta[i];
        dataIndex++;
    }

    //total
    strcpy(lineaFile, (readStringTo(dataTrama, '*')));
    listadoUsuarios->total = atoi(lineaFile);

    //reservamos memoria con el total
    listadoUsuarios->usuarios = malloc(sizeof(Usuario) * listadoUsuarios->total);

    // init
    char *auxString = malloc(sizeof(char));
    strcpy(auxString, "");
    int tramaIndex = listadoUsuarios->total;
    int auxIndex = 0;

    int sizeTrama = strlen(dataTrama);
    bool loopName = true;
    bool loopId = true;

    // usuarios
    for (int i = 0; i < listadoUsuarios->total; ++i) {

        // nombre
        while (loopName) {
            auxString = realloc(auxString, sizeof(char) * auxIndex + 1);
            auxString[auxIndex] = dataTrama[tramaIndex];
            auxIndex++;
            tramaIndex++;

            if (dataTrama[tramaIndex] == '*' || dataTrama[tramaIndex] == '\0' || tramaIndex >= sizeTrama) {
                loopName = false;
            }
        }

        listadoUsuarios->usuarios[i].nombre = malloc(sizeof(char) * strlen(auxString));
        strcpy(listadoUsuarios->usuarios[i].nombre, auxString);
        auxString = NULL;
        liberarMemoria(auxString);
        auxString = malloc(sizeof(char));
        auxIndex = 0;
        tramaIndex++;
        loopName = true;

        // id
        while (loopId) {
            auxString = realloc(auxString, sizeof(char) * auxIndex + 1);
            auxString[auxIndex] = dataTrama[tramaIndex];
            auxIndex++;
            tramaIndex++;
            if (dataTrama[tramaIndex] == '*' || dataTrama[tramaIndex] == '\0' || tramaIndex >= sizeTrama) {
                loopId = false;
            }
        }

        listadoUsuarios->usuarios[i].id = atoi(auxString);
        auxString = NULL;
        liberarMemoria(auxString);
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

    char comando[100] = "";
    strcat(comando, instruccion[0]);

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
            char *data = concatStringsPorAsterico(instruccion[1], instruccion[2]);
            char *trama = obtenerTrama('C', data);
            write(socketFD, trama, MAX_TRAMA_SIZE);


            char tramaRespuesta[MAX_TRAMA_SIZE];
            read(socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

            if (tramaRespuesta[15] == 'O') {
                //idUser
                usuario->id = extraerIdTrama(tramaRespuesta);
                //nombreUser
                usuario->nombre = malloc(strlen(instruccion[1]));
                strcpy(usuario->nombre, instruccion[1]);
                //CodigoPostalUser
                usuario->codigoPostal = malloc(strlen(instruccion[2]));
                strcpy(usuario->codigoPostal, instruccion[2]);
                sprintf(print, "Benvingut %s. Tens ID %d.\nAra estàs connectat a Atreides.\n", usuario->nombre,
                        usuario->id);
                display(print);
                usuario->socketFD = socketFD;
            }


        } else {
            display("Comanda KO. Falta paràmetres\n");
        }
    } else if (strcmp("PHOTO", comando) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");

            write(usuario->socketFD, comando, sizeof(instruccion[0]));

            display("Missatge enviat!\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if ((strcmp("SEARCH", comando) == 0)) {
        if (totalParams == 1) {
            if (usuario->socketFD > 0) {
                char *data = concatStringsPorAstericoSearch(usuario->nombre, usuario->id, instruccion[1]);
                char *trama = obtenerTrama('S', data);

                write(usuario->socketFD, trama, MAX_TRAMA_SIZE);

                char tramaRespuesta[MAX_TRAMA_SIZE];
                read(usuario->socketFD, tramaRespuesta, MAX_TRAMA_SIZE);

                if (tramaRespuesta[15] == 'L') {
                    char auxid[30];

                    ListadoUsuarios *listadoUsuarios = destructTramaRespuesta(tramaRespuesta);

                    display("\n");
                    sprintf(print, "Hi han %d persones humanes a %s\n", listadoUsuarios->total, instruccion[1]);
                    display(print);
                    display("\n");

                    for (int j = 0; j < listadoUsuarios->total; ++j) {
                        sprintf(auxid, "%d", listadoUsuarios->usuarios[j].id);
                        sprintf(print, "%s ", auxid);
                        display(print);
                        display(listadoUsuarios->usuarios[j].nombre);
                        display("\n");
                    }

                    display("\n");
                } else if (tramaRespuesta[15] == 'K') {
                    display("No hay ningun usuario con este codigo postal\n");
                }
            } else {
                display("Has de realizar el login primero\n");
            }
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("SEND", comando) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");
            write(usuario->socketFD, comando, sizeof(instruccion[0]));

            display("Missatge enviat!\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("LOGOUT", comando) == 0) {
        if (strcmp("LOGOUT", comando) == 0) {
            if (usuario->socketFD > 0) {
                char userId[20];
                sprintf(userId, "%d", usuario->id);
                char *data = concatStringsPorAsterico(usuario->nombre, userId);
                char *trama = obtenerTrama('Q', data);
                write(usuario->socketFD, trama, MAX_TRAMA_SIZE);
                display("Desconectado de Atreides! Dew!\n");
            } else {
                display("Has de realizar el login primero\n");
            }
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else {
        return 1;
    }

    return 0;
}

void pedirInstruccion() {
    int socketFD = -1;
    Usuario *usuario = malloc(sizeof(Usuario));
    while (1) {

        char entradaUsuario[40];
        display("$ ");
        read(0, entradaUsuario, 40);

        entradaUsuario[strcspn(entradaUsuario, "\n") + 1] = '\0';

        //char *param = NULL;
        //char *param2 = NULL;
        int totalParams = 0;

        //  Separar instruccion y parametros
        int i = 0;
        char *aux = NULL;
        char caracter = '\0';
        int lenEntradaUser = strlen(entradaUsuario);
        char **paramList = NULL;
        paramList = malloc((sizeof(char *)) * totalParams);

        for (int j = 0; j < lenEntradaUser && caracter != '\n'; ++j) {
            caracter = entradaUsuario[j];

            aux = (char *) realloc(aux, i + 1);
            aux[i] = caracter;
            i++;

            if (caracter == ' ' || caracter == '|' || caracter == '\n') {

                if (caracter == '\n') {
                    aux[strcspn(aux, "\n")] = 0;
                }

                paramList[totalParams] = malloc(sizeof(char) * strlen(aux));
                aux[strcspn(aux, " ")] = 0;
                strcpy(paramList[totalParams], aux);


                totalParams++;
                aux = NULL;
                liberarMemoria(aux);
                i = 0;
            }
        }
        // END Separar instruccion y parametros

        // Añadir el NULL al final del params
        paramList[totalParams] = NULL;

        if (comandosPropios(paramList, totalParams - 1, socketFD, usuario)) {

            // init fork
            pid_t son_pid;
            son_pid = fork();

            switch (son_pid) {
                case -1:
                    // Error
                    display("Error en el fork!\n");
                    break;
                case 0:
                    // Hijo
                    display("------------------------------------------------------------\n");

                    if (execvp(paramList[0], paramList) == -1) {
                        display("No se ha encontrado el comando ");
                        display(paramList[0]);
                        display("\n");
                    }

                    display("------------------------------------------------------------\n");
                    break;
                default:
                    // Padre
                    wait(NULL);
                    display("------------------------------------------------------------\n");
                    break;
            }
        }
    }
}