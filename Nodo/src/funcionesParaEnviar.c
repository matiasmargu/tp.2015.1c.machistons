/*
 * funcionesParaEnviar.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#include "funcionesParaEnviar.h"

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
