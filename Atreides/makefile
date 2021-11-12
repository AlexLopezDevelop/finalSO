all:Atreides

atreides.o: atreides.c ../extras/funciones.h ../extras/ficheros.h ../modelos/configuracion.h
		gcc -c atreides.c -Wall -Wextra

funciones.o: ../extras/funciones.c ../extras/funciones.h
		gcc -c ../extras/funciones.c -Wall -Wextra

ficheros.o: ../extras/ficheros.c ../extras/ficheros.h ../extras/funciones.h
		gcc -c ../extras/ficheros.c -Wall -Wextra

comandos.o: ../extras/comandos.c ../extras/comandos.h
		gcc -c ../extras/comandos.c -Wall -Wextra

Atreides: atreides.o funciones.o ficheros.o comandos.o
		gcc atreides.o funciones.o ficheros.o comandos.o -o Atreides.exe -Wall -Wextra

clean:
		rm -rf *.o *.exe

do:
		./Atreides.exe Config.dat

val:
		valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./Atreides.exe Config.dat