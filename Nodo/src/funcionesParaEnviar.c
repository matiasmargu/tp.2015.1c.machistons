/*
 * funcionesParaEnviar.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"

int recive_y_deserialisa_SET_BLOQUE(estructuraSetBloque *bloque, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	memcpy(&(bloque->bloque), buffer + offset, sizeof(bloque->bloque));
	offset += sizeof(bloque->bloque);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);



	bloque->data = malloc(tamanioDinamico);
	memcpy(bloque->data, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

int recive_y_deserialisa_CHARp(char *script, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	script = malloc(tamanioDinamico);
	memcpy(script, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}



char* serializarIPyPUERTO(char* ip_fs,char* puerto_fs, int tamanioData){
	int offset = 0;
	int size_to_send;

	char *serializedPackage = malloc(tamanioData);

	int tamanioNombre = strlen(ip_fs) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(ip_fs) + 1;
	memcpy(serializedPackage + offset, ip_fs, size_to_send);
	offset += size_to_send;


	tamanioNombre = strlen(puerto_fs) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(puerto_fs) + 1;
	memcpy(serializedPackage + offset, puerto_fs, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}
char* serializarBloqueDeDatos(char* bloque, int tamanioData){
	int offset = 0;
	int size_to_send;

	char *serializedPackage = malloc(tamanioData);

	int tamanioNombre = strlen(bloque) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(bloque) + 1;
	memcpy(serializedPackage + offset, bloque, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}


void handshakeConFS (){
	printf("corre el handshake\n");
	//Esta es la coneccion con el FS
	pthread_t hiloFS;
	char *ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
	char *puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");
	int socket_fs = crearCliente(ip_fs,puerto_fs);
	int entero2 = 2; // handshake con FS
	char* mensaje;

	send(socket_fs,&entero2,sizeof(int),0);
	int tamanioData = sizeof(int) + strlen(ip_nodo) + 1 + sizeof(int) + strlen(string_itoa(puerto_nodo)) + 1;
	send(socket_fs, &tamanioData, sizeof(int), 0);
	mensaje = serializarIPyPUERTO(ip_nodo, string_itoa(puerto_nodo), tamanioData);
	send(socket_fs,mensaje,tamanioData,0);
	printf("%i\n",socket_fs);
	pthread_create(&hiloFS, NULL, &atenderNFS, (void *)socket_fs);

	free(mensaje);
}

void handshakeConJob(int socket_job){
	pthread_t hiloJob;
	pthread_create(&hiloJob, NULL, &atenderJob, (void *)socket_job);
}
