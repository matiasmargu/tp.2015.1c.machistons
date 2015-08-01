/*
 ============================================================================
 Name        : FileSystem.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "librerias_y_estructuras.h"

int main()
{
	// Inicializo la base MongoDB
	mongoc_init ();
	client = mongoc_client_new ("mongodb://localhost:27017/fileSystem");
	directorios = mongoc_client_get_collection (client, "fileSystem", "directorios");
	archivos = mongoc_client_get_collection (client, "fileSystem", "archivos");
	nodos = mongoc_client_get_collection (client, "fileSystem", "nodos");
	//
	//aplicarNodoGlobalYponerNodosNoDisponible();
	nodosActivos = 0;

	fd_set master;
	fd_set read_fds;

	pthread_t hiloConsola;
	pthread_t hiloMarta[1000];
	pthread_t hiloNodo[1000];
	int contHM = 1; // Contador para hilos de marta, varios por si se cae y se vuelve a conectar
	int contHN = 1; // Contador para hilos de Nodo, varios porque se van a conectar muchos.

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax, listener, newfd, yes = 1, addrlen, i;
	int martafd; //Socket de coneccion con Marta

	pthread_create(&hiloConsola, NULL, atenderConsola, NULL);

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/filesystem.conf";

	t_config* archivoConfiguracion;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	int puerto_listen = config_get_int_value(archivoConfiguracion, "PUERTO_LISTEN");
	nodosNecesarios = config_get_int_value(archivoConfiguracion, "LISTA_NODOS");

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	listener = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(puerto_listen);
	memset(&(serveraddr.sin_zero), '\0', 8);
	bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(listener, 10);
	FD_SET(listener, &master);

	fdmax = listener;

	int entero; //Para el handshake
	variableDatos = 0;

	logger = log_create("LOG_FILESYSTEM", "log_filesystem" ,false, LOG_LEVEL_INFO);

	for (;;){
	read_fds = master;
	select(fdmax+1, &read_fds, NULL, NULL, NULL);
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
	    		if(variableDatos == 0){
					if((recv(i, &entero, sizeof(int),0 )) <= 0)
					{
						if (i == martafd){
							log_info(logger,"Se ha perdido la conexion con Marta en el socket: %i\n",i);
							printf("Se ha perdido la conexion con el proceso Marta en el socket: %i\n",i);
							martafd = 0;
						}
						else{
							log_info(logger,"Se ha perdido la conexion con un Nodo en el socket: %i\n",i);
							darDeBajaElNodo(i);
							printf("Se ha perdido la conexion con un proceso Nodo en el socket: %i\n",i);
						}
						close(i); // Coneccion perdida
						FD_CLR(i, &master);
					}
					else{
						switch(entero){
							case 25: // Este es Marta
								martafd = i;
								printf("Se ha conectado un proceso Marta en el socket: %i\n",i);
								pthread_create(&hiloMarta[contHM], NULL, atenderMarta, (void *)martafd);
								log_info(logger,"Se ha conectado un proceso Marta en el socket: %i\n",i);
								contHM++;
								break;
							case 2: // Este es Nodo

								socketNodoGlobal = i;

								printf("Se ha conectado un proceso Nodo en el socket: %i\n",i);
								pthread_create(&hiloNodo[contHN], NULL, agregoNodoaMongo, (void *)i);
								log_info(logger,"Se ha conectado un proceso Nodo en el socket: %i\n",i);
								contHN++;
								break;
							}
						}
	    		}
	    	}
	    }
	}
	}

	mongoc_collection_destroy (directorios);
	mongoc_collection_destroy (nodos);
	mongoc_collection_destroy (archivos);
	mongoc_client_destroy (client);

	config_destroy(archivoConfiguracion);
	log_destroy(logger);

	return EXIT_SUCCESS;
}
