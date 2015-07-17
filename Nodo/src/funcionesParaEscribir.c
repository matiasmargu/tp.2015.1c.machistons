/*
 * funcionesParaEscribir.c
 *
 *  Created on: 19/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"



int escribirScript(char* script_virtual,char* dir, int comando){
	int status = 0;
	char* direccion=malloc(strlen(dir));
	FILE* fileMapper;
	FILE* fileReducer;

	strcpy(direccion,dir);

	printf("Antes de escribir: %s\n",direccion);

	if(comando==1){
		strcat(direccion,"/mapper");
		printf("Antes de escribir: %s\n",direccion);
		fileMapper=fopen(direccion,"w");
		fputs(script_virtual,fileMapper);
		fclose(fileMapper);
	}else{
		strcat(direccion,"/reducer");
		fileReducer=fopen(direccion,"w");
		fputs(script_virtual,fileReducer);
		fclose(fileReducer);
	}
	liberar(&direccion);
	return status;
}
