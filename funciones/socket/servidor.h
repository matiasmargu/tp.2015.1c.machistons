/*
 * servidor.h
 *
 *  Created on: 6/5/2015
 *      Author: utnso
 */

#ifndef SOCKET_SERVIDOR_H_
#define SOCKET_SERVIDOR_H_

#define BACKLOG 5			// Define cuantas conexiones vamos a mantener pendientes al mismo tiempo
#define PACKAGESIZE 1024	// Define cual va a ser el size maximo del paquete a enviar

int crearServidor(char* PUERTO);

#endif /* SOCKET_SERVIDOR_H_ */
