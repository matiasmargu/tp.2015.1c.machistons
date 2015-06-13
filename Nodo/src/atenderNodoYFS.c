/*
 * atenderNodoYFS.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */
#include "atenderNodoYFS.h"

void *atenderNFS(void*arg){

	int status;
	int i;
	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;
	int tamanioTotal;
	estructuraSetBloque set;
	int nroDelBloque;

	printf("%i\n",socket);

	pmap = mapearAMemoriaVirtual();

	while(1){
		//printf("Esto deberia imprimirse una sola vez\n");
		if(recv(socket, &entero, sizeof(int),0) > 0){
		switch(entero){
		//getBloque(numero);
			case 1:
				status = 1;
				recv(socket,&nroDelBloque,sizeof(int),0);
				char* bloque;

				nroDelBloque++;

				if(status>0){
					int tamanioBloque = 20 * 1024 * 1024;
					int tamanioBloqueExacto = tamanioBloque + nroDelBloque;
					memcpy(bloque,pmap + tamanioBloqueExacto,tamanioBloque);
					msync(bloque,strlen(bloque),0);
					status = 0;
				}
				int tamanioData = sizeof(int) + strlen(bloque) + 1;
				mensaje = serializarBloqueDeDatos(bloque,tamanioData);
				send(socket,mensaje,tamanioData,0);
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
				printf("setBloque Activo\n");
				recv(socket,&tamanioTotal,sizeof(int),0);
				status = 1; // Estructura que manjea el status de los recieve.
				status = recive_y_deserialisa_SET_BLOQUE(&set, socket, tamanioTotal);

				if (status>0) {
					// ACA TRABAJAN CON set.numero y set.bloque. Escriben el archivo y toda la bola.
					nroDelBloque = 1 + set.bloque;
					//memcpy(pmap+(1024*1024*20*(nroDelBloque)),set.data,20*1024*1024);
					memcpy(pmap+(nroDelBloque*20),set.data,strlen(set.data));
					msync(pmap,strlen(pmap),0);
					printf("\nse seteo correctamente\n");
					status = 0; //para salir del if
				}
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
