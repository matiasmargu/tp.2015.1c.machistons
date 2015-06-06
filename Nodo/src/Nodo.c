/*
 ============================================================================
 Name        : Nodo.c
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
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <./commons/collections/list.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

t_log* logger; // Log Global

int recive_y_deserialisa(setBloque *bloque, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	memcpy(&(bloque->numero), buffer + offset, sizeof(bloque->numero));
	offset += sizeof(bloque->numero);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	bloque->bloque = malloc(tamanioDinamico);
	memcpy(bloque->bloque, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}


void *atenderNFS(void* arg){

	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;
	int tamanioTotal;
	setBloque set;

	while(1){
		if(recv(socket, &entero, sizeof(int),0) > 0){
		switch(entero){
		//getBloque(numero);
			case 1:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
				recv(socket,&tamanioTotal,sizeof(int),0);
				int status = 1; // Estructura que manjea el status de los recieve.
				status = recive_y_deserialisa(&set, socket, tamanioTotal);
				if (status) {
					// ACA TRABAJAN CON set.numero y set.bloque. Escriben el archivo y toda la bola.
					printf("%i\n",set.numero);
					printf("%s\n",set.bloque);
				}
				//char *bloqueATrabajar = mmap((void *)archivo_bin, size_t length, PROTO_NONE, int flags, int fd, off_t offset);
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//getFileContent(nombre);
			case 3:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		}
	}
	}
	return NULL;
}

int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";
	t_config* archivoConfiguracion;
	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);

	int entero; //Para el handshake con el Job

	pthread_t hiloFS;
	pthread_t hiloJob;
	pthread_t hiloNodo;

///////    Carga del archivo de configuracion       ///////////////////////////////////////////

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);

	log_info(logger, "Se creo correctamente el archivo de configuracion");

	FILE *archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
	char *dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
	char *nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
	char *ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
	int puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");

//Esta es la coneccion con el FS

	char *ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
	char *puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");
	int socket_fs = crearCliente(ip_fs,puerto_fs);
	entero = 2; // handshake con FS
	send(socket_fs,&entero,sizeof(int),0);
	pthread_create(&hiloFS, NULL, atenderNFS, (void *)socket_fs);

//
	while(1){

	}

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(ip_nodo);
	free(archivo_bin);
	free(dir_temp);
	free(nodo_nuevo);

	return EXIT_SUCCESS;
}
