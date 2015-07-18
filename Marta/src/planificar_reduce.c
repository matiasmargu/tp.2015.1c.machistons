/*
 * planificar_reduce.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

t_lista_job *tabla;
t_lista_job *tabla2;
t_lista_job *tabla3;
t_tablaProcesos_porJob *campoDeTabla;
t_tablaProcesos_porJob *campoDeTabla2;
t_tablaProcesos_porJob *campoDeTabla3;
t_tablaProcesos_porJob *campoDeTabla4;
t_tablaProcesos_porJob *campoDeTabla5;
t_tablaProcesos_porJob *campoDeTabla6;
t_list* lista_job_tabla;

typedef struct {
	int idNodo;
	char* puertoNodo;
	char * ipNodo;
	t_list * archivosAReducir;
    char* nombreArchivoResultado;
}t_archivosAReducirPorNodo;



void planificarReduce(){
	// para probar
	tabla->idJob=4;
	tabla->tabla_procesos = list_create();
	tabla2->idJob=7854;
	tabla2->tabla_procesos = list_create();
	tabla3->idJob=1456;
	tabla3->tabla_procesos = list_create();

	campoDeTabla->bloque_archivo = 5;
	campoDeTabla->estado = 3;
	campoDeTabla->id_nodo = 4;
	campoDeTabla->nombre_archivo_resultado = "campoTabla1";

	campoDeTabla2->bloque_archivo = 2;
	campoDeTabla2->estado = 0;
	campoDeTabla2->id_nodo = 2;
	campoDeTabla2->nombre_archivo_resultado = "campoTabla2";

	campoDeTabla3->bloque_archivo = 4;
	campoDeTabla3->estado = 1;
	campoDeTabla3->id_nodo = 7;
	campoDeTabla3->nombre_archivo_resultado = "campoTabla3";

    campoDeTabla4->bloque_archivo = 0;
	campoDeTabla4->estado = 1;
	campoDeTabla4->id_nodo = 23;
	campoDeTabla4->nombre_archivo_resultado = "campoTabla4";

	campoDeTabla5->bloque_archivo = 14;
	campoDeTabla5->estado = 2;
	campoDeTabla5->id_nodo = 54;
	campoDeTabla5->nombre_archivo_resultado = "campoTabla5";

	campoDeTabla6->bloque_archivo = 21;
	campoDeTabla6->estado = 3;
	campoDeTabla6->id_nodo = 54;
	campoDeTabla6->nombre_archivo_resultado = "campoTabla6";

	list_add(tabla->tabla_procesos,campoDeTabla);
	list_add(tabla->tabla_procesos,campoDeTabla2);
	list_add(tabla2->tabla_procesos,campoDeTabla3);
	list_add(tabla2->tabla_procesos,campoDeTabla4);
	list_add(tabla3->tabla_procesos,campoDeTabla5);
	list_add(tabla3->tabla_procesos,campoDeTabla6);

	lista_job_tabla = list_create();
	list_add(lista_job_tabla,tabla);
	list_add(lista_job_tabla,tabla2);
	list_add(lista_job_tabla,tabla3);

	lista_nodos_estado = list_create();
	t_nodo *nodo1;
	t_nodo *nodo2;
	t_nodo *nodo3;
	t_nodo *nodo4;
	t_nodo *nodo5;
	t_nodo *nodo6;

	nodo1->id_nodo = 4;
	nodo1->ip_nodo = "44.25";
	nodo1->puerto_nodo = "888";
	nodo2->id_nodo = 2;
	nodo2->ip_nodo = "44.25";
	nodo2->puerto_nodo = "888";
	nodo3->id_nodo = 7;
	nodo3->ip_nodo = "44.25";
	nodo3->puerto_nodo = "888";
	nodo4->id_nodo = 54;
	nodo4->ip_nodo = "44.25";
	nodo4->puerto_nodo = "888";
	nodo5->id_nodo = 23;
	nodo5->ip_nodo = "44.25";
	nodo5->puerto_nodo = "888";
	nodo6->id_nodo = 44;
	nodo6->ip_nodo = "44.25";
	nodo6->puerto_nodo = "888";

	list_add(lista_nodos_estado,nodo1);
	list_add(lista_nodos_estado,nodo2);
	list_add(lista_nodos_estado,nodo3);
	list_add(lista_nodos_estado,nodo4);
	list_add(lista_nodos_estado,nodo5);
	list_add(lista_nodos_estado,nodo6);


	//
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
	int e;
	int tamanio;
	tamanio = list_size(lista_archivosAReducirPorNodo);
	printf("%i\n",tamanio);
	for(e=0;e<tamanio;e++){
		campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
		printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n\n\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);

	}

}
