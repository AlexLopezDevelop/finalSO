//
// Created by Alex Lopez on 12/11/21.
//
#include "../modelos/configuracion.h"
#include "../extras/funciones.h"
#include "../extras/ficheros.h"


#define printF(x) write(1, x, strlen(x))
// TODO: Revisar de donde saca el modelo de config

int listenFD;

void * comprobarNombres(void arg) {
    int clientFD = *((int *) arg;
    int salir=0;


    printf("clientFD2: %d\n",clientFD);
    char buffer[100];
    while (salir == 0) {
        read(clientFD, buffer, 100);
        display("entra");
        display(buffer);

        if (strcmp(buffer, "salir") == 0) {
            display("\nCliente Desconectado!\n\n");
            close(clientFD);
            salir = 1;
        }
    }
    pthread_cancel(pthread_self());
    pthread_detach(pthread_self());

    return NULL;
}

void sortir(){
    close(listenFD);
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
}

int main(int argc, char *argv[]) {
    int clientFD;
    struct sockaddr_in servidor;

    ConfiguracionArtreides configAtreides;

    errorArgumentos(argc, argv, 2);

    display("SERVIDOR ATREIDES\n");

    leerFicheroArtreides(argv[1], &configAtreides);

    signal(SIGINT, sortir);

    if( (listenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_port = htons(8710);
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        printF("Error fent el bind\n");
    }

    if(listen(listenFD, 10) < 0){
        printF("Error fent el listen\n");
    }


    while(1) {
        printF("Esperant connexions...\n");

        clientFD = accept(listenFD, (struct sockaddr *) NULL, NULL);

        printF("\nNou client connectat!\n");

        pthread_t thrd;
        printf("clientFD: %d\n",clientFD);
        pthread_create(&thrd, NULL, comprobarNombres, clientFD);
    }

}