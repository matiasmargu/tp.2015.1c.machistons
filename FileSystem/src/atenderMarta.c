/*
 * atenderMarta.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"


typedef struct{
	char* archivo;
}t_charpuntero;

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


void *atenderMarta(void*arg){

	int socketMarta = (int)arg;
	printf("%i\n",socketMarta);

	int cantidad;
	int a;
	recv(socketMarta, &cantidad, sizeof(int),0);

	char* listaDeArchivos[cantidad];
	int tamanioTotal;
	char* archivo;

	for(a = 0 ; a < cantidad; a++){
		recv(socketMarta, &tamanioTotal, sizeof(int),0);
		int estado = 1; // Estructura que manjea el status de los recieve.
		archivo= malloc(tamanioTotal);
		estado = recive_y_deserialisa(&archivo, socketMarta, tamanioTotal);
		if(estado){
		printf("el archivo es %s\n ",archivo);
		listaDeArchivos[a] = archivo;
		}
		free(archivo);
	}

	listaDeArchivos[cantidad+1] = NULL;

	return NULL;
}
