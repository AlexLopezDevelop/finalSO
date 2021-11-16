#include "extras/funciones.h"
#include "extras/ficheros.h"
#include "modelos/configuracion.h"
#include "extras/comandos.h"

int main(int argc, char *argv[]) {

    ConfiguracionConfig config;

    errorArgumentos(argc, argv, 2);

    display("Benvingut a Fremen\n");

    if (leerFichero(argv[1], &config)) {
        return 1;
    }

    pedirInstruccion();

    return 0;
}
