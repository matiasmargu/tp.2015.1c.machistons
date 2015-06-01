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
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>
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

typedef struct{
	int dni;
	int name;
	int lastname;
}t_person;

void *atenderMarta(void*arg){

	int socketMarta = (int)arg;
	printf("%i",socketMarta);

	return NULL;
}


void serializarPersona(t_person persona, char** message){
	int offset = 0;
	memcpy(*message, &(persona.dni), sizeof(int));
	offset = sizeof(persona.dni);
	memcpy(*message + offset, &((persona).name), sizeof(int));
	offset = sizeof(int) + offset;
	memcpy(*message + offset, &((persona).lastname), sizeof(int));
}

int main()
{

	t_person persona;
	persona.dni = 37;
	persona.name = 25;
	persona.lastname = 69;

	int packageSize = sizeof(int) + sizeof(int) + sizeof(int);
	char *message = malloc(packageSize);


	fd_set master;
	fd_set read_fds;

	pthread_t hiloConsola;
	pthread_t hiloMarta;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax, listener, newfd, yes = 1, addrlen, i;
	int entero; //Para el handshake
	int martafd; //Socket de coneccion con Marta

	pthread_create(&hiloConsola, NULL, atenderConsola, NULL);

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/filesystem.conf";

	t_config* archivoConfiguracion;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	int puerto_listen = config_get_int_value(archivoConfiguracion, "PUERTO_LISTEN");
	int nodosNecesarios = config_get_int_value(archivoConfiguracion, "LISTA_NODOS");

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

	logger = log_create("LOG_FILESYSTEM", "log_filesystem" ,false, LOG_LEVEL_INFO);

	t_list *listaArchivos;
	//t_list *listaBloquesCopias;

	listaArchivos = list_create();
	//listaBloquesCopias = list_create();

	int z = list_add(listaArchivos, archivo_create("archivo1.txt", 2, "No disponible"));
	int a = list_add(listaArchivos, archivo_create("archivo2.txt", 2, "No disponible"));
	int j = list_add(listaArchivos, archivo_create("archivo3.txt", 2, "No disponible"));


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
				}
	    	}
	    	else
	    	{
	    		if((recv(i, &entero, sizeof(int),0 )) <= 0)
	    		{
	    			close(i); // Coneccion perdida
	    			FD_CLR(i, &master);
	    		}
	    		else
	    		{
	    			switch(entero){
	    			case 3: // Este es Marta
	    				martafd = i;
	    				pthread_create(&hiloMarta, NULL, atenderMarta, (void *)martafd);
	    				log_info(logger,"Hilo Marta creado satisfactoriamente");
	    				break;
	    			case 2: // Este es Nodo
	    				serializarPersona(persona, &message);
    					send(i, message, packageSize, 0);
	    				break;
	    			}
	    		}
	    	}
	    }
	}
	}

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	list_destroy(listaArchivos);
	return EXIT_SUCCESS;
}

