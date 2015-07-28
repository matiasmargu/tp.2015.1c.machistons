/*
 * pedidos_job.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */


#include "librerias_y_estructuras.h"

void serializar_map_job(int socketJob, int tamanio_total, t_marta_job_map *estructura, int bloque){
	int tamanio;
	int offset = 0;
	char *buffer = malloc(tamanio_total);
	//ip
	tamanio = strlen(estructura->ip_nodo) + 1;
	memcpy(buffer+offset,&tamanio,sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset,estructura->ip_nodo,tamanio);
	offset += tamanio;
	//puerto
	tamanio = strlen(estructura->puerto) + 1;
	memcpy(buffer+offset,&tamanio,sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset,estructura->puerto,tamanio);
	offset += tamanio;
	//archivo resultado
	tamanio = strlen(estructura->nombre_archivo_resultado) + 1;
	memcpy(buffer+offset,&tamanio,sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset,estructura->nombre_archivo_resultado,tamanio);
	offset += tamanio;
	//id nodo
	memcpy(buffer+offset,&(estructura->idNodo),sizeof(int));
	offset += sizeof(int);
	//bloque
	memcpy(buffer+offset,&(bloque),sizeof(int));
	offset += sizeof(int);

	send(socketJob,buffer,tamanio_total,0);
}

