//
// Created by Alex Lopez on 22/10/21.
//

#ifndef PRACTICAFINALSO_INSTRUCCION_H
#define PRACTICAFINALSO_INSTRUCCION_H

typedef struct {
    char * nombre;
} Param;

typedef struct {
    char * instruccion; // ls
    Param * params; // -l -a ..
    int totalParams = 0;
} Instruccion;

#endif //PRACTICAFINALSO_INSTRUCCION_H
