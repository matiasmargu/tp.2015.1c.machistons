/*
 * planificar_reduce.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

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
			char* archivo;

			for(a=0;a< tamanioLista; a++){
			archivo = list_get(bloque->listaArchivosTemporales, a);
			tamanioNombre = strlen(archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(archivo) + 1;
			memcpy(serializedPackage + offset, archivo, size_to_send);
			offset += size_to_send;
					}




			return serializedPackage;
		}











// HAY QUE VER DE DONDE SACAMOS EL SOCKET JOB
      //Para mi tiene que venir como parametro cuando llaman a la funcion , ya que marta y job ya
	  // se van a estar comunicando por el mismo socket cuando le pasa la lista inicial el combiner y demas

void planificarReduceConCombiner(){

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
		campoDeTabla4->id_nodo = 54;
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


		        int a,b,c,d,e,aux,variableParaResultadoReduce,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
				int tamanioListaDeNodos,tamanio,tamanioArchivosaReducir;
				t_archivosAReducirPorNodo *campoDeUnNodo;
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

				// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
				//reducir
				tamanioListaJobTabla=list_size(lista_job_tabla);// es 3

				printf("\n\nLos campos de la lista que deberian estar son:\n");

				for(a=0;a<tamanioListaJobTabla;a++){
					campoDeLaLista=list_get(lista_job_tabla,a); // corrroborado que los saca bien
					tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos); // es dos

					for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
						campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
						tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
				    	if(tamanioListaArchivosAReducirPorNodo == 0){
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

	//REDUCE FINAL
	t_job_marta_reduce job_marta;
	int contador2,r,l,tamanioArecibir,p,tamanioStruct, tamanioListaFinal,m;
	 t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo;
	 t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo2;
	 t_archivosAReducirPorNodo * campoDeListaArchivosAReducirPorNodo3;
	 t_list* listaFinalDeArchivosAReducir;
	 t_marta_job_reduce antesDeSerializar;
	 char* structParaJob;
	 char* archivoA;
	 contador2 = 0;
	 listaFinalDeArchivosAReducir = list_create();
	for(a=0;a< contador ; a++){

		recv(socketJob, &tamanioArecibir, sizeof(int),0);
		send(socketJob, &enteroPrueba, sizeof(int),0);
		int estado = 1;
		estado = recive_y_deserializa_job_marta(&job_marta, socketJob, tamanioArecibir);//HACER LA FUNCION
		//AGREGO EL ESTADO A LA LISTA
		if(estado){
			for(r=0; r< list_size(lista_archivosAReducirPorNodo);r++){
				campoDeListaArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,r);
				if(campoDeListaArchivosAReducirPorNodo->idNodo == job_marta.idNodo){
					campoDeListaArchivosAReducirPorNodo->estado = job_marta.resultado;
					//ME FIJO SI ESTAN TODOS LOS ARCHIVOS REDUCIDOS

					for(p=0;p< list_size(lista_archivosAReducirPorNodo); p++){
						campoDeListaArchivosAReducirPorNodo2 = list_get(lista_archivosAReducirPorNodo,p);
						if(campoDeListaArchivosAReducirPorNodo2->estado == 1){
							contador2 += 1;
						}
					}

					if(contador == contador2){
						for(l=0;l< list_size(lista_archivosAReducirPorNodo); l++){
							campoDeListaArchivosAReducirPorNodo3 = list_get(lista_archivosAReducirPorNodo,p);
							list_add(listaFinalDeArchivosAReducir,campoDeListaArchivosAReducirPorNodo3->nombreArchivoResultado);
						}

						//MANDAMOS EL REDUCE FINAL
						send(socketJob, &handshakeJob, sizeof(int),0);
						recv(socketJob, &enteroPrueba, sizeof(int),0);

						antesDeSerializar.archivoResultadoReduce =  //aca va el qe pasa job al principio
						antesDeSerializar.cantidadArchivos = list_size(listaFinalDeArchivosAReducir);
						antesDeSerializar.idNodo = //ver
						antesDeSerializar.ipNodo //ver
						antesDeSerializar.puertoNodo // ver
						list_add_all(antesDeSerializar.listaArchivosTemporales, listaFinalDeArchivosAReducir);

						for(m=0; m< tamanioArchivosaReducir; m++){
							archivoA =	list_get( listaFinalDeArchivosAReducir,m);
							tamanioListaFinal += strlen(archivoA);
							tamanioListaFinal += 1;
						}

						tamanioStruct = (sizeof(int)*2)+strlen(antesDeSerializar.archivoResultadoReduce)+1 + strlen(antesDeSerializar.puertoNodo)+1 + strlen(antesDeSerializar.ipNodo) +1 + tamanioListaFinal;
						send(socketJob, &tamanioStruct , sizeof(int),0);
						recv(socketJob, &enteroPrueba, sizeof(int),0);
						structParaJob = serializar_marta_job_reduce(&antesDeSerializar, tamanioStruct);

						send(socketJob, structParaJob, tamanioStruct, 0);
					}
				}
			}


		}

	}




	free(campoDeLaLista);
	free(campoDeLaListaTablaDeProcesos);
	free(campoArchivosAReducirPorNodo);
	free(campoDeListaDeNodo);
	free(campoAAgregarAListaReducirPorNodo);
}

















