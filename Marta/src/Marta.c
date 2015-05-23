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
	 t_marta_job Marta_Job;
	 t_job_marta_inicio Job_Marta_Inicio;
	 t_job_marta_resultado Job_Marta_Resultado;
	int socketJob = crearServidor("3000");
	int socketFS = crearCliente("127.0.0.1","3001");



	if ((recv(socketJob, &Job_Marta_Inicio, sizeof(t_job_marta_inicio),0 )) != 0){
		printf("se conecto el Job con la operacion numero %i\n",Job_Marta_Inicio.operacionID);
	}

	Marta_Job.NumeroBloqueDeDatos = 152;

	send(socketJob, &Marta_Job, sizeof(t_marta_job),0);

	entero = 3;    // handshake con FS
	send(socketFS,&entero, sizeof(int),0);

	if ((recv(socketFS, &entero, sizeof(int),0 )) != 0){
		printf("fs me respondio esto: %i\n",entero);
	}

	close(socketJob);
	close(socketFS);
	return EXIT_SUCCESS;
}

