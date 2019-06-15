#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "funciones.h"

pthread_mutex_t *mutex;
int FinalFlag = 0;


int main(int argc, char const *argv[])
{

    char* nombre = "/home/gabriel/CLionProjects/Lab2 Sistope/test.csv";
    int c_discos = 5;
    int anchoDisco = 40;
    int bandera = 0;  // 0 no mostrar en pantalla y 1 mostrar en pantalla
    int largoFile = largoArchivo(nombre);
    int size_Buffer = 2;


    Monitor** monitores = (Monitor**)malloc(sizeof(Monitor*)*c_discos);
    pthread_t  hebras[c_discos];


    //##########################################################################################
    /// BLOQUE DE INICIALIZACION DE HEBRAS CONSUMIDORAS HIJAS Y MONITORES RESPECTIVOS
    for (int i = 0; i < c_discos; i++) {

        monitores[i] = (Monitor*)malloc(sizeof(Monitor));
        pthread_cond_init(&monitores[i] -> bufferNotEmpty, NULL);
        pthread_cond_init(&monitores[i] -> bufferNotFull, NULL);
        monitores[i] -> buffer =(Buffer**) malloc(sizeof(Buffer*) * size_Buffer);
        monitores[i] ->cantidad_datos_Monitor = 0;

        for (int j = 0; j < size_Buffer; j++) {
            monitores[i] -> buffer[j] =(Buffer*) malloc(sizeof(Buffer));
        }

        monitores[i] -> id = i;
        monitores[i] -> flag = -1;

        pthread_create(&hebras[i], NULL, consumirDatos, (void*)monitores[i]);
    }
    //##########################################################################################


    //##########################################################################################
    ///------------------------Leer archivo Y Mandar Visibilidades-------------------------

    FILE* archivo = fopen(nombre,"r");
    double u,v,real,imag,ruido;
    int disco,posicion,cont = 0;
    char c1,c2,c3,c4;
    int * radioList = malloc(sizeof(int)*(c_discos));
    for (int i = 0; i < disco; ++i)
        radioList[i] = i*anchoDisco;

    // BLOQUE DE CODIGO DONDE SE SETEAN ATRIBUTOS DE MONITORES CON LINEA LEIDAS POR LA HEBRA PADRE
    while(!feof(archivo)){

        u = 0,v = 0,real = 0,imag = 0,ruido = 0;
        fscanf(archivo, "%lf %c %lf %c %lf %c %lf %c %lf", &u,&c1,&v,&c2,&real,&c3,&imag,&c4,&ruido);
        printf("Lectura: %lf %lf %lf %lf %lf\n", u,v,real,imag,ruido);

        // Se calcula para el disco por correspondiente a la hebra a donde ira la informacion
        disco = calcularDisco(u, v, anchoDisco, c_discos);

        ///---------------------------- Guardando Valores en Monitores  ------------------------------------

        posicion = monitores[disco] -> cantidad_datos_Monitor;
        printf("Posicion[%d]\n",monitores[disco] -> cantidad_datos_Monitor);
        monitores[disco] -> buffer[posicion] -> real = real;
        printf("seteando real: %f\n",monitores[disco] ->buffer[posicion] -> real);
        monitores[disco] -> buffer[posicion] -> imag = imag;
        printf("seteando imag: %f\n",monitores[disco] ->buffer[posicion] -> imag);
        monitores[disco] -> buffer[posicion] -> ruido = ruido;
        printf("seteando ruido: %f\n",monitores[disco] ->buffer[posicion] -> ruido);

        posicion++;
        monitores[disco] -> cantidad_datos_Monitor = posicion;
        //----------------------------------------------------------------------------------------

        int puntero=0;

        if (monitores[disco] -> cantidad_datos_Monitor == size_Buffer)
        {
            //printf("WOLOLO\n");
            monitores[disco] -> flag = 1;

            pthread_cond_signal(&monitores[disco] -> bufferNotFull);
            pthread_cond_wait(&monitores[disco] -> bufferNotEmpty,&monitores[disco] -> mutex);
        }


    }

    fclose(archivo);
    //##########################################################################################

    for (int i = 0; i < c_discos; i++) {
        pthread_join(hebras[i],NULL);
    }

    /*for (int i = 0; i < c_discos; i++) {
        monitores[i] -> flag =  2;
        pthread_cond_signal(&monitores[i] -> bufferNotFull);
        printf("esperando hebra hija\n");
        //pthread_cond_wait(&monitores[disco] -> bufferNotEmpty, &monitores[disco] -> mutex);
        //printf("durmiendo padre\n");
    }*/


    /*int i = 0;
    while (i < c_discos)
    {
      //pthread_mutex_lock(&mutex);
      pthread_create(&hilo[i], NULL, procesarDatos, (void *) hebras[i]);
      //pthread_mutex_unlock(&mutex);
      i++;
    }*/

    return 0;
}