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
struct Marta_Job{
    char* operacionID;  //identifica el numero de operacion
	char* rutina; //mapper o reducer
	int NumerobloqueDeDatos; //donde aplicar el mapper o reduce
	char* nombreNodo;
	char* ipNodo;
	char* archivo_resultado; //donde va a devolverle el resultado
}t_marta_job;

//Job->Marta(inicio)
struct Job_Marta_Inicio{
	char* operacionID;
	char** lista_archivos;
};


//Job->Marta
  struct Job_Marta_Resultado{
   char* operacionID;
   char* archivo_resultado;
   char* combiner;
   }t_job_marta_resultado;



//Marta->FS
struct Marta_FileSystem {
	int prueba3;
	char prueba4;
} t_marta_fs;

//FS->Marta
struct FileSystem_Marta{
	int prueba;
	char prueba2;
} t_fs_marta;

//Nodo->FS
struct Nodo_FileSystem{

}t_nodo_fs;

//FS->Nodo
struct FileSystem_Nodo{
	char** bloques;
}t_fs_nodo;

//Nodo->Nodo
struct Nodo_Nodo{}t_nodo_nodo;

/*//Nodo->Job
struct Nodo_Job{
	char* operacionID;
	char* archivo_resultado;

}t_nodo_job;

*/
/*
//Job->Nodo
struct Job_Nodo{
    char* operacionID;
	char* ip_nodo;
	char* nombre;
	char* archivo_resultado;
    char* rutina;
    int NumerobloqueDeDAtos;

}t_job_nodo;
*/





int crearServidor(char* PUERTO);
int crearCliente (char *IP, char *PUERTO);

#endif /* SOCKET_SOCKET_H_ */
