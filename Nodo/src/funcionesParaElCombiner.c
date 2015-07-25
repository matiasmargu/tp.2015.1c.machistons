/*
 * funcionesParaElCombiner.c
 *
 *  Created on: 25/7/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"

void getFileContent(int socket){
	int tamanioDelNombre;

	char* nombreAguardar;
	char* copiaDir;
	char* fileGetContent;
	char* mensaje;

	send(socket,&tamanioDelNombre,sizeof(int),0);//Para que no se boludee

	recv(socket,&tamanioDelNombre,sizeof(int),0);
	send(socket,&tamanioDelNombre,sizeof(int),0);
	nombreAguardar = malloc(tamanioDelNombre);

	recive_y_deserialisa_CHARp(nombreAguardar, socket, tamanioDelNombre);

	copiaDir=string_duplicate(dir_temp);
	string_append(&copiaDir,nombreAguardar);

	fileGetContent = mapearAMemoriaVirtual(copiaDir);
	char* buffer = malloc(strlen(fileGetContent));

	memcpy(buffer,fileGetContent,strlen(fileGetContent));

	mensaje = serializarBloqueDeDatos(buffer,strlen(fileGetContent));

	int tamanioData = strlen(fileGetContent);
	send(socket,&tamanioData,sizeof(int),0);
	recv(socket,&tamanioDelNombre,sizeof(int),0);
	send(socket,mensaje,strlen(fileGetContent),0);

	free(mensaje);
	free(buffer);

}

void pedirContenidoDeUnArchivo(char* nombre,int socket){
	int tamanioDelNombre = strlen(nombre);
	int tamanioData;

	char* dataDelFile;
	char* copiaDir;

	recv(socket,&tamanioData,sizeof(int),0);//Para que no se boludee

	send(socket,&tamanioDelNombre,sizeof(int),0);
	recv(socket,&tamanioDelNombre,sizeof(int),0);
	send(socket,&nombre,tamanioDelNombre,0);

	recv(socket,&tamanioData,sizeof(int),0);
	send(socket,&tamanioData,sizeof(int),0);

	dataDelFile = malloc(tamanioData);

	recive_y_deserialisa_CHARp(dataDelFile, socket, tamanioData);

	copiaDir=string_duplicate(dir_temp);
	string_append(&copiaDir,nombre);

	FILE* fdAGuardar = fopen(copiaDir,"w");
	fputs(dataDelFile,fdAGuardar);
	fclose(fdAGuardar);


}
