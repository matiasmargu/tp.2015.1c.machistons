/*
 * librerias_y_estructuras.h
 *
 *  Created on: 28/7/2015
 *      Author: utnso
 */

#ifndef LIBRERIAS_Y_ESTRUCTURAS_H_
#define LIBRERIAS_Y_ESTRUCTURAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <socket/socket.h>
#include <./commons/collections/list.h>
#include <./commons/bitarray.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

t_log* logger; // Log Global

char *puerto_marta;
char *ip_marta;
char* rutinaMapper;
char* rutinaReduce;
int contadorHilos;
pthread_mutex_t mutex;

typedef struct{
	char *IP;
	char *PUERTO;
	int bloque;
	char *resultado;
	int id_proceso;
	int id_job;
}t_aplicarMapper;

typedef struct{
	char *IP;
	char *PUERTO;
	char *listaArchivos;
	char *resultado;
}t_aplicarReduce;

typedef struct{
	char *IP_Origen;
	char *PUERTO_Origen;
	char *IP_Destino;
	char *PUERTO_Destino;
	char *nombreArchivo;
}t_moverArchivo;

char* mapearAMemoriaVirtual(char* archivo);
void *aplicarMapperF (t_aplicarMapper *estructura);
void *aplicarReduceF (t_aplicarReduce *estructura);
void *moverArchivoF (t_moverArchivo *estructura);

#endif /* LIBRERIAS_Y_ESTRUCTURAS_H_ */
