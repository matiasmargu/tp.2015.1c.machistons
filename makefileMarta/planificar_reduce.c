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

void serializar_reduce_job(int tamanio, int socketJob, t_list* lista_reduce, int pos, t_nodo *nodo, t_infoJob *job){
	int offset = 0;
	int i,tam;
	char *nombre_map;
	char *buffer = malloc(tamanio);
	t_reduce *reduce_nodo = malloc(sizeof(t_reduce));
	reduce_nodo = list_get(lista_reduce,pos);

	printf("control 1 serializar reduce\n");
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

	memcpy(buffer+offset, &(reduce_nodo->id_reduce), sizeof(int));
	offset += sizeof(int);

	memcpy(buffer+offset, &(job->id_job), sizeof(int));
	offset += sizeof(int);

	tam = list_size(reduce_nodo->lista_nombre_map);
	memcpy(buffer+offset, &tam, sizeof(int));
	offset += sizeof(int);

	for(i=0;i<list_size(reduce_nodo->lista_nombre_map);i++){
		nombre_map = list_get(reduce_nodo->lista_nombre_map,i);
		printf("NOMBRE: %s\n",nombre_map);
		tam = strlen(nombre_map);
		memcpy(buffer+offset, &tam, sizeof(int));
		offset += sizeof(int);
		memcpy(buffer+offset, nombre_map, tam);
		offset += tam;
	}

	send(socketJob, buffer, tamanio, 0);
	printf("control 2 serializar reduce\n");
}

void reduce_con_combiner(int id_job, int socketJob, t_list *lista_reduce){
	int i,j,tamanio_total;
	int handshakeJob = 34;
	t_reduce *reduce_nodo = malloc(sizeof(t_reduce));
	char *nombre_map;
	t_nodo *nodo = malloc(sizeof(t_nodo));
	t_infoJob *job = malloc(sizeof(t_infoJob));
	job = list_get(lista_jobs,id_job);
	//-----------------------------------------OBTENGO TAMAÑO TOTAL-----------------------------------------------------------
	for(i=0;i<list_size(lista_reduce);i++){
		reduce_nodo = list_get(lista_reduce,i);
		nodo = list_get(lista_nodos_estado,i);
		tamanio_total = strlen(nodo->ip_nodo) + strlen(nodo->puerto_nodo) + strlen(job->arch_resultado_final) + sizeof(int)*6;
		for(j=0;j<list_size(reduce_nodo->lista_nombre_map);j++){
			nombre_map = list_get(reduce_nodo->lista_nombre_map,j);
			tamanio_total += strlen(nombre_map) + sizeof(int);
		}
		send(socketJob, &handshakeJob, sizeof(int), 0);
		recv(socketJob, &handshakeJob, sizeof(int), 0);
		send(socketJob, &tamanio_total, sizeof(int), 0);
		recv(socketJob, &handshakeJob, sizeof(int), 0);
		serializar_reduce_job(tamanio_total, socketJob, lista_reduce, i, nodo, job);
	}


}

void planificarReduce(int id_job, int socketJob){
	int i,j,id_reduce;
	t_job_procesos *procesos_job = malloc(sizeof(t_job_procesos));
	t_tablaProcesos_porJob *proceso = malloc(sizeof(t_tablaProcesos_porJob));
	t_nodo *nodo = malloc(sizeof(t_nodo));
	t_reduce *reduce_nodo;
	t_list *lista_reduce = list_create();
	char *nombre;
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
		list_add(lista_reduce,reduce_nodo);
		id_reduce ++;
	}

	reduce_con_combiner(id_job, socketJob, lista_reduce);

}
