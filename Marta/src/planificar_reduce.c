/*
 * planificar_reduce.c
 *
 *  Created on: 31/7/2015
 *      Author: utnso
 */


#include "librerias_y_estructuras.h"
//handshake = 34
//envio tamaño total
//[tam_ip][ip][tam_puerto][puerto][tam_res][res][id_proceso][id_job][cant_archivos_map]//ciclo[tam_arch_map][arch_map][tam_arch_map][arch_map]


//handshake = 33
//envio tamaño total
//[tam_ip_origen][ip_origen][tam_puerto_origen][puerto_origen][tam_ip_dest][ip_dest][tam_puerto_dest][puerto_dest][id_proceso_reduce][id_job][tam_nombre_arch][nombre_arch]

void serializar_mover_reduce(int tamanio_total, int socketjob, t_nodo *nodo_dest, t_nodo *nodo_origen, t_reduce *reduce_nodo){
	char *buffer = malloc(tamanio_total);
	int offset = 0;
	int tam;

	printf("Origen IP: %s, Origen puerto: %s, Dest IP: %s, Dest puerto: %s, ID Reduce: %i, Nombre Reduce: %s",nodo_origen->ip_nodo, nodo_origen->puerto_nodo, nodo_dest->ip_nodo, nodo_dest->puerto_nodo, reduce_nodo->id_reduce, reduce_nodo->nombre_resultado_reduce);

	tam = strlen(nodo_origen->ip_nodo);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, nodo_origen->ip_nodo, tam);
	offset += tam;

	tam = strlen(nodo_origen->puerto_nodo);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, nodo_origen->puerto_nodo, tam);
	offset += tam;

	tam = strlen(nodo_dest->ip_nodo);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, nodo_dest->ip_nodo, tam);
	offset += tam;

	tam = strlen(nodo_dest->puerto_nodo);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, nodo_dest->puerto_nodo, tam);
	offset += tam;

	tam = reduce_nodo->id_reduce;
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);

	tam = reduce_nodo->id_job;
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);

	printf("Nombre archivo: %s\n",reduce_nodo->nombre_resultado_reduce);
	tam = strlen(reduce_nodo->nombre_resultado_reduce);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, reduce_nodo->nombre_resultado_reduce, tam);
	offset += tam;

	send(socketjob, buffer, tamanio_total, 0);
	sleep(2);
	free(buffer);
}

int esperar_que_terminen_reduce(int id_job){
	int i,j;
	bool termino = false;
	bool flag = false;
	t_reduce *procesos_reduce = malloc(sizeof(t_reduce));

	while(termino == false){
		sleep(5);
		j=0;
		while((flag != true)  && (j<cant_nodos)){
			for(i=0;i<list_size(lista_procesos_reduce);i++){
				procesos_reduce = list_get(lista_procesos_reduce,i);
				if(procesos_reduce->estado == 2) return 1;
				else{
					if(procesos_reduce->estado == 0) {
						flag = true;
						break;
					}
					else{
						termino = true;
					}
				}
			}
			j++;
		}
	}
	return 0;
}


void serializar_reduce_job(int tamanio, int socketJob, int pos, t_nodo *nodo, t_infoJob *job){
	int offset = 0;
	int i,tam;
	char *nombre_map;
	char *buffer = malloc(tamanio);
	t_reduce *reduce_nodo = malloc(sizeof(t_reduce));
	reduce_nodo = list_get(lista_procesos_reduce,pos);

	printf("Reduce: IP: %s, Nombre Resultado: %s, ID: %i\n",nodo->ip_nodo, reduce_nodo->nombre_resultado_reduce, reduce_nodo->id_reduce);
	tam = strlen(nodo->ip_nodo);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, nodo->ip_nodo, tam);
	offset += tam;

	tam = strlen(nodo->puerto_nodo);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, nodo->puerto_nodo, tam);
	offset += tam;

	tam = strlen(reduce_nodo->nombre_resultado_reduce);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer+offset, reduce_nodo->nombre_resultado_reduce, tam);
	offset += tam;

	tam = reduce_nodo->id_reduce;
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);

	tam = job->id_job;
	memcpy(buffer+offset, &(job->id_job), sizeof(int));
	offset += sizeof(int);

	tam = list_size(reduce_nodo->lista_nombre_map);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);

	for(i=0;i<list_size(reduce_nodo->lista_nombre_map);i++){
		nombre_map = list_get(reduce_nodo->lista_nombre_map,i);
		//printf("NOMBRE: %s\n",nombre_map);
		tam = strlen(nombre_map);
		memcpy(buffer+offset, &tam, sizeof(int));
		offset += sizeof(int);
		memcpy(buffer+offset, nombre_map, tam);
		offset += tam;
	}
	//pthread_mutex_lock(&mutex_socket_job);
	tam = send(socketJob, buffer, tamanio, 0);
	printf("cantidad que mando %i\n",tam);
	sleep(2);
	//pthread_mutex_unlock(&mutex_socket_job);
	free(buffer);
	printf("control 2 serializar reduce\n");
}

