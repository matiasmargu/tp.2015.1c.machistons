/*
 * funciones.h
 *
 *  Created on: 9/5/2015
 *      Author: utnso
 */

#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <socket/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <./commons/collections/list.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

typedef struct{
	char* archivo;
}t_charpuntero;

typedef struct{
	t_marta_job Marta_Job;
	int	socketMarta;
	int	numeroDeBloque;
	int idNodo;
	t_job_nodo Job_Nodo;
	}t_conectarseAlNodo;


t_log* logger; // Log Global

void conectarseAlNodo(t_conectarseAlNodo CAN);
char* serializarRutina(t_job_nodo* jn, int tamanioTotal);
char* serializarJob_Nodo(t_job_nodo *job_nodo, int tamanioTotal);
char* serializar_charpuntero(t_charpuntero *nombre, int tamanioTotal);
char* serializar_job_marta(t_job_marta *job_marta, int tamanioTotal);
int recive_y_deserialisa_marta_job(t_marta_job *bloque, int socket, uint32_t tamanioTotal);
void liberarMensaje(char *);
void *mandarPruebaAMarta(void*arg);
void *mandarPrueba2ANodo(void*arg);

#endif /* FUNCIONES_H_ */
