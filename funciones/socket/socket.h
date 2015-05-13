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


//Marta->Job
struct Marta_Job {
	int prueba3;
	int prueba4;
} t_marta_job;

//Job->Marta
struct Job_Marta {
	int prueba;
	int prueba2;
} t_job_marta;

//Marta->FS
struct Marta_FileSystem {
	char* nodo;
	char** bloques;
} t_marta_fs;

//FS->Marta
struct FileSystem_Marta{
	char* estado;
} t_fs_marta;

//Nodo->FS
struct Nodo_FileSystem{}t_nodo_fs;

//FS->Nodo
struct FileSystem_Nodo{
	char** bloques;
}t_fs_nodo;

//Nodo->Nodo
struct Nodo_Nodo{}t_nodo_nodo;

//Nodo->Job
struct Nodo_Job{}t_nodo_job;

//Job->Nodo
struct Job_Nodo{
	int numero_bloque;
	char* nombre;

}t_job_nodo;






int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
