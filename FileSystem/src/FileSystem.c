/*
 ============================================================================
 Name        : FileSystem.c
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
#include <../commons/config.h>
#include <../commons/log.h>
#include <../commons/string.h>
#include <../commons/collections/list.h>
#include "consola.h"
#include "manejoDeListas.h"
#include <pthread.h>
#include <socket/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	fd_set master;
	fd_set read_fds;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax;
	int listener;

	int newfd;
	char buf[1024];
	int yes = 1;
	int addrlen;
	int i;
	int status;

	logger = log_create("LOG_FILESYSTEM", "log_filesystem" ,false, LOG_LEVEL_INFO);

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/filesystem.conf";
	t_config* archivoConfiguracion;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	int puerto_listen = config_get_int_value(archivoConfiguracion, "PUERTO_LISTEN");
	char** lista_nodos = config_get_array_value(archivoConfiguracion, "LISTA_NODOS");

	t_list *listaArchivos;
	//t_list *listaBloquesCopias;

	pthread_t h1;

	listaArchivos = list_create();
	//listaBloquesCopias = list_create();

	list_add(listaArchivos, archivo_create("archivo1.txt", 2, "No disponible"));

	pthread_create(&h1, NULL, atenderConsola, NULL);

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	listener = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(puerto_listen);
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
				// inet_ntoa(clientaddr.sin_addr) -> es la IP de donde se conecto.
				// newfd -> es el socket nuevo
			}
			else
			{
				status = recv(i, (void*) buf, PACKAGESIZE, 0);
				if (status != 0) printf("%s", buf);
			}
		}
	}
	}
	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(lista_nodos);
	list_destroy(listaArchivos);
	return EXIT_SUCCESS;
}
