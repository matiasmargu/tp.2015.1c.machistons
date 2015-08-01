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
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <socket/socket.h>
#include <./commons/collections/list.h>
#include <./commons/bitarray.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

t_log* logger; // Log Global

// VARIABLE GLOBAL
int socketFS;
int contador_cant_job;
char *ip_fs;
char *puerto_fs;
int cant_nodos;
int *contadores_nodos;
char *listaNombreArchivosReduce[1000];
char *listaNombreArchivosMap[1000];
int contador_archivos_reduce;
int contador_archivos_map;
pthread_mutex_t mutex_contador_archivos_map;
pthread_mutex_t mutex_contador_archivos_reduce;
t_list *lista_archivos; // guarda t_archivo
t_list *lista_jobs; //t_infoJob
t_list *lista_nodos_estado;
t_list *lista_tabla_procesos;
pthread_mutex_t mutex ;//= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_nodos;// = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cant_nodos;
pthread_mutex_t mutex_contador_job;
pthread_mutex_t mutex_lista_jobs;
pthread_mutex_t mutex_lista_procesos;
pthread_mutex_t mutex_socket_job;

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
	t_copia *copias;
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
	int id_nodo;
	int id_reduce;
	int estado;
	char *nombre_resultado_reduce;
	t_list *lista_nombre_map;
}t_reduce;

typedef struct{
	int idJob;
	t_list *tabla_procesos;
}t_job_procesos;

typedef struct{
	int estado; // 0 = en ejecucion; 1 = Fin; 2 = Error
	int id_map;
	int bloque_archivo;
	char *nombre_archivo;
	char *nombre_archivo_resultado; // del mapeo
	int id_nodo; //de aca saco ip y puerto
}t_tablaProcesos_porJob;

typedef struct{
	char* ip_nodo;
	char* puerto;
	int numeroBloque;
	char* nombre_archivo_resultado; //donde va a devolverle el resultado
	int id_map;
}t_marta_job_map;



int recive_y_guarda_estructura(t_archivo *arch, int socket, uint32_t tamanioTotal);
void recive_y_guarda_infoNodo(int tamanio, int socket, void *array);
void  *conectarseAlJob(void*arg);
void atenderJob(void *arg);
void inicializar_pedido_FS();
void planificarMap(int a,int socketJob);
void planificarReduce(int id_job, int socketJob);


#endif /* LIBRERIAS_Y_ESTRUCTURAS_H_ */
