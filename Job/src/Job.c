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
	char *puerto_marta;
	char *ip_marta;
	char *mapper;
	char *reduce;
	char *textCombiner;
	int combiner,x,entero, a, cantidadDeArchivos;
	char *lista_archivos;
	char **nombreArchivos;
	char *archivo_resultado;
	char *paquete; //paquete para enviar y recibir cosas
	char *paqueteDeseliariza;
	int offset;
	int size_to_send;
	int tamanioTotal, tamanioDinamico;

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

	socketMarta = crearCliente (ip_marta,puerto_marta);

	rutinaMapper = mapearAMemoriaVirtual(mapper);
	rutinaReduce = mapearAMemoriaVirtual(reduce);

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

	// Espero indicaciones de Marta
	char* IP;

	x = 0;
	while (x!=1){
	if(recv(socketMarta, &entero, sizeof(int),0)<0) return EXIT_SUCCESS;
		switch(entero){
			case 30: // Aplicar Map
				aplicarMapper = malloc(sizeof(t_aplicarMapper));
				send(socketMarta, &entero, sizeof(int),0); // basura
				if(recv(socketMarta, &entero, sizeof(int),0)<0) return EXIT_SUCCESS; // recibo el tamanio del paquete

				printf("tamanio paquete: %i\n",entero);

				paqueteDeseliariza = malloc(entero);
				offset = 0;

				send(socketMarta, &entero, sizeof(int),0); // basura
				if(recv(socketMarta, paqueteDeseliariza, sizeof(int),0)<0) return EXIT_SUCCESS; // recibo el paquete
				send(socketMarta, &entero, sizeof(int),0); // basura

				// paquete de Marta [tamanioIP][IP][tamanioPuerto][PUERTO][tamanioResultado][Resultado][IdProceso][Bloque]

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				printf("%i\n",tamanioDinamico);
				IP = malloc(tamanioDinamico);
				memcpy(IP, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				printf("%s\n",IP);

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				memcpy(aplicarMapper->PUERTO, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				printf("%s\n",aplicarMapper->PUERTO);

				memcpy(&tamanioDinamico, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);
				memcpy(aplicarMapper->resultado, paqueteDeseliariza + offset, tamanioDinamico);
				offset += tamanioDinamico;

				printf("%s\n",aplicarMapper->resultado);

				memcpy(&aplicarMapper->id_proceso, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				printf("%i\n",aplicarMapper->id_proceso);

				memcpy(&aplicarMapper->bloque, paqueteDeseliariza + offset, sizeof(int));
				offset += sizeof(int);

				free(paqueteDeseliariza);

				printf("ID Proceso: %i, IP: %s, PUERTO: %s, RESULTADO: %s, BLOQUE: %i\n",aplicarMapper->id_proceso,aplicarMapper->IP, aplicarMapper->PUERTO, aplicarMapper->resultado, aplicarMapper->bloque);

				pthread_create(&hiloMapper[contM], NULL, (void *)aplicarMapperF, (void *)aplicarMapper); // Hilo Mapper

				contM++;
				break;
			case 34: // Aplicar Reduce
				aplicarReduce = malloc(sizeof(t_aplicarReduce));
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

	return EXIT_SUCCESS;
}

void *aplicarMapperF (t_aplicarMapper *aplicarMapper){

	int socketNodo,entero,tamanioTotal, size_to_send, offset, tamanioDinamico;
	char *paquete;

	printf("ID Proceso: %i, IP: %s, PUERTO: %s, RESULTADO: %s, BLOQUE: %i\n",aplicarMapper->id_proceso,aplicarMapper->IP, aplicarMapper->PUERTO, aplicarMapper->resultado, aplicarMapper->bloque);

	socketNodo = crearCliente(aplicarMapper->IP,aplicarMapper->PUERTO);

	entero = 8;
	send(socketNodo, &entero, sizeof(int),0); //Handshake
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		entero = 25;
		send(socketMarta,&entero,sizeof(int),0);
		// enviar id proceso
		return NULL;
	}

	entero = 1;
	send(socketNodo, &entero, sizeof(int),0); // Handshake de mapper
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		entero = 25;
		send(socketMarta,&entero,sizeof(int),0);
		// enviar id proceso
		return NULL;
	}

	// paquete para aplicar Mapper [bloque][tamanioScript][script]

	tamanioTotal = sizeof(int) + sizeof(int) + strlen(rutinaMapper) + 1;

	send(socketNodo, &tamanioTotal, sizeof(int),0); // tamanioTotal del paquete
	if(recv(socketNodo, &entero, sizeof(int),0)<0){ // basura
		entero = 25;
		send(socketMarta,&entero,sizeof(int),0);
		// enviar id proceso
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

	send(socketNodo,  paquete, tamanioTotal, 0);
	free(paquete);

	if(recv(socketNodo, &entero, sizeof(int), 0)<0){   // Espero respuesta del Nodo
		entero = 25;
	}
	// 42 Positivo
	// 25 Negativo
	entero = 42;
	send(socketMarta,&entero,sizeof(int),0);
	return NULL;
}

void *aplicarReduceF (t_aplicarReduce *aplicarReduce){
	return NULL;
}

void *moverArchivoF (t_moverArchivo *moverArchivo){
	return NULL;
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


