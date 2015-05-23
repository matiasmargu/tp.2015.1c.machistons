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

void conectarseAlNodoMapper(int,struct job_nodo);
void conectarseAlNodoReducer(int,struct job_nodo );

//void conectarseAlNodoReducer(char*) Falta hacer
/*void decrementar();
void imprimir();
void imprimir2();
void incrementar();
*/
#endif /* FUNCIONES_H_ */
