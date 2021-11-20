all:Atreides

atreides.o: atreides.c ../extras/funciones.h ../extras/ficheros.h ../modelos/configuracion.h ../extras/utils.h
		gcc -c atreides.c -Wall -Wextra -lpthread

funciones.o: ../extras/funciones.c ../extras/funciones.h
		gcc -c ../extras/funciones.c -Wall -Wextra -lpthread

ficheros.o: ../extras/ficheros.c ../extras/ficheros.h ../extras/funciones.h
		gcc -c ../extras/ficheros.c -Wall -Wextra -lpthread

comandos.o: ../extras/comandos.c ../extras/comandos.h ../modelos/configuracion.h
		gcc -c ../extras/comandos.c -Wall -Wextra -lpthread

utils.o: ../extras/utils.c ../extras/funciones.h ../extras/ficheros.h ../modelos/configuracion.h ../modelos/conexion.h
		gcc -c ../extras/utils.c -Wall -Wextra -lpthread

Atreides: atreides.o funciones.o ficheros.o comandos.o utils.o
		gcc atreides.o funciones.o ficheros.o comandos.o utils.o -o Atreides.exe -Wall -Wextra -lpthread

clean:
		rm -rf *.o *.exe

do:
		./Atreides.exe Config.dat

val:
		valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --track-fds=yes ./Atreides.exe Config.dat