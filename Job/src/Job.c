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
#include "funciones.h"

t_log* logger; // Log Global

int main(void) {
	imprimir();
	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";

	t_config* archivoConfiguracion;

	logger = log_create("LOG_JOB", "log_job" ,false, LOG_LEVEL_INFO);
    int status;
	int puerto_marta;
	char* ip_marta;
	char* mapper;
	char* reduce;
	char* combiner;
	char** lista_archivos;
	char* archivo_resultado;

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
	ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
	mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
	reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
	combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
	lista_archivos = config_get_array_value(archivoConfiguracion, "ARCHIVOS");
	archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");

	printf("%i\n",puerto_marta);

	int servidor = crearCliente (ip_marta, puerto_marta);
		int enviar = 1;
		char message[PACKAGESIZE];

		printf("Conectado al servidor. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n");

		while(true){
			fgets(message, PACKAGESIZE, stdin);			// Lee una linea en el stdin (lo que escribimos en la consola) hasta encontrar un \n (y lo incluye) o llegar a PACKAGESIZE.
			if (!strcmp(message,"exit\n")) enviar = 0;			// Chequeo que el usuario no quiera salir
			if (enviar) send(servidor, message, strlen(message) + 1, 0); 	// Solo envio si el usuario no quiere salir.
			status = recv(servidor, (void*) message , PACKAGESIZE, 0);
			if (status != 0 ) printf("%s", message);
		}

		close(servidor);


	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(rutaArchivoConfiguracion);
	free(lista_archivos);
	free(archivo_resultado);
	free(mapper);
	free(reduce);
	free(ip_marta);
	free(combiner);
	return EXIT_SUCCESS;

}
