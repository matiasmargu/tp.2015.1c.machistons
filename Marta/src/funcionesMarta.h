
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include <stdio.h>
#include <stdlib.h>
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
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

t_log* logger; // Log Global

typedef struct{
	char* archivo;
}t_charpuntero;

typedef struct{
	char* nombre;
	int cantidadDeBloques;
	t_list copia1;
	t_list copia2;
	t_list copia3;

}t_archivo;

typedef struct {
    int Numerobloque;
    int idNodo;

} t_copia;


int recive_y_deserialisa(t_charpuntero* nombre, int socket, uint32_t tamanioTotal);
char* serializar_charpuntero(t_charpuntero* nombre, int tamanioTotal);
void  *conectarseAlJob(void*arg);



#endif /* FUNCTIONS_H_ */
