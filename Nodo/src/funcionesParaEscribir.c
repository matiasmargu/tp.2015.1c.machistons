/*
 * funcionesParaEscribir.c
 *
 *  Created on: 19/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

void escribirScript(char* script_virtual, int comando){

	if(comando==1){

		remove("/tmp/mapper");
		FILE* fileMapper=fopen("/tmp/mapper","w");
		fputs(script_virtual,fileMapper);
		fclose(fileMapper);

	}else{

		remove("/tmp/mapper");
		FILE* fileReducer=fopen("/tmp/reducer","w");
		fputs(script_virtual,fileReducer);
		fclose(fileReducer);
	}

	printf("termino de escribir script\n");
}
