/*
 * atenderMarta.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

void *atenderMarta(void*arg){

	int socketMarta = (int)arg;
	printf("%i",socketMarta);

	return NULL;
}