void reduce_con_combiner(int id_job, int socketJob){
	int i,j,tamanio_total;
	int handshakeJob;
	t_reduce *reduce_nodo = malloc(sizeof(t_reduce));
	char *nombre_map;
	t_nodo *nodo = malloc(sizeof(t_nodo));
	t_infoJob *job = malloc(sizeof(t_infoJob));
	job = list_get(lista_jobs,id_job);
	//-----------------------------------------OBTENGO TAMAÑO TOTAL-----------------------------------------------------------
	for(i=0;i<list_size(lista_procesos_reduce);i++){
		reduce_nodo = list_get(lista_procesos_reduce,i);
		nodo = list_get(lista_nodos_estado,i);
		tamanio_total = strlen(nodo->ip_nodo) + strlen(nodo->puerto_nodo) + strlen(job->arch_resultado_final) + sizeof(int)*6 + 3;
		for(j=0;j<list_size(reduce_nodo->lista_nombre_map);j++){
			nombre_map = list_get(reduce_nodo->lista_nombre_map,j);
			tamanio_total += strlen(nombre_map) + sizeof(int) + 1;
		}
		//pthread_mutex_lock(&mutex_socket_job);
		handshakeJob = 34;
		send(socketJob, &handshakeJob, sizeof(int), 0);
		if(recv(socketJob, &handshakeJob, sizeof(int), 0) < 0) return;
		send(socketJob, &tamanio_total, sizeof(int), 0);
		if(recv(socketJob, &handshakeJob, sizeof(int), 0) < 0) return;
		serializar_reduce_job(tamanio_total, socketJob, i, nodo, job);
		//pthread_mutex_unlock(&mutex_socket_job);
	}


}

void planificarReduce(int id_job, int socketJob){
	int i,j,id_reduce;
	t_job_procesos *procesos_job = malloc(sizeof(t_job_procesos));
	t_tablaProcesos_porJob *proceso = malloc(sizeof(t_tablaProcesos_porJob));
	t_nodo *nodo = malloc(sizeof(t_nodo));
	t_reduce *reduce_nodo;

	char *nombre_map;

	//-------------------------------------------CARGAR PROCESOS POR NODO--------------------------------------------------------
	printf("control 1 planificar\n");
	procesos_job = list_get(lista_tabla_procesos,id_job);
	id_reduce = 0;
	for(j=0;j<cant_nodos;j++){
		printf("control 2 planificar\n");
		nodo = list_get(lista_nodos_estado,j);
		reduce_nodo = malloc(sizeof(t_reduce));
		reduce_nodo->lista_nombre_map = list_create();
		reduce_nodo->id_nodo = nodo->id_nodo;
		reduce_nodo->estado = 0;
		reduce_nodo->id_reduce = id_reduce;
		reduce_nodo->id_job = id_job;
		printf("control 3 planificar\n");
		pthread_mutex_lock(&mutex_contador_archivos_map);
			reduce_nodo->nombre_resultado_reduce = listaNombreArchivosReduce[contador_archivos_reduce];
			contador_archivos_reduce ++;
		pthread_mutex_unlock(&mutex_contador_archivos_map);
		printf("control 4 planificar\n");
		for(i=0;i<list_size(procesos_job->tabla_procesos);i++){
			proceso = list_get(procesos_job->tabla_procesos,i);
			if(proceso->id_nodo == reduce_nodo->id_nodo){
				nombre_map = malloc(strlen(proceso->nombre_archivo_resultado));
				nombre_map = proceso->nombre_archivo_resultado;
				list_add(reduce_nodo->lista_nombre_map,nombre_map);
			}
		}
		pthread_mutex_lock(&mutex_procesos_reduce);
		list_add(lista_procesos_reduce,reduce_nodo);
		pthread_mutex_unlock(&mutex_procesos_reduce);
		id_reduce ++;
	}

	reduce_con_combiner(id_job, socketJob);
	//esperar_que_terminen_reduce(id_job);
	sleep(10);

	printf("EMPIEZA MOVER ARCHIVO \n");
	int handshakeJob;
	reduce_nodo = malloc(sizeof(t_reduce));
	nodo = malloc(sizeof(t_nodo));
	t_nodo *nodo_victima = malloc(sizeof(t_nodo));
	int tamanio_total;
	for(i=0;i < list_size(lista_procesos_reduce);i++){
		reduce_nodo = list_get(lista_procesos_reduce,i);
		//Busco de todos los reduce el que sea del job

		if(reduce_nodo->id_job == id_job){
			//Si es el primero lo elijo como victima y lo busco
			if(i==0){
				//busco el nodo con el id
				for(j=0;j < cant_nodos;j++){
					nodo_victima = list_get(lista_nodos_estado,j);
					printf("control mover archivos 2\n");
					if(nodo_victima->id_nodo == reduce_nodo->id_nodo) break;
				}
			}
			else{
				for(j=0;j < cant_nodos;j++){
					nodo = list_get(lista_nodos_estado,j);
					if(nodo->id_nodo == reduce_nodo->id_nodo) break;
				}
				printf("control mover archivos 1\n");

				pthread_mutex_lock(&mutex_socket_job);
				handshakeJob = 33;
				send(socketJob, &handshakeJob, sizeof(int), 0);
				if(recv(socketJob, &handshakeJob, sizeof(int), 0) < 0) return;
				tamanio_total = strlen(nodo->ip_nodo) + strlen(nodo->puerto_nodo) + strlen(nodo_victima->ip_nodo) + strlen(nodo_victima->puerto_nodo) + strlen(reduce_nodo->nombre_resultado_reduce) + (sizeof(int)*7) + 5;
				send(socketJob, &tamanio_total, sizeof(int), 0);
				if(recv(socketJob, &handshakeJob, sizeof(int), 0) < 0) return;
				serializar_mover_reduce(tamanio_total, socketJob, nodo_victima, nodo, reduce_nodo);
				pthread_mutex_unlock(&mutex_socket_job);
				tamanio_total = 0;
			}
		}
	}
	sleep(60);
}
