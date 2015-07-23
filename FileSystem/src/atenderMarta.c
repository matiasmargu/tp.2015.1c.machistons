/*
 * atenderMarta.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

void *atenderMarta(void*arg){

	int socketMarta = (int)arg;
	int tamanioNombreArchivo;
	char* nombreArchivo;
	char *paqueteAEnviar;
	int tamanioTotal;
	int cantidadNodos;

	int idNodo;
	const char *IPNodo;
	const char *PUERTONodo;

	t_archivo archivo;
	bson_iter_t iter;
	t_copia info;
	int nroBloque;
	int nroCopia;

	bson_t *doc;
	bson_t *query;
	mongoc_cursor_t *cursor;

	int entero;
	int enteroPrueba;
	enteroPrueba = 160;
	char* nombrePrueba;

	int offset;
	int size_to_send;
	int tamanioDinamico;
	send(socketMarta, &enteroPrueba,sizeof(int),0);


	for(;;){
		if(recv(socketMarta, &entero, sizeof(int),0)< 0) return NULL; // Entero para aceptar el pedido de Marta
		switch(entero){
			case 72: // Info Archivo

				send(socketMarta, &enteroPrueba,sizeof(int),0);
				if(recv(socketMarta, &tamanioNombreArchivo, sizeof(int),0)< 0)return NULL;
				send(socketMarta, &enteroPrueba,sizeof(int),0);

				nombrePrueba = malloc(tamanioNombreArchivo);
				if(recv(socketMarta, nombrePrueba, tamanioNombreArchivo, 0)< 0) return NULL;
				nombreArchivo = malloc(tamanioNombreArchivo + 1);
				memcpy(nombreArchivo,nombrePrueba,tamanioNombreArchivo);
				*(nombreArchivo + tamanioNombreArchivo) = '\0';
				printf("%s\n",nombreArchivo);



				// Aca voy a tener el path del archivo a buscar asique tengo que separar los directorios.
				// Buscar en la coleccion directorios el index del ultimo directorio y buscarlo junto con el nombre del archivo

				query = bson_new ();
				offset = 0;

				BSON_APPEND_UTF8(query, "Nombre" , nombreArchivo);
				cursor = mongoc_collection_find (archivos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

				while (mongoc_cursor_next (cursor, &doc)) {
					if (bson_iter_init (&iter, doc)) {
						if(bson_iter_find (&iter, "Cantidad Bloques"))archivo.cantidadBloque = bson_iter_int32(&iter);

						tamanioTotal = sizeof(archivo.cantidadBloque) + sizeof(int) * 7 * archivo.cantidadBloque;
						send(socketMarta, &tamanioTotal, sizeof(int),0);  // Envio el tamanio de toda la info del archivo solicitado
						if(recv(socketMarta, &enteroPrueba, sizeof(int), 0)< 0) return NULL;

						paqueteAEnviar = malloc(tamanioTotal);

						size_to_send =  sizeof(archivo.cantidadBloque);
						memcpy(paqueteAEnviar + offset, &(archivo.cantidadBloque), size_to_send);
						offset += size_to_send;

						for(nroBloque=0;nroBloque<archivo.cantidadBloque;nroBloque++){

							size_to_send =  sizeof(nroBloque);
							memcpy(paqueteAEnviar + offset, &(nroBloque), size_to_send);
							offset += size_to_send;

							for(nroCopia=1;nroCopia<=3;nroCopia++){

								info = infoBloqueyCopia(nroBloque, nroCopia, doc);

								size_to_send =  sizeof(info.id_nodo);
								memcpy(paqueteAEnviar + offset, &(info.id_nodo), size_to_send);
								offset += size_to_send;

								size_to_send =  sizeof(info.bloque);
								memcpy(paqueteAEnviar + offset, &(info.bloque), size_to_send);
								offset += size_to_send;

									}

								}

						send(socketMarta, paqueteAEnviar, tamanioTotal, 0); // Envio toda la info del archivo solicitado
						liberarMensaje(&paqueteAEnviar);
							}
					}
					bson_destroy (query);
					break;
			case 68: // Info Nodos
				query = bson_new ();
				BSON_APPEND_UTF8(query, "Estado", "Disponible");
				BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
				BSON_APPEND_UTF8(query, "Es" , "Nodo");
				cantidadNodos = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
				if(cantidadNodos > 0){
					tamanioTotal = sizeof(int);
					cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
					while (mongoc_cursor_next (cursor, &doc)) {
						if (bson_iter_init (&iter, doc)) {
							if(bson_iter_find (&iter, "ID Nodo"))idNodo = bson_iter_int32(&iter);
							if(bson_iter_find (&iter, "IP"))IPNodo = bson_iter_utf8(&iter,NULL);
							if(bson_iter_find (&iter, "PUERTO"))PUERTONodo = bson_iter_utf8(&iter,NULL);

							tamanioTotal = sizeof(int) + sizeof(int) + strlen(IPNodo)+ 1 + sizeof(int) + strlen(PUERTONodo) +1 + tamanioTotal;
						}
					}

					send(socketMarta, &tamanioTotal, sizeof(int),0);    // Envio el tamanio Total de toda la info Nodos
					paqueteAEnviar = malloc(tamanioTotal);
					offset = 0;
					if(recv(socketMarta, &entero, sizeof(int),0)< 0)return NULL;

					size_to_send =  sizeof(cantidadNodos);
					memcpy(paqueteAEnviar + offset, &(cantidadNodos), size_to_send);
					offset += size_to_send;

					cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
					while (mongoc_cursor_next (cursor, &doc)) {
						if (bson_iter_init (&iter, doc)) {
							if(bson_iter_find (&iter, "ID Nodo"))idNodo = bson_iter_int32(&iter);
							if(bson_iter_find (&iter, "IP"))IPNodo = bson_iter_utf8(&iter,NULL);
							if(bson_iter_find (&iter, "PUERTO"))PUERTONodo = bson_iter_utf8(&iter,NULL);

							size_to_send =  sizeof(idNodo);
							memcpy(paqueteAEnviar + offset, &(idNodo), size_to_send);
							offset += size_to_send;

							tamanioDinamico = strlen(IPNodo) + 1;
							size_to_send = sizeof(int);
							memcpy(paqueteAEnviar + offset, &tamanioDinamico, size_to_send);
							offset += size_to_send;
							size_to_send =  strlen(IPNodo) + 1;
							memcpy(paqueteAEnviar + offset, IPNodo, size_to_send);
							offset += size_to_send;

							tamanioDinamico = strlen(PUERTONodo) + 1;
							size_to_send = sizeof(int);
							memcpy(paqueteAEnviar + offset, &tamanioDinamico, size_to_send);
							offset += size_to_send;
							size_to_send =  strlen(PUERTONodo) + 1;
							memcpy(paqueteAEnviar + offset, PUERTONodo, size_to_send);
							offset += size_to_send;
						}
					}

					send(socketMarta, paqueteAEnviar, tamanioTotal, 0); // Envio toda la info Nodos
					liberarMensaje(&paqueteAEnviar);
				}
				bson_destroy (query);
				break;
		}
	}
	return NULL;
}
