/*
 ============================================================================
 Name        : Job.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "librerias_y_estructuras.h"

int main(void) {

	char* rutaArchivoConfiguracion;
	t_config* archivoConfiguracion;
	rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	contadorHilos=0;
	int socketMarta;
	char *mapper;
	char *reduce;
	char *textCombiner;
	int combiner,x, a, cantidadDeArchivos;
	int entero;
	char *lista_archivos;
	char **nombreArchivos;
	char *archivo_resultado;
	int tamanioPaquete;
	char *paquete; //paquete para enviar y recibir cosas
	char *paqueteDeseliariza;
	int offset;
	int size_to_send;
	int tamanioTotal, tamanioDinamico;
	char *nombreArchivoTemporal;
	int tamanioTotalDeLosArchivosReduce;

	t_aplicarMapper *aplicarMapper;
	t_aplicarReduce *aplicarReduce;
	t_moverArchivo *moverArchivo;

	pthread_t hiloMapper[1000];
	pthread_t hiloReduce[1000];
	pthread_t hiloMoverArch[1000];
	int contM = 1; // Contador para hilos de Mapper
	int contR = 1; // Contador para hilos de Reduce
	int contMA = 1; // Contador para hilos de moverArchivo

	puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
	ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
	mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
	reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
	textCombiner = config_get_string_value(archivoConfiguracion, "COMBINER");
	if(string_equals_ignore_case(textCombiner,"SI")){
		combiner = 1;
	}else{
		combiner = 0;
	}
	lista_archivos = config_get_string_value(archivoConfiguracion, "ARCHIVOS");
	nombreArchivos = string_get_string_as_array(lista_archivos);
	archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");

	logger = log_create("LOG_JOB", "log_job" ,false, LOG_LEVEL_INFO);

//	socketMarta = crearCliente (ip_marta,puerto_marta);

	rutinaMapper = mapearAMemoriaVirtual(mapper);
	rutinaReduce = mapearAMemoriaVirtual(reduce);
/*
	entero = 1;
	send(socketMarta, &entero, sizeof(int),0); // Handshake
	if(recv(socketMarta, &entero, sizeof(int),0)<0) return EXIT_SUCCESS; // basura

	// paquete a enviar a Marta [combiner][tamanioArchivoResultado][nombreResultado][cantidadDeArchivos]([tamanio Archivo1][nombre archivo1])*
	// * tamanio de Archivo y nombre de archivo son ciclicos depediendo de la cantidad de archivos

	offset = 0;

	cantidadDeArchivos = sizeof(nombreArchivos);

	tamanioTotal = sizeof(int) + sizeof(int) + strlen(archivo_resultado) + 1 + sizeof(int) + (sizeof(int) + strlen(lista_archivos) + 1) * cantidadDeArchivos;

	send(socketMarta,&tamanioTotal, sizeof(int),0); // envio tamanio Total del paquete
	if(recv(socketMarta, &entero, sizeof(int),0)<0) return EXIT_SUCCESS; // basura

	paquete = malloc(tamanioTotal);

	size_to_send =  sizeof(combiner);
	memcpy(paquete + offset, &(combiner), size_to_send);
	offset += size_to_send;

	tamanioDinamico = strlen(archivo_resultado) + 1;
	size_to_send = sizeof(int);
	memcpy(paquete + offset, &tamanioDinamico, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivo_resultado) + 1;
	memcpy(paquete + offset, archivo_resultado, size_to_send);
	offset += size_to_send;

	size_to_send =  sizeof(cantidadDeArchivos);
	memcpy(paquete + offset, &(cantidadDeArchivos), size_to_send);
	offset += size_to_send;

	for(a=0;a<cantidadDeArchivos;a++){
		tamanioDinamico = strlen(nombreArchivos[a]) + 1;
		size_to_send = sizeof(int);
		memcpy(paquete + offset, &tamanioDinamico, size_to_send);
		offset += size_to_send;

		size_to_send =  strlen(nombreArchivos[a]) + 1;
		memcpy(paquete + offset, nombreArchivos[a], size_to_send);
		offset += size_to_send;
	}

	send(socketMarta, paquete, tamanioTotal, 0);

	free(paquete);
*/
	// Espero indicaciones de Marta

	a=0;
	entero = 30;

	x = 0;
	while (a!=26){
//	if(recv(socketMarta, &entero, sizeof(int),0)<0) return EXIT_SUCCESS;
		switch(entero){
			case 30: // Aplicar Map
				aplicarMapper = malloc(sizeof(t_aplicarMapper));
/*
				send(socketMarta, &entero, sizeof(int),0); // basura
				if(recv(socketMarta, &tamanioPaquete, sizeof(int),0)<0) return EXIT_SUCCESS; // recibo el tamanio del paquete

				paqueteDeseliariza = malloc(tamanioPaquete);
				offset = 0;

				send(socketMarta, &entero, sizeof(int),0); // basura
				if(recv(socketMarta, paqueteDeseliariza, tamanioPaquete,0)<0) return EXIT_SUCCESS; // recibo el paquete
				send(socketMarta, &entero, sizeof(int),0); // basura

				// paquete de Marta [tamanioIP][IP][tamanioPuerto][PUERTO][tamanioResultado][Resultado][IdProceso][Bloque][Id Job]

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				aplicarMapper->IP = malloc(tamanioDinamico);
				memcpy(aplicarMapper->IP, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				aplicarMapper->PUERTO = malloc(tamanioDinamico);
				memcpy(aplicarMapper->PUERTO, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				aplicarMapper->resultado = malloc(tamanioDinamico);
				memcpy(aplicarMapper->resultado, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&aplicarMapper->id_proceso, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				memcpy(&aplicarMapper->bloque, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				memcpy(&aplicarMapper->id_job, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				free(paqueteDeseliariza);
*/
				aplicarMapper->IP="192.168.1.117";
				aplicarMapper->PUERTO="6000";
				aplicarMapper->bloque = a;
				aplicarMapper->resultado = malloc(strlen("resultadoMap")+string_itoa(a));
				asprintf(&aplicarMapper->resultado,"%s%s","resultadoMap",string_itoa(a));
				a++;
				pthread_create(&hiloMapper[contM], NULL, (void *)aplicarMapperF, (void *)aplicarMapper); // Hilo Mapper
				contM++;
				sleep(2);
				break;

			case 34: // Aplicar Reduce

				aplicarReduce = malloc(sizeof(t_aplicarReduce));

				send(socketMarta, &entero, sizeof(int),0); // basura
				if(recv(socketMarta, &tamanioPaquete, sizeof(int),0)<0) return EXIT_SUCCESS; // recibo el tamanio del paquete

				paqueteDeseliariza = malloc(tamanioPaquete);
				offset = 0;

				send(socketMarta, &entero, sizeof(int),0); // basura
				if(recv(socketMarta, paqueteDeseliariza, tamanioPaquete,0)<0) return EXIT_SUCCESS; // recibo el paquete
				send(socketMarta, &entero, sizeof(int),0); // basura

				// paquete de Marta [tamanioIP][IP][tamanioPuerto][PUERTO][tamanioResultado][Resultado][IdProceso][Id Job][cantidadArchivos]([tamanioArchivo][nombreArchivo])*
				// * cantidad de Archivos es la cantidad de archivos a aplicar el reduce

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				aplicarReduce->IP = malloc(tamanioDinamico);
				memcpy(aplicarReduce->IP, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				aplicarReduce->PUERTO = malloc(tamanioDinamico);
				memcpy(aplicarReduce->PUERTO, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				aplicarReduce->resultado = malloc(tamanioDinamico);
				memcpy(aplicarReduce->resultado, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&aplicarReduce->id_proceso, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				memcpy(&aplicarReduce->id_job, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				memcpy(&aplicarReduce->cantidadArchivos, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				// REVISAR COMO MANDAR

				//char *arrayDeArchivos[aplicarReduce->cantidadArchivos];
				/*
				tamanioTotalDeLosArchivosReduce = 0;

				for(a=0;a<aplicarReduce->cantidadArchivos;a++){
					memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
					offset += sizeof(int);
					nombreArchivoTemporal = malloc(tamanioDinamico);
					memcpy(nombreArchivoTemporal, paqueteDeseliariza + offset, tamanioDinamico);
					offset += tamanioDinamico;
					arrayDeArchivos[a] = nombreArchivoTemporal;

					tamanioTotalDeLosArchivosReduce += tamanioDinamico;
				}

				aplicarReduce->listaArchivos = malloc(tamanioTotalDeLosArchivosReduce);
				offset = 0;

				for(a=0;a<aplicarReduce->cantidadArchivos;a++){
					tamanioDinamico = strlen(arrayDeArchivos[a]) + 1;
					size_to_send = sizeof(int);
					memcpy(aplicarReduce->listaArchivos + offset, &tamanioDinamico, size_to_send);
					offset += size_to_send;

					size_to_send =  strlen(arrayDeArchivos[a]) + 1;
					memcpy(aplicarReduce->listaArchivos + offset, arrayDeArchivos[a], size_to_send);
					offset += size_to_send;
				}
*/
				free(paqueteDeseliariza);

				pthread_create(&hiloReduce[contR], NULL, (void *)aplicarReduceF, (void *)aplicarReduce);
				contR++;
				break;
			case 33: //Mover
				moverArchivo = malloc(sizeof(t_moverArchivo));
				pthread_create(&hiloMoverArch[contMA], NULL, (void *)moverArchivoF, (void *)moverArchivo);
				contMA++;
				break;
			case 90: // Fin de ejecucion del Job
				printf("Se termino de ejecutar el Job\n");
				x = 1;
				break;
			}
	}
	sleep(5);
	return EXIT_SUCCESS;
}

void *aplicarMapperF (t_aplicarMapper *aplicarMapper){

	int socketNodo,entero,tamanioTotal,offset,size_to_send,tamanioDinamico,estado;
	char *paquete;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// INICIO INTERFAZ NODO /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	socketNodo = crearCliente(aplicarMapper->IP,aplicarMapper->PUERTO);

	entero = 8;
	send(socketNodo, &entero, sizeof(int),0); //Handshake
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		// estados Mapper: 1 fin y 2 error.
		estado = 2;
		pthread_mutex_lock(&mutex);
		respuestaAMarta(42,aplicarMapper->id_proceso,aplicarMapper->id_job,estado);
		pthread_mutex_unlock(&mutex);
		return NULL;
	}

	entero = 1;
	send(socketNodo, &entero, sizeof(int),0); // Handshake de mapper
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		// estados Mapper: 1 fin y 2 error.
		estado = 2;
		pthread_mutex_lock(&mutex);
		respuestaAMarta(42,aplicarMapper->id_proceso,aplicarMapper->id_job,estado);
		pthread_mutex_unlock(&mutex);
		return NULL;
	}

	// paquete para aplicar Mapper [bloque][tamanioScript][script][tamanioResultado][Resultado]

	tamanioTotal = sizeof(int) + sizeof(int) + strlen(rutinaMapper) + 1 + sizeof(int) + strlen(aplicarMapper->resultado) + 1;

	send(socketNodo, &tamanioTotal, sizeof(int),0); // tamanioTotal del paquete
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		// estados Mapper: 1 fin y 2 error.
		estado = 2;
		pthread_mutex_lock(&mutex);
		respuestaAMarta(42,aplicarMapper->id_proceso,aplicarMapper->id_job,estado);
		pthread_mutex_unlock(&mutex);
		return NULL;
	}

	paquete = malloc(tamanioTotal);

	offset = 0;

	size_to_send =  sizeof(aplicarMapper->bloque);
	memcpy(paquete + offset, &(aplicarMapper->bloque), size_to_send);
	offset += size_to_send;

	tamanioDinamico = strlen(rutinaMapper) + 1;
	size_to_send = sizeof(int);
	memcpy(paquete + offset, &tamanioDinamico, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(rutinaMapper) + 1;
	memcpy(paquete + offset, rutinaMapper, size_to_send);
	offset += size_to_send;

	tamanioDinamico = strlen(aplicarMapper->resultado) + 1;
	size_to_send = sizeof(int);
	memcpy(paquete + offset, &tamanioDinamico, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(aplicarMapper->resultado) + 1;
	memcpy(paquete + offset, aplicarMapper->resultado, size_to_send);
	offset += size_to_send;

	send(socketNodo,  paquete, tamanioTotal, 0);

	free(paquete);

	if(recv(socketNodo, &entero, sizeof(int), 0)<0){   // Espero respuesta del Nodo
		// estados Mapper: 1 fin y 2 error.
		estado = 2;
		pthread_mutex_lock(&mutex);
		respuestaAMarta(42,aplicarMapper->id_proceso,aplicarMapper->id_job,estado);
		pthread_mutex_unlock(&mutex);
		return NULL;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// FIN INTERFAZ NODO /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// estados Mapper: 1 fin y 2 error.
	estado = 1;
	pthread_mutex_lock(&mutex);
	respuestaAMarta(42,aplicarMapper->id_proceso,aplicarMapper->id_job,estado);
	pthread_mutex_unlock(&mutex);

	free(aplicarMapper);

	return NULL;
}

void *aplicarReduceF (t_aplicarReduce *aplicarReduce){

	int socketNodo,entero,tamanioTotal, size_to_send, offset, tamanioDinamico, estado, socketMarta,a;
	char *paquete;
	char *nombreArchivoTemporal;

	offset = 0;
	char *arrayDeArchivos[aplicarReduce->cantidadArchivos];

	for(a=0;a<aplicarReduce->cantidadArchivos;a++){
		memcpy(&tamanioDinamico, aplicarReduce->listaArchivos + offset, sizeof(int));
		offset += sizeof(int);
		nombreArchivoTemporal = malloc(tamanioDinamico);
		memcpy(nombreArchivoTemporal, aplicarReduce->listaArchivos + offset, tamanioDinamico);
		offset += tamanioDinamico;
		arrayDeArchivos[a] = nombreArchivoTemporal;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// INICIO INTERFAZ NODO /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	socketNodo = crearCliente(aplicarReduce->IP,aplicarReduce->PUERTO);

	entero = 8;
	send(socketNodo, &entero, sizeof(int),0); //Handshake
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		// estados Reduce: 1 fin y 2 error.
		estado = 2;
		pthread_mutex_lock(&mutex_reduce);
		respuestaAMarta(25,aplicarReduce->id_proceso,aplicarReduce->id_job,estado);
		pthread_mutex_unlock(&mutex_reduce);
		return NULL;
	}

	entero = 2;
	send(socketNodo, &entero, sizeof(int),0); // Handshake de reduce
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		// estados Reduce: 1 fin y 2 error.
		estado = 2;
		pthread_mutex_lock(&mutex_reduce);
		respuestaAMarta(25,aplicarReduce->id_proceso,aplicarReduce->id_job,estado);
		pthread_mutex_unlock(&mutex_reduce);
		return NULL;
	}

	// paquete para aplicar Reduce



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// FIN INTERFAZ NODO /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// estados Reduce: 1 fin y 2 error.
	estado = 1;
	pthread_mutex_lock(&mutex_reduce);
	respuestaAMarta(25,aplicarReduce->id_proceso,aplicarReduce->id_job,estado);
	pthread_mutex_unlock(&mutex_reduce);
	return NULL;
}

void *moverArchivoF (t_moverArchivo *moverArchivo){
	return NULL;
}

void respuestaAMarta(int tipo,int idProceso, int idJob, int estado){
/*
	int socketNodo,entero,tamanioTotal, size_to_send, offset, tamanioDinamico, socketMarta;
	char *paquete;

	socketMarta = crearCliente(ip_marta,puerto_marta);

	// paquete a enviar Marta [estado][id proceso][id job]

	send(socketMarta,&tipo,sizeof(int),0); // Handshake
	if(recv(socketMarta,&entero,sizeof(int),0)<0) return ; // basura

	tamanioTotal = sizeof(int)*3;
	offset = 0;

	send(socketMarta,&tamanioTotal, sizeof(int),0); // envio tamanio Total del paquete
	if(recv(socketMarta, &entero, sizeof(int),0)<0) return ; // basura

	paquete = malloc(tamanioTotal);

	size_to_send =  sizeof(estado);
	memcpy(paquete + offset, &(estado), size_to_send);
	offset += size_to_send;

	size_to_send =  sizeof(idProceso);
	memcpy(paquete + offset, &(idProceso), size_to_send);
	offset += size_to_send;

	size_to_send =  sizeof(idJob);
	memcpy(paquete + offset, &(idJob), size_to_send);
	offset += size_to_send;

	send(socketMarta, paquete, tamanioTotal, 0); // envio paquete*/
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


