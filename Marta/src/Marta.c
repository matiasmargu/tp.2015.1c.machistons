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

	struct Job_Marta recibo_Job;
	struct Marta_Job envio_Job;
	struct Marta_FileSystem envio_FS;
	struct FileSystem_Marta recibo_FS;

	int socketJob = crearServidor("3000");
	int socketFS = crearCliente("127.0.0.1","3000");

	int status = recv(socketJob, &recibo_Job, sizeof(struct Job_Marta),0 );
	if (status != 0) printf("%i%i\n", recibo_Job.prueba, recibo_Job.prueba2);

	envio_FS.prueba3 = 56;
	envio_FS.prueba4 = "holis";

	send(socketFS, &envio_FS, sizeof(struct Marta_FileSystem),0 );

	int status2 = recv(socketFS, &recibo_FS, sizeof(struct Job_Marta),0 );
	if (status2 != 0) printf("%i%s\n", recibo_FS.prueba, recibo_FS.prueba2);

	close(socketFS);
	close(socketJob);
	return EXIT_SUCCESS;
}

