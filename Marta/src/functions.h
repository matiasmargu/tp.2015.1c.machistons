
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <sys/socket.h>
#include <socket/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>

int recive_y_deserialisa(char *, int);


/*
 * 	Definicion de estructuras
 *
 * 	Es importante destacar que utilizamos el tipo uint_32, incluida en el header <stdint.h> para mantener un estandar en la cantidad
 * 	de bytes del paquete.
 */

/*
struct t_Package {
	char username[MAXUSERNAME];
	uint32_t username_long;
	char message[MAX_MESSAGE_SIZE];
	uint32_t message_long;
} ;

/*
 * 	Definicion de funciones
 */

//int recieve_and_deserialize(struct job_marta_inicio *,int);


#endif /* FUNCTIONS_H_ */
