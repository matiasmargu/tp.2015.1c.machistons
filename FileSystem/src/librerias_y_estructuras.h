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
	char* name; //Nombre
	int size; // Tamaño
	int parent_directory; // Direccion Padre
	char* path; // Direccion Fisica
	int status; // Estado, 1 Disponible, 0 No Disponible
	t_list *blocks; // Lista de bloques
} t_archivo;

typedef struct {
	int id;
	t_list *copies;
} t_archivo_bloque;

typedef struct {
	int copy;
	char* content;
	int node;
	int node_block;
}t_archivo_copias;

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

//Varibables globales

t_log* logger; // Log Global
int entero; //Para el handshake
char *mensaje; // Para mandar mensajes serializados
int nodosNecesarios;
int apagarFS; // 0 Prendida, 1 Apagar

// Estructuras de Interfaz con Nodo
	estructuraSetBloque escribirBloque;
	int socketNodoGlobal;
	int tamanioTotalMensaje;
	estructuraIPyNodo ipyPuertoNodo;
//

// Variables MongoDB
mongoc_client_t *client;

mongoc_collection_t *directorios;
mongoc_collection_t *archivos;
mongoc_collection_t *nodos;

mongoc_cursor_t *cursor;

bson_t *doc;
bson_t *doc2;
bson_t *doc3;
bson_t *update;
bson_t *query;
bson_error_t error;
bson_oid_t oid;

char *str;

// funcionesParaEnviarEstructuras

char* serializarParaGetBloque(estructuraSetBloque *bloque);
void liberarMensaje(char **package);
int recive_y_deserialisa_IPyPUERTO_Nodo(estructuraIPyNodo *bloque, int socket, uint32_t tamanioTotal);
void *atenderMarta(void*arg);

// funciones para Agregar Datos a Mongo

void agregoNodoaMongo (int socket);
void insertarArchivoAMongo (t_archivo archivo);

#endif /* LIBRERIAS_Y_ESTRUCTURAS_H_ */
