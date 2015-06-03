/*
 * manejoDeListas.c
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <commons/string.h>
#include "librerias_y_estructuras.h"

t_archivo *archivo_create(char *nombre, int direccion, char* estado ) {
    t_archivo *new = malloc(sizeof(t_archivo));
    new->nombre = strdup(nombre);
    new->direccion = direccion;
    new->estado = strdup(estado);
    return new;
}

void archivo_destroy(t_archivo *self) {
	free(self->nombre);
	free(self->estado);
    free(self);
}
