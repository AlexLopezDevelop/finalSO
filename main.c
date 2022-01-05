#include "extras/funciones.h"
#include "extras/ficheros.h"
#include "modelos/configuracion.h"
#include "extras/comandos.h"

Configuracion config;

void salir() {
    //free(config.directorio);
    //free(config.ip);
    //Valgrind muestra error de que hay Open file descriptor 1: /dev/pts/3  <inherited from parent>
    close(0);
    close(1);
    close(2);
    close(4);
    exit(1);
}

int main(int argc, char *argv[]) {
    funciones_error_argumentos(argc, argv, 2);

    funciones_display("Benvingut a Fremen\n");

    char * fileName;
    fileName = strdup(argv[1]);

    if (ficheros_leer_fichero(fileName, &config)) {
        return 1;
    }

    signal(SIGINT, salir);

    comandos_pedir_instruccion();

    funciones_liberar_memoria(fileName);

    return 0;
}
