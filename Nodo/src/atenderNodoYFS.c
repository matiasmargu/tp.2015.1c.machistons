/*
 * atenderNodoYFS.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */
#include "atenderNodoYFS.h"

void *atenderNFS(void*arg){

	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;
	int tamanioTotal;
	int fd;
	estructuraSetBloque set;
	struct stat mystat;
	char* pmap;
	int nroDelBloque;

	printf("%i\n",socket);

	while(1){
		//printf("Esto deberia imprimirse una sola vez\n");
		if(recv(socket, &entero, sizeof(int),0) > 0){
		switch(entero){
		//getBloque(numero);
			case 1:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
				printf("setBloque Activo\n");
				recv(socket,&tamanioTotal,sizeof(int),0);
				int status = 1; // Estructura que manjea el status de los recieve.
				status = recive_y_deserialisa_SET_BLOQUE(&set, socket, tamanioTotal);
				if (status) {
					// ACA TRABAJAN CON set.numero y set.bloque. Escriben el archivo y toda la bola.
					printf("%i\n",set.bloque);
					printf("%s\n",set.data);
					nroDelBloque = set.bloque;
					pmap[nroDelBloque * 20 * 1024 * 1024] = set.data;
				}
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//getFileContent(nombre);
			case 3:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		}
	}
	}
	return NULL;
}
