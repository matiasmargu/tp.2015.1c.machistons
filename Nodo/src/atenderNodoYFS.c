/*
 * atenderNodoYFS.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */
#include "atenderNodoYFS.h"

void getBloque(int status,char* mensaje){
	int nroDelBloque;
	int tamanioBloque;

	status = 1;
	recv(socket,&nroDelBloque,sizeof(int),0);
	recv(socket,&tamanioBloque,sizeof(int),0);

	char* bloque=malloc(tamanioBloque);

	if(status>0){
		int tamanioBloqueExacto = (nroDelBloque)*tamanioBloque;
		memcpy(bloque,pmap + tamanioBloqueExacto,tamanioBloque);
		status = 0;
	}

	int tamanioData = sizeof(int) + strlen(bloque) + 1;
	//printf("%i\n",tamanioData);
	mensaje = serializarBloqueDeDatos(bloque,tamanioData);
	send(socket,&tamanioData,sizeof(int),0);
	send(socket,mensaje,tamanioData,0);
}

void setBloque(int status){

	estructuraSetBloque set;
	int tamanioTotal;
	int nroDelBloque;

	recv(socket,&tamanioTotal,sizeof(int),0);
	status = 1; // Estructura que manjea el status de los recieve.
	status = recive_y_deserialisa_SET_BLOQUE(&set, socket, tamanioTotal);

	if (status>0) {
		nroDelBloque = set.bloque;//
		//memcpy(pmap+(1024*1024*20*(nroDelBloque)),set.data,20*1024*1024);
		memcpy(pmap+(nroDelBloque),set.data,strlen(set.data));
		msync(pmap,strlen(pmap),0);
		printf("\nse seteo correctamente\n");
	}
}

void *atenderNFS(void*arg){

	char *mensaje;
	int status;
	int i;
	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;

	printf("%i\n",socket);

	pmap = mapearAMemoriaVirtual();

	while(1){
		//printf("Esto deberia imprimirse una sola vez\n");
		if(recv(socket, &entero, sizeof(int),0) > 0){
		switch(entero){
		//getBloque(numero);
			case 1:
				getBloque(status,mensaje);
				//ok = 20;
				//send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
				setBloque(status);
				//ok = 20;
				//send(socket,&ok, sizeof(int),0);
			break;
		//getFileContent(nombre);
			case 3:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		}
	}
	}
	munmap(pmap,strlen(pmap));
	return NULL;
}
