//
// Created by Alex Lopez on 22/10/21.
//

#include "comandos.h"
#include "funciones.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>

//#define PATH "/users/home/joan.ballber/finalSO-master"
#define PATH "/Users/alexlopez/Downloads/PracticaFinalSO"

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


int comandosPropios(char **instruccion, int totalParams, int socketFD) {
    int i = 0;

    display(instruccion[0]);

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
            display("Comanda OK\n");
            // Test Sockets
            char **paramList = NULL;
            paramList = malloc((sizeof(char *)) * 3);
            strcpy(paramList[0], "paco");
            strcpy(paramList[1], "manolo");
            strcpy(paramList[2], "frederico");

            write(socketFD, paramList, sizeof(paramList));

            display("Missatge enviat!\n");

        } else {
            display("Comanda KO. Falta paràmetres\n");
        }
    } else if (strcmp("PHOTO", comando) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");
            write(socketFD, comando, sizeof(instruccion[0]));

            display("Missatge enviat!\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("SEARCH", comando) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");
            write(socketFD, comando, sizeof(instruccion[0]));

            display("Missatge enviat!\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("SEND", comando) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");
            write(socketFD, comando, sizeof(instruccion[0]));

            display("Missatge enviat!\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("LOGOUT", comando) == 0) {
        if (strcmp("LOGOUT", comando) == 0) {
            display("Comanda OK\n");
            write(socketFD, "salir", sizeof("salir"));

            display("Missatge enviat!\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else {
        return 1;
    }

    return 0;
}

void pedirInstruccion() {
    int socketFD = establecerConexion();
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

        if (comandosPropios(paramList, totalParams - 1, socketFD)) {

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