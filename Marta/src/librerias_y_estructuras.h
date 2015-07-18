/*
 * librerias_y_estructuras.h
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#ifndef LIBRERIAS_Y_ESTRUCTURAS_H_
#define LIBRERIAS_Y_ESTRUCTURAS_H_



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

t_log* logger; // Log Global

// VARIABLE GLOBAL
int socketFS;
int idJobGlobal;
t_list *lista_archivos; // guarda t_archivo
t_list *lista_jobs;
int cant_nodos;
t_list *lista_nodos_estado;
pthread_mutex_t mutex ;//= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_nodos;// = PTHREAD_MUTEX_INITIALIZER;


typedef struct{
	char* nombre;
	int cantidadDeBloques;
	t_list *bloques;
}t_archivo;

typedef struct {
	int idNodo;
    int Numerobloque;
} t_copia;

typedef struct{
	int NumeroBloque;
	int estado;
	t_list *copias;
}t_bloque;

typedef struct{
	int id_job;
	char *arch_resultado_final;
	int combiner; //1 = si 0 = no
	t_list *archivos_job;
}t_infoJob;

typedef struct{
	int id_nodo;
	char *ip_nodo;
	char *puerto_nodo;
}t_nodo;

typedef struct{
	char* nombre;
	t_list *datos;
}t_nombre_archivo;

typedef struct{
	char* nombre_arch;
	int id_job;
	int bloque;
	char estado; // M = MAP; R = REDUCE
}t_datos;

typedef struct{
	char* nombre_arch;
	int bloque_arch;
	t_bitarray *bitmap;
}t_cargaBitarray_aux;

typedef struct{
	int idNodo;
	int cantidadArchivosTemporales;
	char** vectorArchivosTemporales;
}t_nodoPorArchivo;

typedef struct{
	int socketJob;
	int idNodo;
	int cantidadArchivosTemporales;
	t_list* vectorArchivosTemporales;
	char* archivoResultadoReduce;
}t_aplicarReduce;

typedef struct{
	int idNodo;
	char* ipNodo;
	char* puertoNodo;
	int cantidadArchivosTemporales;
	char** vector_archivos_temporales;
	char* archivoResultadoReduce;
}t_nodos;



int recive_y_guarda_estructura(t_archivo arch, int socket, uint32_t tamanioTotal);
void  *conectarseAlJob(void*arg);
//int recive_y_deserialisa_job(t_job_marta* job_marta, int socket,uint32_t tamanioTotal );
//char* serializar_estructura_t_marta_a_job(t_marta_job estructura_t_marta_a_job, int tamanioTotal);
void aplicarReduce(t_aplicarReduce structRecibido);
void *atenderJob(void *arg);


#endif /* LIBRERIAS_Y_ESTRUCTURAS_H_ */
