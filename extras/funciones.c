//
// Created by joanb on 14/10/2021.
//

#include "funciones.h"
#include "../modelos/configuracion.h"
#include "comandos.h"
#include "../modelos/ficheros.h"

void liberarMemoria(void *ptr) {
    // Libermaos Memoria
    free(ptr);
    // Dejamos apuntando a NULL
    ptr = NULL;
}

void display(char *string) {
    write(1, string, sizeof(char) * strlen(string));
}

char *readStringTo(char *string, char hasta) {

    int i = 0;
    char *aux = malloc(sizeof(char));
    char caracter = '\0';

    int lenString = strlen(string);
    for (int j = 0; j < lenString || caracter != hasta; j++) {
        caracter = string[j];
        if (caracter != hasta) {
            aux = (char *) realloc(aux, i + 1);
            aux[i] = caracter;
            i++;
        }

    }
    aux[i - 1] = '\0';
    return aux;
}

char *concatStringsPorAsterico(char *string1, char *string2) {
    char *concatString = NULL;
    int stringSize = strlen(string1) + strlen(string2) + 1;
    concatString = malloc(stringSize * sizeof(char));

    strcpy(concatString, string1);
    strcat(concatString, "*");
    strcat(concatString, string2);

    return concatString;
}

/*char *concatStringsPorAstericoSearch(char *string1, int id, char *string3) {
    char *concatString = NULL;
    char aux[30];
    sprintf(aux, "%d", id);
    int stringSize = strlen(string1) + strlen(aux) + strlen(string3) + 1;
    concatString = malloc(stringSize * sizeof(char));

    strcpy(concatString, string1);
    strcat(concatString, "*");
    strcat(concatString, aux);
    strcat(concatString, "*");
    strcat(concatString, string3);

    return concatString;
}*/

int errorArgumentos(int argc, char *argv[], int num_argumentos) {

    if (argc != num_argumentos) {
        display("\nERROR en el numero de ficheros\n");
        return 1;
    } else {
        if (strcmp(argv[1], FILE1) != 0) {
            display("\nERROR, el fichero no es correcto\n");
            return 1;
        } else {
            display("\nFicheros recibidos correctamente\n");
            return 0;
        }
    }
}

int errorAbrir(int fd, char *nombre_f) {
    char aux[200];

    if (fd < 0) {
        sprintf(aux, "\nERROR al abrir el fichero\n");
        display(aux);
        return 1;
    } else {
        sprintf(aux, "\nEl fichero: %s se ha abierto correctamente\n", nombre_f);
        display(aux);
        return 0;
    }
}

void readInput(char **string) {
    int i = 0;
    char caracter = ' ';

    // Bucle para leer caracteres entrados por teclado hasta Enter
    while (caracter != '\n') {
        // Lectura de caracter por caracter
        read(0, &caracter, sizeof(char));

        // Redimensiona la cadena por cada caracter del usuario
        *string = (char *) realloc(*string, (sizeof(char) * (i + 1)));

        // Asigna el caracter a la cadena
        (*string)[i] = caracter;

        // Cuando el caracter sea un salto de linea (enter) marcamos el fianl de la cadena
        if (caracter == '\n') {
            (*string)[i] = '\0';
        }
        i++;
    }
}

char *readLineFile(int fd, char hasta) {
    int i = 0, size;
    char c = '\0';
    char *string = (char *) malloc(sizeof(char));

    while (1) {
        size = read(fd, &c, sizeof(char));

        if (c != hasta && size > 0) {
            string = (char *) realloc(string, sizeof(char) * (i + 2));
            string[i++] = c;
        } else {
            break;
        }

    }

    string[i] = '\0';

    return string;

}

int checkEOF(int fd) {
    int num_bytes;
    char car;

    // Controlamos cuantos bytes retorna la funcion read()
    num_bytes = read(fd, &car, sizeof(char));

    // Si retorna 0bytes porque no hay mas caracteres es el End Of File
    if (num_bytes == 0) {
        return 1;
    }

    // Reposicionamos el puntero a la posicion que acaba de leer
    lseek(fd, -1, SEEK_CUR);

    return 0;
}

int getFileSize(char *fileName) {
    struct stat sb;

    if (stat(fileName, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    return sb.st_size;
}

char *generateMd5sum(char *string) {
    char *args[] = {"md5sum", string, 0};
    int fd = open(MD5FILE, O_CREAT | O_WRONLY, S_IRWXU);
    pid_t pid = fork();

    if (!pid) {
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execvp(args[0], args);
    }

    fd = open(MD5FILE, O_RDONLY);

    char *md5String = malloc(sizeof(char) * 33);

    if (errorAbrir(fd, MD5FILE)) {
        return md5String;
    }

    strcpy(md5String, readLineFile(fd, ' '));

    close(fd);

    return md5String;
}

int sendImage(int socket, char *fileName) {
    int picture;
    char *datosBinarios;
    picture = open(fileName, O_RDONLY);

    if (errorAbrir(picture, fileName)) {
        display("Error Opening Image File");
        return 1;
    }

    FicheroFoto *ficheroFoto = malloc(sizeof(FicheroFoto));
    ficheroFoto->totalTramas = 0;
    ficheroFoto->tramas = malloc(sizeof(char *));
    ficheroFoto->tramas[0] = malloc(sizeof(char));

    int i = 0;
    char c = '\0';

    while (!checkEOF(picture)) {
        read(picture, &c, sizeof(char));

        ficheroFoto->tramas[ficheroFoto->totalTramas][i] = c;

        if (i == TRAMA_DATA_SIZE) {
            ficheroFoto->totalTramas++;
            ficheroFoto->tramas = realloc(ficheroFoto->tramas, sizeof(char *) * (ficheroFoto->totalTramas + 1));
            ficheroFoto->tramas[ficheroFoto->totalTramas] = malloc(sizeof(char));
            i = 0;
        } else {
            i++;
        }

        ficheroFoto->tramas[ficheroFoto->totalTramas] = realloc(ficheroFoto->tramas[ficheroFoto->totalTramas],sizeof(char) * (i + 1));
    }

    ficheroFoto->totalTramas++;

    close(picture);

    // send to server
    for (int j = 0; j < ficheroFoto->totalTramas; j++) {
        char *trama = obtenerTrama('D', ficheroFoto->tramas[j]);
        write(socket, trama, TRAMA_DATA_SIZE);
    }

    display("Foto Enviada");
    return 0;
}

