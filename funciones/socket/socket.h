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
typedef struct{
    int operacionID;  //identifica el numero de operacion
	char* rutina; //mapper o reducer
	int NumeroBloqueDeDatos; //donde aplicar el mapper o reduce
	char* nombreNodo;
	char* ipNodo;
	char* puertoNodo;
	char* archivo_resultado; //donde va a devolverle el resultado
}t_marta_job;


//Job->Marta(inicio)
 typedef struct{
	int operacionID;
	char** lista_archivos;
}job_marta_inicio;


//Job->Marta
 typedef struct{
   char* operacionID;
   char* archivo_resultado;
   char* combiner;
   }t_job_marta_resultado;



//Marta->FS
struct Marta_FileSystem {
	char** lista_archivos;
}t_marta_fs;

//FS->Marta

struct infoNodos{
	char* ip_nodo;
	char* nombre;
	char* puerto;
} t_fs_marta;

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
typedef struct{
	int operacionID;
	char* archivo_resultado;

}t_nodo_job;

//Job->Nodo
typedef struct{
    int operacionID;
	char* ip_nodo;
	char* nombre;
	char* archivo_resultado;
    char* rutina;
    int NumerobloqueDeDAtos;

}t_job_nodo;





int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
