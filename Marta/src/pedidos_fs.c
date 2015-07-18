/*
 * pedidos_fs.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

void inicializar_pedido_FS(){
		char *nombre_archivo = "201303hourly.txt";
		int tam = strlen(nombre_archivo);
		int handshakeFS = 72; // pido info de archivos
	   	send(socketFS,&handshakeFS, sizeof(int), 0);
	   	recv(socketFS,&handshakeFS, sizeof(int),0);

	   	send(socketFS,&tam, sizeof(int), 0);
	   	recv(socketFS,&handshakeFS, sizeof(int),0);

	   	send(socketFS,nombre_archivo, tam, 0);
}

void recive_y_guarda_infoNodo(t_nodo *infoNodo, int tamanio, int socket, t_list *infoNodos){
	//Esquema de la estructura que recibo del FileSystem:
	//[ID_NODO][TAMAÑO_IP][IP_NODO][TAMAÑO_PUERTO][PUERTO_NODO]....
	//Previamente recibo el tamaño total de esto
	int offset = 0;
	void *buffer = malloc(tamanio);
	recv(socket,buffer,tamanio,0);
	int i;
	for(i=0;i<cant_nodos;i++){
		memcpy(&(infoNodo->id_nodo),buffer + offset,sizeof(int));
		offset += sizeof(int);

		memcpy(&tamanio,buffer+offset,sizeof(int));
		offset += sizeof(int);
		memcpy(&(infoNodo->ip_nodo),buffer + offset,tamanio);
		offset += tamanio;

		memcpy(&tamanio,buffer+offset,sizeof(int));
		offset += sizeof(int);
		memcpy(&(infoNodo->puerto_nodo),buffer + offset,tamanio);

		list_add_in_index(infoNodos,(infoNodo->id_nodo),infoNodo);
	}
}

int recive_y_guarda_estructura(t_archivo *arch, int socket, uint32_t tamanioTotal){
	char *buffer = malloc(tamanioTotal);
	int offset = 0;
	recv(socket, buffer, tamanioTotal, 0);
	printf("recibe el buffer\n\n");
	//LO QUE RECIBO TIENE ESTA ESTRCTURA: [TAM_ESTRUC][CANT_BLOQ]   [NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ]
	// COPIO LA CANT_BLOQUES DEL ARCHIVO
	memcpy(&(arch->cantidadDeBloques), buffer+offset, sizeof(int));
	printf("aaaaaaaaaaaaaa\n");
	offset += sizeof(int);
	// COPIO LOS BLOQUES QUE CONTIENE EL ARCHIVO A UN ESTRUCTURA DE LISTA
	int i;
	t_bloque *bloque;
	t_bloque *bloque2 = malloc(sizeof(t_bloque));
	//t_copia **copia = malloc(3*sizeof(t_copia));
	t_copia copia_aux;
	printf("antes de entrar al for\n\n");
	for(i=0; i<arch->cantidadDeBloques; i++){
		bloque = malloc(sizeof(t_bloque));
		memcpy(&(bloque->NumeroBloque), buffer+offset, sizeof(int));
		offset += sizeof(int);
		printf("numero de bloque: %i\n",bloque->NumeroBloque);


		memcpy(&(copia_aux.idNodo), buffer+offset, sizeof(int));
		bloque->copia1_idnodo = copia_aux.idNodo;
		offset += sizeof(int);
		memcpy(&(copia_aux.Numerobloque), buffer+offset, sizeof(int));
		bloque->copia1_numbloque = copia_aux.Numerobloque;
		offset += sizeof(int);
		printf("idnodo: %i, numbloque: %i\n", bloque->copia1_idnodo, bloque->copia1_numbloque);

		memcpy(&(copia_aux.idNodo), buffer+offset, sizeof(int));
		bloque->copia2_idnodo = copia_aux.idNodo;
		offset += sizeof(int);
		memcpy(&(copia_aux.Numerobloque), buffer+offset, sizeof(int));
		bloque->copia2_numbloque = copia_aux.Numerobloque;
		offset += sizeof(int);
		printf("idnodo: %i, numbloque: %i\n", bloque->copia2_idnodo, bloque->copia2_numbloque);

		memcpy(&(copia_aux.idNodo), buffer+offset, sizeof(int));
		bloque->copia3_idnodo = copia_aux.idNodo;
		offset += sizeof(int);
		memcpy(&(copia_aux.Numerobloque), buffer+offset, sizeof(int));
		bloque->copia3_numbloque = copia_aux.Numerobloque;
		offset += sizeof(int);
		printf("idnodo: %i, numbloque: %i\n", bloque->copia3_idnodo, bloque->copia3_numbloque);
		printf("numero de bloque prueba: %i\n\n",bloque->NumeroBloque);

		list_add_in_index(arch->bloques, bloque->NumeroBloque, &bloque);
		//t_bloque *bloquePrueba = list_get(arch.bloques, bloque.NumeroBloque);
		//printf("numero de bloque desp de guardarlo: %i\n",bloquePrueba->NumeroBloque);


	}
	/*
	int *das2 = 20;
	list_add_in_index(arch.bloques, 2,&das2);
	*das2 = 70;
	list_add_in_index(arch.bloques,1,&das2);

	int *das = 0;
	das = list_get(arch.bloques,2);
	printf("%i\n",*das);
	das = list_get(arch.bloques,1);
	printf("%i\n",*das);
	*/

	return 0;
}
