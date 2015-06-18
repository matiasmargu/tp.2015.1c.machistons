/*
 * atenderNodoYFS.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */
#include "atenderNodoYFS.h"


void *atenderNFS(void*arg){

	char *mensaje;
	int status;
	int i;
	int socket= (int)arg;
	int entero; // handshake para saber quien es: FS(23)
	int ok;
	int nroDelBloque;
	int tamanioBloque;
	estructuraSetBloque set;


	pmap = mapearAMemoriaVirtual();

	div_t divisor = div(strlen(pmap),20);
	int array[divisor.quot];

	for(i=0;i<divisor.quot;i++){
		array[i]=0;
	}

	printf("%i\n",socket);

	while(1){

		//printf("Esto deberia imprimirse una sola vez\n");
		if(recv(socket, &entero, sizeof(int),0) > 0){
		switch(entero){
		//getBloque(numero);
			case 1:
				status = 1;
				recv(socket,&nroDelBloque,sizeof(int),0);

				tamanioBloque = array[nroDelBloque];

				char* bloque=malloc(tamanioBloque);

				if(status>0){
					int tamanioBloqueExacto = (nroDelBloque)*tamanioBloque;
					memcpy(bloque,pmap + tamanioBloqueExacto,tamanioBloque);
					status = 0;
				}
				printf("Element[%d] = %d\n", nroDelBloque, array[nroDelBloque]);
				int tamanioData = sizeof(int) + strlen(bloque) + 1;
				//printf("%i\n",tamanioData);
				mensaje = serializarBloqueDeDatos(bloque,tamanioData);
				send(socket,&tamanioData,sizeof(int),0);
				send(socket,mensaje,tamanioData,0);
				//ok = 20;
				//send(socket,&ok, sizeof(int),0);
			break;
		//setBloque(numero,[datos]);
			case 2:
				recv(socket,&tamanioBloque,sizeof(int),0);
				status = 1; // Estructura que manjea el status de los recieve.
				status = recive_y_deserialisa_SET_BLOQUE(&set, socket, tamanioBloque);

				printf("%i\n",strlen(set.data));
				array[set.bloque]=strlen(set.data);

				if (status>0) {
					nroDelBloque = set.bloque;//
					//memcpy(pmap+(1024*1024*20*(nroDelBloque)),set.data,20*1024*1024);
					memcpy(pmap+(nroDelBloque*10),set.data,strlen(set.data));
					msync(pmap,strlen(pmap),0);
					printf("se seteo correctamente\n");
				}
				printf("Element[%d] = %d\n", nroDelBloque, array[nroDelBloque]);
				//ok = 20;
				//send(socket,&ok, sizeof(int),0);
			break;
		//getFileContent(nombre);
			case 3:
				ok = 20;
				send(socket,&ok, sizeof(int),0);
			break;
			case 4:
		//FORMATEO
				for(i=0;i<strlen(pmap);i++){
					memcpy(pmap+i,"0",sizeof(char));
				}
				msync(pmap,strlen(pmap),0);
				printf("se formatero el archivo binario\n");
			break;
		}
	}
	}
	munmap(pmap,strlen(pmap));
	return NULL;
}
