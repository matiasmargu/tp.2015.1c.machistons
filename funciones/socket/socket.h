/*
 * servidor.h
 *
 *  Created on: 6/5/2015
 *      Author: utnso
 */

#ifndef SOCKET_SOCKET_H_
#define SOCKET_SOCKET_H_

#define BACKLOG 5			// Define cuantas conexiones vamos a mantener pendientes al mismo tiempo
#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar

#include <sys/socket.h>

typedef struct {
	int fdmax;
	fd_set master;
	int listener;
} t_socket_select;

//Marta->Job
typedef struct {
	char* nodo;
	char** bloques;
} t_marta_job;

//Job->Marta
typedef struct {
	char* estado;
} t_job_marta;

//Marta->FS
typedef struct {
	char* nodo;
	char** bloques;
} t_marta_fs;

//FS->Marta
typedef struct {
	char* estado;
} t_fs_marta;

int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
