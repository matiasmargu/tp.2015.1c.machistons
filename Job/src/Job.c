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
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>

char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";

int cliente_marta(int PUERTO,char* ip_marta){
	return 0;
}

int main(void) {

	t_config* archivoConfiguracion;
	t_log* logger;

	logger = log_create("LOG_JOB", "log_marta" ,false, LOG_LEVEL_INFO);

	int puerto_marta;
	char* ip_marta;
	char* mapper;
	char* reduce;
	char* combiner;
	char** lista_archivos;
	char* archivo_resultado;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////    Validacion y Carga del archivo de configuracion       ///////////////////////////////////////////

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	log_info(logger, "Se creo correctamente el archivo de configuracion");

	if(true == config_has_property(archivoConfiguracion,"PUERTO_MARTA")){
			puerto_marta = config_get_int_value(archivoConfiguracion, "PUERTO_MARTA");
		}else{
			log_error(logger, "Falta Puerto Marta");
			return EXIT_SUCCESS;
	}

	if(true == config_has_property(archivoConfiguracion,"IP_MARTA")){
			ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
		}else{
			log_error(logger, "Falta IP Marta");
			return EXIT_SUCCESS;
	}

	if(true == config_has_property(archivoConfiguracion,"MAPPER")){
			mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
		}else{
			log_error(logger, "Falta Programa Mapper");
			return EXIT_SUCCESS;
	}

	if(true == config_has_property(archivoConfiguracion,"REDUCE")){
			reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
		}else{
			log_error(logger, "Falta Programa Reduce");
			return EXIT_SUCCESS;
	}

	if(true == config_has_property(archivoConfiguracion,"COMBINER")){
		    combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
		}else{
			log_error(logger, "Falta Combiner");
			return EXIT_SUCCESS;
	}

	if(true == config_has_property(archivoConfiguracion,"ARCHIVOS")){
		    lista_archivos = config_get_string_value(archivoConfiguracion, "ARCHIVOS");
		}else{
			log_error(logger, "Faltan lista de Archivos");
			return EXIT_SUCCESS;
	}

	if(true == config_has_property(archivoConfiguracion,"RESULTADO")){
			archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");
		}else{
			log_error(logger, "Faltan archivo de Resultado");
			return EXIT_SUCCESS;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("%i\n%s\n%s\n%s\n%s\n%s\n",puerto_marta,ip_marta,mapper,reduce,combiner,archivo_resultado);

	int socketMarta = cliente_marta(puerto_marta,ip_marta);
	printf("%i",socketMarta);

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(rutaArchivoConfiguracion);
	free(lista_archivos);
	return EXIT_SUCCESS;
}
