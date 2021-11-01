all:PracticaFinal

main.o: main.c extras/funciones.h extras/ficheros.h modelos/configuracion extras/comandos.h
        gcc -c main.c -Wall -Wextra

funciones.o: extras/funciones.c extras/funciones.h
        gcc -c extras/funciones.c -Wall -Wextra

ficheros.o: extras/ficheros.c extras/ficheros.h extras/funciones.h
        gcc -c extras/ficheros.c -Wall -Wextra

comandos.o: extras/comandos.c extras/comandos.h
        gcc -c extras/comandos.c -Wall -Wextra

PracticaFinal: main.o funciones.o ficheros.o comandos.o
        gcc main.o funciones.o ficheros.o comandos.o -o practicaFinal.exe

clean:
    rm -rf *.o *.exe

do:
    ./practicaFinal.exe Config.dat

val:
    valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./practicaFinal.exe Config.dat

tar:
    tar -cvf PracticaFinal.tar *.c *.h Config.dat