/*
 ============================================================================
 Name        : Job2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "funcionesJob2.h"

int main(void) {

	char* rutaArchivoConfiguracion;
	t_config* archivoConfiguracion;
	char *puerto_job;
	char *puerto_marta;
	char *ip_marta;
	char *mapper;
	char *reduce;
	char* combinerAEnviar;
	char *combiner;
	char *lista_archivos;
	char *archivo_resultado;
	char* archivo_resultado_final;
	int entero, entero2, p, tamanioCombiner,y,tamanio_archivo_resultado;
	int tamanioTotal, tamanioTotalMapper , numeroRutina;
	int handshake, handshakeNodo, numero, socketNodo;
	t_marta_job_map Marta_Job_Map;
	char* rutinaReduceAEnviar;
	char* rutinaMapperTraducida;
	char* rutinaReduceTraducida;
	t_marta_job_reduce Marta_Job_Reduce;
	int enteroPrueba, tipoOperacion, tamanioStruct,x;
	pthread_t hilomap;
	pthread_t hiloreduce;
	t_hilo_map map;
	t_hilo_reduce datosParaLaReduccion;
	char* rutinaAEnviarNodo;

	logger = log_create("LOG_JOB", "log_job" ,false, LOG_LEVEL_INFO);

	rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);


	puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
	ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
	mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
	reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
	combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
	lista_archivos = config_get_string_value(archivoConfiguracion, "ARCHIVOS");
	archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");

	int socketMarta = crearCliente (ip_marta,puerto_marta);

	rutinaMapperTraducida = mapearAMemoriaVirtual(mapper);
	rutinaReduceTraducida = mapearAMemoriaVirtual(reduce);
	//printf("rutinaMappper TRADUCIDA  %s\n",rutinaMapperTraducida);

	///MANDAMOS LA LISTA DE ARCHIVOS A MARTA Y EL COMBINER

	handshake = 1;
	send(socketMarta, &handshake, sizeof(int),0);
	//COMBINER
	tamanioCombiner = strlen(combiner)+1;
	recv(socketMarta, &enteroPrueba, sizeof(int),0);
	log_info(logger,"Se ha establecido la conexion con Marta, su ip es %s y su puerto %s",ip_marta,puerto_marta);
	printf("Conexion establecida con Marta, su ip es %s y su puerto %s  \n",ip_marta,puerto_marta);
	send(socketMarta, &tamanioCombiner, sizeof(int),0);
	recv(socketMarta, &enteroPrueba, sizeof(int),0);
	send(socketMarta,combiner,tamanioCombiner,0);


	//LISTA DE ARCHIVOS

	tamanioTotal =  strlen(lista_archivos)+1;
	recv(socketMarta, &enteroPrueba, sizeof(int),0);
	send(socketMarta, &tamanioTotal, sizeof(int),0);
	recv(socketMarta, &enteroPrueba, sizeof(int),0);
	send(socketMarta,lista_archivos,tamanioTotal,0);
	recv(socketMarta, &enteroPrueba, sizeof(int),0);

	//ACA MANDAMOS EL ARCHIVO RESULTADO A MARTA

	archivo_resultado_final=strrchr(archivo_resultado,'/');
	archivo_resultado_final = string_substring_from(archivo_resultado_final, 1);
	printf("EL ARCHIVOOOOOOOOOO %s \n", archivo_resultado_final);
	tamanio_archivo_resultado = strlen(archivo_resultado_final)+1;
	send(socketMarta,&tamanio_archivo_resultado,sizeof(int),0);
	recv(socketMarta,&enteroPrueba,sizeof(int),0);
	send(socketMarta,archivo_resultado_final,tamanio_archivo_resultado,0);

	//ACA RECIBIMOS DE MARTA LA OPERACION A REALIZAR
	x = 0;
	while (x!=1){
	recv(socketMarta, &tipoOperacion, sizeof(int),0);
	switch(tipoOperacion){
		case 30: // REALIZAR MAP
			send(socketMarta, &enteroPrueba, sizeof(int),0);
			recv(socketMarta, &tamanioStruct, sizeof(int),0);
			send(socketMarta, &enteroPrueba, sizeof(int),0);
			int status = 1; // Estructura que manjea el status de los recieve.
			status = recive_y_deserialisa_marta_job_mapper(&Marta_Job_Map, socketMarta, tamanioStruct);
			printf("llego el archivo %s\n",Marta_Job_Map.nombre_archivo_resultado);
			int caca;
			caca = list_get(Marta_Job_Map.bloques, 2);
			printf("numero de la listaa %i\n\n", caca);

			if(status){
				//HANDSHAKE NODO
			    socketNodo = crearCliente (Marta_Job_Map.ip_nodo, Marta_Job_Map.puerto);
				handshakeNodo = 8;
				send(socketNodo,&handshakeNodo,sizeof(int),0);
				recv(socketNodo, &enteroPrueba, sizeof(int),0);
				//ACA LE MANDA LA RUTINA MAPPER AL NODO
				numeroRutina = 1; // LE AVISA QUE MANDA UNA RUTINA MAP
				send(socketNodo,&numeroRutina,sizeof(int),0);
				tamanioTotalMapper = strlen(rutinaMapperTraducida)+1;
				recv(socketNodo, &enteroPrueba, sizeof(int),0);
				printf("tamanio script %i\n",tamanioTotalMapper);
				send(socketNodo, &tamanioTotalMapper, sizeof(int),0);
				recv(socketNodo, &enteroPrueba, sizeof(int),0);
				send(socketNodo, rutinaMapperTraducida, tamanioTotalMapper,0);


			//ACA LEVANTA UN HILO POR CADA BLOQUE A ENVIAR A MAPEAR
			for(p = 0; p< Marta_Job_Map.cantidadBloques ; p++){
			map.bloque = list_get(Marta_Job_Map.bloques, p);
			map.socketNodo = socketNodo;
			map.nombreArchivoResultado = Marta_Job_Map.nombre_archivo_resultado;
			map.socketMarta = socketMarta;
			map.idNodo = Marta_Job_Map.idNodo;
			pthread_create(&hilomap,NULL, mapearBloque,(void *)&map);
			}
		}
			break;
		case 34: // REALIZAR REDUCE

			send(socketMarta, &enteroPrueba, sizeof(int),0);
			recv(socketMarta, &tamanioStruct, sizeof(int),0);
			send(socketMarta, &enteroPrueba, sizeof(int),0);
			int estado = 1; // Estructura que manjea el status de los recieve.
			estado = recive_y_deserialisa_marta_job_reduce(&Marta_Job_Reduce, socketMarta, tamanioStruct);
			printf("llego el archivo %s\n",Marta_Job_Reduce.archivoResultadoReduce);
			int caca2 = list_get(Marta_Job_Reduce.listaArchivosTemporales, 2);
			printf("archivo de la listaa %s\n\n", caca2);
			if(estado){
				//HANDSHAKE NODO
			socketNodo = crearCliente (Marta_Job_Reduce.ipNodo, Marta_Job_Reduce.puertoNodo);
			handshakeNodo = 8;
			send(socketNodo,&handshakeNodo,sizeof(int),0);
			recv(socketNodo, &enteroPrueba, sizeof(int),0);
			//ACA LE MANDA LA RUTINA REDUCE AL NODO
			numeroRutina = 2; //Le avisa que manda reduce
			send(socketNodo,&numeroRutina,sizeof(int),0);
			recv(socketNodo, &enteroPrueba, sizeof(int),0);
			int tamanioTotalReduce =  strlen(rutinaReduceTraducida)+1;
			send(socketNodo, &tamanioTotalReduce, sizeof(int),0);
			recv(socketNodo, &enteroPrueba, sizeof(int),0);
			send(socketNodo,rutinaReduceTraducida,tamanioTotal,0);
			recv(socketNodo, &enteroPrueba, sizeof(int),0);

			//ACA LEVANTA UN HILO PARA APLICAR EL REDUCE
			datosParaLaReduccion.socketNodo = socketNodo;
			datosParaLaReduccion.nombreArchivoResultado = Marta_Job_Reduce.archivoResultadoReduce;
			datosParaLaReduccion.archivos = list_create();
			list_add_all(datosParaLaReduccion.archivos, Marta_Job_Reduce.listaArchivosTemporales) ; //COPIA UNA LISTA A LA OTRA
			pthread_create(&hiloreduce,NULL, reducirArchivos,(void *)&datosParaLaReduccion);

			break;
		case 5: // NO HAY MAS OPERACIONES
			x= 1 ;
			break;
		}

	}



}

	log_info(logger,"Se ha desconectado con Marta, su ip es %s y su puerto %s",ip_marta,puerto_marta);
	printf("Se ha desconectado con Marta, su ip es %s y su puerto %s  \n",ip_marta,puerto_marta);
	return EXIT_SUCCESS;
}
