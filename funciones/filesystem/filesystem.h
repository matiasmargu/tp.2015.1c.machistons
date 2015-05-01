/*
 * filesystem.h
 *
 *  Created on: 29/4/2015
 *      Author: gcrivelli
 */

#ifndef FILESYSTEM_FILESYSTEM_H_
#define FILESYSTEM_FILESYSTEM_H_

#include <commons/collections/list.h>

#define MAXSIZE_COMANDO 50
#define MAXCOMANDOS 6
#define MAXBUFERTECLADO 1000

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
	char* nombre;
	u_int32_t tam;
	int direccion;
	char* estado;
	t_list bloques_copias;
} t_archivo;

void imprimirMenu(void);
t_archivo *archivo_create(char *nombre, int direccion, char* estado );
void archivo_destroy(t_archivo *self);
void *atenderConsola(void*arg);

#endif /* FILESYSTEM_FILESYSTEM_H_ */
