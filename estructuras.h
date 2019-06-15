//
// Created by gabriel on 15-06-19.
//

#ifndef LAB2_SISTOPE_ESTRUCTURAS_H
#define LAB2_SISTOPE_ESTRUCTURAS_H

#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define TRUE 1
int FinalFlag;

typedef struct Buffer
{
    double real;
    double imag;
    double ruido;
} Buffer;


typedef struct Resultados
{
    double mediaReal;
    double mediaImag;
    double potenciaTotal;
    double ruidoTotal;
} Resultado;


typedef struct Monitor
{

    int id;
    int flag;
    int cantidad_datos_Monitor;
    Buffer** buffer;
    pthread_mutex_t mutex;
    pthread_cond_t bufferNotEmpty;
    pthread_cond_t bufferNotFull;


} Monitor;






#endif //LAB2_SISTOPE_ESTRUCTURAS_H
