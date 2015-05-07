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

t_log* logger; // Log Global

int main(void) {

	t_list *listaArchivos;
	//t_list *listaBloquesCopias;

	//pthread_t h1;
	//char* m1 = "h1";

	listaArchivos = list_create();
	//listaBloquesCopias = list_create();

	list_add(listaArchivos, archivo_create("archivo1.txt", 2, "No disponible"));

	//pthread_create(&h1, NULL, atenderConsola, NULL);

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
}
