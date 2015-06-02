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
#include "functions.h"

t_log* logger; // Log Global



int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	char* puerto_fs ;
	char* ip_fs ;
	char* puerto ;
	int entero;

	struct job_marta_inicio Job_Marta_Inicio;
	struct marta_job Marta_Job;
	struct job_marta_resultado Job_Marta_Resultado;
	struct fs_marta Fs_Marta;

	t_config* archivoConfiguracion;

	puerto = "3000" ;

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");

	// printf("el ip de fs es %i y su puerto %i\n",ip_fs,puerto_fs );


	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// No importa si uso IPv4 o IPv6
	hints.ai_flags = AI_PASSIVE;		// Asigna el address del localhost: 127.0.0.1
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

	getaddrinfo(NULL, puerto, &hints, &serverInfo); // Notar que le pasamos NULL como IP, ya que le indicamos que use localhost en AI_PASSIVE

 	//Mediante socket(), obtengo el File Descriptor que me proporciona el sistema (un integer identificador).

	/* Necesitamos un socket que escuche las conecciones entrantes */

	int servidorEscucha ; //listenningSocket;

	servidorEscucha = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	/*
	 * ya tengo un archivo que puedo utilizar para analizar las conexiones entrantes.... ¿Por donde?
	 *
	 * 	Necesito decirle al sistema que voy a utilizar el archivo que me proporciono para escuchar las conexiones por un puerto especifico.
	 *  Todavia no estoy escuchando las conexiones entrantes!
	  */
	bind(servidorEscucha,serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo);

	/*
	 * 	Ya tengo un medio de comunicacion (el socket)
	*/

	listen(servidorEscucha, BACKLOG); //listen() es una syscall BLOQUEANTE.
	 //* 	El sistema esperara hasta que reciba una conexion entrante...

	struct sockaddr_in addr;			// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);

	int socketJob = accept(servidorEscucha, (struct sockaddr *) &addr, &addrlen);

	struct t_Package package;

	int status = 1;		// Estructura que manjea el status de los recieve.

			printf("Cliente conectado. Esperando Envío de mensajes.\n");

			while (status){
				status = recieve_and_deserialize(&package, socketJob);					// Ver el "Deserializando estructuras dinamicas" en el comentario de la funcion.
				if (status) printf("%s says: %s", package.username, package.message);
			}


			printf("Cliente Desconectado.\n");
			/*
			 * 	Terminado el intercambio de paquetes, cerramos todas las conexiones y nos vamos a mirar Game of Thrones, que seguro nos vamos a divertir mas...
			 *
			 *
			 * 																					~ Divertido es Disney ~
			 *
			 */
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

