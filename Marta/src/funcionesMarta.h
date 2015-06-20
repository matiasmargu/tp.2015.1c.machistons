
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include <stdio.h>
#include <stdlib.h>
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
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

t_log* logger; // Log Global

// VARIABLE GLOBAL
t_list *lista_archivos;
t_list *lista_nodos_estado;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_nodos = PTHREAD_MUTEX_INITIALIZER;

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
	t_copia **copias;
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
	int id_job;
	int bloque;
	char estado; // M = MAP; R = REDUCE
}t_datos;




int recive_y_deserialisa(t_charpuntero* nombre, int socket, uint32_t tamanioTotal);
int recive_y_guarda_estructura(t_archivo arch, int socket, uint32_t tamanioTotal);
char* serializar_charpuntero(t_charpuntero* nombre, int tamanioTotal);
void  *conectarseAlJob(void*arg);



#endif /* FUNCTIONS_H_ */
