/*
 * atenderNodoYFS.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"


void *atenderNFS(void*arg){

	char *mensaje;
	char *direccion;
	char *bufferSet;
	char* paquetito;

	int i,offset,tamanioDelPaquetito;
	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int nroDelBloque;
	int tamanioBloque;
	int n;
	int cantidadDePaquetitos;

	int variableDelPaquetito = 64*1024;
	int tamanioReal;

	char* pmap = mapearAMemoriaVirtual(archivo_bin);

	printf("Este es el socket: %i\n",socket);


	while(1){
	//printf("Esto deberia imprimirse una sola vez\n");
	if(recv(socket, &entero, sizeof(int),0) > 0){
	switch(entero){
	//getBloque(numero);
		case 1:
			send(socket,&entero,sizeof(int),0);
			recv(socket,&nroDelBloque,sizeof(int),0);

			tamanioBloque=tamanioEspecifico(pmap,nroDelBloque);

			printf("%i\n",tamanioBloque);
			char* bloque=malloc(tamanioBloque);

			int tamanioBloqueExacto = nroDelBloque * 1024 * 1024* 20;
			memcpy(bloque,pmap + tamanioBloqueExacto,tamanioBloque);

			int tamanioData = sizeof(int) + strlen(bloque) + 1;
			//printf("%i\n",tamanioData);
			mensaje = serializarBloqueDeDatos(bloque,tamanioData);
			send(socket,&tamanioData,sizeof(int),0);
			send(socket,mensaje,tamanioData,0);
			free(mensaje);
			free(bloque);
		//ok = 20;
		//send(socket,&ok, sizeof(int),0);
		break;
	//setBloque(numero,[datos]);
		case 2:
			send(socket,&entero,sizeof(int),0);
			recv(socket,&nroDelBloque,sizeof(int),0);

			send(socket,&entero,sizeof(int),0);
			recv(socket,&tamanioBloque,sizeof(int),0);

			printf("El tamaño del buffer: %i\n",tamanioBloque);

			send(socket,&entero,sizeof(int),0);
			bufferSet=malloc(tamanioBloque);

			offset = 0;
			while(tamanioBloque != offset){

				if((tamanioBloque-offset)<variableDelPaquetito){
					tamanioDelPaquetito = tamanioBloque-offset;
				}else{
					tamanioDelPaquetito = variableDelPaquetito;
				}

				paquetito = malloc(tamanioDelPaquetito);

				tamanioReal = recv(socket,paquetito,tamanioDelPaquetito,0);

				memcpy(bufferSet + offset,paquetito,tamanioReal);
				offset += tamanioReal;

				liberar(&paquetito);
			}

			printf("Este es el tamaño del buffer set DESPUES: %i\n",strlen(bufferSet));

			memcpy(pmap+(1024*1024*20*(nroDelBloque)),bufferSet,strlen(bufferSet));
			msync(pmap,strlen(pmap),0);
			printf("se seteo correctamente\n");
			liberar(&bufferSet);
			//ok = 20;
			//send(socket,&ok, sizeof(int),0);
		break;
		//getFileContent(nombre);
		case 3:
			recv(socket,&tamanioBloque,sizeof(int),0);
			recive_y_deserialisa_CHARp(direccion, socket, tamanioBloque);

			char* direccionAuxiliar = malloc(strlen(direccion)+strlen(dir_temp)+1);

			direccionAuxiliar = dir_temp;

			strcat(direccionAuxiliar,"/");
			strcat(direccionAuxiliar,direccion);

			char* file = mapearAMemoriaVirtual(direccionAuxiliar);
			char* buffer = malloc(strlen(file));

			memcpy(buffer,file,strlen(file));

			mensaje = serializarBloqueDeDatos(buffer,strlen(file));

			tamanioData = strlen(file);
			send(socket,&tamanioData,sizeof(int),0);
			send(socket,mensaje,strlen(file),0);
			free(mensaje);
			free(buffer);

			//ok = 20;
			//send(socket,&ok, sizeof(int),0);
		break;
		case 4:
		//FORMATEO
			n = strlen(pmap);
			for(i=0;i<n;i++){
				pmap[i]='/';
			}
			msync(pmap,strlen(pmap),0);
			printf("se formateo el archivo binario\n");
		break;
	}
}
}
	munmap(pmap,strlen(pmap));
	return NULL;
}
