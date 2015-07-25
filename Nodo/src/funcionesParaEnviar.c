/*
 * funcionesParaEnviar.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"


int recive_y_deserializa_NODO_C(t_para_nodo *bloque, int socket, uint32_t tamanioTotal){
	int status;
	char* buffer = malloc(tamanioTotal);
	int offset = 0;
	int tamanioDinamico;

	recv(socket, buffer, tamanioTotal, 0);
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	bloque->ip = malloc(tamanioDinamico);
	memcpy(bloque->ip, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	bloque->puerto = malloc(tamanioDinamico);
	memcpy(bloque->puerto, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	bloque->archivo = malloc(tamanioDinamico);
	memcpy(bloque->archivo, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	return status;
	}



int recive_y_deserializa_EST_REDUCE(t_job_nodo_reduce *bloque, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;
	int tamanioDinamico;

	recv(socket, buffer, tamanioTotal, 0);

	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	bloque->nombreArchivoResultado = malloc(tamanioDinamico);
	memcpy(bloque->nombreArchivoResultado, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	memcpy(&(bloque->cantidadArchivos),buffer + offset, sizeof(bloque->cantidadArchivos));
	offset += sizeof(bloque->cantidadArchivos);

	int a;
	char* archivo;

	//deserializa la lista
	bloque->archivosAreducir = list_create();
	for(a=0;a<  (bloque->cantidadArchivos) ; a++){
		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);
		archivo = malloc(tamanioDinamico);
		memcpy(archivo, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;
		printf("el archivooo %s \n\n",archivo);
		list_add(bloque->archivosAreducir,archivo);
	}
	free(buffer);
	free(archivo);
	return status;
}

int recive_y_deserialisa_SET_BLOQUE(estructuraSetBloque *bloque, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;
	int tamanioDinamico;

	recv(socket, buffer, tamanioTotal, 0);

	memcpy(&(bloque->bloque), buffer + offset, sizeof(bloque->bloque));
	offset += sizeof(bloque->bloque);

	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	bloque->data = malloc(tamanioDinamico);
	memcpy(bloque->data, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

int recive_y_deserialisa_CHARp(char *script, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	script = malloc(tamanioDinamico);
	memcpy(script, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

char* serializarCHARp(char* Aserializar, int tamanioData){
	int offset = 0;
	int size_to_send;

	char *serializedPackage = malloc(tamanioData);

	int tamanioNombre = strlen(Aserializar) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(Aserializar) + 1;
	memcpy(serializedPackage + offset, ip_nodo, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}



char* serializarIP_PUERTO(char* ip_fs,char* puerto_fs, int tamanioData){
	int offset = 0;
	int size_to_send;

	char *serializedPackage = malloc(tamanioData);

	int tamanioNombre = strlen(ip_fs) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(ip_fs) + 1;
	memcpy(serializedPackage + offset, ip_fs, size_to_send);
	offset += size_to_send;

	tamanioNombre = strlen(puerto_fs) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(puerto_fs) + 1;
	memcpy(serializedPackage + offset, puerto_fs, size_to_send);
	offset += size_to_send;

	char* archivo = mapearAMemoriaVirtual(archivo_bin);

	FILE* fdAux = fopen(archivo_bin,"r");
	fseek(fdAux, 0L, SEEK_SET);
	fseek(fdAux, 0L, SEEK_END);

	tamanioArchivo_BIN = ftell(fdAux);

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioArchivo_BIN, size_to_send);
	munmap(archivo,tamanioArchivo_BIN);


	fclose(fdAux);
	return serializedPackage;
}

char* serializarBloqueDeDatos(char* bloque, int tamanioData){
	int offset = 0;
	int size_to_send;

	char *serializedPackage = malloc(tamanioData);

	int tamanioNombre = strlen(bloque) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(bloque) + 1;
	memcpy(serializedPackage + offset, bloque, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}


void handshakeConFS (){
	printf("corre el handshake\n");
	//Esta es la coneccion con el FS
	pthread_t hiloFS;

	char* copiaIPFS = ip_fs;
	char* copiaBIN = archivo_bin;
	char* copiaTMP = dir_temp;
	char* copiaIPNODO = ip_nodo;
	char* copiaPUERTO_N = string_itoa(puerto_nodo);
	char* copiaPUERTO_F = puerto_fs;

	int socket_fs = crearCliente(ip_fs,puerto_fs);

	int entero2 = 2; // handshake con FS
	char* mensaje;

	send(socket_fs,&entero2,sizeof(int),0);

	if(string_equals_ignore_case(nodo_nuevo,"SI")){

		int nodo;
		recv(socket_fs,&nodo,sizeof(int),0);

		nodo = 32;
		send(socket_fs,&nodo,sizeof(int),0);

		int tamanioData = sizeof(int) + strlen(ip_nodo) + 1 + sizeof(int) + strlen(string_itoa(puerto_nodo)) + 1 + sizeof(int);

		recv(socket_fs,&nodo,sizeof(int),0);

		send(socket_fs, &tamanioData, sizeof(int), 0);
		mensaje = serializarIP_PUERTO(ip_nodo, string_itoa(puerto_nodo), tamanioData);
		send(socket_fs,mensaje,tamanioData,0);

		recv(socket_fs, &id_nodo, sizeof(int), 0);

		nuevoArchivo = fopen(rutaArchivoConfiguracion,"w");

		printf("%i\n",id_nodo);
		fputs("PUERTO_FS=",nuevoArchivo);
		fputs(copiaPUERTO_F,nuevoArchivo);
		fputc('\n',nuevoArchivo);

		fputs("IP_FS=",nuevoArchivo);
		fputs(copiaIPFS,nuevoArchivo);
		fputc('\n',nuevoArchivo);

		fputs("ARCHIVO_BIN=",nuevoArchivo);
		fputs(copiaBIN,nuevoArchivo);
		fputc('\n',nuevoArchivo);

		fputs("DIR_TEMP=",nuevoArchivo);
		fputs(copiaTMP,nuevoArchivo);
		fputc('\n',nuevoArchivo);

		fputs("NODO_NUEVO=NO\n",nuevoArchivo);
		nodo_nuevo = "NO";

		fputs("IP_NODO=",nuevoArchivo);
		fputs(copiaIPNODO,nuevoArchivo);
		fputc('\n',nuevoArchivo);

		fputs("PUERTO_NODO=",nuevoArchivo);
		fputs(copiaPUERTO_N,nuevoArchivo);
		fputc('\n',nuevoArchivo);

		fputs("ID_NODO=",nuevoArchivo);
		fputs(string_itoa(id_nodo),nuevoArchivo);

		fclose(nuevoArchivo);


		archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
		dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
		nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
		ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
		puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");
		ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
		puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");

		if(string_equals_ignore_case(nodo_nuevo,"NO")){
			id_nodo = config_get_int_value(archivoConfiguracion, "ID_NODO");
		}


	}else{

		int nodo=48;
		send(socket_fs,&nodo,sizeof(int),0);

		recv(socket_fs,&nodo,sizeof(int),0);
		send(socket_fs,&id_nodo, sizeof(int), 0);

		recv(socket_fs,&nodo,sizeof(int),0);

		int tamanioIPNODO = sizeof(int) + strlen(ip_nodo) + 1;
		mensaje =  serializarCHARp(ip_nodo,tamanioIPNODO);


		send(socket_fs,&tamanioIPNODO, sizeof(int),0);
		recv(socket_fs,&nodo,sizeof(int),0);;
		send(socket_fs,mensaje,tamanioIPNODO,0);
	}


	printf("Este es el socket: %i\n",socket_fs);
	pthread_create(&hiloFS, NULL, atenderNFS, (void *)socket_fs);


}

void handshakeConJob(int socket_job){
	pthread_t hiloJob;
	pthread_create(&hiloJob, NULL, &atenderJob, (void *)socket_job);
}
