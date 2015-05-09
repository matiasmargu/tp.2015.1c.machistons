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


t_log* logger; // Log Global

#define PORT 2020

int main(int argc, char *argv[])
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

FD_ZERO(&master);
FD_ZERO(&read_fds);

listener = socket(AF_INET, SOCK_STREAM, 0);
setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

serveraddr.sin_family = AF_INET;
serveraddr.sin_addr.s_addr = INADDR_ANY;
serveraddr.sin_port = htons(PORT);
memset(&(serveraddr.sin_zero), '\0', 8);
bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
listen(listener, 10);
FD_SET(listener, &master);

fdmax = listener;

for(;;)
{
read_fds = master;
select(fdmax+1, &read_fds, NULL, NULL, NULL);
printf("Server-select() is OK...\n");

for(i = 0; i <= fdmax; i++)
{
    if(FD_ISSET(i, &read_fds))
    {
    if(i == listener)
     {
        addrlen = sizeof(clientaddr);
        if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1)
        {
        	perror("Server-accept() error lol!");
        }
        else
        {
        	printf("Server-accept() is OK...\n");

        	FD_SET(newfd, &master);
        	if(newfd > fdmax)
        	{
        		fdmax = newfd;
        	}
        	printf("%s: New connection from %s on socket %d\n", argv[0], inet_ntoa(clientaddr.sin_addr), newfd);
        }
     }
    else
    {
    	status = recv(i, (void*) buf, PACKAGESIZE, 0);
    	if (status != 0) printf("%s", buf);
    }
    }
}
}
return 0;
}

/*

	t_list *listaArchivos;
	//t_list *listaBloquesCopias;

	pthread_t h1;

	listaArchivos = list_create();
	//listaBloquesCopias = list_create();

	list_add(listaArchivos, archivo_create("archivo1.txt", 2, "No disponible"));

	pthread_create(&h1, NULL, atenderConsola, NULL);

	printf("%i\n",list_size(listaArchivos));

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/filesystem.conf";
	t_config* archivoConfiguracion;
	logger = log_create("LOG_FILESYSTEM", "log_filesystem" ,false, LOG_LEVEL_INFO);
	int puerto_listen;
	char** lista_nodos;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_listen = config_get_int_value(archivoConfiguracion, "PUERTO_LISTEN");
	lista_nodos = config_get_array_value(archivoConfiguracion, "LISTA_NODOS");

	printf("%i\n",puerto_listen);

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(lista_nodos);
	list_destroy(listaArchivos);
	return EXIT_SUCCESS;
	*/
