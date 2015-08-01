/*
 * servidor.h
 *
 *  Created on: 6/5/2015
 *      Author: utnso
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#define BACKLOG 5			// Define cuantas conexiones vamos a mantener pendientes al mismo tiempo
#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar

#include  "log.h"
#include "list.h"




//Marta->Job
 /*typedef struct{
	char* ip_nodo;
	char* puerto;
	int numeroBloque;
	char* nombre_archivo_resultado; //donde va a devolverle el resultado
	int idNodo;
}t_marta_job_map;
*/

typedef struct{
	int idNodo;
	t_list* listaArchivosTemporales;
	char* archivoResultadoReduce;
	char* ipNodo;
	char* puertoNodo;
	int cantidadArchivos;

}t_marta_job_reduce;

typedef struct{
	char* ipAmover;
	char* puertoAmover;
	char* archivoAmover;
	char* puertoAconectarse;
	char* ipAConectarse;
}t_para_job;


//JOB -> MARTA
typedef struct{
	int numeroBloque;
	int rutina;
	int resultado; // 0 = FALLO , 1 = EXITOSO
	int idNodo;
	char* nombreArchivo;   //AGREGAR ESTO EN LA PARTE DEL SEND A MARTA DEL NODOO
}t_job_marta_map;

typedef struct{
	int rutina;
	int idNodo;
	int resultado; // 0 = FALLO , 1 = EXITOSO
	char* nombreArchivo;
}t_job_marta_reduce;

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
	t_list* archivosAreducir;
	char* nombreArchivoResultado;
	int cantidadArchivos;
}t_job_nodo_reduce;



typedef struct{
 	char* ip;
 	char* puerto;
 	char* archivo;
 }t_para_nodo;

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
	int tamanioArchivoDatos;
}estructuraIPyNodo;

typedef struct{
	char* data;
	int tamanioData;
}estructuraGetBloque;

typedef struct{// a la hora de mandar el tamanio total tenemos que ver si es map o reduce lo que nos va  a llegar
	int tamanio;
    int rutina;
} t_tamanio;

typedef struct{
	int rutina; // 1 mapper 2 reducer
	char* ip_nodo;
	char* puerto;
	t_list* lista_nombres_archivos_temporales; // donde devuelve el resultado
	int idNodo;
}t_marta_job_archivo_reduce;

typedef struct{
	char* nombreArchivo;
	t_list listaDeBloques;
	int idNodo;
}t_datos_map;


int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
