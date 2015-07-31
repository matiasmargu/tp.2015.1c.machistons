/*
 * planificar_reduce.c
 *
 *  Created on: 30/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

typedef struct{
	int idNodo;
	char* resultadoMap;
};

void aplicarReduce(int idJob){
	int cantidadDeProcesos;
	t_job_procesos *JobProcesos = malloc(sizeof(t_job_procesos));
	t_tablaProcesos_porJob *proceso = malloc(sizeof(t_tablaProcesos_porJob));
	t_infoJob infoJob = malloc(sizeof(t_infoJob));
	t_list *lista_tabla_procesos_reduce = list_create();



	infoJob = list_get(lista_jobs,idJob);
	cantidadDeProcesos = list_size(JobProcesos->tabla_procesos);

	if(infoJob.combiner==1){ // Combiner

		proceso->

	}else{ // Sin Combiner




	}

	JobProcesos = list_get(lista_tabla_procesos,idJob);

	//proceso = list_get(JobProcesos->tabla_procesos,
}
