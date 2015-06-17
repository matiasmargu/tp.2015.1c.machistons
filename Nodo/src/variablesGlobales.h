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
#include "atenderNodoYFS.h"

char *ip_nodo;
int puerto_nodo;

char *archivo_bin;
char *dir_temp;
char *nodo_nuevo;

char* pmap;
int fd;
struct stat mystat;

t_log* logger; // Log Global
t_config* archivoConfiguracion;

#endif /* VARIABLESGLOBALES_H_ */
