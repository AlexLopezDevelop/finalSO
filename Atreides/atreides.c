//
// Created by Alex Lopez on 12/11/21.
//

#include "../extras/funciones.h"
#include "../extras/ficheros.h"
// TODO: Revisar de donde saca el modelo de config
#include "../modelos/configuracion.h"

int main(int argc, char *argv[]) {
    ConfiguracionConfig config;

    errorArgumentos(argc, argv, 2);

    display("SERVIDOR ATREIDES\n");

    display("Llegit el fitxer de configuració\n");
    // TODO: Adaptar o hacer otra funcion para leer el config de atreides por que esta pensado para Fermen
    leerFichero(argv[1], &config);

    display("Esperant connexions...");

}