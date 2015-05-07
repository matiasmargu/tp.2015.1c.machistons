/*
 ============================================================================
 Name        : Job.c
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>

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

	int socketCliente = crearServidor("6667");

	char package[PACKAGESIZE];
	int status = 1;		// Estructura que manjea el status de los recieve.
	printf("Cliente conectado. Esperando mensajes:\n");

	while (status != 0){
		status = recv(socketCliente, (void*) package, PACKAGESIZE, 0);
		if (status != 0) printf("%s", package);
		}

	close(socketCliente);

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("%s\n%i\n%s\n%i\n%s\n%s\n%s\n",ip_fs,puerto_fs,ip_nodo,puerto_nodo,archivo_bin,dir_temp,nodo_nuevo);

	log_info(logger, "Correcta lectura del archivo de configuracion");




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
