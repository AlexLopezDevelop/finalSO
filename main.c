#include "extras/funciones.h"
#include "extras/ficheros.h"
#include "modelos/configuracion.h"
#include "extras/comandos.h"

Configuracion config;

void salir() {
    free(config.directorio);
    free(config.ip);
    //Valgrind muestra error de que hay Open file descriptor 1: /dev/pts/3  <inherited from parent>
    close(0);
    close(1);
    close(2);
    close(4);
    exit(1);
}

int main(int argc, char *argv[]) {


    errorArgumentos(argc, argv, 2);

    display("Benvingut a Fremen\n");

    if (leerFichero(argv[1], &config)) {
        return 1;
    }

    signal(SIGINT, salir);

    pedirInstruccion();

    return 0;
}
