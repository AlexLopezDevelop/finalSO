//
// Created by Alex Lopez on 22/10/21.
//

#include "comandos.h"
#include "funciones.h"
#include <string.h>
#include <stdio.h>

//#define PATH "/users/home/joan.ballber/finalSO-master"
#define PATH "/Users/alexlopez/Downloads/PracticaFinalSO"

void comandosPropios(char *instruccion, int totalParams) {
    char cmd[20];
    int i = 0;


    while (instruccion[i] != '\0') {
        if (instruccion[i] >= 'a' && instruccion[i] <= 'z') {
            instruccion[i] = instruccion[i] - 32;
        }
        i++;
    }

    int lenInstruccion = strlen(instruccion);

    for (int j = 0; j < lenInstruccion; ++j) {
        cmd[j] = instruccion[j];
    }
    char aux[30];

    sprintf(aux, "|%s|", cmd);
    display(aux);

    if (strcmp("LOGIN", cmd) == 0) {
        if (totalParams == 2) {
            display("Comanda OK\n");
            /*   int socketFD;
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

               write(socketFD, cmd, strlen(cmd));

               display("Missatge enviat!\n");

               close(socketFD);
   */
        } else {
            display("Comanda KO. Falta paràmetres\n");
        }
    } else if (strcmp("PHOTO", cmd) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("SEARCH", cmd) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("SEND", cmd) == 0) {
        if (totalParams == 1) {
            display("Comanda OK\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else if (strcmp("LOGOUT", cmd) == 0) {
        if (totalParams <= 1) {
            display("Comanda OK\n");
        } else {
            display("Comanda KO. Massa paràmetres\n");
        }
    } else {
        display("No se ha encontrado el comando ");
        display(instruccion);
        display("\n");
    }
}

void pedirInstruccion() {

    while (1) {

        char entradaUsuario[40];

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


        // Varables_locales
        pid_t son_pid;


        // Fork
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
                    comandosPropios(paramList[0], totalParams - 1);
                    display("------------------------------------------------------------\n");
                }
                break;
            default:
                // Padre
                wait(NULL);
                display("------------------------------------------------------------\n");
                break;
        }

        /*
        pid_t child_pid;

        child_pid = fork();
        if(child_pid == 0) {
            execvp(paramList[0], paramList);
            comandosPropios(paramList[0], totalParams - 1);
        }
        else {
            wait(NULL);
        }

        int pid = fork();


        if (pid == 0) {

        if (execvp(paramList[0], paramList) == -1) {
            comandosPropios(paramList[0], totalParams - 1);
        } else{
            do {
                pid_t tpid = wait(&child_status);
                if(tpid != child_pid) process_terminated(tpid);
            } while(tpid != child_pid);

            return child_status;
        }
        }*/
    }
}