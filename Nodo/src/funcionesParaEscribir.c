/*
 * funcionesParaEscribir.c
 *
 *  Created on: 19/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"


int escribirScript(char* script_virtual,char* dir_temp, int comando){
	int status = 0;
	char* direccion=dir_temp;

	if(comando==1){
		strcat(direccion,"/map.txt");
		fileMapper=fopen(dir_temp,"w");
		fputs(script_mapper,fileMapper);
		fclose(fileMapper);
	}else{
		strcat(direccion,"/reducer.txt");
		fileReducer=fopen(dir_temp,"w");
		fputs(script_reducer,fileReducer);
		fclose(fileReducer);
	}
	free(direccion);
	return status;
}
