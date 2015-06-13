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


int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";

	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);

	int fd;
	struct stat mystat;

	int entero;

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

	archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
	dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
	nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
	ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
	puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");


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

		pmap = mmap(0,mystat.st_size, PROT_READ|PROT_WRITE ,MAP_SHARED,fd,0);
		if(pmap == MAP_FAILED){
			printf("Error al mapear a memoria\n");
			close(fd);
			exit(1);
		}


	handshakeConFS();

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
