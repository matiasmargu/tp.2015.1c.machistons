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

	char* pmap = mapearAMemoriaVirtual(archivo_bin);

	if(string_equals_ignore_case(nodo_nuevo,"SI")){
		formatearArchivo(pmap);
	}


	tamanioArchivo_BIN= tamanioDelArchivoBIN();

	printf("Este es el socket: %i\n",socket);


	while(1){
	//printf("Esto deberia imprimirse una sola vez\n");
	if(recv(socket, &entero, sizeof(int),0) > 0){
	switch(entero){
	//getBloque(numero);
		case 1:
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

			memcpy(pmap+(1024*1024*20*(nroDelBloque)),bufferSet,tamanioBloque);

			msync(pmap,tamanioArchivo_BIN,0);
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

			int tamanioData = strlen(file);
			send(socket,&tamanioData,sizeof(int),0);
			send(socket,mensaje,strlen(file),0);
			free(mensaje);
			free(buffer);

			//ok = 20;
			//send(socket,&ok, sizeof(int),0);
		break;
		case 4:
		//FORMATEO
			printf("Me mando a formatear el archivo\n");
			formatearArchivo(pmap);
			printf("Se formateo correctamente\n");
		break;
		case 5:
		//FORMATEO DE BLOQUE
			send(socket,&entero,sizeof(int),0);
			recv(socket,&nroDelBloque,sizeof(int),0);

			printf("Se esta formateando el bloque...\n");
			formatearBloque(pmap,nroDelBloque);
			printf("Formateo del bloque %i completado\n",nroDelBloque);

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
