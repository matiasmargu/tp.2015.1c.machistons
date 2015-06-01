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
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>

t_log* logger; // Log Global



int main(void) {

	/*char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	char* puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");
	char* ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
*/
	int entero;

		struct job_marta_inicio Job_Marta_Inicio;
			    struct marta_job Marta_Job;
			    struct job_marta_resultado Job_Marta_Resultado;
			    struct fs_marta Fs_Marta;

	int socketJob = crearServidor("3000");

	int socketFS = crearCliente("10.0.2.15","3001");



	if ((recv(socketJob, &Job_Marta_Inicio, sizeof(struct job_marta_inicio),0 )) != 0){
		printf("se conecto el Job con la operacion numero %i\n",Job_Marta_Inicio.operacionID);

		send(socketFS,&Job_Marta_Inicio.lista_archivos, sizeof(char**),0);

		if ((recv(socketFS, &Fs_Marta, sizeof(struct fs_marta),0 )) != 0){



		}

	}

/*	Marta_Job.NumeroBloqueDeDatos = 12;
	Marta_Job.rutina = "mapper";
    Marta_Job.operacionID = 3;

	send(socketJob, &Marta_Job, sizeof(struct marta_job),0);

	Marta_Job.rutina = "reduc";
	Marta_Job.operacionID = 4;
	Marta_Job.NumeroBloqueDeDatos = 16;

	send(socketJob, &Marta_Job, sizeof(struct marta_job),0);

	entero = 3;    // handshake con FS
	send(socketFS,&entero, sizeof(int),0);

	if ((recv(socketFS, &entero, sizeof(int),0 )) != 0){
		printf("fs me respondio esto: %i\n",entero);
	}
*/
	close(socketJob);
	close(socketFS);
	return EXIT_SUCCESS;
}

