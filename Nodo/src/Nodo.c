/*
 ============================================================================
 Name        : Nodo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <./commons/collections/list.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

t_log* logger; // Log Global

int recive_y_deserialisa(setBloque *bloque, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	memcpy(&(bloque->numero), buffer + offset, sizeof(bloque->numero));
	offset += sizeof(bloque->numero);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	bloque->bloque = malloc(tamanioDinamico);
	memcpy(bloque->bloque, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}


void *atenderNFS(void* arg){
	int socket=(int) arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;
	int tamanioTotal;
	setBloque set;

	while((recv(socket, &entero, sizeof(int),0)>0)){
		switch(entero){
		//getBloque(numero);
			case 1:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
				recv(socket,&tamanioTotal,sizeof(set.tamanioDatos),0);

				int status = 1; // Estructura que manjea el status de los recieve.
				status = recive_y_deserialisa(&set, socket, tamanioTotal);
				if (status) {
					printf("%i\n",set.numero);
					printf("%s\n",set.bloque);
				}
				//char *bloqueATrabajar = mmap((void *)archivo_bin, size_t length, PROTO_NONE, int flags, int fd, off_t offset);
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//getFileContent(nombre);
			case 3:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		}
	}
	return NULL;
}

int main(void) {

	printf("Arranca el Nodo\n");

	setvbuf(stdout, NULL, _IOLBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";

	t_config* archivoConfiguracion;

	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);

	char* ip_fs;
	FILE *archivo_bin;
	char* dir_temp;
	char* nodo_nuevo;
	fd_set master;
	fd_set read_fds;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax;
	int listener;
	int newfd;
	int yes = 1;
	int addrlen;
	int i;
	int entero; //Para el handshake con el Job
	int nodofd; //Socket para los demas Nodos
	int jobfd; //Socket para los Job

	pthread_t fs;
	pthread_t job;
	pthread_t nodo;

//Estos los vamos a usar cuando probemos las conecciones entre nodo y nodo
	char* ip_nodo;

///////    Carga del archivo de configuracion       ///////////////////////////////////////////

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);

	log_info(logger, "Se creo correctamente el archivo de configuracion");
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
	archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
	dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
	nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
	ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");

	char* puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");
	int puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");

//Esta es la coneccion con el FS

	int socket_fs = crearCliente(ip_fs,puerto_fs);
	entero = 2; // handshake con FS
	send(socket_fs,&entero,sizeof(int),0);
	pthread_create(&fs,NULL,atenderNFS, (void *) socket_fs);


//Este es el select
/*
	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	listener = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(puerto_nodo);
	memset(&(serveraddr.sin_zero), '\0', 8);
	bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(listener, 10);
	FD_SET(listener, &master);

	fdmax = listener;

	FILE* script;//   ?

	for(;;)
	{
	read_fds = master;
	select(fdmax+1, &read_fds, NULL, NULL, NULL);
	printf("select activo\n");
	for(i = 0; i <= fdmax; i++)
	{
		if(FD_ISSET(i, &read_fds))
	    {
	    	if(i == listener)
	    	{
				addrlen = sizeof(clientaddr);
				if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1)
				{
				}
				else
				{
					FD_SET(newfd, &master);
					if(newfd > fdmax)
					{
						fdmax = newfd;
					}
				}
	    	}
	    	 else
	    	{
	    		if((recv(i, &entero, sizeof(int),0 )) <= 0)
	    		{
	    			close(i); // Coneccion perdida
	    			FD_CLR(i, &master);
	    		}
	    		else
	    		{
	    			switch(entero){
	    			case 8: // Este es Job
	    				entero = 45;
	    				send(i,&entero, sizeof(int),0); // al mandarle este entero le estamos diciendo que recibimos la conexion
	    				log_info(logger,"Hilo Job creado satisfactoriamente");
	    				// ahora van a venir hilos de mapper o reduce a decirme que aplique las rutinas mapping o reduce
	    				// hay que hacer un handshake para ver que onda, si es una rutina mapping o una rutina reduce, a traves de
	    				// una estructura, o sea la estructura tendria que tener un numero ( 1 o 2 ponele) que diga es mapping es reduce
	    				//if(entero==1){
	   					//pthread_create(&job,NULL,rutinaMapping, (void *) socket_job);
	   					//}else{
	   					//pthread_create(&job,NULL,rutinaReduce, (void *) socket_job);
	    				break;
	    			case 7: // Este es otro Nodo
	    				entero = 1;
	    				send(i,&entero, sizeof(int),0);
	    				pthread_create(&nodo,NULL,atenderNFS, (void *) nodofd);
	   				}
	   			}
	   		}
	   	}
		}
	}
*/
//	close(socket_fs);
//	close(socket_job);

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(ip_nodo);
	free(archivo_bin);
	free(dir_temp);
	free(nodo_nuevo);

	return EXIT_SUCCESS;
}
