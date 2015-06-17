/*
 * funcionesParaEnviar.h
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESPARAENVIAR_H_
#define FUNCIONESPARAENVIAR_H_

#include "variablesGlobales.h"

int recive_y_deserialisa_SET_BLOQUE(estructuraSetBloque *bloque, int socket, uint32_t tamanioTotal);
char* serializarIPyPUERTO(char* ip_fs,char* puerto_fs, int tamanioData);
char* serializarBloqueDeDatos(char* bloque, int tamanioData);
void handshakeConFS ();
void handshakeConJob(int socket_job);

#endif /* FUNCIONESPARAENVIAR_H_ */
