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
	uint32_t dni;
	char* name;
	char* lastname;
	uint32_t tamanioTotal;
}t_person;

void *atenderMarta(void*arg){

	int socketMarta = (int)arg;
	printf("%i",socketMarta);

	return NULL;
}

char* serializarPersona(t_person *persona){
	char *serializedPackage = malloc(persona->tamanioTotal);

	int offset = 0;
	int size_to_send;

	size_to_send =  sizeof(persona->dni);
	memcpy(serializedPackage + offset, &(persona->dni), size_to_send);
	offset += size_to_send;

	size_to_send =  sizeof(strlen(persona->name) + 1);
	memcpy(serializedPackage + offset, persona->name, size_to_send);
	offset += size_to_send;

	size_to_send =  sizeof(persona->lastname);
	memcpy(serializedPackage + offset, &(persona->lastname), size_to_send);
	offset += size_to_send;

	return serializedPackage;
}

void liberarMensaje(char **package){
	free(*package);
}

void completarMensajePersona(t_person *persona){
	//(persona->name)[strlen(persona.name)] = '\0';
	(persona->lastname)[strlen(persona->lastname)] = '\0';
	persona->tamanioTotal = sizeof(persona->dni) + sizeof(strlen(persona->name) + 1) + sizeof(persona->lastname);
}

int main()
{
	t_person persona;
	persona.dni = 37;
	persona.name = "holis";
	persona.lastname = "a ver si anda";

	char *mensaje;

	completarMensajePersona(&persona);

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
	    				mensaje = serializarPersona(&persona);
    					send(i, mensaje, persona.tamanioTotal, 0);
    					liberarMensaje(&mensaje);
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