void atenderJob(void *arg){
	int socketJob = (int)arg;

	// recibir lista de archivos, combiner y resultado del job. Lo guardas en una lista.
	t_infoJob *job = malloc(sizeof(t_infoJob));
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
	if(string_equals_ignore_case(combiner ,"SI")) job->combiner = 1;
	else job->combiner = 0;
	printf("%i\n",job->combiner);

	send(socketJob, &enteroPrueba, sizeof(int),0);
	//ACA RECIBIMOS LA LISTA DE ARCHIVOS DE JOB COMO UN CHAR*
	recv(socketJob, &tamanio, sizeof(int),0);

	char *listaArchivosJob = malloc(tamanio);
	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, listaArchivosJob, tamanio,0);

	printf("lista de archivos job %s\n",listaArchivosJob);
	char **arrayArchivosJob = string_get_string_as_array(listaArchivosJob);
	printf("\n\n\n %s \n\n\n", arrayArchivosJob[0]);

	//---------------------------------CANTIDAD DE ARCHIVOS----------------------------------------

	//----

	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, &tamanio, sizeof(int),0);
	send(socketJob, &enteroPrueba, sizeof(int),0);
	char* archivoResultadoFinal = malloc(tamanio);
	recv(socketJob, archivoResultadoFinal, tamanio,0);
	job->arch_resultado_final = archivoResultadoFinal;

	printf("archivo resultado final %s\n",job->arch_resultado_final);
	job->id_job = contador_cant_job;

	//----------------------------------------CREO LA LISTA DE ARCHIVOS DEL JOB-----------------------------------
	job->archivos_job = list_create();

	//----------------------------------------CREO LA TABLA DE PROCESOS PARA EL MAP-------------------------------
	t_job_procesos *job_con_tabla = malloc(sizeof(t_job_procesos));
	job_con_tabla->idJob = job->id_job;
	job_con_tabla->tabla_procesos = list_create();
	pthread_mutex_lock(&mutex_lista_procesos);
		list_add_in_index(lista_tabla_procesos,job->id_job,job_con_tabla);
	pthread_mutex_unlock(&mutex_lista_procesos);
	//------------------------------------------------------------------------------------------------------------
	//-----------------------------------------AUMENTO LOS IDs DEL JOB--------------------------------------------
	pthread_mutex_lock(&mutex_contador_job);
		contador_cant_job ++;
	pthread_mutex_unlock(&mutex_contador_job);
	//------------------------------------------------------------------------------------------------------------

	//--------------------------------ACA SE CONECTA CON FS-------------------------------------------------------
	//pido lista de nodos
	printf("\n\n-------------------NODOS--------------------- \n");
	int handshakeFS,k;
	int cant_archivos = 1;
	job->archivos_job = list_create();
	char *nombreArch = "201304hourly.txt";

	socketFS = crearCliente(ip_fs, puerto_fs);
	handshakeFS = 25;
	send(socketFS,&handshakeFS,sizeof(int),0);
	recv(socketFS,&handshakeFS, sizeof(int),0);

	handshakeFS = 68;
	send(socketFS,&handshakeFS,sizeof(int),0);
	int tamanio_total = 0;
	lista_nodos_estado = list_create();

	recv(socketFS,&tamanio_total,sizeof(int),0);
	send(socketFS,&handshakeFS,sizeof(int),0);
	recive_y_guarda_infoNodo(tamanio_total, socketFS, lista_nodos_estado);

	//pido los archivos
	for(k=0;k<cant_archivos;k++){
		//char *nombre_archivo = arrayArchivosJob[0];
		//printf("archivo Job: %s\n", arrayArchivosJob[0]);
		inicializar_pedido_FS(nombreArch);
		/*
		char **array_aux = string_split(arrayArchivosJob[0],"\n");
		tamanio_total = strlen(array_aux[0]);
		printf("archivo Job: %s\n", array_aux[0]);
		printf("tamanio archivo job: %i\n",tamanio_total);
		handshakeFS = 72; // pido info de archivos
		send(socketFS,&handshakeFS, sizeof(int), 0);
		recv(socketFS,&handshakeFS, sizeof(int),0);

		send(socketFS,&tamanio_total, sizeof(int), 0);
		recv(socketFS,&handshakeFS, sizeof(int),0);

		send(socketFS,array_aux[0], tamanio_total, 0);
		 */

		recv(socketFS,&tamanio_total,sizeof(int),0);
		send(socketFS,&handshakeFS,sizeof(int),0);

		printf("tamamnio total: %i\n", tamanio_total);
		t_archivo *archivo_prueba = malloc(sizeof(t_archivo));
		archivo_prueba->bloques = list_create();
		archivo_prueba->nombre = nombreArch;
		recive_y_guarda_estructura(archivo_prueba,socketFS,tamanio_total);
		archivo_prueba->cantidadDeBloques = list_size(archivo_prueba->bloques);
		list_add(job->archivos_job,archivo_prueba);
		//cargo el job con sus archivos a la lista global
		pthread_mutex_lock(&mutex_lista_jobs);
			list_add_in_index(lista_jobs,job->id_job,job);
		pthread_mutex_unlock(&mutex_lista_jobs);

		//t_bloque *bloque = list_get(archivo_prueba.bloques,1);
		//printf("numero de bloque final: %i\n\n\n",bloque->NumeroBloque);
		//printf("idcopiafinal: %i, numbloquefinal: %i\n",bloque->copia1_idnodo,bloque->copia1_numbloque);

		printf("Tamaño de la lista: %i\n\n", list_size(archivo_prueba->bloques));

		//t_bloque *bloque = list_get(archivo_prueba->bloques,0);
		//printf("numero de bloque final posta: %i\n\n", bloque->NumeroBloque);
	}

	planificarMap(job->id_job,socketJob);

	int i;
	int bloque;
	t_marta_job_map *enviar_nodo = malloc(sizeof(t_marta_job_map));
	t_nodo *nodo = malloc(sizeof(t_nodo));

	t_job_procesos *tabla_procesos_job = malloc(sizeof(t_job_procesos));
	tabla_procesos_job = list_get(lista_tabla_procesos,job->id_job);
	t_tablaProcesos_porJob *tablaProcesos = malloc(sizeof(t_tablaProcesos_porJob));
	char *nombre_resultado_final = malloc(sizeof(*nombreArch));

	for(i=0;i<list_size(tabla_procesos_job->tabla_procesos);i++){
		tablaProcesos = list_get(tabla_procesos_job->tabla_procesos,i);
		nodo = list_get(lista_nodos_estado,tablaProcesos->id_nodo);
		//cargo los datos para enviar
		enviar_nodo->ip_nodo = nodo->ip_nodo;
		enviar_nodo->puerto = nodo->puerto_nodo;
		enviar_nodo->idNodo = tablaProcesos->id_nodo;
		string_append(&nombre_resultado_final,string_itoa(i));
		enviar_nodo->nombre_archivo_resultado = nombre_resultado_final;
		bloque = tablaProcesos->bloque_archivo;
		//inicializo la conexion
		int handshakeJob = 30;
		send(socketJob,&handshakeJob,sizeof(int),0);
		recv(socketJob,&handshakeJob,sizeof(int),0);

		send(socketJob,&tamanio_total,sizeof(int),0);
		recv(socketJob,&handshakeJob,sizeof(int),0);

		serializar_map_job(socketJob, tamanio_total, enviar_nodo, bloque);
	}



	//return NULL;
}
