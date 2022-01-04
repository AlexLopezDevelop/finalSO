cmake_minimum_required(VERSION 3.20)
project(finalSO C)

set(CMAKE_C_STANDARD 99)

add_executable(finalSO main.c extras/funciones.c extras/funciones.h extras/ficheros.c extras/ficheros.h extras/comandos.c extras/comandos.h modelos/configuracion.h modelos/instruccion.h modelos/conexion.h)
