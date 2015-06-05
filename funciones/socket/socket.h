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


struct infoNodo{
	char* ip_nodo;
	char* nombre;
	char* puerto;
};


//Marta->Job
 struct marta_job{
    int operacionID;  //identifica el numero de operacion
	int rutina; //1=mapper o 2=reducer
	struct infoNodo InfoNodo; // estructura que contiene el nombre, ip y puerto del nodo
	char** ListaDeBloques;
	char* nombre_archivo_resultado; //donde va a devolverle el resultado
};


//Job->Marta(inicio)
 struct job_marta_inicio{
	int operacionID;
	char* combiner;
	char** lista_archivos;

};


//Job->Marta
struct job_marta_resultado{
   char* operacionID;
   char* archivo_resultado;

   };



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

typedef struct{
	int numero;
	char *bloque;
}setBloque;

//Nodo->Nodo
struct Nodo_Nodo{}t_nodo_nodo;

//Nodo->Job
struct nodo_job{
	int operacionID;
	char* archivo_resultado;

};

//Job->Nodo
struct job_nodo{
    int operacionID;
	char* ip_nodo;
	char* nombre;
	char* archivo_resultado;
    char* rutina;
    int NumerobloqueDeDAtos;

};





int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
