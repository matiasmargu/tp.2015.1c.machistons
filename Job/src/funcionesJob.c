/*
 * funcionesJob2.c
 *
 *  Created on: 9/7/2015
 *      Author: utnso
 */

#include "funcionesJob.h"


int recive_y_deserialisa_marta_job_reduce(t_marta_job_reduce *bloque, int socket, uint32_t tamanioTotal){
			int status;
			char *buffer = malloc(tamanioTotal);
			int offset=0;

			recv(socket, buffer, tamanioTotal, 0);

			memcpy(&(bloque->cantidadArchivos), buffer + offset, sizeof(bloque->cantidadArchivos));
			offset += sizeof(bloque->cantidadArchivos);

			int tamanioDinamico;
			memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
			offset += sizeof(int);

			bloque->ipNodo = malloc(tamanioDinamico);
			memcpy(bloque->ipNodo, buffer + offset, tamanioDinamico);
			offset += tamanioDinamico;

			memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
			offset += sizeof(int);

			bloque->puertoNodo = malloc(tamanioDinamico);
			memcpy(bloque->puertoNodo, buffer + offset, tamanioDinamico);
			offset += tamanioDinamico;

			memcpy(&(bloque->idNodo), buffer + offset, sizeof(bloque->idNodo));
			offset += sizeof(bloque->idNodo);

			memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
			offset += sizeof(int);

			bloque->archivoResultadoReduce = malloc(tamanioDinamico);
			memcpy(bloque->archivoResultadoReduce, buffer + offset, tamanioDinamico);
			offset += tamanioDinamico;


			int a;
			char* archivo;

			//deserializa la lista
			bloque->listaArchivosTemporales = list_create();

			for(a=0;a<  (bloque->cantidadArchivos) ; a++){
				memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
				offset += sizeof(int);
				archivo = malloc(tamanioDinamico);
		    	memcpy(archivo, buffer + offset, tamanioDinamico);
				offset += tamanioDinamico;
				printf("el archivooo %s \n\n",archivo);
				list_add(bloque->listaArchivosTemporales,archivo);
							}


			free(buffer);
			free(archivo);
			return status;
}

