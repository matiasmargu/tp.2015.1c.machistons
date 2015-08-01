/*
 * funcionesParaElCombiner.c
 *
 *  Created on: 25/7/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"

void getFileContent(int socket){
	int tamanioDelNombre;
	int entero = 45;

	char* nombreAguardar;
	char* copiaDir;
	char* fileGetContent;
	char* mensaje;

	pthread_mutex_lock(&mutexJob);
	send(socket,&entero,sizeof(int),0);//Para que no se boludee
	recv(socket,&tamanioDelNombre,sizeof(int),0);
	nombreAguardar = malloc(tamanioDelNombre);
	send(socket,&entero,sizeof(int),0);
	pthread_mutex_unlock(&mutexJob);

	recv(socket,nombreAguardar,tamanioDelNombre,0);
	*(nombreAguardar + tamanioDelNombre) = '\0';
	printf("tam: %i\n",tamanioDelNombre);
	printf("nom: %s\n",nombreAguardar);
	asprintf(&copiaDir,"%s%s","/tmp/",nombreAguardar);
	*(copiaDir + tamanioDelNombre + strlen("/tmp/")) = '\0';
	printf("dir: %s\n",copiaDir);

	fileGetContent = mapearAMemoriaVirtual(copiaDir);
	char* buffer = malloc(strlen(fileGetContent));

	memcpy(buffer,fileGetContent,strlen(fileGetContent));


	int tamanioData = strlen(fileGetContent);
	printf("tam data: %i\n",tamanioData);
	send(socket,&tamanioData,sizeof(int),0);
	recv(socket,&tamanioDelNombre,sizeof(int),0);
	send(socket,buffer,tamanioData,0);
}

void pedirContenidoDeUnArchivo(char* nombre,int socket){
	int tamanioDelNombre = strlen(nombre);
	int tamanioData,tamanioReal, tamanioLeido, entero;
	char* contenidoArchivoAux;

	char* dataDelFile;
	char* copiaDir;

	recv(socket,&tamanioData,sizeof(int),0);//Para que no se boludee

	send(socket,&tamanioDelNombre,sizeof(int),0);
	recv(socket,&tamanioDelNombre,sizeof(int),0);
	send(socket,nombre,tamanioDelNombre,0);

	recv(socket,&tamanioData,sizeof(int),0);
	send(socket,&tamanioData,sizeof(int),0);

	dataDelFile = malloc(tamanioData);

	tamanioLeido=0;
	while(tamanioLeido!=tamanioData){
		contenidoArchivoAux=malloc(64*1024);
		tamanioReal=recv(socket,contenidoArchivoAux,64*1024,0);
		if(tamanioReal<0) return;
		memcpy(dataDelFile+tamanioLeido,contenidoArchivoAux,tamanioReal);
		tamanioLeido+=tamanioReal;
	}

	asprintf(&copiaDir,"%s%s","/tmp/",nombre);
	FILE* fdAGuardar = fopen(copiaDir,"w");
	fputs(dataDelFile,fdAGuardar);
	fclose(fdAGuardar);


}
