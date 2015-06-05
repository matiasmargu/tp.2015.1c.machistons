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
void serializarMapper();
void serializadorJob_Marta_Inicio();

void liberarMensaje(char **);
/*void decrementar();
void imprimir();
void imprimir2();
void incrementar();
*/
#endif /* FUNCIONES_H_ */
