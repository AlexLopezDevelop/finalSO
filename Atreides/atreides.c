//
// Created by Alex Lopez on 12/11/21.
//
#include "../modelos/configuracion.h"
#include "../extras/funciones.h"
#include "../extras/ficheros.h"
#include "../extras/utils.h"


#define printF(x) write(1, x, strlen(x))

int main(int argc, char *argv[]) {

    ConfiguracionArtreides configAtreides;

    errorArgumentos(argc, argv, 2);

    display("SERVIDOR ATREIDES\n");

    leerFicheroArtreides(argv[1], &configAtreides);

    gestorDeSockets();
}