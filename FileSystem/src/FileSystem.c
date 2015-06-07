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

void *atenderMarta(void*arg){

	int socketMarta = (int)arg;
	printf("%i",socketMarta);

	return NULL;
}

int main()
{
	// Inicializo la base MongoDB
	mongoc_init ();
	client = mongoc_client_new ("mongodb://localhost:27017/fileSystem");
	directorios = mongoc_client_get_collection (client, "fileSystem", "directorios");
	archivos = mongoc_client_get_collection (client, "fileSystem", "archivos");
	nodos = mongoc_client_get_collection (client, "fileSystem", "nodos");
	//
	doc = bson_new ();
	if (!mongoc_collection_delete (nodos, MONGOC_DELETE_NONE, doc, NULL, &error)) {
	        printf ("Delete failed: %s\n", error.message);
	}


	fd_set master;
	fd_set read_fds;

	pthread_t hiloConsola;
	pthread_t hiloMarta;

	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;

	int fdmax, listener, newfd, yes = 1, addrlen, i;
	int martafd; //Socket de coneccion con Marta

	pthread_create(&hiloConsola, NULL, atenderConsola, NULL);

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/filesystem.conf";

	t_config* archivoConfiguracion;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	int puerto_listen = config_get_int_value(archivoConfiguracion, "PUERTO_LISTEN");
	int nodosNecesarios = config_get_int_value(archivoConfiguracion, "LISTA_NODOS");

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

	logger = log_create("LOG_FILESYSTEM", "log_filesystem" ,false, LOG_LEVEL_INFO);

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
	    			case 3: // Este es Marta
	    				martafd = i;
	    				pthread_create(&hiloMarta, NULL, atenderMarta, (void *)martafd);
	    				log_info(logger,"Hilo Marta creado satisfactoriamente");
	    				break;
	    			case 2: // Este es Nodo
	    				socketNodoGlobal = i;

	    				doc = bson_new ();
	    			    bson_oid_init (&oid, NULL);
	    			    BSON_APPEND_OID (doc, "_id", &oid);
	    			    BSON_APPEND_INT32(doc, "Socket", i);
	    			    BSON_APPEND_UTF8 (doc, "IP", "192.168.42.7");
	    			    BSON_APPEND_UTF8(doc, "PUERTO" , "2004");
	    			    BSON_APPEND_UTF8(doc, "Estado", "No disponible");
	    			    if (!mongoc_collection_insert (nodos, MONGOC_INSERT_NONE, doc, NULL, &error)) {
	    			           log_error(logger, error.message);
	    			    }
	    			    bson_destroy (doc);

	    			    query = BCON_NEW ("PUERTO", "2004");
	    			    update = BCON_NEW ("$set", "{",
	    			                              "Nombre", "Carlos",
	    			                              "updated", BCON_BOOL (true),
	    			                       "}");

	    			    if (!mongoc_collection_update (nodos, MONGOC_UPDATE_NONE, query, update, NULL, &error)) {
	    			            printf ("%s\n", error.message);

	    			     }

	    			    query = bson_new ();
	    			    cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

	    			    while (mongoc_cursor_next (cursor, &doc)) {
	    			    	str = bson_as_json (doc, NULL);
	    			        printf ("%s\n", str);
	    			        bson_free (str);
	    			    }

	    				printf("registro al nodo en Mongo DB\n");
	    				break;
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
	bson_destroy (doc);
	bson_destroy (query);
    bson_destroy (update);

	config_destroy(archivoConfiguracion);
	log_destroy(logger);

	return EXIT_SUCCESS;
}

