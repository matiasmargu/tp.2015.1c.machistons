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
#include "config.h"
#include "log.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"
#include "list.h"
#include "bitarray.h"
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
pthread_mutex_t mutex_reduce;

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
	int cantidadArchivos;
	char *listaArchivos; // paquete serializado
	char *resultado;
	int id_job;
	int id_proceso;
}t_aplicarReduce;

typedef struct{
	char *IP_Origen;
	char *PUERTO_Origen;
	char *IP_Destino;
	char *PUERTO_Destino;
	char *nombreArchivo;
	int id_job;
	int id_proceso;
}t_moverArchivo;

char* mapearAMemoriaVirtual(char* archivo);
void *aplicarMapperF (t_aplicarMapper *estructura);
void *aplicarReduceF (t_aplicarReduce *estructura);
void *moverArchivoF (t_moverArchivo *estructura);
void respuestaAMarta(int tipo,int idProceso, int idJob, int estado);

#endif /* LIBRERIAS_Y_ESTRUCTURAS_H_ */
