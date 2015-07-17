
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


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
t_list *lista_archivos; // guarda t_archivo
t_list *lista_nodos_estado;
pthread_mutex_t mutex ;//= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_nodos;// = PTHREAD_MUTEX_INITIALIZER;
t_list* listaDeArchivos; // global



typedef struct{
	char* archivo;
}t_charpuntero;

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
}t_bloque;

typedef struct{
	int id_nodo;
	t_list *archivos;
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

typedef struct{
	int idNodo;
		int cantidadArchivosTemporales;
		t_list* vectorArchivosTemporales;
		char* archivoResultadoReduce;

}t_mandarAlHilo;

typedef struct {
	char* nombre_archivo_temporal;
	int resultado;
	int idNodo;
} t_respuesta_reduce;

typedef struct {
	char* archivoTemporal;
	int idNodo;
}t_respuestaCorrecta;

int recive_y_deserialisa(t_charpuntero* nombre, int socket, uint32_t tamanioTotal);
int recive_y_guarda_estructura(t_archivo arch, int socket, uint32_t tamanioTotal);
char* serializar_charpuntero(t_charpuntero* nombre, int tamanioTotal);
void  *conectarseAlJob(void*arg);
int recive_y_deserialisa_job(t_job_marta* job_marta, int socket,uint32_t tamanioTotal );
int recive_y_deserialisa_paquete_nodos(t_charpuntero* ip, t_charpuntero* puerto, int ipnodo, uint32_t tamanioTotal);
char* serializar_estructura_t_marta_a_job(t_marta_job estructura_t_marta_a_job, int tamanioTotal);
void *aplicarReduce(t_aplicarReduce structRecibido);
char* serializar_aplicarReduce(t_mandarAlHilo *estructura, int tamanioTotal);
void *recibirResultadoReduce(t_respuesta_reduce *respuesta_reduce);

#endif /* FUNCTIONS_H_ */
