/*
 * funciones.h
 *
 *  Created on: 9/5/2015
 *      Author: utnso
 */

#ifndef FUNCIONES_H_
#define FUNCIONES_H_
#include <sys/socket.h>
#include <socket/socket.h>
#include <stdio.h>
#include <stdlib.h>

void conectarseAlNodo(t_conectarseAlNodo);
char* serializarMapper(t_job_nodo jn);
char* serializarJob_Nodo_Mapper(t_job_nodo_mapper );
//int recive_y_deserialisa(t_marta_job2 *bloque, int socket, uint32_t tamanioTotal);

void liberarMensaje(char *);

#endif /* FUNCIONES_H_ */
