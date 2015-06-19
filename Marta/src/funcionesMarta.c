/*
 * functionsMarta.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#include "funcionesMarta.h"


int recive_y_deserialisa(t_charpuntero* nombre, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	nombre->archivo = malloc(tamanioDinamico);
	memcpy(nombre->archivo, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

char* serializar_charpuntero(t_charpuntero *nombre, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

			int tamanioNombre = strlen(nombre->archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(nombre->archivo) + 1;
			memcpy(serializedPackage + offset, nombre->archivo, size_to_send);
			offset += size_to_send;

			return serializedPackage;
		}

//FUNCION QUE EJECUTA EL HILO QUE ATIENDE A CADA JOB
void  *conectarseAlJob(void*arg){

	int socket = (int)arg;
	int saludo = 9 ;
	int cantidad,tamanioTotal;
	char* listaArchivosJob;
	char* combiner;
	int tamanioCombiner;

	//MANDAMOS HANDSHAKE A JOB
	send(socket,&saludo,sizeof(int),0);

	//ACA RECIBIMOS LA LISTA DE ARCHIVOS DE JOB COMO UN CHAR*
   	recv(socket, &tamanioTotal, sizeof(int),0);
   	int estado2 = 1; // Estructura que manjea el status de los recieve.
   	listaArchivosJob = malloc(tamanioTotal);
   	estado2 = recive_y_deserialisa(&listaArchivosJob, socket, tamanioTotal);
   	if(estado2){
                printf("el string es %s\n",listaArchivosJob);
   	}

   	//ACA RECIBIMOS LA INFORMACION SOBRE EL COMBINER(SI TIENE O NO)
   	recv(socket, &tamanioCombiner, sizeof(int),0);
  	int estadoCombiner = 1; // Estructura que manjea el status de los recieve.
   	combiner = malloc(tamanioCombiner);
   	estadoCombiner = recive_y_deserialisa(&combiner, socket, tamanioCombiner);
   	if(estadoCombiner){
   		printf("el combiner es %s\n",combiner);
   	}

   	//ACA SE CONECTA CON FS
   	char* puerto_fs, ip_fs ;
   	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
   	t_config* archivoConfiguracion;
   	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
   	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
   	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
   	int socketFS,handshakeFS;
   	socketFS = crearCliente (ip_fs, puerto_fs);
   	handshakeFS = 25;
   	send(socketFS,&handshakeFS,sizeof(int),0);


   	char* archivoAEnviar;
   	t_charpuntero nombre;
   	archivo archivo;
   	char **archivos_separados = string_get_string_as_array(listaArchivosJob); //CONVERTIMOS EL STRING A UN ARRAY
   	int s = 0;
   	int a;
   	char* un_archivo;
   	un_archivo = archivos_separados[s];

   	//CONTAMOS LA CANTIDAD DE ARCHIVOS Y SE LA MANDAMOS A FS
   	while(un_archivo != NULL){
   		cantidad = cantidad + 1;     //cantidad = TAMANIO LISTA DE ARCHIVOS JOB
   		s = s+1;
   		un_archivo = archivos_separados[s];
   	}
   	send(socketFS,&cantidad,sizeof(int),0);\
   	printf("cantidad%i",cantidad);


   	t_list* listaDeArchivosGuardados; //UNA LISTA DE STRUCT ARCHIVO
   	listaDeArchivosGuardados =	list_create();
  bool condicion = 	nombre.archivo == archivo.nombre;

   	//MANDAMOS CADA ARCHIVO POR SEPARADO A FS(SOLO LOS QUE NO TENEMOS GUARDADOS) Y RECIBIMOS LA MATRIZ DE CADA ARCHIVO
   	for(a = 0 ; a < cantidad; a++){
   		nombre.archivo = archivos_separados[a];
   		if(!(list_any_satisfy(listaDeArchivosGuardados, condicion))){
   		tamanioTotal = sizeof(int)+ strlen(nombre.archivo)+1;
   		send(socketFS, &tamanioTotal, sizeof(int),0);
   		archivoAEnviar =  serializar_charpuntero( &nombre, tamanioTotal);
   		send(socketFS,archivoAEnviar,tamanioTotal,0);

   		//ACA TENEMOS QUE RECIBIR LAS MATRICES DE CADA ARCHIVO Y GUARDARLAS
   		archivo.nombre = nombre.archivo;
   	//ACA GUARDAMOS LA MATRIZ	archivo.matriz =
   		list_add(listaDeArchivosGuardados,archivo );
   		}
   	}




   	close(socketFS);
   	return NULL;

}




