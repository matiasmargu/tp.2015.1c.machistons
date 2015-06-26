/*
 * variablesGlobales.h
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#ifndef VARIABLESGLOBALES_H_
#define VARIABLESGLOBALES_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <./commons/collections/list.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>


FILE* fileMapper;
FILE* fileReducer;
FILE* registroDeLosBloques;

char* rutaArchivoConfiguracion;

char *ip_nodo;
int puerto_nodo;
char *archivo_bin;
char *dir_temp;
char *nodo_nuevo;

char* script_mapper;
char* script_reducer;

t_log* logger; // Log Global
t_config* archivoConfiguracion;

//Funciones de mapper y reducer
void *mapper(void* arg);
void *reducer(void* arg);

//Funciones de memoria
char* mapearAMemoriaVirtual(char* archivo_bin);
int tamanioEspecifico(char* pmap,int nroDelBloque);
void escribeEnArchivoSegunNroDeBloque(FILE* fd,int nroDelBloque,int tamanio);
void formateoElRegistro(FILE* fdf);

//Funciones de serializacion y deserializacion
int recive_y_deserialisa_CHARp(char *script, int socket, uint32_t tamanioTotal);
int recive_y_deserialisa_SET_BLOQUE(estructuraSetBloque *bloque, int socket, uint32_t tamanioTotal);
char* serializarIPyPUERTO(char* ip_fs,char* puerto_fs, int tamanioData);
char* serializarBloqueDeDatos(char* bloque, int tamanioData);

//Funciones de handshake
void handshakeConFS ();
void handshakeConJob(int socket_job);

//Funciones de atender
void *atenderNFS(void*arg);
void *atenderJob(void* arg);

//Funciones para escribir archivos
int escribirScript(char* script_virtual,char* dir_temp, int comando);


#endif /* VARIABLESGLOBALES_H_ */
