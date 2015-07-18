/*
 * funcionesParaEscribir.c
 *
 *  Created on: 19/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"



int escribirScript(char* script_virtual, int comando){
	int status = 0;
	FILE* fileMapper;
	FILE* fileReducer;

	printf("prueba escribir script\n");


	if(comando==1){
		fileMapper=fopen("/tmp/mapper","w");
		fputs(script_virtual,fileMapper);
		fclose(fileMapper);
	}else{
		fileReducer=fopen("/tmp/reducer","w");
		fputs(script_virtual,fileReducer);
		fclose(fileReducer);
	}

	printf("termino de escribir script\n");

	return status;
}
