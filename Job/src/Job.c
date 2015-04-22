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

int main(void) {
	t_config* archivo_configuracion;

	archivo_configuracion = config_create("/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf");

	int puerto_marta = config_get_int_value(archivo_configuracion, "PUERTO_MARTA");
	char* ip_marta = config_get_string_value(archivo_configuracion, "IP_MARTA");


	config_destroy(archivo_configuracion);
	return EXIT_SUCCESS;
}
