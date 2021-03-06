/*
 * pedidos_fs.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

void inicializar_pedido_FS(char *nombreArch){
		printf("NOMBRE ARCHIVO EJEMPLO: %s\n",nombreArch);
		int tam = strlen(nombreArch);
		int handshakeFS = 72; // pido info de archivos
	   	send(socketFS,&handshakeFS, sizeof(int), 0);
	   	recv(socketFS,&handshakeFS, sizeof(int),0);
	   	send(socketFS,&tam, sizeof(int), 0);
	   	recv(socketFS,&handshakeFS, sizeof(int),0);
	   	printf("Nombre_archivo: %s\n",nombreArch);
	   	printf("tamanio antes del send: %i\n",tam);
	   	tam = send(socketFS,nombreArch, tam, 0);
	   	printf("tamanio despues del send: %i\n",tam);
}

int comparar(t_nodo *infoNodo){
	if (infoNodo->id_nodo == 1) return true;
	else return false;
}

void recive_y_guarda_infoNodo(int tamanio, int socket, void *lista_nodos){
	//Esquema de la estructura que recibo del FileSystem:
	//[ID_NODO][TAMAÑO_IP][IP_NODO][TAMAÑO_PUERTO][PUERTO_NODO]....
	//Previamente recibo el tamaño total de esto
	int offset = 0;
	t_nodo *infoNodo;
	void *buffer = malloc(tamanio);
	recv(socket,buffer,tamanio,0);
	pthread_mutex_lock(&mutex_cant_nodos);
		memcpy(&cant_nodos,buffer + offset,sizeof(int));
	pthread_mutex_unlock(&mutex_cant_nodos);
	offset += sizeof(int);
	contadores_nodos = malloc(sizeof(int)*cant_nodos);
	int i;
	for(i=0;i<cant_nodos;i++){
		infoNodo = malloc(sizeof(t_nodo));
		memcpy(&(infoNodo->id_nodo),buffer + offset,sizeof(int));
		offset += sizeof(int);

		memcpy(&tamanio,buffer+offset,sizeof(int));
		offset += sizeof(int);
		infoNodo->ip_nodo = malloc(tamanio);
		memcpy(infoNodo->ip_nodo,buffer + offset,tamanio);
		offset += tamanio;

		memcpy(&tamanio,buffer+offset,sizeof(int));
		offset += sizeof(int);
		infoNodo->puerto_nodo = malloc(tamanio);
		memcpy(infoNodo->puerto_nodo,buffer + offset,tamanio);
		offset += tamanio;


		printf("idNodo: %i\n ipNodo: %s\n puertoNodo: %s\n",infoNodo->id_nodo, infoNodo->ip_nodo, infoNodo->puerto_nodo);
		list_add(lista_nodos,infoNodo);
		printf("tamaño de la lista dentro de la funcion: %i\n\n",list_size(lista_nodos));

		//inicializo los contadores para planificar en 0
		contadores_nodos[i] = 0;
	}
	//infoNodo = list_find(lista_nodos, (void*) comparar);
	//printf("ID del que probamos: %i\n", infoNodo->id_nodo);

}

int recive_y_guarda_estructura(t_archivo *arch, int socket, uint32_t tamanioTotal){
	char *buffer = malloc(tamanioTotal);
	int offset = 0;
	recv(socket, buffer, tamanioTotal, 0);
	//LO QUE RECIBO TIENE ESTA ESTRCTURA: [TAM_ESTRUC][CANT_BLOQ]   [NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ]
	// COPIO LA CANT_BLOQUES DEL ARCHIVO
	memcpy(&(arch->cantidadDeBloques), buffer+offset, sizeof(int));
	offset += sizeof(int);
	// COPIO LOS BLOQUES QUE CONTIENE EL ARCHIVO A UN ESTRUCTURA DE LISTA
	int i,j;
	t_bloque *bloque;
	t_copia copia_aux;
	for(i=0; i<arch->cantidadDeBloques; i++){
		bloque = malloc(sizeof(t_bloque));
		memcpy(&(bloque->NumeroBloque), buffer+offset, sizeof(int));
		offset += sizeof(int);
		//printf("numero de bloque: %i\n",bloque->NumeroBloque);
		bloque->copias = malloc(sizeof(t_copia)*3);

		for(j=0;j<3;j++){
			memcpy(&(copia_aux.idNodo), buffer+offset, sizeof(int));
			bloque->copias[j].idNodo = copia_aux.idNodo;
			offset += sizeof(int);
			memcpy(&(copia_aux.Numerobloque), buffer+offset, sizeof(int));
			bloque->copias[j].Numerobloque = copia_aux.Numerobloque;
			offset += sizeof(int);
			//printf("idnodo: %i, numbloque: %i\n", bloque->copias[j].idNodo, bloque->copias[j].Numerobloque);
		}
		list_add_in_index(arch->bloques, bloque->NumeroBloque, bloque);
	}


	return 0;
}
