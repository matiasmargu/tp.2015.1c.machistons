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
    char** lista_archivos;
	struct marta_job Marta_Job;
	struct job_marta_resultado Job_Marta_Resultado;
	int socketJob = crearServidor("3000");
	int socketFS = crearCliente("127.0.0.1","3001");



	if ((recv(socketJob, &lista_archivos, sizeof(lista_archivos),0 )) != 0){// recibo lista nombres de archivos del job
		send(socketFS,&lista_archivos,sizeof(lista_archivos),0);
/* aca hay que ver la forma en que el FS nos pasa la lista de bloques

		if ((recv(socketFS, &lista de bloques, sizeof(lista de bloques),0 )) != 0){

			send(socketJob, &lista de bloques, sizeof(lista de bloques),0 ); aca le mando los bloques al job perp
tambien le tengo que mandar que hacer con que rutina y todo eso, yo creo que habria que hacer una estructura que indique
la lista de bloques si es mapping o reduce y no se si algo mas...

		}
*/	}




	Marta_Job.NumeroBloqueDeDatos = 12;
	Marta_Job.rutina = "mapper";
    Marta_Job.operacionID = 3;

	send(socketJob, &Marta_Job, sizeof(struct marta_job),0);

	/*Marta_Job.rutina = "reduc";
	Marta_Job.operacionID = 4;
	Marta_Job.NumeroBloqueDeDatos = 16;

	send(socketJob, &Marta_Job, sizeof(struct marta_job),0);
*/
	entero = 3;    // handshake con FS
	send(socketFS,&entero, sizeof(int),0);

	if ((recv(socketFS, &entero, sizeof(int),0 )) != 0){
		printf("fs me respondio esto: %i\n",entero);
	}

	close(socketJob);
	close(socketFS);
	return EXIT_SUCCESS;
}

