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
	printf("TAMAÑO TOTAL: %i\n",tamanio_total);
	char *buffer = malloc(tamanio_total);
	//ip
	printf("ip: %s\n", estructura->ip_nodo);
	tamanio = strlen(estructura->ip_nodo) + 1;
	memcpy(buffer+offset,&tamanio,sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset,estructura->ip_nodo,tamanio);
	offset += tamanio;
	//puerto
	printf("puerto: %s\n", estructura->puerto);
	tamanio = strlen(estructura->puerto) + 1;
	memcpy(buffer+offset,&tamanio,sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset,estructura->puerto,tamanio);
	offset += tamanio;
	//archivo resultado
	printf("resultado final: %s\n", estructura->nombre_archivo_resultado);
	tamanio = strlen(estructura->nombre_archivo_resultado) + 1;
	memcpy(buffer+offset,&tamanio,sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset,estructura->nombre_archivo_resultado,tamanio);
	offset += tamanio;
	//id map
	printf("id: %i\n", estructura->id_map);
	memcpy(buffer+offset,&(estructura->id_map),sizeof(int));
	offset += sizeof(int);
	//bloque
	printf("bloque: %i", bloque);
	memcpy(buffer+offset,&(bloque),sizeof(int));
	offset += sizeof(int);

	printf("buffer: %s\n",buffer);

	send(socketJob,buffer,tamanio_total,0);
}

