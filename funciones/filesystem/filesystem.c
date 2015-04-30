/*
 * filesystem.c
 *
 *  Created on: 29/4/2015
 *      Author: gcrivelli
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "filesystem.h"

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

void imprimirMenu(void){

	printf("> Los comandos se ingresan con su numero \n"
			"> 		COMANDOS \n"
			"	Formatear el MDFS: 0 \n"
			"	Operaciones sobre Archivos  \n"
			"	  Eliminar: 1 \n"
			"	  Renonmbrar: 2 \n"
			"	  Mover: 3 \n"
			"	Operaciones sobre Directorios  \n"
			"	  Crear: 4 \n"
			"	  Eliminar: 5 \n"
			"	  Renombrar: 6 \n"
			"	  Mover: 7 \n"
			"	Operaciones sobre bloques de un archivo \n"
			" 	  Ver: 8 \n"
			" 	  Borrar: 9 \n"
			" 	  Copiar: 10 \n"
			" 	Operaciones sobre nodos de datos:\n"
			"	  Agregar: 11 \n"
			"	  Eliminar: 12 \n"
			"	Copiar un archivo local al MDFS: 13 \n"
			"	Copiar un archivo del MDFS al filesystem local: 14 \n"
			"	Solicitar el MD5 de un archivo en MDFS: 15 \n"
			"	SALIR 16 \n");
	return ;
}
