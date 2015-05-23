/*
 * manejoDeListas.h
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#ifndef MANEJODELISTAS_H_
#define MANEJODELISTAS_H_

#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>

typedef struct {
	int index; // Indice del directorio
	char* directorio; // Nombre de la direccion
	int padre; // En caso de ser 0, es el origen
} t_directorio;

typedef struct {
	int bloque;
	char* nodoCopia1;

	char* copia2;
	char* copia3;
} t_bloque_copias;

typedef struct {
	char nombre;
	int socket;
	unsigned long espacioTotal;
	unsigned long espacioOcupado;
	t_list bloque;
	char* ip;
	char* puerto;
	char estado;
}t_nodo;

typedef struct {
	char* nombre; //Nombre
	unsigned long tam; // Tamaño
	int direccion; // Direccion Padre
	char* estado; // Estado
	t_list bloques_copias; // Lista de bloques
} t_archivo;

t_archivo *archivo_create(char *nombre, int direccion, char* estado );
void archivo_destroy(t_archivo *self);

//Varibables globales

t_log* logger; // Log Global

#endif /* MANEJODELISTAS_H_ */
