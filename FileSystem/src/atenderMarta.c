/*
 * atenderMarta.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

typedef struct{
	char* archivo;
}t_charpuntero;

int recive_y_deserialisa(t_charpuntero* nombre, int socket, uint32_t tamanioTotal){
	int status = 1;
	char *buffer = malloc(tamanioTotal);
	int offset=0;
	recv(socket, buffer, tamanioTotal, 0);
	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	nombre->archivo = malloc(tamanioDinamico);
	memcpy(nombre->archivo, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;
	free(buffer);
	return status;
}


void *atenderMarta(void*arg){

	int socketMarta = (int)arg;
	printf("%i\n",socketMarta);
	int tamanioArchivo;
	t_charpuntero nombreArchivo;

	int tamanioTotal;

	char *paqueteAEnviar;

	t_archivo archivo;
	bson_iter_t iter;
	t_copia info;
	int nroBloque;
	int nroCopia;

	int offset;
	int size_to_send;

	for(;;){
		recv(socketMarta, &tamanioArchivo, sizeof(tamanioArchivo),0);
		if(recive_y_deserialisa(&nombreArchivo, socketMarta, tamanioArchivo)){

			offset = 0;

			query = BCON_NEW("Nombre",nombreArchivo.archivo);
			cursor = mongoc_collection_find (archivos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

			while (mongoc_cursor_next (cursor, &doc)) {
				if (bson_iter_init (&iter, doc)) {
					if(bson_iter_find (&iter, "Cantidad Bloques"))archivo.cantidadBloque = bson_iter_int32(&iter);

					tamanioTotal = sizeof(archivo.cantidadBloque) + sizeof(int) * 7 * archivo.cantidadBloque;
					send(socketMarta, &tamanioTotal, sizeof(int),0);

					char *paqueteAEnviar = malloc(tamanioTotal);

					size_to_send =  sizeof(archivo.cantidadBloque);
					memcpy(paqueteAEnviar + offset, &(archivo.cantidadBloque), size_to_send);
					offset += size_to_send;

					for(nroBloque=0;nroBloque<=archivo.cantidadBloque;nroBloque++){

						for(nroCopia=1;nroCopia<=3;nroCopia++){

						info = infoBloqueyCopia(nroBloque, nroCopia, doc);

						size_to_send =  sizeof(nroBloque);
						memcpy(paqueteAEnviar + offset, &(nroBloque), size_to_send);
						offset += size_to_send;

						size_to_send =  sizeof(info.id_nodo);
						memcpy(paqueteAEnviar + offset, &(info.id_nodo), size_to_send);
						offset += size_to_send;

						size_to_send =  sizeof(info.bloque);
						memcpy(paqueteAEnviar + offset, &(info.bloque), size_to_send);
						offset += size_to_send;

						}

					}
					free(paqueteAEnviar);
				}
			}
		}

	}

	return NULL;
}

/*
					if(bson_iter_find (&iter, "Nombre")) archivo.nombre = bson_iter_utf8(&iter,NULL);
					if(bson_iter_find (&iter, "Tamanio"))archivo.tamanio = bson_iter_int32(&iter);
					if(bson_iter_find (&iter, "Directorio Padre"))archivo.directorioPadre = bson_iter_int32(&iter);
					if(bson_iter_find (&iter, "Direccion Fisica"))archivo.path = bson_iter_utf8(&iter,NULL);
					if(bson_iter_find (&iter, "Estado"))archivo.estado = bson_iter_int32(&iter);
					*/
