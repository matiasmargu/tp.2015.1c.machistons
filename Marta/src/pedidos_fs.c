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

int recive_y_guarda_estructura(t_archivo arch, int socket, uint32_t tamanioTotal){
	char *buffer = malloc(tamanioTotal);
	int offset = 0;
	recv(socket, buffer, tamanioTotal, 0);
	//LO QUE RECIBO TIENE ESTA ESTRCTURA: [TAM_ESTRUC][CANT_BLOQ]   [NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ]
	// COPIO LA CANT_BLOQUES DEL ARCHIVO
	memcpy(&(arch.cantidadDeBloques), buffer+offset, sizeof(int));
	offset += sizeof(int);
	// COPIO LOS BLOQUES QUE CONTIENE EL ARCHIVO A UN ESTRUCTURA DE LISTA
	int i,j;
	t_bloque bloque;
	t_copia *copia;
	t_copia copia_aux;
	for(i=0; i<arch.cantidadDeBloques; i++){
		memcpy(&(bloque.NumeroBloque), buffer+offset, sizeof(int));
		offset += sizeof(int);
		bloque.copias = list_create();
		for(j=0; j<3; j++){
			memcpy(&(copia_aux.idNodo), buffer+offset, sizeof(int));
			copia->idNodo = copia_aux.idNodo;
			offset += sizeof(int);
			memcpy(&(copia_aux.Numerobloque), buffer+offset, sizeof(int));
			copia->Numerobloque = copia_aux.Numerobloque;
			offset += sizeof(int);

			list_add_in_index(bloque.copias,j,copia);
		}
		list_add_in_index(arch.bloques, bloque.NumeroBloque, &bloque);
	}

	return 0;
}