int recive_y_deserialisa_marta_job_reduce_final(t_marta_job_reduce *bloque, int socket, uint32_t tamanioTotal){
			int status;
			char *buffer = malloc(tamanioTotal);
			int offset=0;

			recv(socket, buffer, tamanioTotal, 0);

			memcpy(&(bloque->cantidadArchivos), buffer + offset, sizeof(bloque->cantidadArchivos));
			offset += sizeof(bloque->cantidadArchivos);

			int tamanioDinamico;
			memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
			offset += sizeof(int);

			bloque->ipNodo = malloc(tamanioDinamico);
			memcpy(bloque->ipNodo, buffer + offset, tamanioDinamico);
			offset += tamanioDinamico;

			memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
			offset += sizeof(int);

			bloque->puertoNodo = malloc(tamanioDinamico);
			memcpy(bloque->puertoNodo, buffer + offset, tamanioDinamico);
			offset += tamanioDinamico;

			memcpy(&(bloque->idNodo), buffer + offset, sizeof(bloque->idNodo));
			offset += sizeof(bloque->idNodo);

			memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
			offset += sizeof(int);

			bloque->archivoResultadoReduce = malloc(tamanioDinamico);
			memcpy(bloque->archivoResultadoReduce, buffer + offset, tamanioDinamico);
			offset += tamanioDinamico;


			int a;
			t_para_nodo* archivo;


			//deserializa la lista
			bloque->listaArchivosTemporales = list_create();

			for(a=0;a<  (bloque->cantidadArchivos) ; a++){
				memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
				offset += sizeof(int);
				archivo->archivo = malloc(tamanioDinamico);
		    	memcpy(archivo->archivo, buffer + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
				offset += sizeof(int);
				archivo->ip = malloc(tamanioDinamico);
				memcpy(archivo->ip, buffer + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
				offset += sizeof(int);
				archivo->puerto = malloc(tamanioDinamico);
		    	memcpy(archivo->puerto, buffer + offset, tamanioDinamico);
		    	offset += tamanioDinamico;
				list_add(bloque->listaArchivosTemporales,archivo);
							}


			free(buffer);
			free(archivo);
			return status;
}


int recive_y_deserialisa_marta_job_mapper(t_marta_job_map *bloque, int socket, uint32_t tamanioTotal){
		int status;
		char *buffer = malloc(tamanioTotal);
		int offset=0;

		recv(socket, buffer, tamanioTotal, 0);

		memcpy(&(bloque->cantidadBloques), buffer + offset, sizeof(bloque->cantidadBloques));
		offset += sizeof(bloque->cantidadBloques);

		int tamanioDinamico;
		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->ip_nodo = malloc(tamanioDinamico);
		memcpy(bloque->ip_nodo, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;

		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);



		bloque->puerto = malloc(tamanioDinamico);
		memcpy(bloque->puerto, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;


		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->nombre_archivo_resultado = malloc(tamanioDinamico);
		memcpy(bloque->nombre_archivo_resultado, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;


		memcpy(&(bloque->idNodo), buffer + offset, sizeof(bloque->idNodo));
		offset += sizeof(bloque->idNodo);


		//deserializa la lista
				bloque->bloques = list_create();
				int numero,a;
				for(a=0;a<  (bloque->cantidadBloques) ; a++){
					memcpy(&(numero), buffer + offset, sizeof(numero));
					offset += sizeof(numero);
					list_add(bloque->bloques,numero);
				}

		free(buffer);
		return status;

	}

char* mapearAMemoriaVirtual(char* archivo){
	char* pmap;
	int fd;
	struct stat mystat;

	fd = open(archivo,O_RDWR);
		if(fd == -1){
		printf("Error al leer el ARCHIBO_BIN\n");
		exit(1);
	}

	if(fstat(fd,&mystat) < 0){
		printf("Error al establecer fstat\n");
		close(fd);
		exit(1);
	}

	pmap = mmap(0,mystat.st_size, PROT_READ|PROT_WRITE ,MAP_SHARED,fd,0);
	if(pmap == MAP_FAILED){
		printf("Error al mapear a memoria\n");
		close(fd);
		exit(1);
	}
	return pmap;
}

char* serializar_job_nodo_reduce(t_job_nodo_reduce *Job_Nodo, int tamanio){

	char *serializedPackage = malloc(tamanio);
			int offset = 0;
			int size_to_send;
			int a,cantidadArchivosLista;
			char* archivo;

			int tamanioResultado = strlen(Job_Nodo->nombreArchivoResultado) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioResultado, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(Job_Nodo->nombreArchivoResultado) + 1;
			memcpy(serializedPackage + offset, Job_Nodo->nombreArchivoResultado, size_to_send);
			offset += size_to_send;


			Job_Nodo->cantidadArchivos = list_size(Job_Nodo->archivosAreducir);
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &Job_Nodo->cantidadArchivos, size_to_send);
			offset += size_to_send;

			//LISTA

			for(a=0;a< list_size(Job_Nodo->archivosAreducir); a++){
			archivo = list_get(Job_Nodo->archivosAreducir, a);
			size_to_send = sizeof(int);

			int tamanioResultado2 = strlen(archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioResultado2, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(archivo) + 1;
			memcpy(serializedPackage + offset, archivo, size_to_send);
			offset += size_to_send;

					}

			return serializedPackage;


}


char* serializar_job_nodo_reduce_final(t_job_nodo_reduce *Job_Nodo, int tamanio){

	char *serializedPackage = malloc(tamanio);
			int offset = 0;
			int size_to_send;
			int a,cantidadArchivosLista,tamanioNombre;


			int tamanioResultado = strlen(Job_Nodo->nombreArchivoResultado) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioResultado, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(Job_Nodo->nombreArchivoResultado) + 1;
			memcpy(serializedPackage + offset, Job_Nodo->nombreArchivoResultado, size_to_send);
			offset += size_to_send;


			Job_Nodo->cantidadArchivos = list_size(Job_Nodo->archivosAreducir);
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &Job_Nodo->cantidadArchivos, size_to_send);
			offset += size_to_send;

			//LISTA

						int tamanioLista = list_size(Job_Nodo->archivosAreducir);

						t_para_nodo *archivo;

						for(a=0;a< tamanioLista; a++){
						archivo = list_get(Job_Nodo->archivosAreducir, a);
						tamanioNombre = strlen(archivo->archivo) + 1;
						size_to_send = sizeof(int);
						memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
						offset += size_to_send;

						size_to_send =  strlen(archivo->archivo) + 1;
						memcpy(serializedPackage + offset, archivo->archivo, size_to_send);
						offset += size_to_send;

						tamanioNombre = strlen(archivo->ip) + 1;
						size_to_send = sizeof(int);
						memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
						offset += size_to_send;

						size_to_send =  strlen(archivo->ip) + 1;
						memcpy(serializedPackage + offset, archivo->ip, size_to_send);
						offset += size_to_send;

						tamanioNombre = strlen(archivo->puerto) + 1;
						size_to_send = sizeof(int);
						memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
						offset += size_to_send;

						size_to_send =  strlen(archivo->puerto) + 1;
						memcpy(serializedPackage + offset, archivo->puerto, size_to_send);
						offset += size_to_send;

					}

			return serializedPackage;


}




char *serializar_job_marta_reduce(t_job_marta_reduce *job_marta,int tamanioTotal){
	char *serializedPackage = malloc(tamanioTotal);
	int offset = 0;
	int size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->rutina, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->idNodo, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->resultado, size_to_send);
	offset += size_to_send;

	int tamanioResultado = strlen(job_marta->nombreArchivo) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioResultado, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(job_marta->nombreArchivo) + 1;
	memcpy(serializedPackage + offset, job_marta->nombreArchivo, size_to_send);
	offset += size_to_send;

	offset += size_to_send;
	return serializedPackage;
}


char* serializar_job_marta(t_job_marta_map *job_marta, int tamanioTotal){
	char *serializedPackage = malloc(tamanioTotal);
	int offset = 0;
	int size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->numeroBloque, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->resultado, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->rutina, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->idNodo, size_to_send);
	offset += size_to_send;

	int tamanioResultado = strlen(job_marta->nombreArchivo) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioResultado, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(job_marta->nombreArchivo) + 1;
	memcpy(serializedPackage + offset, job_marta->nombreArchivo, size_to_send);
	offset += size_to_send;

	offset += size_to_send;
	return serializedPackage;

}




void* mapearBloque(t_hilo_map *structRecibido){

	log_info(logger,"Se levanto un hilo para enviar los datos de cada bloque a mapear,los parametros recibidos son: numero bloque:%i, socketNodo:%i, nombre del archivo de resultado: %s, socketMarta %i, id del Nodo %i\n",structRecibido->bloque,structRecibido->socketNodo,structRecibido->nombreArchivoResultado,structRecibido->socketMarta,structRecibido->idNodo);
	printf("Se levanto un hilo para enviar los datos de cada bloque a mapear,los parametros recibidos son: numero bloque:%i, socketNodo:%i, nombre del archivo de resultado: %s, socketMarta %i, id del Nodo %i\n",structRecibido->bloque,structRecibido->socketNodo,structRecibido->nombreArchivoResultado,structRecibido->socketMarta,structRecibido->idNodo);


	int enteroPrueba, tamanioTotal, tamanio, tamanioArchivo,resultadoMap,tamanioMarta,handshakeResultadoMap=2;
	char* datosParaNodo;
	char* archivoSerializado;
	char* resultado;
	t_job_marta_map datosParaMarta;
	char* structParaMarta;
	//MANDAMOS AL NODO LOS DATOS
	printf("ARRIBA\n");
	printf("socket nodo %i\n",structRecibido->socketNodo);
	recv(structRecibido->socketNodo, &enteroPrueba, sizeof(int),0);
	printf("el numero de bloqe %i\n",structRecibido->bloque);
	send(structRecibido->socketNodo, &structRecibido->bloque, sizeof(int),0);
	recv(structRecibido->socketNodo, &enteroPrueba, sizeof(int),0);
	tamanioArchivo =  strlen(structRecibido->nombreArchivoResultado)+1;
	send(structRecibido->socketNodo, &tamanioArchivo, sizeof(int),0);
	recv(structRecibido->socketNodo, &enteroPrueba, sizeof(int),0);
	send(structRecibido->socketNodo,structRecibido->nombreArchivoResultado , tamanioArchivo,0);
	printf("ABAJO\n");
	//Aca Recibimos Resultado
	recv(structRecibido->socketNodo, &resultadoMap, sizeof(int),0);
	//ACA MANDAMOS A MARTA EL RESULTADO
	if(resultadoMap){
		resultado = "SE REALIZO EL MAP";
		datosParaMarta.idNodo = structRecibido->idNodo;
		datosParaMarta.nombreArchivo = structRecibido->nombreArchivoResultado;
		datosParaMarta.numeroBloque = structRecibido->bloque;
		datosParaMarta.resultado = resultadoMap;
		datosParaMarta.rutina = 1;
		tamanioMarta = (sizeof(int)*4) + strlen(datosParaMarta.nombreArchivo) +1;
		printf("aaa\n");
		send(structRecibido->socketMarta, &handshakeResultadoMap, sizeof(int),0);
		recv(structRecibido->socketMarta, &enteroPrueba, sizeof(int),0);
		send(structRecibido->socketMarta, &tamanioMarta, sizeof(int),0);
		printf("aaa\n");
		recv(structRecibido->socketMarta, &enteroPrueba, sizeof(int),0);
		structParaMarta = serializar_job_marta(&datosParaMarta, tamanioMarta);
		send(structRecibido->socketMarta, structParaMarta, tamanioMarta,0);
	}else{ resultado = "FALLO EL MAP";}
		log_info(logger,"Finalizo el hilo para mapear el bloque %i  en el nodo %i ,el resultado fue %s \n",structRecibido->bloque,structRecibido->idNodo,resultado);
		printf("Finalizo el hilo para mapear el bloque %i  en el nodo %i ,el resultado fue %s \n",structRecibido->bloque,structRecibido->idNodo,resultado);


	return NULL;

}


void* reducirArchivos(t_hilo_reduce *structRecibido){




	t_job_nodo_reduce Job_Nodo;
	t_job_marta_reduce datosParaMarta;
	int tamanioLista,a, tamanio,enteroPrueba,resultadoReduce, tamanioMarta;
	char* datosParaNodo;
	char* archivo;
	char* resultado;
	char* structParaMarta;
	char* unArchivo;
	char* listaDeArchivos;

	listaDeArchivos = "[";
		for(a=0; a< list_size(structRecibido->archivos); a++){
		unArchivo = list_get(structRecibido->archivos, a);
		 string_append(&listaDeArchivos, unArchivo);
		 string_append(&listaDeArchivos, ",");
		}
		string_append(&listaDeArchivos, "]");


		log_info(logger,"Se levanto un hilo para enviar los archivos a reducir,los parametros recibidos son: socketNodo:%i, nombre del archivo de resultado: %s, socketMarta %i, id del Nodo %i y la lista de archivos %s\n",structRecibido->socketNodo,structRecibido->nombreArchivoResultado,structRecibido->socketMarta,structRecibido->idNodo,listaDeArchivos);
		printf("Se levanto un hilo para enviar los archivos a reducir,los parametros recibidos son: socketNodo:%i, nombre del archivo de resultado: %s, socketMarta %i, id del Nodo %i y la lista de archivos %s\n",structRecibido->socketNodo,structRecibido->nombreArchivoResultado,structRecibido->socketMarta,structRecibido->idNodo,listaDeArchivos);







	Job_Nodo.nombreArchivoResultado = structRecibido->nombreArchivoResultado;
	Job_Nodo.archivosAreducir = list_create();
	list_add_all(Job_Nodo.archivosAreducir,structRecibido->archivos);
	for(a=0;a<(list_size(Job_Nodo.archivosAreducir));a++){
		archivo = list_get(Job_Nodo.archivosAreducir, a);
		tamanioLista += (strlen(archivo)+1);

	}
	tamanio =sizeof(int)+(strlen(Job_Nodo.nombreArchivoResultado)+1)+(tamanioLista);
	send(structRecibido->socketNodo, &tamanio, sizeof(int), 0);
	recv(structRecibido->socketNodo, &enteroPrueba , sizeof(int),0);
	datosParaNodo = serializar_job_nodo_reduce(&Job_Nodo, tamanio);
	send(structRecibido->socketNodo, datosParaNodo, tamanio, 0);

	//Aca Recibimos Resultado
	recv(structRecibido->socketNodo, &resultadoReduce, sizeof(int),0);
		//ACA MANDAMOS A MARTA EL RESULTADO
		if(resultadoReduce){
			resultado = "SE REALIZO EL REDUCE";
			datosParaMarta.idNodo = structRecibido->idNodo;
			datosParaMarta.nombreArchivo = structRecibido->nombreArchivoResultado;
			datosParaMarta.resultado = resultadoReduce;
			datosParaMarta.rutina = 1;
			tamanioMarta = (sizeof(int)*4) + strlen(datosParaMarta.nombreArchivo) +1;
			send(structRecibido->socketMarta, &tamanioMarta, sizeof(int),0);
			recv(structRecibido->socketMarta, &enteroPrueba, sizeof(int),0);
			structParaMarta = serializar_job_marta_reduce(&datosParaMarta, tamanioMarta);
			send(structRecibido->socketMarta, structParaMarta, tamanioMarta,0);

				}else{
		resultado = "NO SE PUDO REALIZAR EL REDUCE";}
		log_info(logger,"Finalizo el hilo para enviar los archivos a reducir en el nodo %i ,el resultado fue %s \n",structRecibido->idNodo,resultado);
		printf("Finalizo el hilo para enviar los archivos a reducir en el nodo %i,el resultado fue %s\n",structRecibido->idNodo,resultado);
				return NULL;
}


void* reducirArchivosFinal(t_hilo_reduce *structRecibido){




	t_job_nodo_reduce Job_Nodo;
	t_job_marta_reduce datosParaMarta;
	int tamanioLista,a, tamanio,enteroPrueba,resultadoReduce, tamanioMarta;
	char* datosParaNodo;
	char* archivo;
	char* resultado;
	char* structParaMarta;
	char* unArchivo;
	char* listaDeArchivos;

	listaDeArchivos = "[";
		for(a=0; a< list_size(structRecibido->archivos); a++){
		unArchivo = list_get(structRecibido->archivos, a);
		 string_append(&listaDeArchivos, unArchivo);
		 string_append(&listaDeArchivos, ",");
		}
		string_append(&listaDeArchivos, "]");


		log_info(logger,"Se levanto un hilo para enviar los archivos a reducir,los parametros recibidos son: socketNodo:%i, nombre del archivo de resultado: %s, socketMarta %i, id del Nodo %i y la lista de archivos %s\n",structRecibido->socketNodo,structRecibido->nombreArchivoResultado,structRecibido->socketMarta,structRecibido->idNodo,listaDeArchivos);
		printf("Se levanto un hilo para enviar los archivos a reducir,los parametros recibidos son: socketNodo:%i, nombre del archivo de resultado: %s, socketMarta %i, id del Nodo %i y la lista de archivos %s\n",structRecibido->socketNodo,structRecibido->nombreArchivoResultado,structRecibido->socketMarta,structRecibido->idNodo,listaDeArchivos);







	Job_Nodo.nombreArchivoResultado = structRecibido->nombreArchivoResultado;
	Job_Nodo.archivosAreducir = list_create();
	list_add_all(Job_Nodo.archivosAreducir,structRecibido->archivos);
	for(a=0;a<(list_size(Job_Nodo.archivosAreducir));a++){
		archivo = list_get(Job_Nodo.archivosAreducir, a);
		tamanioLista += (strlen(archivo)+1);

	}
	tamanio =sizeof(int)+(strlen(Job_Nodo.nombreArchivoResultado)+1)+(tamanioLista);
	send(structRecibido->socketNodo, &tamanio, sizeof(int), 0);
	recv(structRecibido->socketNodo, &enteroPrueba , sizeof(int),0);
	datosParaNodo = serializar_job_nodo_reduce_final(&Job_Nodo, tamanio);
	send(structRecibido->socketNodo, datosParaNodo, tamanio, 0);

	//Aca Recibimos Resultado
	recv(structRecibido->socketNodo, &resultadoReduce, sizeof(int),0);
		//ACA MANDAMOS A MARTA EL RESULTADO
		if(resultadoReduce){
			resultado = "SE REALIZO EL REDUCE";
			datosParaMarta.idNodo = structRecibido->idNodo;
			datosParaMarta.nombreArchivo = structRecibido->nombreArchivoResultado;
			datosParaMarta.resultado = resultadoReduce;
			datosParaMarta.rutina = 1;
			tamanioMarta = (sizeof(int)*4) + strlen(datosParaMarta.nombreArchivo) +1;
			send(structRecibido->socketMarta, &tamanioMarta, sizeof(int),0);
			recv(structRecibido->socketMarta, &enteroPrueba, sizeof(int),0);
			structParaMarta = serializar_job_marta_reduce(&datosParaMarta, tamanioMarta);
			send(structRecibido->socketMarta, structParaMarta, tamanioMarta,0);

				}else{
		resultado = "NO SE PUDO REALIZAR EL REDUCE";}
		log_info(logger,"Finalizo el hilo para enviar los archivos a reducir en el nodo %i ,el resultado fue %s \n",structRecibido->idNodo,resultado);
		printf("Finalizo el hilo para enviar los archivos a reducir en el nodo %i,el resultado fue %s\n",structRecibido->idNodo,resultado);
				return NULL;
}

char* serializar_archivoAMover(t_archivoParaMover * archivoAMover,int tamanioSerializacionMover){
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


