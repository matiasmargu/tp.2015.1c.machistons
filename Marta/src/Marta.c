/*
 ============================================================================
 Name        : Marta.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "functions.h"


int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	char* puerto_fs ;
	char* ip_fs ;
	t_charpuntero archivoAFS;
	char* archivoAFSAEnviar;
	int cantidad;
	int handShake;
	char* combiner;
	int tamanioTotalAFS;
	int saludo;
	int tamanioCombiner;
	int socketFS;
	int d;
	int handshakeFS;


	t_config* archivoConfiguracion;

    // para recibir la cantidad de archivos

    int a;



	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");



int socketjob  = crearServidor("3000");
recv(socketjob, &handShake, sizeof(int),0);
saludo = 3;
send(socketjob, &saludo, sizeof(int),0);
printf("Se conecto el job con el hadshake: %i \n",handShake);
recv(socketjob, &cantidad, sizeof(int),0);
char* listaDeArchivos[cantidad];
int tamanioTotal;
char* archivo;

   	   for(a = 0 ; a < cantidad; a++){

   		recv(socketjob, &tamanioTotal, sizeof(int),0);
   		int estado = 1; // Estructura que manjea el status de los recieve.
   		archivo= malloc(tamanioTotal);
   		estado = recive_y_deserialisa(&archivo, socketjob, tamanioTotal);
   		 if(estado){

   			printf("el archivo es %s\n ",archivo);
   			listaDeArchivos[a] = archivo;
   		 }
   		free(archivo);
   	   }

   	   listaDeArchivos[cantidad+1] = NULL;
   	recv(socketjob, &tamanioCombiner, sizeof(int),0);
   	int estadoCombiner = 1; // Estructura que manjea el status de los recieve.
  	combiner = malloc(tamanioCombiner);

   	estadoCombiner = recive_y_deserialisa(&combiner, socketjob, tamanioCombiner);
   	if(estadoCombiner){
   		printf("el combiner es %s\n",combiner);
   		socketFS = crearCliente (ip_fs, puerto_fs);
   		handshakeFS = 25;
   		send(socketFS,&handshakeFS,sizeof(int),0);
   		printf("mando a fs %i\n",handshakeFS);
   		send(socketFS,&cantidad,sizeof(int),0);
   		for(d = 0 ; d < cantidad; d++){
   			archivoAFS.archivo = listaDeArchivos[d];
   			tamanioTotalAFS = sizeof(int) + strlen(archivoAFS.archivo)+1;
   			send(socketFS, &tamanioTotalAFS, sizeof(int),0);
   			archivoAFSAEnviar = serializar_charpuntero(&archivoAFS, tamanioTotalAFS);
   			send(socketFS,archivoAFSAEnviar,tamanioTotalAFS,0);
   		}






















   	}


	close(socketjob);
	free(combiner);
	close(socketFS);
	return EXIT_SUCCESS;
}

