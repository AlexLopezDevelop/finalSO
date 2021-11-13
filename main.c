#include "extras/funciones.h"
#include "extras/ficheros.h"
#include "modelos/configuracion.h"
#include "extras/comandos.h"

int main(int argc, char *argv[]) {

    ConfiguracionConfig config;

    errorArgumentos(argc, argv, 2);

    display("Benvingut a Fremen\n");

    leerFichero(argv[1], &config);

    pedirInstruccion();
}
