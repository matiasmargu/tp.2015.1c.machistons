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
#include <filesystem/filesystem.h>
#include <cspecs/cspec.h>
#include <pthread.h>

int main(void) {

	imprimirMenu();

	t_list *listaArchivos;
	//t_list *listaBloquesCopias;

	pthread_t h1;
	char* m1 = "h1";

	listaArchivos = list_create();
	//listaBloquesCopias = list_create();

	list_add(listaArchivos, archivo_create("archivo1.txt", 2, "No disponible"));

	pthread_create(&h1, NULL, atenderConsola, NULL);

	printf("%i\n",list_size(listaArchivos));

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/filesystem.conf";

	t_config* archivoConfiguracion;
	t_log* logger;

	logger = log_create("LOG_FILESYSTEM", "log_filesystem" ,false, LOG_LEVEL_INFO);

	int puerto_listen;
	char** lista_nodos;

///////    Validacion y Carga del archivo de configuracion       ///////////////////////////////////////////

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	log_info(logger, "Se creo correctamente el archivo de configuracion");

	if(true == config_has_property(archivoConfiguracion,"PUERTO_LISTEN")){
			puerto_listen = config_get_int_value(archivoConfiguracion, "PUERTO_LISTEN");
		}else{
			log_error(logger, "Falta Puerto Listen");
			return EXIT_FAILURE;
	}

	if(true == config_has_property(archivoConfiguracion,"LISTA_NODOS")){
		lista_nodos = config_get_array_value(archivoConfiguracion, "LISTA_NODOS");
		}else{
			log_error(logger, "Falta lista de Nodos minimos");
			return EXIT_FAILURE;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("%i\n",puerto_listen);

	log_info(logger, "Correcta lectura del archivo de configuracion");

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(lista_nodos);
	list_destroy(listaArchivos);
	return EXIT_SUCCESS;
}
