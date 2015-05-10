/*
 * manejoDeListas.h
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#ifndef MANEJODELISTAS_H_
#define MANEJODELISTAS_H_

#include "../commons/collections/list.h"
#include <../commons/config.h>
#include <../commons/log.h>
#include <../commons/string.h>

typedef struct {
	int socket; // Socket del Nodo para comunicacion
	char* nodo; // Nombre del Nodo
	char* estado; // Estado disponible o no disponible del nodo
	t_list bloque; // Lista de bloques libres
} t_socket_bloqueslibres;

typedef struct {
	int index; // Indice del directorio
	char* directorio; // Nombre de la direccion
	int padre; // En caso de ser 0, es el origen
} t_directorio;

typedef struct {
	int bloque;
	char* copia1;
	char* copia2;
	char* copia3;
} t_bloque_copias;

typedef struct {
	char* nombre; //Nombre
	u_int32_t tam; // Tamaño
	int direccion; // Direccion Padre
	char* estado; // Estado
	t_list bloques_copias; // Lista de bloques
} t_archivo;

t_archivo *archivo_create(char *nombre, int direccion, char* estado );
void archivo_destroy(t_archivo *self);

t_log* logger; // Log Global

#endif /* MANEJODELISTAS_H_ */
