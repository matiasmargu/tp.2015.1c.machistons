/*
 * consola.h
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

#define MAXSIZE_COMANDO 50
#define MAXCOMANDOS 6
#define MAXBUFERTECLADO 1000

#include "manejoDeListas.h"

void imprimirMenu(void);
void *atenderConsola(void*arg);

#endif /* CONSOLA_H_ */
