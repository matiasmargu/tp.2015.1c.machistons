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

void conectarseAlNodo(t_conectarseAlNodo);
char* serializarMapper(FILE*);
char* serializarJob_Nodo_Mapper(t_job_nodo_mapper );

void liberarMensaje(char **);
/*void decrementar();
void imprimir();
void imprimir2();
void incrementar();
*/
#endif /* FUNCIONES_H_ */
