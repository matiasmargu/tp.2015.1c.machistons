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



//int cantidadDeBloques;

//Marta->Job
 typedef struct{
    int operacionID;  //identifica el numero de operacion
	int rutina; //1=mapper o 2=reducer
	char* ip_nodo;
	char* nombreNodo;
	char* puerto;
	int cantidadDeBloques;
//	int ListaDeBloques[cantidadDeBloques];
	char* nombre_archivo_resultado; //donde va a devolverle el resultado

}t_marta_job;




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

//FS->Nodo
struct FileSystem_Nodo{
	char** bloques;
}t_fs_nodo;

//Nodo->Nodo
struct Nodo_Nodo{}t_nodo_nodo;

//Nodo->Job
typedef struct {
	int resultado; // 1 = EXITOSO; 2 = FALLO

}nodo_job;

//Job->Nodo
typedef struct{


	char* resultado;
	int nombreRutina; //1=mapper o 2=reducer
    int NumerobloqueDeDAtos;

}t_job_nodo_mapper;

typedef struct{

}t_job_nodo_reduce;

// // FILE* rutinaEjecutable;




typedef struct{
	t_marta_job Marta_Job;
	int	socketMarta;
	int	numeroDeBloque;
	}t_conectarseAlNodo;

int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
