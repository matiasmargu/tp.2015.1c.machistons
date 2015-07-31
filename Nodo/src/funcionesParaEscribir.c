/*
 * funcionesParaEscribir.c
 *
 *  Created on: 19/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

char* escribirScript(char* script_virtual, int comando,int socket){

	char* resultado;


	if(comando==1){

		asprintf(&resultado,"%s%s","/tmp/mapper",string_itoa(socket));
		//remove("/tmp/mapper");
		FILE* fileMapper=fopen(resultado,"w");
		fputs(script_virtual,fileMapper);
		fclose(fileMapper);

	}else{

		asprintf(&resultado,"%s%s","/tmp/reduce",string_itoa(socket));
		//remove("/tmp/mapper");
		FILE* fileReducer=fopen(resultado,"w");
		fputs(script_virtual,fileReducer);
		fclose(fileReducer);

	}
	return resultado;
}
