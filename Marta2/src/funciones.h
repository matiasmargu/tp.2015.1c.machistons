/*
 * funciones.h
 *
 *  Created on: 9/7/2015
 *      Author: utnso
 */

#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <socket/socket.h>
#include <./commons/collections/list.h>
#include <./commons/bitarray.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Estructuras para manejar archivos

typedef struct{
	char* nombre;
	char* idJob;
	int cantidadDeBloques;
	t_list *bloques;
}t_archivo;

typedef struct {
	int idNodo;
    int Numerobloque;
} t_copia;


typedef struct{
	int NumeroBloque;
	t_copia *copias;
	int estado; // 0= Pendiente, 1=Espera, 2= Fin, 3=Error
}t_bloque;




typedef struct{
	char *nombre;
	int idJob;
	int cantidadBloques;
	t_list* copias;
}t_archivo_job;

typedef struct{
	int idJob;
	int combiner;
	int socket;
	int cantidadDeArchivos;
}t_job;


typedef struct {
	char* nombre_archivo_temporal;
	int resultado;
	int idNodo;
} t_respuesta_reduce;


t_log* logger; // Log Global

// Variables Hilos

pthread_mutex_t mutex;

// Listas Globales para manejo de datos del archivo

t_list* listaArchivos;
t_list* bloquesCopias;
t_list* listaJobs;
t_list *listaDeRespuestaCorrecta;
char* nombre;

// mutex para el reduce
pthread_mutex_t mutex;


void * recibirListaDeArchivos(void*arg);
int cantidadArchivosEnArray(char** strings);
char* guardarPrimeroDelArray(char** strings);
char *string_substring_from_modificado(char *text, int start);
char** eliminarPrimeroDelArray(char** strings);
void *recibirResultadoReduce(t_respuesta_reduce *respuesta_reduce);
char* serializar_marta_job_map(t_marta_job_map *bloque, int tamanioTotal);
bool estaEnLaLista(t_list* lista, char* archivo);
#endif /* FUNCIONES_H_ */
