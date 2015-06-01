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


void *atenderNFS(void* arg){
	int socket=(int) arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;

	while((recv(socket, &entero, sizeof(int),0)>0)){
		switch(entero){
		//getBloque(numero);
			case 1:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
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

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";

	t_config* archivoConfiguracion;

	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);

	char* puerto_fs;
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
	int entero; //Para el handshake
	int nodofd;


	pthread_t fs;

//Estos los vamos a usar cuando probemos las conecciones entre nodo y nodo
	char* ip_nodo;
	char* puerto_nodo;


///////    Carga del archivo de configuracion       ///////////////////////////////////////////
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	log_info(logger, "Se creo correctamente el archivo de configuracion");
	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
	archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
	dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
	nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
	ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
	puerto_nodo = config_get_string_value(archivoConfiguracion, "PUERTO_NODO");

	///hacemos mmap sobre el archivo_bin
	//el mmap tiene q estar en una variable de la cual se pueda acceder facilmente,
	//la misma retorna una direccion a la particion de memoria
	//Estaria copado usar una lista de la cual creamos y llenamos particiones
	//segun el tamaño del archivo y no andar viendo cuantas variables crear.

	int tamanioBloque = 20*1024*1024;//el size_t era como ejemplo en el man mmap() de linux

	void *mmap (void *archivo_bin, int tamanioBloque, int __prot,int __flags, int __fd, __off_t __offset);

	 // addr direccion del archivo, puede ser NULL
	 //len tamaño de los bloques
	 //prot es para escribir leer o ejecutar
	 // flags si es publica o privada
	 // fd stdin o stdout
	 // offset pone el puntero donde queremos que empiece a dividir


	int socket_fs = crearCliente(ip_fs,puerto_fs);
	entero = 2; // handshake con FS
	send(socket_fs,&entero,siszeof(int),0);
	pthread_create(&fs,NULL,atenderNFS, (void *) socket_fs);


	int socket_job = crearServidor(puerto_nodo);

	int prueba;

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

	for(;;){
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
	    			printf("Nueva coneccion %s en %d\n", inet_ntoa(clientaddr.sin_addr), newfd);
	    		}
	    	}
	    	else
	    	{
	    		if((recv(i, &entero, sizeof(int),0 )) <= 0)
	    		{
	    			printf("socket caido\n");
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




	    				break;
	    				    			}
	    		}
	    	}
	    }
	}
	}


//	close(socket_fs);
	close(socket_job);

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	//free(ip_nodo);
	//free(archivo_bin);
	//free(dir_temp);
	//free(nodo_nuevo);
	return EXIT_SUCCESS;
}
