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

	struct Job_Marta pruebaRecibo;
	struct Marta_Job pruebaEnvio;
	pruebaEnvio.prueba3 = 56;
	pruebaEnvio.prueba4 = 75;
	int socketEscucha = crearServidor("2020");
	recv(socketEscucha,&pruebaRecibo,sizeof(struct Job_Marta),0);
	printf("%i%i",pruebaRecibo.prueba,pruebaRecibo.prueba2);
	send(socketEscucha,&pruebaEnvio,sizeof(struct Job_Marta),0);
	close(socketEscucha);
	return EXIT_SUCCESS;
}

