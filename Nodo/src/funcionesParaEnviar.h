/*
 * funcionesParaEnviar.h
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESPARAENVIAR_H_
#define FUNCIONESPARAENVIAR_H_

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
#include "atenderNodoYFS.h"

char *mensaje;

char *ip_nodo;
int puerto_nodo;

char *archivo_bin;
char *dir_temp;
char *nodo_nuevo;

char* pmap;

t_log* logger; // Log Global
t_config* archivoConfiguracion;

int recive_y_deserialisa_SET_BLOQUE(estructuraSetBloque *bloque, int socket, uint32_t tamanioTotal);
char* serializarIPyPUERTO(char* ip_fs,char* puerto_fs, int tamanioData);
char* serializarBloqueDeDatos(char* bloque, int tamanioData);
void handshakeConFS ();

#endif /* FUNCIONESPARAENVIAR_H_ */
