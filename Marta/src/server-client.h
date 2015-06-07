/*
 * client.h
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#ifndef SERVER_CLIENT_H_

#define SERVER_CLIENT_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>
#include "functions.h"

#define PUERTO "3000"
#define BACKLOG 5
#define MAX_PACKAGE_SIZE 1024	//El servidor no admitira paquetes de mas de 1024 bytes
#define MAX_MESSAGE_SIZE 300
#define MAXUSERNAME 30

int crearsocketServer(char *ip,char* puerto ); //crear un socket para conectarme a un servidor
int crearsocketEscucha(int puertoEsc);
void escuchar(int socketEscuchador);
int aceptarConexion(int socket);

#endif /* SERVER_CLIENT_H_ */
