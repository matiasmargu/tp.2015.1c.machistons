/*
 ============================================================================
 Name        : Marta2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */




/*
 * planificar_reduce.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */
/*
#include "librerias_y_estructuras.h"

typedef struct{
	int id_nodo;
	char *ip_nodo;
	char *puerto_nodo;
}t_nodo2;


typedef struct{
	int idJob;
	t_list *tabla_procesos;
}t_lista_job2;

typedef struct{
	int estado; // 0 = todavia no se mando a ejecutar; 1 = en ejecucion; 2 = Fin; 3 = Error
	int bloque_archivo;
	char* nombreArchivo;
	char *nombre_archivo_resultado;
	int id_nodo;
}t_tablaProcesos_porJob2;


typedef struct {
	int idNodo;
	char* puertoNodo;
	char * ipNodo;
	t_list * archivosAReducir;
    char* nombreArchivoResultado;
    int estado;
}t_archivosAReducirPorNodo;

typedef struct {
	char *puertoNodo;
	char *ipNodo;
	t_list * archivosAMover;
}t_moverArchivos;

typedef struct {
	char *puertoNodo;
	char *ipNodo;
	char * archivoAMover;
}t_serializarUnArchivoParaMover;

typedef struct {
	int contador;
	int idNodo;
}t_contadorNodo;


t_lista_job2 *tabla;
t_lista_job2 *tabla2;
t_lista_job2 *tabla3;
t_tablaProcesos_porJob2 *campoDeTabla;
t_tablaProcesos_porJob2 *campoDeTabla2;
t_tablaProcesos_porJob2 *campoDeTabla3;
t_tablaProcesos_porJob2 *campoDeTabla4;
t_tablaProcesos_porJob2 *campoDeTabla5;
t_tablaProcesos_porJob2 *campoDeTabla6;
t_list* lista_job_tabla;





char* serializar_moverParaReduceFinal(t_para_job * archivoAMover,int tamanioSerializacionMover){
	char *serializedPackage = malloc(tamanioSerializacionMover);
	int offset = 0;
	int size_to_send;

	int tamanioNombre;

	tamanioNombre = strlen(archivoAMover->ipAmover) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->ipAmover) + 1;
	memcpy(serializedPackage + offset, archivoAMover->ipAmover, size_to_send);
	offset += size_to_send;


	tamanioNombre = strlen(archivoAMover->puertoAmover) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->puertoAmover) + 1;
	memcpy(serializedPackage + offset, archivoAMover->puertoAmover, size_to_send);
	offset += size_to_send;


	tamanioNombre = strlen(archivoAMover->archivoAmover) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->archivoAmover) + 1;
	memcpy(serializedPackage + offset, archivoAMover->archivoAmover, size_to_send);
	offset += size_to_send;



	tamanioNombre = strlen(archivoAMover->ipAConectarse) + 1;
		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
		offset += size_to_send;

		size_to_send =  strlen(archivoAMover->ipAConectarse) + 1;
		memcpy(serializedPackage + offset, archivoAMover->ipAConectarse, size_to_send);
		offset += size_to_send;

		tamanioNombre = strlen(archivoAMover->puertoAconectarse) + 1;
		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
		offset += size_to_send;

		size_to_send =  strlen(archivoAMover->puertoAconectarse) + 1;
		memcpy(serializedPackage + offset, archivoAMover->puertoAconectarse, size_to_send);
		offset += size_to_send;




	return serializedPackage;
}





char* serializar_archivoAMover(t_serializarUnArchivoParaMover * archivoAMover,int tamanioSerializacionMover){
	char *serializedPackage = malloc(tamanioSerializacionMover);
	int offset = 0;
	int size_to_send;

	int tamanioNombre;
	tamanioNombre = strlen(archivoAMover->puertoNodo) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->puertoNodo) + 1;
	memcpy(serializedPackage + offset, archivoAMover->puertoNodo, size_to_send);
	offset += size_to_send;

	tamanioNombre = strlen(archivoAMover->ipNodo) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->ipNodo) + 1;
	memcpy(serializedPackage + offset, archivoAMover->ipNodo, size_to_send);
	offset += size_to_send;

	tamanioNombre = strlen(archivoAMover->archivoAMover) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->archivoAMover) + 1;
	memcpy(serializedPackage + offset, archivoAMover->archivoAMover, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}




//AGREGO LA SERIALIZACION , ESTA IGUAL QUE COMO SE DESERIALIZA EN JOB

char* serializar_marta_job_reduce(t_marta_job_reduce *bloque, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

			size_to_send =  sizeof(bloque->cantidadArchivos);
			memcpy(serializedPackage + offset, &(bloque->cantidadArchivos), size_to_send);
			offset += size_to_send;

			int tamanioNombre = strlen(bloque->ipNodo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->ipNodo) + 1;
			memcpy(serializedPackage + offset, bloque->ipNodo, size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->puertoNodo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->puertoNodo) + 1;
			memcpy(serializedPackage + offset, bloque->puertoNodo, size_to_send);
			offset += size_to_send;


			size_to_send =  sizeof(bloque->idNodo);
			memcpy(serializedPackage + offset, &(bloque->idNodo), size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->archivoResultadoReduce) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->archivoResultadoReduce) + 1;
			memcpy(serializedPackage + offset, bloque->archivoResultadoReduce, size_to_send);
			offset += size_to_send;

			//SERIALIZACION DE LA LISTA DE ARCHIVOS
			int tamanioLista = list_size(bloque->listaArchivosTemporales);
			int a;
			t_para_nodo* archivo;

			for(a=0;a< tamanioLista; a++){
			archivo = list_get(bloque->listaArchivosTemporales, a);
			tamanioNombre = strlen(archivo->archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(archivo->archivo) + 1;
			memcpy(serializedPackage + offset, archivo->archivo, size_to_send);
			offset += size_to_send;

					}




			return serializedPackage;
		}

int recive_y_deserializa_job_marta(t_job_marta_reduce *bloque, int socket, uint32_t tamanioTotal){
		int status;
		char *buffer = malloc(tamanioTotal);
		int offset=0;

		recv(socket, buffer, tamanioTotal, 0);

		memcpy(&(bloque->rutina), buffer + offset, sizeof(bloque->rutina));
		offset += sizeof(bloque->rutina);

		memcpy(&(bloque->idNodo), buffer + offset, sizeof(bloque->idNodo));
		offset += sizeof(bloque->idNodo);

		memcpy(&(bloque->resultado), buffer + offset, sizeof(bloque->resultado));
		offset += sizeof(bloque->resultado);

		int tamanioDinamico;
		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->nombreArchivo = malloc(tamanioDinamico);
		memcpy(bloque->nombreArchivo, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;

		free(buffer);
		return status; // aca hay que ver si va status o no

	}







// HAY QUE VER DE DONDE SACAMOS EL SOCKET JOB
      //Para mi tiene que venir como parametro cuando llaman a la funcion , ya que marta y job ya
	  // se van a estar comunicando por el mismo socket cuando le pasa la lista inicial el combiner y demas
/
void planificarReduceConCombiner(char* archivoFinal){

	t_lista_job2 *tabla;
	t_lista_job2 *tabla2;
	t_lista_job2* tabla3;
	t_tablaProcesos_porJob2 *campoDeTabla;
	t_tablaProcesos_porJob2 *campoDeTabla2;
	t_tablaProcesos_porJob2 *campoDeTabla3;
	t_tablaProcesos_porJob2 *campoDeTabla4;
	t_tablaProcesos_porJob2 *campoDeTabla5;
	t_tablaProcesos_porJob2 *campoDeTabla6;
	t_list* lista_job_tabla;

		// para probar
        int tamanioTabla,tamanioCampoDeTabla,tamanioNodo, contador;
        tamanioTabla=sizeof(int);
        tamanioCampoDeTabla= sizeof(int) * 3 + sizeof(char);
        tamanioNodo = sizeof(char) * 3 + sizeof(int);

        tabla = malloc(tamanioTabla);
		tabla2 = malloc(tamanioTabla);
		tabla3 = malloc(tamanioTabla);
        campoDeTabla = malloc(tamanioCampoDeTabla);
        campoDeTabla2 = malloc(tamanioCampoDeTabla);
        campoDeTabla3 = malloc(tamanioCampoDeTabla);
        campoDeTabla4 = malloc(tamanioCampoDeTabla);
        campoDeTabla5 = malloc(tamanioCampoDeTabla);
        campoDeTabla6 = malloc(tamanioCampoDeTabla);
        tabla->idJob=4;
		tabla->tabla_procesos = list_create();

		tabla2->idJob=7854;
		tabla2->tabla_procesos = list_create();

		tabla3->idJob=5;

		tabla3->tabla_procesos = list_create();


		 campoDeTabla->bloque_archivo = 5;
					campoDeTabla->estado = 3;
					campoDeTabla->id_nodo = 4;
					campoDeTabla->nombre_archivo_resultado = "campoTabla1";
					campoDeTabla->nombreArchivo = "archivo1";

					campoDeTabla2->bloque_archivo = 2;
					campoDeTabla2->estado = 0;
					campoDeTabla2->id_nodo = 2;
					campoDeTabla2->nombre_archivo_resultado = "campoTabla2";
					campoDeTabla2->nombreArchivo = "archivo2";

					campoDeTabla3->bloque_archivo = 4;
					campoDeTabla3->estado = 1;
					campoDeTabla3->id_nodo = 7;
					campoDeTabla3->nombre_archivo_resultado = "campoTabla3";
					campoDeTabla3->nombreArchivo = "archivo3";

				    campoDeTabla4->bloque_archivo = 0;
					campoDeTabla4->estado = 1;
					campoDeTabla4->id_nodo = 54;
					campoDeTabla4->nombre_archivo_resultado = "campoTabla4";
					campoDeTabla4->nombreArchivo = "archivo4";

					campoDeTabla5->bloque_archivo = 14;
					campoDeTabla5->estado = 2;
					campoDeTabla5->id_nodo = 54;
					campoDeTabla5->nombre_archivo_resultado = "campoTabla5";
					campoDeTabla5->nombreArchivo = "archivo5";

					campoDeTabla6->bloque_archivo = 21;
					campoDeTabla6->estado = 3;
					campoDeTabla6->id_nodo = 54;
					campoDeTabla6->nombre_archivo_resultado = "campoTabla6";
					campoDeTabla6->nombreArchivo = "archivo6";


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
		t_nodo2 *nodo1;
		t_nodo2 *nodo2;
		t_nodo2 *nodo3;
		t_nodo2 *nodo4;
		t_nodo2 *nodo5;
		t_nodo2 *nodo6;

		nodo1=malloc(tamanioNodo);
		nodo2=malloc(tamanioNodo);
		nodo3=malloc(tamanioNodo);
		nodo4=malloc(tamanioNodo);
		nodo5=malloc(tamanioNodo);
		nodo6=malloc(tamanioNodo);


		nodo1->id_nodo = 4;
		nodo1->ip_nodo = "44.251";
		nodo1->puerto_nodo = "887";
		nodo2->id_nodo = 2;
		nodo2->ip_nodo = "44.250";
		nodo2->puerto_nodo = "886";
		nodo3->id_nodo = 7;
		nodo3->ip_nodo = "44.252";
		nodo3->puerto_nodo = "885";
		nodo4->id_nodo = 54;
		nodo4->ip_nodo = "44.253";
		nodo4->puerto_nodo = "884";
		nodo5->id_nodo = 23;
		nodo5->ip_nodo = "44.251";
		nodo5->puerto_nodo = "883";
		nodo6->id_nodo = 44;
		nodo6->ip_nodo = "44.254";
		nodo6->puerto_nodo = "882";

		list_add(lista_nodos_estado,nodo1);
		list_add(lista_nodos_estado,nodo2);
		list_add(lista_nodos_estado,nodo3);
		list_add(lista_nodos_estado,nodo4);
		list_add(lista_nodos_estado,nodo5);
		list_add(lista_nodos_estado,nodo6);


		        int a,b,c,d,e=0,auxiliar = 0,aux,variableParaResultadoReduce=0,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
				int tamanioListaDeNodos,tamanio,tamanioArchivosaReducir;
				t_archivosAReducirPorNodo *campoDeUnNodo;
				t_archivosAReducirPorNodo * campoParaLiberar;
				t_marta_job_reduce structAserializar;
				int handshakeJob, enteroPrueba,tamanioAenviar,tamanioTotalVector, socketJob;
				char* archivo1;
				char* structAEnviarAJob;
			    t_lista_job2 *campoDeLaLista;
			    campoDeLaLista = malloc(tamanioTabla);
				t_tablaProcesos_porJob2 *campoDeLaListaTablaDeProcesos;
		        campoDeLaListaTablaDeProcesos = malloc(tamanioCampoDeTabla);
			    t_list * lista_archivosAReducirPorNodo;
			    t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
		        campoArchivosAReducirPorNodo= malloc(tamanioCampoDeTabla);
			    t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
			    t_nodo2 *campoDeListaDeNodo;
			    campoDeListaDeNodo = malloc(tamanioNodo);
				lista_archivosAReducirPorNodo = list_create();
				list_clean(lista_archivosAReducirPorNodo);
				//REDUCE FINAL
				t_job_marta_reduce job_marta;
				t_job_marta_reduce job_marta2;
				int contador2,r,l,tamanioArecibir,tamanioArecibir2,tamanioCampoParaNodo,tamaniolistaFinalDeArchivosAReducir,p, v,tamanioListaFinal,m, reduceFinal,tamanioListaFinalAEnviar,tamanioStruct2;
				t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo;
				t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo2;
				t_archivosAReducirPorNodo * campoDeListaArchivosAReducirPorNodo3;
				t_list* listaFinalDeArchivosAReducir;
				t_marta_job_reduce antesDeSerializar;
				char* structParaJob;
				contador2 = 0;
				char *structParaJob2;
				t_para_nodo* campoListaParaNodo;
				t_para_nodo* archivoA;
				t_para_nodo* campoAAgregar;
                t_para_job * archivoParaJob;
				// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
				//reducir
				tamanioListaJobTabla=list_size(lista_job_tabla);

				printf("\n\nLos campos de la lista que deberian estar son:\n");

				for(a=0;a<tamanioListaJobTabla;a++){
					campoDeLaLista=list_get(lista_job_tabla,a);
					tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos);

					for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
						campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
						tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
				    	if(tamanioListaArchivosAReducirPorNodo == 0){
				    		tamanioCampoDeTabla= sizeof(int) * 2 + strlen(campoDeLaListaTablaDeProcesos->nombreArchivo) + 1 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo_resultado) + 1;
				    		campoAAgregarAListaReducirPorNodo = malloc(tamanioCampoDeTabla);
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
				    		campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    		printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    		e++;
				    	}
				    	else{
				    	aux=0;
				    		for(c=0;c <tamanioListaArchivosAReducirPorNodo && aux ==0;c++){
				    			campoArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,c);
				    			if(campoArchivosAReducirPorNodo->idNodo == campoDeLaListaTablaDeProcesos->id_nodo){
				    				list_add(campoArchivosAReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
				    				aux =1;
				    			}
				    		}
				    			if(aux==0){//cuando no esta el id del nodo, entonces tiene que agregar todo directo
				    				printf("Aca deberia entrar 4 veces \n");
				    				tamanioCampoDeTabla= sizeof(int) * 2 + strlen(campoDeLaListaTablaDeProcesos->nombreArchivo) + 1 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo_resultado) + 1;
				    				campoAAgregarAListaReducirPorNodo = malloc(tamanioCampoDeTabla);
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
				    				campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    				printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    				e++;

				    			}
				    	}
					}
					}



           tamanio = list_size(lista_archivosAReducirPorNodo);
           printf("\n\nEl tamanio final de la lista es: %i\n",tamanio);
           printf("Lo que imprime a fuera del for es:\n");
           for(e=0;e<tamanio;e++){
        	   campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
        	   printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
        	   a=list_size(campoAAgregarAListaReducirPorNodo->archivosAReducir);
        	   printf("%i\n",a);
          	   for(b=0;b<a;b++){
        		   printf("Los campos de la lista de archivos a reducir son: %s \n",list_get(campoAAgregarAListaReducirPorNodo->archivosAReducir,b));
        	   }
        	   }


//sigo el reduce


	handshakeJob = 34;
	contador = 0;
	//tamanio es el tamanio de la lista
	//el socketJob tiene que venir como parametro
	for(a=0; a<tamanio; a++){
	campoDeUnNodo=list_get(lista_archivosAReducirPorNodo,a);
	send(socketJob, &handshakeJob, sizeof(int),0);
	recv(socketJob, &enteroPrueba, sizeof(int),0);
	tamanioArchivosaReducir = list_size(campoDeUnNodo->archivosAReducir);
	for(b=0; b< tamanioArchivosaReducir; b++){
	archivo1 =	list_get(campoDeUnNodo->archivosAReducir,b);
	tamanioTotalVector += strlen(archivo1);
	tamanioTotalVector += 1;
	}
	tamanioAenviar = (sizeof(int)*2)+ strlen(campoDeUnNodo->ipNodo)+1 +  strlen(campoDeUnNodo->puertoNodo)+1 +  strlen(campoDeUnNodo->nombreArchivoResultado)+1 + tamanioTotalVector;
	send(socketJob, &tamanioAenviar, sizeof(int), 0);
	recv(socketJob, &enteroPrueba, sizeof(int),0);
	structAserializar.archivoResultadoReduce = campoDeUnNodo->nombreArchivoResultado;
	structAserializar.cantidadArchivos = list_size(campoDeUnNodo->archivosAReducir);
	structAserializar.idNodo = campoDeUnNodo->idNodo;
	structAserializar.ipNodo = campoDeUnNodo->ipNodo;
	structAserializar.puertoNodo = campoDeUnNodo->puertoNodo;
	list_add_all(structAserializar.listaArchivosTemporales, campoDeUnNodo->archivosAReducir);
	structAEnviarAJob = serializar_marta_job_reduce(&structAserializar, tamanioAenviar);
	send(socketJob, structAEnviarAJob, tamanioAenviar, 0);
	contador += 1;
	}

	//ACA EMPIEZA REDUCE FINAL CON COMBINER

	 listaFinalDeArchivosAReducir = list_create();
	 for(a=0;a< contador ; a++){
		recv(socketJob, &tamanioArecibir, sizeof(int),0);
		send(socketJob, &enteroPrueba, sizeof(int),0);
		int estado = 1;
		estado = recive_y_deserializa_job_marta(&job_marta, socketJob, tamanioArecibir);//LO HICE ARRIBA
		//AGREGO EL ESTADO A LA LISTA
		if(estado){
			for(r=0; r< list_size(lista_archivosAReducirPorNodo);r++){
				campoDeListaArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,r);
				if(campoDeListaArchivosAReducirPorNodo->idNodo == job_marta.idNodo){
					campoDeListaArchivosAReducirPorNodo->estado = job_marta.resultado;
					//ME FIJO SI ESTAN TODOS LOS ARCHIVOS REDUCIDOS

					for(p=0;(p< list_size(lista_archivosAReducirPorNodo)) & (auxiliar == 0); p++){
						campoDeListaArchivosAReducirPorNodo2 = list_get(lista_archivosAReducirPorNodo,p);
						if(campoDeListaArchivosAReducirPorNodo2->estado == 1){
							contador2 += 1;
						}else{auxiliar = 1;}
					}
					if(contador == contador2){
						for(l=0;l< list_size(lista_archivosAReducirPorNodo); l++){
							campoDeListaArchivosAReducirPorNodo3 = list_get(lista_archivosAReducirPorNodo,l);
							tamanioCampoParaNodo = 3 + strlen(campoDeListaArchivosAReducirPorNodo3->nombreArchivoResultado) + strlen(campoDeListaArchivosAReducirPorNodo3->puertoNodo) + strlen(campoDeListaArchivosAReducirPorNodo3->ipNodo);
							campoListaParaNodo = malloc(tamanioCampoParaNodo);
							campoListaParaNodo->archivo = campoDeListaArchivosAReducirPorNodo3->nombreArchivoResultado;
							campoListaParaNodo->ip = campoDeListaArchivosAReducirPorNodo3->ipNodo;
							campoListaParaNodo->puerto = campoDeListaArchivosAReducirPorNodo3->puertoNodo;

							list_add(listaFinalDeArchivosAReducir,campoListaParaNodo);
						}

						//MANDAMOS EL REDUCE FINAL
						send(socketJob, &handshakeJob, sizeof(int),0);
						recv(socketJob, &enteroPrueba, sizeof(int),0);
						reduceFinal = 33;
						send(socketJob, &reduceFinal, sizeof(int),0);
						recv(socketJob, &enteroPrueba, sizeof(int),0);
						tamaniolistaFinalDeArchivosAReducir = list_size(listaFinalDeArchivosAReducir);
						send(socketJob, &tamaniolistaFinalDeArchivosAReducir, sizeof(int),0);
						recv(socketJob, &enteroPrueba, sizeof(int),0);


						// le mando a mover los archivos al nodo


						campoDeListaArchivosAReducirPorNodo3 = list_get(lista_archivosAReducirPorNodo,1);
						archivoParaJob->ipAConectarse =campoDeListaArchivosAReducirPorNodo3->ipNodo ;
						archivoParaJob->puertoAconectarse = campoDeListaArchivosAReducirPorNodo3->puertoNodo;
						for(m=0; m< list_size(listaFinalDeArchivosAReducir); m++){
							archivoA =	list_get( listaFinalDeArchivosAReducir,m);
							archivoParaJob->archivoAmover = archivoA->archivo ;
							archivoParaJob->ipAmover = archivoA->ip ;
							archivoParaJob->puertoAmover = archivoA->puerto ;
							tamanioListaFinal += strlen(archivoParaJob->archivoAmover);
							tamanioListaFinal += strlen(archivoParaJob->ipAmover);
							tamanioListaFinal += strlen(archivoParaJob->puertoAmover);
							tamanioListaFinal += strlen(archivoParaJob->puertoAconectarse);
							tamanioListaFinal += strlen(archivoParaJob->ipAConectarse);
							send(socketJob, &tamanioListaFinal , sizeof(int),0);
							recv(socketJob, &enteroPrueba, sizeof(int),0);
							structParaJob =  serializar_moverParaReduceFinal(&archivoParaJob, tamanioListaFinal);
							send(socketJob, structParaJob, tamanio, 0);
							//FALTS VRE SI SE CAE EL NODO
						}


						//hacer reduce final de todos los archivos de la listafinal

						send(socketJob, &handshakeJob, sizeof(int),0);
						recv(socketJob, &enteroPrueba, sizeof(int),0);
						reduceFinal = 20;
						send(socketJob, &reduceFinal, sizeof(int),0);
						recv(socketJob, &enteroPrueba, sizeof(int),0);

					antesDeSerializar.archivoResultadoReduce = archivoFinal;
					antesDeSerializar.cantidadArchivos = list_size(listaFinalDeArchivosAReducir);
					antesDeSerializar.idNodo = campoDeListaArchivosAReducirPorNodo3->idNodo;
					antesDeSerializar.puertoNodo = archivoParaJob->puertoAconectarse;
					antesDeSerializar.ipNodo = archivoParaJob->ipAConectarse;


						for(v=0; v < list_size(listaFinalDeArchivosAReducir); v++){
							campoAAgregar = list_get(listaFinalDeArchivosAReducir, v);
					list_add(antesDeSerializar.listaArchivosTemporales,campoAAgregar->archivo );
					tamanioListaFinalAEnviar += strlen(campoAAgregar->archivo);
					tamanioListaFinalAEnviar += 1;
						}
					tamanioStruct2 = (sizeof(int)*2) + strlen(antesDeSerializar.ipNodo) +1 + strlen(antesDeSerializar.puertoNodo)+1+ strlen(antesDeSerializar.archivoResultadoReduce)+1 + tamanioListaFinalAEnviar;
					send(socketJob, &tamanioStruct2 , sizeof(int),0);
					recv(socketJob, &enteroPrueba, sizeof(int),0);
					structParaJob2 =  serializar_marta_job_reduce(&antesDeSerializar, tamanioStruct2);
					send(socketJob, structParaJob2, tamanioStruct2, 0);
					//RECIBIMOS EL RESULTADO
					recv(socketJob, &tamanioArecibir2, sizeof(int),0);
					send(socketJob, &enteroPrueba, sizeof(int),0);
					int estado2 = 1;
					estado2 = recive_y_deserializa_job_marta(&job_marta2, socketJob, tamanioArecibir2);
					if(estado2){
						if(job_marta2.resultado){
							printf("Termino de forma exitosa el reduce final\n");
						}else{
							printf("Fallo el reduce final\n");
						}
				}
					}
				}
			}


		}

	}

	 for(a=0; a<tamanio; a++){
	 	campoParaLiberar=list_get(lista_archivosAReducirPorNodo,a);
	 	free(campoParaLiberar);

	 }

	 for(a=0;a<list_size(listaFinalDeArchivosAReducir);a++){
		 campoListaParaNodo = list_get(listaFinalDeArchivosAReducir,a);
		 free(campoListaParaNodo);
	 }
	// despues hay que ver cual free va y cual no, una vez que no vaya todo lo harcodeado por nosotros

	/*
	free(campoDeLaLista);
	free(campoDeLaListaTablaDeProcesos);
	free(campoArchivosAReducirPorNodo);
	free(campoDeListaDeNodo);
	free(campoAAgregarAListaReducirPorNodo);

	free(campoDeListaArchivosAReducirPorNodo);
	free(campoDeListaArchivosAReducirPorNodo2);
	free(campoDeListaArchivosAReducirPorNodo3);
	free(listaFinalDeArchivosAReducir);
	free(archivoA);
	free(campoListaParaNodo);
	free(campoAAgregar);

	}


// A PARTIR DE ACA ES PARA EL SIN COMBINER

void planificarSincombiner(char* archivoFinal){

	t_lista_job2 *tabla;
	t_lista_job2 *tabla2;
	t_lista_job2* tabla3;
	t_tablaProcesos_porJob2 *campoDeTabla;
	t_tablaProcesos_porJob2 *campoDeTabla2;
	t_tablaProcesos_porJob2 *campoDeTabla3;
	t_tablaProcesos_porJob2 *campoDeTabla4;
	t_tablaProcesos_porJob2 *campoDeTabla5;
	t_tablaProcesos_porJob2 *campoDeTabla6;
	t_list* lista_job_tabla;

		// para probar
        int tamanioTabla,tamanioCampoDeTabla,tamanioNodo, contador;
        tamanioTabla=sizeof(int);
        tamanioCampoDeTabla= sizeof(int) * 3 + sizeof(char);
        tamanioNodo = sizeof(char) * 3 + sizeof(int);

        tabla = malloc(tamanioTabla);
		tabla2 = malloc(tamanioTabla);
		tabla3 = malloc(tamanioTabla);
        campoDeTabla = malloc(tamanioCampoDeTabla);
        campoDeTabla2 = malloc(tamanioCampoDeTabla);
        campoDeTabla3 = malloc(tamanioCampoDeTabla);
        campoDeTabla4 = malloc(tamanioCampoDeTabla);
        campoDeTabla5 = malloc(tamanioCampoDeTabla);
        campoDeTabla6 = malloc(tamanioCampoDeTabla);
        tabla->idJob=4;
		tabla->tabla_procesos = list_create();

		tabla2->idJob=7854;
		tabla2->tabla_procesos = list_create();

		tabla3->idJob=5;

		tabla3->tabla_procesos = list_create();


		 campoDeTabla->bloque_archivo = 5;
					campoDeTabla->estado = 3;
					campoDeTabla->id_nodo = 4;
					campoDeTabla->nombre_archivo_resultado = "campoTabla1";
					campoDeTabla->nombreArchivo = "archivo1";

					campoDeTabla2->bloque_archivo = 2;
					campoDeTabla2->estado = 0;
					campoDeTabla2->id_nodo = 2;
					campoDeTabla2->nombre_archivo_resultado = "campoTabla2";
					campoDeTabla2->nombreArchivo = "archivo2";

					campoDeTabla3->bloque_archivo = 4;
					campoDeTabla3->estado = 1;
					campoDeTabla3->id_nodo = 7;
					campoDeTabla3->nombre_archivo_resultado = "campoTabla3";
					campoDeTabla3->nombreArchivo = "archivo3";

				    campoDeTabla4->bloque_archivo = 0;
					campoDeTabla4->estado = 1;
					campoDeTabla4->id_nodo = 54;
					campoDeTabla4->nombre_archivo_resultado = "campoTabla4";
					campoDeTabla4->nombreArchivo = "archivo4";

					campoDeTabla5->bloque_archivo = 14;
					campoDeTabla5->estado = 2;
					campoDeTabla5->id_nodo = 54;
					campoDeTabla5->nombre_archivo_resultado = "campoTabla5";
					campoDeTabla5->nombreArchivo = "archivo5";

					campoDeTabla6->bloque_archivo = 21;
					campoDeTabla6->estado = 3;
					campoDeTabla6->id_nodo = 54;
					campoDeTabla6->nombre_archivo_resultado = "campoTabla6";
					campoDeTabla6->nombreArchivo = "archivo6";


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
		t_nodo2 *nodo1;
		t_nodo2 *nodo2;
		t_nodo2 *nodo3;
		t_nodo2 *nodo4;
		t_nodo2 *nodo5;
		t_nodo2 *nodo6;

		nodo1=malloc(tamanioNodo);
		nodo2=malloc(tamanioNodo);
		nodo3=malloc(tamanioNodo);
		nodo4=malloc(tamanioNodo);
		nodo5=malloc(tamanioNodo);
		nodo6=malloc(tamanioNodo);


		nodo1->id_nodo = 4;
		nodo1->ip_nodo = "44.251";
		nodo1->puerto_nodo = "887";
		nodo2->id_nodo = 2;
		nodo2->ip_nodo = "44.250";
		nodo2->puerto_nodo = "886";
		nodo3->id_nodo = 7;
		nodo3->ip_nodo = "44.252";
		nodo3->puerto_nodo = "885";
		nodo4->id_nodo = 54;
		nodo4->ip_nodo = "44.253";
		nodo4->puerto_nodo = "884";
		nodo5->id_nodo = 23;
		nodo5->ip_nodo = "44.251";
		nodo5->puerto_nodo = "883";
		nodo6->id_nodo = 44;
		nodo6->ip_nodo = "44.254";
		nodo6->puerto_nodo = "882";

		list_add(lista_nodos_estado,nodo1);
		list_add(lista_nodos_estado,nodo2);
		list_add(lista_nodos_estado,nodo3);
		list_add(lista_nodos_estado,nodo4);
		list_add(lista_nodos_estado,nodo5);
		list_add(lista_nodos_estado,nodo6);

		t_moverArchivos moverArchivos;
		t_serializarUnArchivoParaMover archivoAMover;
		t_serializarUnArchivoParaMover archivoAr;
		        int a,b,c,d,e=0,aux,variableParaResultadoReduce=0,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
				int tamanioListaDeNodos,tamanio,tamanioArchivosaReducir;
				t_archivosAReducirPorNodo *campoDeUnNodo;
				t_archivosAReducirPorNodo * campoParaLiberar;
				t_marta_job_reduce structAserializar;
				int handshakeJob, enteroPrueba,tamanioAenviar,tamanioTotalVector, socketJob,tamanioStruct;
				char* archivo1;
				char* structAEnviarAJob;
			    t_lista_job2 *campoDeLaLista;
			    campoDeLaLista = malloc(tamanioTabla);
				t_tablaProcesos_porJob2 *campoDeLaListaTablaDeProcesos;
		        campoDeLaListaTablaDeProcesos = malloc(tamanioCampoDeTabla);
			    t_list * lista_archivosAReducirPorNodo;
			    t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
		        campoArchivosAReducirPorNodo= malloc(tamanioCampoDeTabla);
			    t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
			    t_nodo2 *campoDeListaDeNodo;
			    campoDeListaDeNodo = malloc(tamanioNodo);
				lista_archivosAReducirPorNodo = list_create();
				list_clean(lista_archivosAReducirPorNodo);
				t_contadorNodo contadorNodo;
				//REDUCE FINAL
				t_job_marta_reduce job_marta;
				t_job_marta_reduce job_marta2;
				int contador2,r,l,tamanioArecibir,tamanioArecibir2,tamanioCampoParaNodo,tamaniolistaFinalDeArchivosAReducir,p, v,tamanioListaFinal,m, reduceFinal,tamanioListaFinalAEnviar,tamanioStruct2;
				t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo;
				t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo2;
				t_archivosAReducirPorNodo * campoDeListaArchivosAReducirPorNodo3;
				t_list* listaFinalDeArchivosAReducir;
				t_marta_job_reduce antesDeSerializar;
				char* structParaJob;
				char* archivo;
				contador2 = 0;
				char *structParaJob2;
				char* archivoASerializar;
				t_para_nodo* campoListaParaNodo;
				t_para_nodo* archivoA;
				t_para_nodo* campoAAgregar;
                t_para_job * archivoParaJob;
				// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
				//reducir
				tamanioListaJobTabla=list_size(lista_job_tabla);

				printf("\n\nLos campos de la lista que deberian estar son:\n");

				for(a=0;a<tamanioListaJobTabla;a++){
					campoDeLaLista=list_get(lista_job_tabla,a);
					tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos);

					for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
						campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
						tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
				    	if(tamanioListaArchivosAReducirPorNodo == 0){
				    		tamanioCampoDeTabla= sizeof(int) * 2 + strlen(campoDeLaListaTablaDeProcesos->nombreArchivo) + 1 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo_resultado) + 1;
				    		campoAAgregarAListaReducirPorNodo = malloc(tamanioCampoDeTabla);
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
				    		campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    		printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    		e++;
				    	}
				    	else{
				    	aux=0;
				    		for(c=0;c <tamanioListaArchivosAReducirPorNodo && aux ==0;c++){
				    			campoArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,c);
				    			if(campoArchivosAReducirPorNodo->idNodo == campoDeLaListaTablaDeProcesos->id_nodo){
				    				list_add(campoArchivosAReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
				    				aux =1;
				    			}
				    		}
				    			if(aux==0){//cuando no esta el id del nodo, entonces tiene que agregar todo directo
				    				printf("Aca deberia entrar 4 veces \n");
				    				tamanioCampoDeTabla= sizeof(int) * 2 + strlen(campoDeLaListaTablaDeProcesos->nombreArchivo) + 1 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo_resultado) + 1;
				    				campoAAgregarAListaReducirPorNodo = malloc(tamanioCampoDeTabla);
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
				    				campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    				printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    				e++;

				    			}
				    	}
					}
					}



           tamanio = list_size(lista_archivosAReducirPorNodo);
           printf("\n\nEl tamanio final de la lista es: %i\n",tamanio);
           printf("Lo que imprime a fuera del for es:\n");
           for(e=0;e<tamanio;e++){
        	   campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
        	   printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
        	   a=list_size(campoAAgregarAListaReducirPorNodo->archivosAReducir);
        	   printf("%i\n",a);
          	   for(b=0;b<a;b++){
        		   printf("Los campos de la lista de archivos a reducir son: %s \n",list_get(campoAAgregarAListaReducirPorNodo->archivosAReducir,b));
        	   }
        	   }

//BUSCAMOS EL NODO QUE MAS ARCHIVOS A REDUCIR TIENE

	 tamanio = list_size(lista_archivosAReducirPorNodo);

	 contadorNodo.contador = 0;
	 for(e=0;e<tamanio;e++){
		 campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
		 a=list_size(campoAAgregarAListaReducirPorNodo->archivosAReducir);

		 if (a > contadorNodo.contador) {
			 contadorNodo.contador = a;
			 contadorNodo.idNodo = campoAAgregarAListaReducirPorNodo->idNodo;
		 }

	 }


			 // con este for ya vamos a tener el puerto el ip y la lista de archivos a mover
			 aux=0;
			 for(d=0;d<tamanioListaDeNodos && aux==0;d++){
				 campoDeListaDeNodo = list_get(lista_nodos_estado,d);
				 if(contadorNodo.idNodo==campoDeListaDeNodo->id_nodo){
					 moverArchivos.ipNodo =  campoDeListaDeNodo->ip_nodo;
					 moverArchivos.puertoNodo = campoDeListaDeNodo->puerto_nodo;
					 aux=1;

				 	 }
			 }

			 printf("\ncontador %i ,id %i , ip %s , puerto %s \n",contadorNodo.contador,contadorNodo.idNodo,moverArchivos.ipNodo,moverArchivos.puertoNodo);

				// le mando a mover los archivos al nodo


			 // ahora hay que decirle al nodo que los mueva

			 int tamanioSerializacionMover;
			 tamanioSerializacionMover = sizeof(char) * 3;
			 handshakeJob = 34;
			 send(socketJob, &handshakeJob, sizeof(int),0);
			 recv(socketJob, &enteroPrueba, sizeof(int),0);
			 handshakeJob = 33;
			 send(socketJob, &handshakeJob, sizeof(int),0);
			 recv(socketJob, &enteroPrueba, sizeof(int),0);
			 tamanio = list_size(moverArchivos.archivosAMover);
			 send(socketJob, &tamanio, sizeof(int),0);
			 recv(socketJob, &enteroPrueba, sizeof(int),0);
			 for(a=0;a < tamanio;a++){
				 archivoAMover.ipNodo =moverArchivos.ipNodo;
				 archivoAMover.puertoNodo = moverArchivos.puertoNodo;
				 archivo = list_get(moverArchivos.archivosAMover, a);
				 archivoAMover.archivoAMover = archivo;
				 archivoASerializar = serializar_archivoAMover(&archivoAMover,tamanioSerializacionMover);
				 send(socketJob, &tamanioSerializacionMover, sizeof(int),0);
				 recv(socketJob, &enteroPrueba, sizeof(int),0);
				 send(socketJob,archivoASerializar,tamanioSerializacionMover,0);
				 recv(socketJob,&enteroPrueba,sizeof(int),0);
			 }


			 // ahora hay que decirle al nodo que haga el reduce final sobre los archivos mapeados

			 handshakeJob = 34;

			 send(socketJob, &handshakeJob, sizeof(int),0);
			 recv(socketJob, &enteroPrueba, sizeof(int),0);

			 antesDeSerializar.archivoResultadoReduce = archivoFinal;
			 antesDeSerializar.cantidadArchivos = list_size(moverArchivos.archivosAMover);
			 antesDeSerializar.idNodo = contadorNodo.idNodo;
			 antesDeSerializar.ipNodo = moverArchivos.ipNodo;
			 antesDeSerializar.puertoNodo = moverArchivos.puertoNodo;
			 list_add_all(antesDeSerializar.listaArchivosTemporales, moverArchivos.archivosAMover);

			 for(m=0; m< list_size(moverArchivos.archivosAMover); m++){
				 archivoA =	list_get( moverArchivos.archivosAMover,m);
				 tamanioListaFinal += strlen(archivoA);
				 tamanioListaFinal += 1;
			 }

			 tamanioStruct = (sizeof(int)*2)+strlen(antesDeSerializar.archivoResultadoReduce)+1 + strlen(antesDeSerializar.puertoNodo)+1 + strlen(antesDeSerializar.ipNodo) +1 + tamanioListaFinal;
			 send(socketJob, &tamanioStruct , sizeof(int),0);
			 recv(socketJob, &enteroPrueba, sizeof(int),0);
			 structParaJob = serializar_marta_job_reduce(&antesDeSerializar, tamanioStruct);
			 send(socketJob, structParaJob, tamanioStruct, 0);

			 send(socketJob, &tamanioStruct , sizeof(int),0);
			 recv(socketJob, &enteroPrueba, sizeof(int),0);
			 structParaJob = serializar_marta_job_reduce(&antesDeSerializar, tamanioStruct);
			 send(socketJob, structParaJob, tamanioStruct, 0);

			 //RECIBIMOS EL RESULTADO
			 recv(socketJob, &tamanioArecibir2, sizeof(int),0);
			 send(socketJob, &enteroPrueba, sizeof(int),0);
			 int estado2 = 1;
			 estado2 = recive_y_deserializa_job_marta(&job_marta2, socketJob, tamanioArecibir2);
			 if(estado2){
				 if(job_marta2.resultado){
					 printf("Termino de forma exitosa el reduce final\n");
				 }else{
					 printf("Fallo el reduce final\n");
				 }
			 }

			 for(a=0;a<list_size(lista_archivosAReducirPorNodo);a++){
				 campoArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,a);
				 free(campoArchivosAReducirPorNodo);
			 }

}








*/
#include "funciones.h"

