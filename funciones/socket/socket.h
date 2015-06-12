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

#include <./commons/log.h>



//int cantidadDeBloques;


//Marta->Job
 typedef struct{
	int rutina; //1=mapper o 2=reducer
	char* ip_nodo;
	char* nombreNodo;
	char* puerto;
	int cantidadDeBloques;
	char* nombre_archivo_resultado; //donde va a devolverle el resultado

}t_marta_job2;

//Job->Marta
typedef struct{
   char* operacionID;
   char* archivo_resultado;

   }t_job_marta_resultado;



//Marta->FS
struct Marta_FileSystem {
	char** lista_archivos;
}t_marta_fs;

//FS->Marta

struct fs_marta{

};

struct infoArchivos{
	char* nombre;

}t_fsarchivo_marta;

//Nodo->FS
struct Nodo_FileSystem{

}t_nodo_fs;

//Nodo->Nodo
struct Nodo_Nodo{}t_nodo_nodo;

//Nodo->Job
typedef struct {
	int resultado; // 1 = EXITOSO; 2 = FALLO

}nodo_job;

//Job->Nodo
typedef struct{


	int resultado;
	int nombreRutina; //1=mapper o 2=reducer
    int NumerobloqueDeDAtos;

}t_job_nodo_mapper;

typedef struct{

}t_job_nodo_reduce;

typedef struct{
 char* rutinaEjecutable; //debe ser FILE REVISAR
 int tipoRutina;
}t_job_nodo;

typedef struct{
}t_mar;


// FileSystem a Nodo

typedef struct{
	int bloque;
	char* data;
	int tamanioData;
}estructuraSetBloque;

//Nodo a FileSystem

typedef struct{
	char* IP;
	char* PUERTO;
}estructuraIPyNodo;

//JOB
typedef struct{
	t_marta_job2 Marta_Job;
	int	socketMarta;
	int	numeroDeBloque;
	t_job_nodo Job_Nodo;
	t_log* logger;
	}t_conectarseAlNodo;

int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
