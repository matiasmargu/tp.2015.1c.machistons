/*
 ============================================================================
 Name        : Marta.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>
#include <./commons/collections/list.h>
#include "functions.h"
#include "server-client.h"


t_log* logger; // Log Global



typedef struct{
	char* nombre;
	int cantidadDeBloques;
	t_list copia1;
	t_list copia2;
	t_list copia3;

}t_archivo;

typedef struct {
    int Numerobloque;
    int idNodo;

} t_copia;



static t_copia *copia_create(int NumeroBloque,int idNodo) {
    t_copia *new = malloc(sizeof(t_copia));
    new->Numerobloque = NumeroBloque;
    new->idNodo = idNodo;
    return new;
}

static void copia_destroy(t_copia *self) {
    free(self);
    free(self);
}


describe("copia"){
	t_list *copia;
before{
copia = list_create();
}end
after {
           list_destroy_and_destroy_elements(copia, (void*) copia_destroy);
       } end
} end




int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	char* puerto_fs ;
	char* ip_fs ;
	char* puerto ;
	int entero;
	t_archivo archivo;

	int tipo_mens ;
	struct fs_marta Fs_Marta;

	t_config* archivoConfiguracion;

    // para recibir la cantidad de archivos
	int cantidad;
    int a;

    //

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");

	int cantidad;

	int socketEscJob  = crearsocketEscuha(PUERTO);

	escuchar(socketEscJob);

   printf("SERVIDOR EN ESPERA...\n");

   int socketJob = aceptarconexion(socketEscJob);

   recv(socketJob,&tipo_mens,sizeof(int),0 );

   switch(tipo_mens)
   {
   	case 001:
   	   int socketjob = aceptarconexion(socketEscJob);

   	   recv(socketjob, &cantidad, sizeof(int),0);

   	       for(a = 0 ; a <= cantidad; a++){

   			 recv(socketJob,&archivo_Inicio,strlen(archivo)+1,0);

   			//Agregar a un array

   		}
   		//acciones
   		break;
   	case valor_2:
   		//acciones
   		break;

   	case valor_n:
   		//acciones
   		break;
   	defalut:
   		//acciones
   		break;
   }

   if (tipo_mens == 0001){


   }

	recv(socketJob,&combiner,strlen(combiner)+1,0);


	int socketFS = crearsocketCliente(ip_fs,puerto_fs);

	int tamanioArch; //tamaño del archivo que recibimos

	char*archivo;

	socketFS = crearsocketServer (ip_fs, puerto_fs);

	send(socketFS,archivo,tamanioArch,0);


	printf("Conectado al servidor Filesystem.\n");

	//send(socketFS, &listadearchivos );


	//recv(socketFs, &cantidadNodos)
	int n;

	for(n = 0 ; n <= cantidadNodos; n++){

	//	recv(socketFS , &STRUCTDELNODO);
	}

	//recv(socketFS, &archivo ,  )

	//deserailizar



	//* 	El sistema esperara hasta que reciba una conexion entrante...

	printf("SERVIDOR EN ESPERA...\n");

	struct sockaddr_in addr;			// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);

	int socketJob = accept(servidorEscucha, (struct sockaddr *) &addr, &addrlen);

	struct job_marta_inicio package;

	int status = 1;		// Estructura que maneja el status de los recieve.

	printf("Cliente conectado. Esperando Envío de mensajes.\n");

			while (status){
				status = recieve_and_deserialize(&package, socketJob);
				if (status) printf("%i es: %s \n", package.operacionID, package.lista_archivos);
			}


			printf("Cliente Desconectado.\n");

			close(socketJob);
			close(servidorEscucha);



			return 0;
		}


	/*
	 int socketJob = crearServidor(puerto);
	int socketFS = crearCliente(ip_fs, puerto_fs);



	if ((recv(socketJob, &Job_Marta_Inicio, sizeof(struct job_marta_inicio),0 )) != 0){

		printf ("Conexion establecida con el proceso Job\n");
		//printf("se conecto el Job con la operacion numero %i\n",Job_Marta_Inicio.operacionID);

		send(socketFS,&Job_Marta_Inicio.lista_archivos, sizeof(char**),0);

		if ((recv(socketFS, &Fs_Marta, sizeof(struct fs_marta),0 )) != 0){



		}

	}
*/
/*	Marta_Job.NumeroBloqueDeDatos = 12;
	Marta_Job.rutina = "mapper";
    Marta_Job.operacionID = 3;

	send(socketJob, &Marta_Job, sizeof(struct marta_job),0);

	Marta_Job.rutina = "reduc";
	Marta_Job.operacionID = 4;
	Marta_Job.NumeroBloqueDeDatos = 16;

	send(socketJob, &Marta_Job, sizeof(struct marta_job),0);

	entero = 3;    // handshake con FS
	send(socketFS,&entero, sizeof(int),0);

	if ((recv(socketFS, &entero, sizeof(int),0 )) != 0){
		printf("fs me respondio esto: %i\n",entero);
	}
 *//*
	close(socketJob);
	close(socketFS);
	return EXIT_SUCCESS;
}*/

