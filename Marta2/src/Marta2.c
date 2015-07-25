/*
 ============================================================================
 Name        : Marta2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "funciones.h"

t_list *lista_nodos_estado;

typedef struct{
	int id_nodo;
	char *ip_nodo;
	char *puerto_nodo;
}t_nodo2;

typedef struct{
	int estado; // 0 = todavia no se mando a ejecutar; 1 = en ejecucion; 2 = Fin; 3 = Error
	int bloque_archivo;
	char *nombre_archivo_resultado;
	int id_nodo;
	char * nombreArchivo;
}t_tablaProcesos_porJob2;

typedef struct{
	int idJob;
	t_list *tabla_procesos;
}t_lista_job2;

typedef struct {
	int idNodo;
	char* puertoNodo;
	char * ipNodo;
	t_list* archivosAReducir;
    char* nombreArchivoResultado;
    char* archivo;
}t_archivosAReducirPorNodo;

typedef struct {
	int contador;
	int idNodo;
}t_contadorNodo;

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


int main(void){

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



/*


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


			        int a,b,c,d,e=0,aux,variableParaResultadoReduce,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
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
	           }*/
return EXIT_SUCCESS;

}








