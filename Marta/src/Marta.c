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








int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	char* puerto_fs ;
	char* ip_fs ;
	char* puerto ;
	char* un_archivo;
	int cantidad;
	//char* listaDeArchivos[cantidad];
	int handShake;
	char* combiner;
	int b;
	int saludo;



	int tipo_mens ;
	t_marta_job2 mj;
	struct fs_marta Fs_Marta;

	t_config* archivoConfiguracion;

    // para recibir la cantidad de archivos

    int a;



	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");



	int socketjob  = crearServidor("3000");


   printf("SERVIDOR EN ESPERA...\n");

   recv(socketjob, &handShake, sizeof(int),0);

   saludo = 3;

   send(socketjob, &saludo, sizeof(int),0);

   printf("Se conecto el job con el hadshake: %i \n",handShake);


   recv(socketjob, &cantidad, sizeof(int),0);
/*
   	   for(a = 0 ; a <= cantidad; a++){

   			 recv(socketjob,&un_archivo,strlen(un_archivo)+1,0);

   			listaDeArchivos[a] = un_archivo;

   		}



	recv(socketjob,&combiner,strlen(combiner)+1,0);

	// LLENAR LOS CAMPOS DE MJ CON LA PLANIFICACION DE LO QUE LE VA A MANDAR AL JOB
int tamanioTotal = sizeof(int)+sizeof(int)+ strlen(mj.ip_nodo) + 1 + strlen(mj.nombreNodo)+1 + strlen(mj.nombre_archivo_resultado)+1+strlen(mj.puerto)+1;
	send(socketjob, &tamanioTotal , sizeof(int), 0);

	for(b = 0 ; b <= cantidad; b++){

		int numero;
	//	numero = lista_bloques[b];  ESTA LISTA SALE DE FS

		send(socketjob,&numero,sizeof(numero),0);
	}


///job



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
 *//**/
	close(socketjob);
//	close(socketFS);
	return EXIT_SUCCESS;
}

