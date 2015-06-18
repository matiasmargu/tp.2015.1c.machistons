/*
 * functions.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#include "functions.h"


int recive_y_deserialisa(t_charpuntero* nombre, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	nombre->archivo = malloc(tamanioDinamico);
	memcpy(nombre->archivo, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

char* serializar_charpuntero(t_charpuntero *nombre, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

			int tamanioNombre = strlen(nombre->archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(nombre->archivo) + 1;
			memcpy(serializedPackage + offset, nombre->archivo, size_to_send);
			offset += size_to_send;

			return serializedPackage;
		}


void  *conectarseAlJob(void*arg){

	int socket = (int)arg;

	int saludo = 9 ;
	int cantidad,tamanioTotal;
	char* archivoARecibir;

	send(socket,&saludo,sizeof(int),0);
   	recv(socket, &tamanioTotal, sizeof(int),0);
   	int estado2 = 1; // Estructura que manjea el status de los recieve.
  	archivoARecibir = malloc(tamanioTotal);
   	estado2 = recive_y_deserialisa(&archivoARecibir, socket, tamanioTotal);
   	if(estado2){
                printf("el string es %s\n",archivoARecibir);
   	}
   	return NULL;

}




