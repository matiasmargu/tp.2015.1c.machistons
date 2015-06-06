/*
 * funcionesParaEnviarEstructuras.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

char* serializarParaGetBloque(setBloque *bloque){

	char *serializedPackage = malloc(bloque->tamanioDatos);

	int offset = 0;
	int size_to_send;

	size_to_send =  sizeof(bloque->numero);
	memcpy(serializedPackage + offset, &(bloque->numero), size_to_send);
	offset += size_to_send;

	int tamanioNombre = strlen(bloque->bloque) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(bloque->bloque) + 1;
	memcpy(serializedPackage + offset, bloque->bloque, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}

void liberarMensaje(char **package){
	free(*package);
}

