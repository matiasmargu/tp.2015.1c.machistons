/*
 * pedidos_job.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */


#include "librerias_y_estructuras.h"

void *atenderJob(void *arg){
	int socketJob = (int)arg;

	// recibir lista de archivos, combiner y resultado del job. Lo guardas en una lista.
	t_infoJob job;
	int enteroPrueba = 0;
	int tamanio;
	lista_jobs = list_create();
	send(socketJob, &enteroPrueba, sizeof(int),0);
	//ACA RECIBIMOS LA INFORMACION SOBRE EL COMBINER(SI TIENE O NO)
	recv(socketJob, &tamanio, sizeof(int),0);
	char *combiner = malloc(tamanio);

	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, combiner, tamanio,0);
	printf("el combiner es %s\n",combiner);

	if(string_equals_ignore_case(combiner ,"SI")) job.combiner = 1;
	else job.combiner = 0;

	send(socketJob, &enteroPrueba, sizeof(int),0);
	//ACA RECIBIMOS LA LISTA DE ARCHIVOS DE JOB COMO UN CHAR*
	recv(socketJob, &tamanio, sizeof(int),0);

	char *listaArchivosJob = malloc(tamanio);
	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, listaArchivosJob, tamanio,0);

	printf("lista de archivos job %s\n",listaArchivosJob);
	char **arrayArchivosJob = string_get_string_as_array(listaArchivosJob);

	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, &tamanio, sizeof(int),0);
	send(socketJob, &enteroPrueba, sizeof(int),0);
	char* archivoResultadoFinal = malloc(tamanio);
	recv(socketJob, archivoResultadoFinal, tamanio,0);
	printf("archivo resultado final %s\n",archivoResultadoFinal);
	// Le pido al FS

	return NULL;
}
