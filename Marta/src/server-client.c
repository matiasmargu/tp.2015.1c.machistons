/*
 * client.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */
#include "functions.h"
#include "server-client.h"
#include <pthread.h>
#include <socket/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//socket para que el cliente comience a comunicarse con el servidor
int crearsocketServer(char *ip,char* puerto ){ //crear un socket para conectarme a un servidor

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(ip, puerto, &hints, &serverInfo);	// Carga en serverInfo los datos de la conexion


	int serverSocket;
	serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	connect(serverSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo);
	return serverSocket ;
}

//para que el servidor acepte conexiones y se comunique con algun cliente

   int crearsocketEscucha(int puertoEsc){

    struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// No importa si uso IPv4 o IPv6
	hints.ai_flags = AI_PASSIVE;		// Asigna el address del localhost: 127.0.0.1
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

	getaddrinfo(NULL, puertoEsc, &hints, &serverInfo);

	int listenningSocket;
	listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	if (listenningSocket <0)
		 {
			 perror("Error de apertura de socket");
		     exit(-1);
		 }
		// ya tengo un archivo que puedo utilizar para analizar las conexiones entrantes.
	if(bind(listenningSocket,serverInfo->ai_addr, serverInfo->ai_addrlen)==-1)
			{
				printf("error en bind() \n");
			    exit(-1);
			};

	freeaddrinfo(serverInfo); // Ya no lo vamos a necesitar
    return listenningSocket;
    }

   void escuchar(int socketEscuchador){  //funcion bloqueante..hasta que no recibe llamadas no retorna

     if(listen(socketEscuchador, BACKLOG) == -1)
   	    {
   	      printf("error en listen()\n");
   	      exit(-1);
   	    }
   }


	int aceptarConexion(int socket){

	struct sockaddr_in addr;			// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);
	int socketCliente = accept(socket, (struct sockaddr *) &addr, &addrlen);
	printf("Cliente conectado. Esperando Envío de mensajes.\n");
	return socketCliente ;
	}
