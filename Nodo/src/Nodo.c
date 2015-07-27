/*
 ============================================================================
 Name        : Nodo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "variablesGlobales.h"


int main(void) {


	pthread_t hiloJob[1000];
	pthread_t hiloNodo[1000];

	int contJ=0,contN=0;

	logger = log_create("LOG_Nodo", "log_nodo" ,false, LOG_LEVEL_INFO);
	int entero;

	fd_set master;
	fd_set read_fds;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax, listener, newfd, yes = 1, addrlen, i;

	log_info(logger, "Se creo correctamente el archivo de configuracion");
	//printf("Se escribio en el log\n");

	rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);


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
		    				printf("Se ha conectado un Job\n");
		    				pthread_create(&hiloJob[contJ], NULL, &atenderJob, (void *)i);
		    				contJ++;
		    				break;
		    			case 7: // Este es otro Nodo
		    				printf("Se hha conectado un Nodo\n");
		    				pthread_create(&hiloNodo[contN],NULL, &atenderNFS, (void*)i);
		    				contN++;
		    				break;
		    			}
		    		}
		    	}
		    }
		}
		}

	config_destroy(archivoConfiguracion);
	log_destroy(logger);
	liberar(&rutaArchivoConfiguracion);
	free(ip_nodo);
	free(archivo_bin);
	free(dir_temp);
	free(nodo_nuevo);

	return EXIT_SUCCESS;
}
