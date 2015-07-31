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
int idJobGlobal;
int cant_nodos;
int *contadores_nodos;
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
	t_list *lista;
}t_prueba;

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


typedef struct {
	int idNodo;
	char* puertoNodo;
	char * ipNodo;
	t_list * archivosAReducir;
    char* nombreArchivoResultado;
    int estado;
}t_archivosAReducirPorNodo;

typedef struct {
	char *puertoNodo;
	char *ipNodo;
	t_list * archivosAMover;
}t_moverArchivos;

typedef struct {
	char *puertoNodo;
	char *ipNodo;
	char * archivoAMover;
}t_serializarUnArchivoParaMover;

typedef struct {
	int contador;
	int idNodo;
}t_contadorNodo;

int recive_y_guarda_estructura(t_archivo *arch, int socket, uint32_t tamanioTotal);
void recive_y_guarda_infoNodo(int tamanio, int socket, void *array);
void  *conectarseAlJob(void*arg);
//int recive_y_deserialisa_job(t_job_marta* job_marta, int socket,uint32_t tamanioTotal );
//char* serializar_estructura_t_marta_a_job(t_marta_job estructura_t_marta_a_job, int tamanioTotal);

void atenderJob(void *arg);
void inicializar_pedido_FS();
void planificarMap(int a,int socketJob);
void aplicarReduce(int idJob);


#endif /* LIBRERIAS_Y_ESTRUCTURAS_H_ */
