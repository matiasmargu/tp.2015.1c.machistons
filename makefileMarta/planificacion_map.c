/*
 * planificacion_map.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

int buscarPorContadores(t_bloque *bloque){
	int *contadores_nodos_aux = malloc(sizeof(int)*cant_nodos);
	*contadores_nodos_aux = *contadores_nodos;
	int k;
	for(k=0;k<cant_nodos;k++){
		contadores_nodos_aux[k] = contadores_nodos[k];
	}

	int i,pos_copia;
	int min = contadores_nodos_aux[0];
	int pos_id_nodo=0;
	bool flag = false;
	t_nodo *data_nodo = malloc(sizeof(t_nodo));
	do
	{
	//---------------------------------------BUSCO EL MAS CHICO DE LOS CONTADORES--------------------------------
		min = contadores_nodos_aux[0];
		pos_id_nodo = 0;
		for(i=1;i<cant_nodos;i++){
			if(contadores_nodos_aux[i] < min){
				min = contadores_nodos_aux[i];
				pos_id_nodo = i;
			}
		}
		data_nodo = list_get(lista_nodos_estado,pos_id_nodo);
	//-------------------------------------BUSCO SI LA COPIA ESTA EN EL NODO MAS CHICO---------------------------
		for(i=0;i<3;i++){
			if((data_nodo->id_nodo) == (bloque->copias[i].idNodo)){
				//pos_id_nodo = i;
				pos_copia = i;
				flag = true;
				break;
			}
		}

		if(flag == false) contadores_nodos_aux[pos_id_nodo] = 4444;

	} while(flag == false);
	contadores_nodos[pos_id_nodo] ++;
	free(contadores_nodos_aux);
	return pos_copia;
}

void planificarMap(int id_job, int socketJob){

	char *str_job;
	char *str_id_map;
	int job_prueba;
	job_prueba = 1;
    //---------------------OBTENGO LA INFO DEL JOB--------------------------------------
    t_infoJob *job = malloc(sizeof(t_infoJob));
    job = list_get(lista_jobs,id_job);
    //----------------------------------------------------------------------------------

    t_job_procesos *tabla_procesos_job = malloc(sizeof(t_job_procesos));
    tabla_procesos_job = list_get(lista_tabla_procesos,id_job);
    t_tablaProcesos_porJob *tabla_procesos;
    int j,i,k;
    int numero_copia = 0;
    t_archivo *un_archivo = malloc(sizeof(t_archivo));
    t_bloque *un_bloque = malloc(sizeof(t_bloque));

    printf("--------------CONTADORES----------------\n");
    for(k=0;k<cant_nodos;k++){
    	printf("CONTADOR NODO %i: %i\n",k,contadores_nodos[k]);
    }
    int contador_id = 0;
    char *nombre;
    char *nombre2;
    printf("|        id       |       estado     |        id      |      bloque     |        nm       |         nm       |\n");
    for(i=0;i<list_size(job->archivos_job);i++){
    	un_archivo = list_get(job->archivos_job,i);
    	for(j=0;j<list_size(un_archivo->bloques);j++){
    		printf("control 1\n");
    		un_bloque = list_get(un_archivo->bloques,j);
    		numero_copia = buscarPorContadores(un_bloque);
    		tabla_procesos = malloc(sizeof(t_tablaProcesos_porJob));
    		tabla_procesos->estado = 0; //Estado 0 = espera por enviarse
    		tabla_procesos->id_nodo = un_bloque->copias[numero_copia].idNodo;
    		tabla_procesos->bloque_archivo = un_bloque->copias[numero_copia].Numerobloque;
    		tabla_procesos->nombre_archivo = un_archivo->nombre;
    		tabla_procesos->id_map = contador_id;

			pthread_mutex_lock(&mutex_contador_archivos_reduce);
    		tabla_procesos->nombre_archivo_resultado = listaNombreArchivosMap[contador_archivos_map];
    		contador_archivos_map ++;
    		pthread_mutex_unlock(&mutex_contador_archivos_reduce);

    		pthread_mutex_lock(&mutex_lista_procesos);
    		list_add(tabla_procesos_job->tabla_procesos,tabla_procesos);
    		pthread_mutex_unlock(&mutex_lista_procesos);
    		contador_id ++;
    		printf("       %i        |       %i        |        %i      |        %i       |        %s       |        %s       |\n",tabla_procesos->id_map,tabla_procesos->estado,tabla_procesos->id_nodo,tabla_procesos->bloque_archivo,tabla_procesos->nombre_archivo,tabla_procesos->nombre_archivo_resultado);

    	}
    }

    printf("--------------CONTADORES----------------\n");
    for(k=0;k<cant_nodos;k++){
    	printf("CONTADOR NODO %i: %i\n",k,contadores_nodos[k]);
    }

    free(un_archivo);
    free(un_bloque);

}
