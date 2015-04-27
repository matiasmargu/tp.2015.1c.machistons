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
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>

typedef struct {
	int socket; // Socket del Nodo para comunicacion
	char* nodo; // Nombre del Nodo
	char* estado; // Estado disponible o no disponible del nodo
	t_list bloque; // Lista de bloques libres
} t_socket_bloqueslibres;

typedef struct {
	int index; // Socket del Nodo para comunicacion
	char* directorio; // Nombre del Nodo
	int padre;
} t_directorio;

typedef struct {
	int bloque;

} t_bloque_copias;

typedef struct {
	char* nombre;
	char* direccion;
	char* estado;
	t_bloque_copias bloque;
} t_archivo;

void imprimirMenu(void){

	printf("> Los comandos se ingresan con su numero \n"
			"> 		COMANDOS \n"
			"	Formatear el MDFS: 0 \n"
			"	Operaciones sobre Archivos  \n"
			"	  Eliminar: 1 \n"
			"	  Renonmbrar: 2 \n"
			"	  Mover: 3 \n"
			"	Operaciones sobre Directorios  \n"
			"	  Crear: 4 \n"
			"	  Eliminar: 5 \n"
			"	  Renombrar: 6 \n"
			"	  Mover: 7 \n"
			"	Operaciones sobre bloques de un archivo \n"
			" 	  Ver: 8 \n"
			" 	  Borrar: 9 \n"
			" 	  Copiar: 10 \n"
			" 	Operaciones sobre nodos de datos:\n"
			"	  Agregar: 11 \n"
			"	  Eliminar: 12 \n"
			"	Copiar un archivo local al MDFS: 13 \n"
			"	Copiar un archivo del MDFS al filesystem local: 14 \n"
			"	Solicitar el MD5 de un archivo en MDFS: 15 \n"
			"	SALIR 16 \n");
	return ;
}
int main(void) {

	imprimirMenu();
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
	free(rutaArchivoConfiguracion);
	free(lista_nodos);
	return EXIT_SUCCESS;
}
