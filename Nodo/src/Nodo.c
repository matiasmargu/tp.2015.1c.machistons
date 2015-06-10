/*
 ============================================================================
 Name        : Nodo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "funcionesParaEnviar.h"

typedef struct{
	int socket;
	char* archivoATrabajar;
}estructura_de_nfs;

char* pmap;

void *atenderNFS(void*arg){

	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;
	int tamanioTotal;
	int fd;
	estructuraSetBloque set;
	struct stat mystat;
	char* pmap;
	int nroDelBloque;

	printf("%i\n",socket);

	while(1){
		//printf("Esto deberia imprimirse una sola vez\n");
		if(recv(socket, &entero, sizeof(int),0) > 0){
		switch(entero){
		//getBloque(numero);
			case 1:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
				printf("setBloque Activo\n");
				recv(socket,&tamanioTotal,sizeof(int),0);
				int status = 1; // Estructura que manjea el status de los recieve.
				status = recive_y_deserialisa_SET_BLOQUE(&set, socket, tamanioTotal);
				if (status) {
					// ACA TRABAJAN CON set.numero y set.bloque. Escriben el archivo y toda la bola.
					printf("%i\n",set.bloque);
					printf("%s\n",set.data);
					nroDelBloque = set.bloque;
					pmap[nroDelBloque * 20 * 1024 * 1024] = set.data;
				}
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
	}
	return NULL;
}

int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";
	t_config* archivoConfiguracion;
	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);

	int entero; //Para el handshake

	int fd;
	struct stat mystat;

	pthread_t hiloFS;
	pthread_t hiloJob;
	pthread_t hiloNodo;

	fd_set master;
	fd_set read_fds;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax, listener, newfd, yes = 1, addrlen, i;

///////    Carga del archivo de configuracion       ///////////////////////////////////////////

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);

	log_info(logger, "Se creo correctamente el archivo de configuracion");

	char *archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
	char *dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
	char *nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
	char *ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
	int puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");

//Esta es la coneccion con el FS

	char *ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
	char *puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");
	int socket_fs = crearCliente(ip_fs,puerto_fs);

	fd = open(archivo_bin,O_RDWR);
		if(fd == -1){
			printf("Error al leer el ARCHIBO_BIN\n");
			exit(1);
		}

		if(fstat(fd,&mystat) < 0){
			printf("Error al establecer fstat\n");
			close(fd);
			exit(1);
		}

		pmap = mmap(0,mystat.st_size, PROT_READ ,MAP_SHARED,fd,0);
		if(pmap == MAP_FAILED){
			printf("Error al mapear a memoria\n");
			close(fd);
			exit(1);
		}

	entero = 2; // handshake con FS
	send(socket_fs,&entero,sizeof(int),0);
	mensaje = serializarIPyPUERTO(ip_fs, puerto_fs);
	printf("%i\n",socket_fs);
	pthread_create(&hiloFS, NULL, &atenderNFS, (void *)socket_fs);

//Esta es el select

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
		    			case 8: // Este es el Job

		    				break;
		    			case 7: // Este es otro Nodo

		    				break;
		    			}
		    		}
		    	}
		    }
		}
		}

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	free(ip_nodo);
	free(archivo_bin);
	free(dir_temp);
	free(nodo_nuevo);

	return EXIT_SUCCESS;
}