char* serializar_marta_job_map(t_marta_job_map *bloque, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;


			int tamanioNombre = strlen(bloque->ip_nodo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->ip_nodo) + 1;
			memcpy(serializedPackage + offset, bloque->ip_nodo, size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->puerto) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->puerto) + 1;
			memcpy(serializedPackage + offset, bloque->puerto, size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->nombre_archivo_resultado) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->nombre_archivo_resultado) + 1;
			memcpy(serializedPackage + offset, bloque->nombre_archivo_resultado, size_to_send);
			offset += size_to_send;

			size_to_send =  sizeof(bloque->idNodo);
			memcpy(serializedPackage + offset, &(bloque->idNodo), size_to_send);
			offset += size_to_send;

			size_to_send =  sizeof(bloque->numeroBloque);
			memcpy(serializedPackage + offset, &(bloque->numeroBloque), size_to_send);
			offset += size_to_send;




			return serializedPackage;
		}


int main(void){
    int tipoOperacion,enteroPrueba;
    int socketJob;
    socketJob = crearServidor("3000");
    tipoOperacion = 30;
    send(socketJob,&tipoOperacion,sizeof(int),0);
    recv(socketJob, &enteroPrueba, sizeof(int),0);
    t_marta_job_map structPrueba;
    structPrueba.ip_nodo = "192.168.1.9";
    structPrueba.puerto = "6000";
    structPrueba.idNodo = 3;
    structPrueba.numeroBloque = 5;
    structPrueba.nombre_archivo_resultado = "pepitoloco";
    int tamanioStructPrueba = (sizeof(int)*5)+ strlen(structPrueba.ip_nodo) +strlen(structPrueba.puerto) + strlen(structPrueba.nombre_archivo_resultado) + 3;
    send(socketJob,&tamanioStructPrueba,sizeof(int),0);
    recv(socketJob, &enteroPrueba, sizeof(int),0);
    char* structAEnviar =  serializar_marta_job_map(&structPrueba, tamanioStructPrueba);
    send(socketJob,structAEnviar,tamanioStructPrueba,0);
    return EXIT_SUCCESS;
}
