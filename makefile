all:PracticaFinal

main.o: main.c extras/funciones.h extras/ficheros.h modelos/configuracion.h extras/comandos.h
		gcc -c -g main.c -Wall -Wextra

funciones.o: extras/funciones.c extras/funciones.h
		gcc -c -g extras/funciones.c -Wall -Wextra

ficheros.o: extras/ficheros.c extras/ficheros.h extras/funciones.h
		gcc -c -g extras/ficheros.c -Wall -Wextra

comandos.o: extras/comandos.c extras/comandos.h modelos/configuracion.h
		gcc -c -g extras/comandos.c -Wall -Wextra

PracticaFinal: main.o funciones.o ficheros.o comandos.o
		gcc main.o funciones.o ficheros.o comandos.o -o practicaFinal.exe

clean:
		rm -rf *.o *.exe

do:
		./practicaFinal.exe Config.dat

val:
		valgrind --dsymutil=yes --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./practicaFinal.exe Config.dat

tar:
		tar cf Fremen.tar *.c extras/*.h extras/*.c modelos/*.h Config.dat makefile