void atenderJob(void *arg){
	int socketJob = (int)arg;

	// recibir lista de archivos, combiner y resultado del job. Lo guardas en una lista.
	t_infoJob *job = malloc(sizeof(t_infoJob));
	int enteroPrueba = 0;
	int offset = 0;
	int tamanio, cantidad_archivos,i;
	lista_jobs = list_create();


	//EMPIEZA LA INTERFAZ CON FS
	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, &tamanio, sizeof(int), 0);
	send(socketJob, &enteroPrueba, sizeof(int), 0);
	char *buffer = malloc(tamanio);
	recv(socketJob, buffer, tamanio, 0);

	//ACA RECIBIMOS LA INFORMACION SOBRE EL COMBINER(SI TIENE O NO)
	memcpy(&(job->combiner), buffer + offset, sizeof(int));
	offset += sizeof(int);

	//RECIBO EL NOMBRE DEL RESULTADO FINAL
	memcpy(&tamanio,buffer+offset,sizeof(int));
	offset += sizeof(int);
	job->arch_resultado_final = malloc(sizeof(tamanio));
	memcpy(job->arch_resultado_final, buffer+offset, tamanio);
	offset += tamanio;

	printf("\n\n-------------------NODOS--------------------- \n");
	int handshakeFS;
	job->archivos_job = list_create();
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

	//ACA RECIBIMOS LA LISTA DE ARCHIVOS DE JOB COMO UN CHAR*
	memcpy(&cantidad_archivos, buffer+offset, sizeof(int));
	offset += sizeof(int);
	printf("cantidad de archivos del job: %i\n",cantidad_archivos);
	for(i=0;i<cantidad_archivos;i++){

		memcpy(&tamanio,buffer+offset,sizeof(int));
		offset += sizeof(int);
		char *nombreArch = malloc(tamanio);
		memcpy(nombreArch, buffer+offset, tamanio);
		offset += tamanio;
		printf("ARCHIVO DEL JOB %s\n", nombreArch);
	//	*(nombreArch + tamanio) = '\0';

		job->id_job = contador_cant_job;

		//----------------------------------------CREO LA TABLA DE PROCESOS PARA EL MAP-------------------------------
		t_job_procesos *job_con_tabla = malloc(sizeof(t_job_procesos));
		job_con_tabla->idJob = job->id_job;
		job_con_tabla->tabla_procesos = list_create();
		pthread_mutex_lock(&mutex_lista_procesos);
		list_add_in_index(lista_tabla_procesos,job->id_job,job_con_tabla);
		pthread_mutex_unlock(&mutex_lista_procesos);
		//------------------------------------------------------------------------------------------------------------


		//--------------------------------ACA SE CONECTA CON FS-------------------------------------------------------
		inicializar_pedido_FS(nombreArch);

		recv(socketFS,&tamanio_total,sizeof(int),0);
		send(socketFS,&handshakeFS,sizeof(int),0);

		printf("tamamnio total: %i\n", tamanio_total);
		t_archivo *archivo_prueba = malloc(sizeof(t_archivo));
		archivo_prueba->bloques = list_create();
		archivo_prueba->nombre = nombreArch;
		recive_y_guarda_estructura(archivo_prueba,socketFS,tamanio_total);
		archivo_prueba->cantidadDeBloques = list_size(archivo_prueba->bloques);
		list_add(job->archivos_job,archivo_prueba);

		printf("Tamaño de la lista: %i\n\n", list_size(archivo_prueba->bloques));

	}

	//cargo el job con sus archivos a la lista global
	pthread_mutex_lock(&mutex_lista_jobs);
	printf("ID DEL JOB ANTES DEL MAP: %i\n",job->id_job);
		list_add_in_index(lista_jobs,job->id_job,job);
	pthread_mutex_unlock(&mutex_lista_jobs);
	//-----------------------------------------AUMENTO LOS IDs DEL JOB--------------------------------------------
	pthread_mutex_lock(&mutex_contador_job);
		contador_cant_job ++;
	pthread_mutex_unlock(&mutex_contador_job);
	//------------------------------------------------------------------------------------------------------------

	printf("TAMAÑO DE LA LISTA ANTES DE MAP: %i\n",list_size(lista_jobs));

	planificarMap(job->id_job,socketJob);

	int bloque;
	t_marta_job_map *enviar_nodo = malloc(sizeof(t_marta_job_map));
	t_nodo *nodo = malloc(sizeof(t_nodo));

	t_job_procesos *tabla_procesos_job = malloc(sizeof(t_job_procesos));
	tabla_procesos_job = list_get(lista_tabla_procesos,job->id_job);
	t_tablaProcesos_porJob *tablaProcesos = malloc(sizeof(t_tablaProcesos_porJob));
	printf("TAMAÑO DE LA LISTA EN EL CONTROL: %i\n",list_size(tabla_procesos_job->tabla_procesos));
	char *ip = "192.168.1.45";
	for(i=0;i<list_size(tabla_procesos_job->tabla_procesos);i++){
		tablaProcesos = list_get(tabla_procesos_job->tabla_procesos,i);
		//printf("ID DEL NODO: %i", tablaProcesos->id_nodo);
		nodo = list_get(lista_nodos_estado,(tablaProcesos->id_nodo) - 1);
		//cargo los datos para enviar
		enviar_nodo->ip_nodo = nodo->ip_nodo;
		enviar_nodo->puerto = nodo->puerto_nodo;
		enviar_nodo->id_map = tablaProcesos->id_map;
		enviar_nodo->nombre_archivo_resultado = tablaProcesos->nombre_archivo_resultado;
		bloque = tablaProcesos->bloque_archivo;
		//inicializo la conexion
		int handshakeJob = 30;
		send(socketJob,&handshakeJob,sizeof(int),0);
		if(recv(socketJob,&handshakeJob,sizeof(int),0)<0) return;
		printf("%i\n",i);
		tamanio_total = strlen(enviar_nodo->ip_nodo) + strlen(enviar_nodo->puerto) + strlen(enviar_nodo->nombre_archivo_resultado) + (sizeof(int)*5);
		send(socketJob,&tamanio_total,sizeof(int),0);
		if(recv(socketJob,&handshakeJob,sizeof(int),0)<0) return;
		//serializar_map_job(socketJob, tamanio_total, enviar_nodo, bloque);

			offset = 0;
			printf("TAMAÑO TOTAL: %i\n",tamanio_total);
			free(buffer);
			buffer = malloc(tamanio_total);
			//ip
			printf("ip: %s\n", enviar_nodo->ip_nodo);
			tamanio = strlen(ip);
			printf("TAMAÑO IP: %i\n",tamanio);
			memcpy(buffer+offset,&tamanio,sizeof(int));
			offset += sizeof(int);
			memcpy(buffer+offset,ip,tamanio);
			offset += tamanio;
			//puerto
			printf("puerto: %s\n", enviar_nodo->puerto);
			tamanio = strlen(enviar_nodo->puerto);
			memcpy(buffer+offset,&tamanio,sizeof(int));
			offset += sizeof(int);
			memcpy(buffer+offset,enviar_nodo->puerto,tamanio);
			offset += tamanio;
			//archivo resultado
			printf("resultado final: %s\n", enviar_nodo->nombre_archivo_resultado);
			tamanio = strlen(enviar_nodo->nombre_archivo_resultado);
			memcpy(buffer+offset,&tamanio,sizeof(int));
			offset += sizeof(int);
			memcpy(buffer+offset,enviar_nodo->nombre_archivo_resultado,tamanio);
			offset += tamanio;
			//id map
			printf("id: %i\n", enviar_nodo->id_map);
			memcpy(buffer+offset,&(enviar_nodo->id_map),sizeof(int));
			offset += sizeof(int);
			//bloque
			printf("bloque: %i", bloque);
			memcpy(buffer+offset,&(bloque),sizeof(int));
			offset += sizeof(int);

			printf("buffer: %s\n",buffer);

			send(socketJob,buffer,tamanio_total,0);
	}


}
