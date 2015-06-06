/*
 ============================================================================
 Name        : FileSystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "librerias_y_estructuras.h"

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

char* serializarParaGetBloque(setBloque *bloque){

	char *serializedPackage = malloc(bloque->tamanioDatos);

	int offset = 0;
	int size_to_send;

	size_to_send =  sizeof(bloque->numero);
	memcpy(serializedPackage + offset, &(bloque->numero), size_to_send);
	offset += size_to_send;

	int tamanioNombre = strlen(bloque->bloque) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(bloque->bloque) + 1;
	memcpy(serializedPackage + offset, bloque->bloque, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}

char* serializarPersona(t_person *persona){
	char *serializedPackage = malloc(persona->tamanioTotal);

	int offset = 0;
	int size_to_send;

	size_to_send =  sizeof(persona->dni);
	memcpy(serializedPackage + offset, &(persona->dni), size_to_send);
	offset += size_to_send;

	int tamanioNombre = strlen(persona->name) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(persona->name) + 1;
	memcpy(serializedPackage + offset, persona->name, size_to_send);
	offset += size_to_send;

	tamanioNombre = strlen(persona->lastname) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(persona->lastname) + 1;
	memcpy(serializedPackage + offset, persona->lastname, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}

void liberarMensaje(char **package){
	free(*package);
}

void completarMensajePersona(t_person *persona){
	//(persona->name)[strlen(persona->name)] = '\0';
	//(persona->lastname)[strlen(persona->lastname)] = '\0';
	persona->tamanioTotal = sizeof(persona->dni) + sizeof(int) + sizeof(int) + strlen(persona->name) + 1 + strlen(persona->lastname) + 1;
}

int main()
{
	t_person persona;
	persona.dni = 37;
	persona.name = "holisadass";
	persona.lastname = "DAVID LA PUTA QUE TE PARIO";
	char *mensaje;

	fd_set master;
	fd_set read_fds;

	// Estructuras de Interfaz con Nodo

	setBloque enviarBloqueAEscribir;

	//

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

	// Inicializo la base MongoDB
	mongoc_init ();
	client = mongoc_client_new ("mongodb://localhost:27017/");
	collection = mongoc_client_get_collection (client, "test", "test");

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
	    				enviarBloqueAEscribir.numero = 72;
	    				enviarBloqueAEscribir.bloque = "david la puta que te pario";
	    				enviarBloqueAEscribir.tamanioDatos = sizeof(int) + sizeof(int) + strlen(enviarBloqueAEscribir.bloque) + 1;
	    				send(i, &enviarBloqueAEscribir.tamanioDatos, sizeof(enviarBloqueAEscribir.tamanioDatos), 0);
	    				mensaje = serializarParaGetBloque(&enviarBloqueAEscribir);
	    				send(i, mensaje, enviarBloqueAEscribir.tamanioDatos, 0);
	    				liberarMensaje(&mensaje);
	    				/*completarMensajePersona(&persona);
	    				send(i, &persona.tamanioTotal, sizeof(persona.tamanioTotal),0);
	    				mensaje = serializarPersona(&persona);
    					send(i, mensaje, persona.tamanioTotal, 0);
    					liberarMensaje(&mensaje);*/
	    				break;
	    			}
	    		}
	    	}
	    }
	}
	}

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);
	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	return EXIT_SUCCESS;
}

