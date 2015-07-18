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
	printf("%i\n",job.combiner);

	send(socketJob, &enteroPrueba, sizeof(int),0);
	//ACA RECIBIMOS LA LISTA DE ARCHIVOS DE JOB COMO UN CHAR*
	recv(socketJob, &tamanio, sizeof(int),0);

	char *listaArchivosJob = malloc(tamanio);
	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, listaArchivosJob, tamanio,0);

	printf("lista de archivos job %s\n",listaArchivosJob);
	char **arrayArchivosJob = string_get_string_as_array(listaArchivosJob);
	printf("\n\n\n %s \n\n\n", arrayArchivosJob[0]);

	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, &tamanio, sizeof(int),0);
	send(socketJob, &enteroPrueba, sizeof(int),0);
	char* archivoResultadoFinal = malloc(tamanio);
	recv(socketJob, archivoResultadoFinal, tamanio,0);
	job.arch_resultado_final = archivoResultadoFinal;

	printf("archivo resultado final %s\n",job.arch_resultado_final);
	job.id_job = contador_cant_job;
	pthread_mutex_lock(&mutex_contador_job);
	contador_cant_job ++;
	pthread_mutex_unlock(&mutex_contador_job);


	//ACA SE CONECTA CON FS
	int handshakeFS;
	socketFS = crearCliente(ip_fs, puerto_fs);

	handshakeFS = 25;
	send(socketFS,&handshakeFS,sizeof(int),0);
	recv(socketFS,&handshakeFS, sizeof(int),0);

	inicializar_pedido_FS();
	int tamanio_total;
	recv(socketFS,&tamanio_total,sizeof(int),0);
	send(socketFS,&handshakeFS,sizeof(int),0);

	printf("tamamnio total: %i\n", tamanio_total);

	t_archivo *archivo_prueba = malloc(sizeof(t_archivo));
	archivo_prueba->bloques = list_create();
	printf("entro al recive\n\n");
	recive_y_guarda_estructura(archivo_prueba,socketFS,tamanio_total);
	printf("dsadasdas\n");

	//t_bloque *bloque = list_get(archivo_prueba.bloques,1);
	//printf("numero de bloque final: %i\n\n\n",bloque->NumeroBloque);
	//printf("idcopiafinal: %i, numbloquefinal: %i\n",bloque->copia1_idnodo,bloque->copia1_numbloque);

	printf("Tamaño de la lista: %i\n\n", list_size(archivo_prueba->bloques));

	t_bloque *bloque = list_get(archivo_prueba->bloques,1);
	printf("numero de bloque final posta: %i\n", bloque->NumeroBloque);

	bloque = list_get(archivo_prueba->bloques,2);
	printf("numero de bloque final posta: %i\n", bloque->NumeroBloque);
	printf("dsadas");

	//printf("gaston traga penes\n");

	return NULL;
}
