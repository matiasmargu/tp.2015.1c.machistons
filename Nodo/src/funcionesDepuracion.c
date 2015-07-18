/*
 * funcionesDepuracion.c
 *
 *  Created on: 17/7/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"


//Elimina los enters de un buffer
void eliminarEnters(char* buffer, int tamanio){
	int a;

	for(a=0;a<tamanio;a++){
		char exami= buffer[a];

		if(exami == '\n'){
			buffer[a]='\0';
		}

	}

}
