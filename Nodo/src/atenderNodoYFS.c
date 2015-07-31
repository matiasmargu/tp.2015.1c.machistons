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
	char* bloqueGet;

	int offset,tamanioDelPaquetito;
	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int nroDelBloque;
	int tamanioBloque;
	int tamanioBloqueExacto;
	int loQueMande;

	int variableDelPaquetito = 64*1024;
	int tamanioReal;

	char* pmap;

	if(string_equals_ignore_case(nodo_nuevo,"SI")){
		pmap = mapearAMemoriaVirtual(archivo_bin);

		formatearArchivo(pmap);

		munmap(pmap,tamanioArchivo_BIN);
	}


	tamanioArchivo_BIN= tamanioDelArchivoBIN();

	printf("Este es el socket: %i\n",socket);


	while(1){
	//printf("Esto deberia imprimirse una sola vez\n");
	if(recv(socket, &entero, sizeof(int),0) > 0){
	switch(entero){
	//getBloque(numero);
		case 1:

			pmap = mapearAMemoriaVirtual(archivo_bin);

			send(socket,&entero,sizeof(int),0);
			recv(socket,&nroDelBloque,sizeof(int),0);

			printf("%i\n",nroDelBloque);

			tamanioBloque=tamanioEspecifico(pmap,nroDelBloque);

			printf("Este es el tamaño del bloque por tamanioESP es: %i\n",tamanioBloque);

			bloqueGet=malloc(tamanioBloque);

			tamanioBloqueExacto = nroDelBloque * 1024 * 1024* 20;
			memcpy(bloqueGet,pmap + tamanioBloqueExacto,tamanioBloque);

			printf("Este es el posta: %i\n",strlen(bloqueGet));

			send(socket,&tamanioBloque,sizeof(int),0);
			recv(socket,&entero,sizeof(int),0);

			loQueMande = send(socket,bloqueGet,tamanioBloque,0);
			printf("Lo envie y esto fue el tamaño que envie: %i\n",loQueMande);

			munmap(pmap,tamanioArchivo_BIN);

		//ok = 20;
		//send(socket,&ok, sizeof(int),0);
		break;
	//setBloque(numero,[datos]);
		case 2:

			pmap = mapearAMemoriaVirtual(archivo_bin);

			send(socket,&entero,sizeof(int),0);
			recv(socket,&nroDelBloque,sizeof(int),0);

			printf("El nro de bloque: %i\n",nroDelBloque);

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

			memcpy(pmap+(1024*1024*20*(nroDelBloque)),bufferSet,tamanioBloque);

			msync(pmap,tamanioArchivo_BIN,0);
			printf("se seteo correctamente\n");
			liberar(&bufferSet);

			munmap(pmap,tamanioArchivo_BIN);
			//ok = 20;
			//send(socket,&ok, sizeof(int),0);
		break;
		//getFileContent(nombre);
		case 3:
			getFileContent(socket);

			//ok = 20;
			//send(socket,&ok, sizeof(int),0);
		break;
		case 4:
		//FORMATEO

			pmap = mapearAMemoriaVirtual(archivo_bin);

			printf("Me mando a formatear el archivo\n");
			formatearArchivo(pmap);
			printf("Se formateo correctamente\n");

			munmap(pmap,tamanioArchivo_BIN);
		break;
		case 5:
		//FORMATEO DE BLOQUE
			pmap = mapearAMemoriaVirtual(archivo_bin);

			send(socket,&entero,sizeof(int),0);
			recv(socket,&nroDelBloque,sizeof(int),0);

			printf("Se esta formateando el bloque...\n");
			formatearBloque(pmap,nroDelBloque);
			printf("Formateo del bloque %i completado\n",nroDelBloque);

			munmap(pmap,tamanioArchivo_BIN);
		break;
	}
}
}
	liberar(&mensaje);
	liberar(&direccion);
	liberar(&bufferSet);
	liberar(&paquetito);
	liberar(&bloqueGet);

	munmap(pmap,tamanioArchivo_BIN);
	return NULL;
}
