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

	int socketEscucha = crearServidor("3000");

	int status = recv(socketEscucha, &pruebaRecibo, sizeof(struct Job_Marta),0 );
	if (status != 0) printf("%i%i\n", pruebaRecibo.prueba, pruebaRecibo.prueba2);

	pruebaEnvio.prueba3 = 56;
	pruebaEnvio.prueba4 = 75;

	send(socketEscucha, &pruebaEnvio, sizeof(struct Job_Marta),0 );

	int status2 = recv(socketEscucha, &pruebaRecibo, sizeof(struct Job_Marta),0 );
	if (status2 != 0) printf("%i%i\n", pruebaRecibo.prueba, pruebaRecibo.prueba2);

	close(socketEscucha);
	return EXIT_SUCCESS;
}

