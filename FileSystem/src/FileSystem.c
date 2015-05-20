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
#include <sys/mman.h>
#include <arpa/inet.h>

void *atenderNodo(void*arg){

	int socketNodo = (int)arg;
	int entero;

	entero = 56;

	send(socketNodo, &entero, sizeof(int),0);

	if((recv(socketNodo, &entero, sizeof(int),0 )) <= 0)
	{
		printf("socket Nodo caido\n");
	}
	else
	{
		printf("%i\n",entero);
	}
	return NULL;
}

int main()
{
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
	int entero; //Para el handshake
	int nodofd;

	logger = log_create("LOG_FILESYSTEM", "log_filesystem" ,false, LOG_LEVEL_INFO);

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/filesystem.conf";
	t_config* archivoConfiguracion;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	int puerto_listen = config_get_int_value(archivoConfiguracion, "PUERTO_LISTEN");
	char** lista_nodos = config_get_array_value(archivoConfiguracion, "LISTA_NODOS");

	t_list *listaArchivos;
	//t_list *listaBloquesCopias;

	pthread_t h1;
	pthread_t* hilosNodo;

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
	printf("select activo\n");
	for(i = 0; i <= fdmax; i++)
	{
	    if(FD_ISSET(i, &read_fds))
	    {
	    	if(i == listener)
	    	{
	        addrlen = sizeof(clientaddr);
	        if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1)
	        {
	        }
	        else
	        {
	        	FD_SET(newfd, &master);
	        	if(newfd > fdmax)
	        	{
	        		fdmax = newfd;
	        	}
	        	printf("Nueva coneccion %s en %d\n", inet_ntoa(clientaddr.sin_addr), newfd);
	        }
	    	}
	    	else
	    	{
	    		if((recv(i, &entero, sizeof(int),0 )) <= 0)
	    		{
	    			printf("socket caido\n");
	    			close(i); // Coneccion perdida
	    			FD_CLR(i, &master);
	    		}
	    		else
	    		{
	    			switch(entero){
	    			case 3: // Este es Marta
	    				entero = 45;
	    				send(i,&entero, sizeof(int),0);
	    				log_info(logger,"Hilo Marta creado satisfactoriamente");
	    				break;
	    			case 2: // Este es Nodo
	    				entero = 45;
	    				printf("socket nodo\n");
	    				send(i,&entero, sizeof(int),0);
	    				/*nodofd = i;
	    				hilosNodo = realloc(hilosNodo, sizeof(pthread_t)*i);
	    				pthread_create(hilosNodo+i, NULL, atenderNodo, (void *)nodofd);
	    				log_info(logger,"Hilo Nodo creado satisfactoriamente");*/
	    				break;
	    			}
	    		}
	    	}
	    }
	}
	}
	free(hilosNodo);
	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(lista_nodos);
	list_destroy(listaArchivos);
	return EXIT_SUCCESS;
}
