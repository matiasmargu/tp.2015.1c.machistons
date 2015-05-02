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
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>


int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";

	t_config* archivoConfiguracion;
	t_log* logger;

	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);

	int puerto_fs;
	char* ip_fs;
	char* archivo_bin;
	char* dir_temp;
	char* nodo_nuevo;
	char* ip_nodo;
	int puerto_nodo;

///////    Validacion y Carga del archivo de configuracion       ///////////////////////////////////////////

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	log_info(logger, "Se creo correctamente el archivo de configuracion");

	if(true == config_has_property(archivoConfiguracion,"PUERTO_FS")){
			puerto_fs = config_get_int_value(archivoConfiguracion, "PUERTO_FS");
		}else{
			log_error(logger, "Falta Puerto Filesystem");
			return EXIT_FAILURE;
	}

	if(true == config_has_property(archivoConfiguracion,"IP_FS")){
			ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
		}else{
			log_error(logger, "Falta IP Filesystem");
			return EXIT_FAILURE;
	}

	if(true == config_has_property(archivoConfiguracion,"ARCHIVO_BIN")){
			archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
		}else{
			log_error(logger, "Falta Archivo Bin");
			return EXIT_FAILURE;
	}

	if(true == config_has_property(archivoConfiguracion,"DIR_TEMP")){
			dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
		}else{
			log_error(logger, "Falta Dir Temp");
			return EXIT_FAILURE;
	}

	if(true == config_has_property(archivoConfiguracion,"NODO_NUEVO")){
		    nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
		}else{
			log_error(logger, "Falta Nodo Nuevo");
			return EXIT_FAILURE;
	}

	if(true == config_has_property(archivoConfiguracion,"IP_NODO")){
		    ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
		}else{
			log_error(logger, "Falta IP Nodo");
			return EXIT_FAILURE;
	}

	if(true == config_has_property(archivoConfiguracion,"PUERTO_NODO")){
			puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");
		}else{
			log_error(logger, "Falta Puerto Nodo");
			return EXIT_FAILURE;
	}

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
