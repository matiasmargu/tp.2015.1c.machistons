/*
 * manejoDeListas.h
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#ifndef LIBRERIAS_Y_ESTRUCTURAS_H_
#define LIBRERIAS_Y_ESTRUCTURAS_H_

#include <pthread.h>
#include <socket/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>

//Libreria para Consola
#include "consola.h"

// Libreria para mongodb
#include <bson.h>
#include <mongoc.h>

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

//Varibables globales

t_log* logger; // Log Global
int entero; //Para el handshake
char *mensaje; // Para mandar mensajes serializados

// Estructuras de Interfaz con Nodo
	setBloque enviarBloqueAEscribir;

	int socketNodoGlobal;
//


// Variables MongoDB
mongoc_client_t *client;
mongoc_collection_t *collection;
mongoc_cursor_t *cursor;
const bson_t *doc;
bson_t *query;
char *str;

// funcionesParaEnviarEstructuras

char* serializarParaGetBloque(setBloque *bloque);
void liberarMensaje(char **package);

#endif /* LIBRERIAS_Y_ESTRUCTURAS_H_ */
