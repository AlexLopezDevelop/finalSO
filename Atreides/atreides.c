//
// Created by Alex Lopez on 12/11/21.
//
#include "../modelos/configuracion.h"
#include "../extras/funciones.h"
#include "../extras/ficheros.h"

// TODO: Revisar de donde saca el modelo de config


int main(int argc, char *argv[]) {
    ConfiguracionArtreides configAtreides;

    errorArgumentos(argc, argv, 2);

    display("SERVIDOR ATREIDES\n");

    leerFicheroArtreides(argv[1], &configAtreides);


    display("Esperant connexions...");

}