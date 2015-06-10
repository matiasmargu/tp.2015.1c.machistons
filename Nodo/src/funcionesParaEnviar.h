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

char *mensaje;


t_log* logger; // Log Global

int recive_y_deserialisa_SET_BLOQUE(estructuraSetBloque *bloque, int socket, uint32_t tamanioTotal);

#endif /* FUNCIONESPARAENVIAR_H_ */
