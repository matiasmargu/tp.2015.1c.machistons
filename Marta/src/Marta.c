/*
 ============================================================================
 Name        : Marta.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <../commons/config.h>
#include <../commons/log.h>
#include <../commons/string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>

t_log* logger; // Log Global

int main(void) {

	int entero;

	int socketJob = crearServidor("3000");
	int socketFS = crearCliente("127.0.0.1","3001");

	if ((recv(socketJob, &entero, sizeof(int),0 )) != 0){
		printf("se conecto el Job %i\n",entero);
	}

	entero = 98;
	send(socketJob, &entero, sizeof(int),0);

	entero = 3;    // handshake con FS
	send(socketFS,&entero, sizeof(int),0);

	if ((recv(socketFS, &entero, sizeof(int),0 )) != 0){
		printf("fs me respondio esto: %i\n",entero);
	}

	close(socketJob);
	close(socketFS);
	return EXIT_SUCCESS;
}

