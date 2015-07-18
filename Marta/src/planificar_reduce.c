/*
 * planificar_reduce.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

t_lista_job tabla;
t_tablaProcesos_porJob campoDeTabla;
t_list* lista_job_tabla;

typedef struct {
	int idNodo;
	char* puertoNodo;
	char * ipNodo;
	t_list * archivosAReducir;
    char* nombreArchivoResultado;
}t_archivosAReducirPorNodo;



void planificarReduce(){
    int a,b,c,d,aux,variableParaResultadoReduce,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
	int tamanioListaDeNodos;
    t_lista_job *campoDeLaLista;
	t_tablaProcesos_porJob *campoDeLaListaTablaDeProcesos;
    t_list * lista_archivosAReducirPorNodo;
    t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
    t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
    t_nodo *campoDeListaDeNodo;
	lista_archivosAReducirPorNodo = list_create();

	// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
	//reducir
	tamanioListaJobTabla=list_size(lista_job_tabla);
	for(a=0;a<tamanioListaJobTabla;a++){
	    campoDeLaLista=list_get(lista_job_tabla,a);
	    tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos);

	    for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
	    	campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
	    	tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
	    	aux=0;
	    	for(c=0;c <tamanioListaArchivosAReducirPorNodo && aux ==0;c++){
	    		campoArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,c);
	    		if(campoArchivosAReducirPorNodo->idNodo == campoDeLaListaTablaDeProcesos->id_nodo){
	    			list_add(campoArchivosAReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
	    		    aux =1;
	    		}
	    		else {//cuando no esta el id del nodo, entonces tiene que agregar todo directo
	    			campoAAgregarAListaReducirPorNodo->idNodo =  campoDeLaListaTablaDeProcesos->id_nodo;
	    			campoAAgregarAListaReducirPorNodo->archivosAReducir = list_create();
	    			list_add(campoAAgregarAListaReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
	                asprintf(&campoAAgregarAListaReducirPorNodo->nombreArchivoResultado,"%s%i","archivo",variableParaResultadoReduce);
	                variableParaResultadoReduce ++;
	                // buscamos el ip y el puerto nodo
	               tamanioListaDeNodos =list_size(lista_nodos_estado);
	               aux=0;
	                for(d=0;d<tamanioListaDeNodos && aux==0;d++){
	                	campoDeListaDeNodo = list_get(lista_nodos_estado,d);
	                	if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
	                		campoAAgregarAListaReducirPorNodo->ipNodo =  campoDeListaDeNodo->ip_nodo;
	                		campoAAgregarAListaReducirPorNodo->puertoNodo =  campoDeListaDeNodo->puerto_nodo;
	                	    aux=1;
	                	}
	                }// agrega el campo a la lista
	                list_add(lista_archivosAReducirPorNodo,campoAAgregarAListaReducirPorNodo);
	    		}
	    }
	}
	}
}
