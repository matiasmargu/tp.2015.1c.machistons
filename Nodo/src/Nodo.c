/*
 ============================================================================
 Name        : Nodo.c
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
#include <./commons/collections/list.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


t_log* logger; // Log Global

int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";

	t_config* archivoConfiguracion;

	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);

	int puerto_fs;
	char* ip_fs;
	char* archivo_bin;
	char* dir_temp;
	char* nodo_nuevo;
	char* ip_nodo;
	int puerto_nodo;

///////    Carga del archivo de configuracion       ///////////////////////////////////////////

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	log_info(logger, "Se creo correctamente el archivo de configuracion");
	puerto_fs = config_get_int_value(archivoConfiguracion, "PUERTO_FS");
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
	archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
	dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
	nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
	ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
	puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");

////////////////////// Creo El Select ////////////////////////////////////////////////////

	    fd_set master;
		fd_set read_fds;

		struct sockaddr_in serveraddr;
		struct sockaddr_in clientaddr;

		int fdmax;
		int listener;
		int newfd;
		int yes = 1;
		int addrlen;
		int i;


		//pthread_t h1;

		FD_ZERO(&master);
		FD_ZERO(&read_fds);

		listener = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = INADDR_ANY;
		serveraddr.sin_port = htons(puerto_fs);
		memset(&(serveraddr.sin_zero), '\0', 8);
		bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
		listen(listener, 10);
		FD_SET(listener, &master);

		fdmax = listener;

		for(;;)
		{
		read_fds = master;
		select(fdmax+1, &read_fds, NULL, NULL, NULL);
		for(i = 0; i <= fdmax; i++)
		{
			if(FD_ISSET(i, &read_fds))
			{
				if(i == listener)
				{
					addrlen = sizeof(clientaddr);
					newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen);
					FD_SET(newfd, &master);
					if(newfd > fdmax)
					{
						fdmax = newfd;
					}
					// Nueva coneccion
					// inet_ntoa(clientaddr.sin_addr) -> es la IP de donde se conecto. // creo que es 127.0.0.2
					// newfd -> es el socket nuevo
					// Aca tienen que crear los hilos, aca deben hacer el handshake de si es Nodo, FS o Job y por aca uno levantar un hilo para atenderlo.
					// Ejemplo:
					// Si lo que se conecto es un hilo de job: pthread_create(&h1, NULL, atenderJob, NULL);
					// Si lo que se conecto es un Nodo: pthread_create(&h1, NULL, atenderNodo, NULL);
					// Si lo que se conecto es el FS: pthread_create(&h1, NULL, atenderFS, NULL);


					/* podemos hacer un switch en base a la direccion del cliente, y con los case vamos haciendo los distintos hilos
					   tendria que tener algo de cada caso, que me indique que es


					   switch () {
					 * case "respecto al job":
					 *    pthread_create(&h_job, NULL, atenderJob, NULL);
					 *    break;
					 * case "respecto al Nodo":
					 *    pthread_create(&h_nodo, NULL, atenderJob, NULL);
					 *    break;
					 * case "respecto al FS":
					 *    pthread_create(&h_FS, NULL, atenderJob, NULL);
					 *    break;
					 * }
					 *
					 */
				}
				else
				{
					// Aca entra cuando la coneccion ya existe, todavia no sabemos bien que hacer con esto. Hay que definir bien que hacer.
				}
			}
        }
   }
	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(rutaArchivoConfiguracion);
	free(ip_fs);
	free(ip_nodo);
	free(archivo_bin);
	free(dir_temp);
	free(nodo_nuevo);
	return EXIT_SUCCESS;
}
