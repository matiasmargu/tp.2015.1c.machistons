/*
 * planificar_reduce.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"


char* serializar_moverParaReduceFinal(t_para_job* archivoAMover,int tamanioSerializacionMover){
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




void planificarReduceConCombiner(int socketJob, int idJob){


 int a,b,c,d,e=0,auxiliar = 0,aux,variableParaResultadoReduce=0,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
 int tamanioListaDeNodos,tamanio,tamanioArchivosaReducir,contador,tamanioTotalLista,tamanioStruct,tamanioCampoParaNodo;
 t_archivosAReducirPorNodo *campoDeUnNodo = malloc(sizeof(t_archivosAReducirPorNodo));
 t_archivosAReducirPorNodo * campoParaLiberar = malloc(sizeof(t_archivosAReducirPorNodo));
 t_marta_job_reduce* structAserializar = malloc(sizeof(t_marta_job_reduce));
 int handshakeJob, enteroPrueba,tamanioAenviar;
 char* archivo1;
 char* structAEnviarAJob;
 t_infoJob *campoDeLaLista = malloc(sizeof(t_infoJob));

 t_tablaProcesos_porJob*campoDeLaListaTablaDeProcesos = malloc(sizeof(t_tablaProcesos_porJob));
 t_list * lista_archivosAReducirPorNodo;
 t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo =  malloc(sizeof(t_archivosAReducirPorNodo));
 t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo = malloc(sizeof(t_archivosAReducirPorNodo));
 t_nodo *campoDeListaDeNodo = malloc(sizeof(t_nodo));


 lista_archivosAReducirPorNodo = list_create();
 //REDUCE FINAL
 t_job_marta_reduce* job_marta = malloc(sizeof(t_job_marta_reduce));

 int contador2,r,l,tamanioArecibir,p, v,tamanioListaFinal,m, reduceFinal,tamanioListaFinalAEnviar;
 t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo = malloc(sizeof(t_archivosAReducirPorNodo));
 t_list* listaFinalDeArchivosAReducir;
 t_marta_job_reduce* antesDeSerializar = malloc(sizeof(t_marta_job_reduce));
 char* structParaJob;
 contador2 = 0;
 t_para_nodo* campoListaParaNodo = malloc(sizeof(t_para_nodo));
 t_para_nodo* archivoA = malloc(sizeof(t_para_nodo));
 t_para_nodo* campoAAgregar = malloc(sizeof(t_para_nodo));
 t_para_job * archivoParaJob = malloc(sizeof(t_para_job));
 // lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
 //reducir

 tamanioListaJobTabla=list_size(lista_jobs);

 printf("\n\nLos campos de la lista que deberian estar son:\n");

 aux=0;
 //BUSCAMOS LOS DATOS DEL JOB EN LA LISTA DE JOBS
 for(a=0;a<tamanioListaJobTabla && aux==0;a++){
	 campoDeLaLista=list_get(lista_jobs,a);
	 if(campoDeLaLista->id_job == idJob){
		 aux=1;
	 }
 }


 tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->archivos_job);

 for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
	 campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->archivos_job,b);
	 tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
	 if(tamanioListaArchivosAReducirPorNodo == 0){

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





	 tamanio = list_size(lista_archivosAReducirPorNodo);
	 printf("\n\nEl tamanio final de la lista es: %i\n",tamanio);

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

	for(a=0; a<tamanio; a++){
		campoDeUnNodo=list_get(lista_archivosAReducirPorNodo,a);
		send(socketJob, &handshakeJob, sizeof(int),0);
		recv(socketJob, &enteroPrueba, sizeof(int),0);
		tamanioArchivosaReducir = list_size(campoDeUnNodo->archivosAReducir);
	for(b=0; b< tamanioArchivosaReducir; b++){
		archivo1 =	list_get(campoDeUnNodo->archivosAReducir,b);
		tamanioTotalLista += strlen(archivo1);
		tamanioTotalLista += 1;
		}
	tamanioAenviar = (sizeof(int)*2)+ strlen(campoDeUnNodo->ipNodo)+1 +  strlen(campoDeUnNodo->puertoNodo)+1 +  strlen(campoDeUnNodo->nombreArchivoResultado)+1 + tamanioTotalLista;
	send(socketJob, &tamanioAenviar, sizeof(int), 0);
	recv(socketJob, &enteroPrueba, sizeof(int),0);
	structAserializar->archivoResultadoReduce = campoDeUnNodo->nombreArchivoResultado;
	structAserializar->cantidadArchivos = list_size(campoDeUnNodo->archivosAReducir);
	structAserializar->idNodo = campoDeUnNodo->idNodo;
	structAserializar->ipNodo = campoDeUnNodo->ipNodo;
	structAserializar->puertoNodo = campoDeUnNodo->puertoNodo;
	list_add_all(structAserializar->listaArchivosTemporales, campoDeUnNodo->archivosAReducir);
	structAEnviarAJob = serializar_marta_job_reduce(structAserializar, tamanioAenviar);
	send(socketJob, structAEnviarAJob, tamanioAenviar, 0);
	contador += 1;
		}

	//ACA EMPIEZA REDUCE FINAL CON COMBINER

	 listaFinalDeArchivosAReducir = list_create();
	 for(a=0;a< contador ; a++){
		 recv(socketJob, &tamanioArecibir, sizeof(int),0);
		 send(socketJob, &enteroPrueba, sizeof(int),0);
		 int estado = 1;
		 estado = recive_y_deserializa_job_marta(job_marta, socketJob, tamanioArecibir);//LO HICE ARRIBA
		//AGREGO EL ESTADO A LA LISTA
		 if(estado){
			 for(r=0; r< list_size(lista_archivosAReducirPorNodo);r++){
				 campoDeListaArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,r);
				 if(campoDeListaArchivosAReducirPorNodo->idNodo == job_marta->idNodo){
					 campoDeListaArchivosAReducirPorNodo->estado = job_marta->resultado;

					//ME FIJO SI ESTAN TODOS LOS ARCHIVOS REDUCIDOS

	for(p=0;(p< list_size(lista_archivosAReducirPorNodo)) & (auxiliar == 0); p++){
		campoDeListaArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,p);
		if(campoDeListaArchivosAReducirPorNodo->estado == 1){
			contador2 += 1;
		}else{auxiliar = 1;}
	}
	if(contador == contador2){ //SI LA CANTIDAD DE REDUCE QUE MANDE A REDUCIR ES IGUAL A LA CANTIDAD REALIZADOS CON EXITO
		for(l=0;l< list_size(lista_archivosAReducirPorNodo); l++){ //GUARDO EN LISTAFINALAREDUCIR LOS DATOS DE CADA ARCHIVO REDUCIDO
			campoDeListaArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,l);
			tamanioCampoParaNodo = 3 + strlen(campoDeListaArchivosAReducirPorNodo->nombreArchivoResultado) + strlen(campoDeListaArchivosAReducirPorNodo->puertoNodo) + strlen(campoDeListaArchivosAReducirPorNodo->ipNodo);

			campoListaParaNodo->archivo = campoDeListaArchivosAReducirPorNodo->nombreArchivoResultado;
			campoListaParaNodo->ip = campoDeListaArchivosAReducirPorNodo->ipNodo;
			campoListaParaNodo->puerto = campoDeListaArchivosAReducirPorNodo->puertoNodo;

			list_add(listaFinalDeArchivosAReducir,campoListaParaNodo);
		}





		//ELEGIMOS UN NODO AL AZAR PARA HACER EL REDUCE FINAL
		campoDeListaArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,1);
		archivoParaJob->ipAConectarse =campoDeListaArchivosAReducirPorNodo->ipNodo ;
		archivoParaJob->puertoAconectarse = campoDeListaArchivosAReducirPorNodo->puertoNodo;

		// le mando a mover los archivos al nodo
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
			structParaJob =  serializar_moverParaReduceFinal(archivoParaJob, tamanioListaFinal);
			send(socketJob, structParaJob, tamanio, 0);
			//FALTA VER SI SE CAE EL NODO
		}


		//hacer reduce final de todos los archivos de la listafinal

		send(socketJob, &handshakeJob, sizeof(int),0);
		recv(socketJob, &enteroPrueba, sizeof(int),0);
		reduceFinal = 20;
		send(socketJob, &reduceFinal, sizeof(int),0);
		recv(socketJob, &enteroPrueba, sizeof(int),0);

		antesDeSerializar->archivoResultadoReduce = campoDeLaLista->arch_resultado_final;
		antesDeSerializar->cantidadArchivos = list_size(listaFinalDeArchivosAReducir);
		antesDeSerializar->idNodo = campoDeListaArchivosAReducirPorNodo->idNodo;
		antesDeSerializar->puertoNodo = archivoParaJob->puertoAconectarse;
		antesDeSerializar->ipNodo = archivoParaJob->ipAConectarse;

		//AGREGO CADA ARCHIVO A LA LISTA A ENVIAR A REDUCIR (PARA REDUCE FINAL) Y CALCULO EL TAMANIO
		for(v=0; v < list_size(listaFinalDeArchivosAReducir); v++){
			campoAAgregar = list_get(listaFinalDeArchivosAReducir, v);
			list_add(antesDeSerializar->listaArchivosTemporales,campoAAgregar->archivo );
			tamanioListaFinalAEnviar += strlen(campoAAgregar->archivo);
			tamanioListaFinalAEnviar += 1;
		}

		tamanioStruct = (sizeof(int)*2) + strlen(antesDeSerializar->ipNodo) +1 + strlen(antesDeSerializar->puertoNodo)+1+ strlen(antesDeSerializar->archivoResultadoReduce)+1 + tamanioListaFinalAEnviar;
		send(socketJob, &tamanioStruct , sizeof(int),0);
		recv(socketJob, &enteroPrueba, sizeof(int),0);
		structParaJob =  serializar_marta_job_reduce(antesDeSerializar, tamanioStruct);
		send(socketJob, structParaJob, tamanioStruct, 0);

		//RECIBIMOS EL RESULTADO
		recv(socketJob, &tamanioArecibir, sizeof(int),0);
		send(socketJob, &enteroPrueba, sizeof(int),0);
		int estado = 1;
		estado = recive_y_deserializa_job_marta(job_marta, socketJob, tamanioArecibir);
		if(estado){
			if(job_marta->resultado){
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
				}



	free(campoDeLaLista);
	free(campoDeLaListaTablaDeProcesos);
	free(campoArchivosAReducirPorNodo);
	free(campoDeListaDeNodo);
	free(campoAAgregarAListaReducirPorNodo);
	free(campoDeListaArchivosAReducirPorNodo);
	free(listaFinalDeArchivosAReducir);
	free(archivoA);
	free(campoParaLiberar);
	free(antesDeSerializar);
	free(campoDeUnNodo);
	free(archivoParaJob);
	free(job_marta);
	free(structAserializar);
	free(campoListaParaNodo);
	free(campoAAgregar);


	}


// A PARTIR DE ACA ES PARA EL SIN COMBINER

void planificarSincombiner(int idJob, int socketJob){

		// para probar
        int tamanioTabla,tamanioCampoDeTabla;
        tamanioTabla=sizeof(int);
        tamanioCampoDeTabla= sizeof(int) * 3 + sizeof(char);



				t_moverArchivos moverArchivos;
				t_serializarUnArchivoParaMover archivoAMover;
				int a,b,c,d,e=0,aux,variableParaResultadoReduce=0,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
				int tamanioListaDeNodos,tamanio;
				int handshakeJob, enteroPrueba,tamanioStruct;
			    t_infoJob *campoDeLaLista;
			    campoDeLaLista = malloc(tamanioTabla);
			    t_tablaProcesos_porJob *campoDeLaListaTablaDeProcesos;
		        campoDeLaListaTablaDeProcesos = malloc(tamanioCampoDeTabla);
			    t_list * lista_archivosAReducirPorNodo;
			    t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
		        campoArchivosAReducirPorNodo= malloc(tamanioCampoDeTabla);
			    t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
			    t_nodo *campoDeListaDeNodo = malloc(sizeof(t_nodo));

				lista_archivosAReducirPorNodo = list_create();
				list_clean(lista_archivosAReducirPorNodo);
				t_contadorNodo contadorNodo;
				//REDUCE FINAL
				t_job_marta_reduce job_marta2;
				int tamanioArecibir2, tamanioListaFinal,m;
				t_marta_job_reduce antesDeSerializar;
				char* structParaJob;
				char* archivo;
				char* archivoASerializar;
				t_para_nodo* archivoA;
				// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
				//reducir
				tamanioListaJobTabla=list_size(lista_jobs);

				printf("\n\nLos campos de la lista que deberian estar son:\n");
                aux=0;
				for(a=0;a<tamanioListaJobTabla && aux==0;a++){ // VER ESTO CON MATII PARA MI NO ES NECESARIO!!!!!!!!
					campoDeLaLista=list_get(lista_jobs,a);
					if(campoDeLaLista->id_job == idJob){
					aux=1;
					}
				}
				tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->archivos_job);

				for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
						campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->archivos_job,b);
						tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
				    	if(tamanioListaArchivosAReducirPorNodo == 0){
				    		tamanioCampoDeTabla= sizeof(int) * 2 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo) + 1 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo_resultado) + 1;
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
				    				tamanioCampoDeTabla= sizeof(int) * 2 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo) + 1 + strlen(campoDeLaListaTablaDeProcesos->nombre_archivo_resultado) + 1;
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

			 antesDeSerializar.archivoResultadoReduce = campoDeLaLista->arch_resultado_final;
			 antesDeSerializar.cantidadArchivos = list_size(moverArchivos.archivosAMover);
			 antesDeSerializar.idNodo = contadorNodo.idNodo;
			 antesDeSerializar.ipNodo = moverArchivos.ipNodo;
			 antesDeSerializar.puertoNodo = moverArchivos.puertoNodo;
			 list_add_all(antesDeSerializar.listaArchivosTemporales, moverArchivos.archivosAMover);

			 for(m=0; m< list_size(moverArchivos.archivosAMover); m++){
				 archivoA =	list_get( moverArchivos.archivosAMover,m);
				 tamanioListaFinal += strlen(archivoA->puerto);
				 tamanioListaFinal += strlen(archivoA->ip);
				 tamanioListaFinal += strlen(archivoA->archivo);
				 tamanioListaFinal += 3;
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


}











