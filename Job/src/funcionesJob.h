/*
 * funcionesJob2.h
 *
 *  Created on: 9/7/2015
 *      Author: utnso
 */

#ifndef JOB_SRC_FUNCIONESJOB2_H_
#define JOB_SRC_FUNCIONESJOB2_H_

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
pthread_mutex_t mutex;




typedef struct{
	int bloque;
	int socketNodo;
	char* nombreArchivoResultado;
    int socketMarta;
    int idNodo;
}t_hilo_map;

typedef struct{
	int socketNodo;
	t_list* archivos;
	char* nombreArchivoResultado;
	int socketMarta;
	int idNodo;
}t_hilo_reduce;

int recive_y_deserialisa_marta_job_mapper(t_marta_job_map *bloque, int socket, uint32_t tamanioTotal);
void* mapearBloque(t_hilo_map *structRecibido);
char* serializar_job_marta(t_job_marta_map *job_marta, int tamanioTotal);
void* reducirArchivos(t_hilo_reduce *structRecibido);
int recive_y_deserialisa_marta_job_reduce(t_marta_job_reduce *bloque, int socket, uint32_t tamanioTotal);
char* mapearAMemoriaVirtual(char* archivo);
char *serializar_job_marta_reduce(t_job_marta_reduce *job_marta,int tamanioTotal);


#endif /* JOB_SRC_FUNCIONESJOB2_H_ */
