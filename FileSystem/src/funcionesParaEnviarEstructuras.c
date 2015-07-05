/*
 * funcionesParaEnviarEstructuras.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

char* serializarParaGetBloque(estructuraSetBloque *bloque){

	char *serializedPackage = malloc(bloque->tamanioData);

	int offset = 0;
	int size_to_send;

	size_to_send =  sizeof(bloque->bloque);
	memcpy(serializedPackage + offset, &(bloque->bloque), size_to_send);
	offset += size_to_send;

	int tamanioNombre = strlen(bloque->data) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(bloque->data) + 1;
	memcpy(serializedPackage + offset, bloque->data, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}

void liberarMensaje(char **package){
	free(*package);
}

int recive_y_deserialisa_IPyPUERTO_Nodo(estructuraIPyNodo *bloque, int socket, uint32_t tamanioTotal){
	int status = 1;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	bloque->IP = malloc(tamanioDinamico);
	memcpy(bloque->IP, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	bloque->PUERTO = malloc(tamanioDinamico);
	memcpy(bloque->PUERTO, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

// Funciones de Nodos

void *agregoNodoaMongo (void*arg){
	int socket = (int)arg;
	bson_t *doc;
	bson_error_t error;
	int tamanioTotalMensaje;
	estructuraIPyNodo ipyPuertoNodo;

	recv(socket, &tamanioTotalMensaje, sizeof(int), 0);
	if(recive_y_deserialisa_IPyPUERTO_Nodo(&ipyPuertoNodo, socket, tamanioTotalMensaje)){
		doc = bson_new ();
		BSON_APPEND_UTF8(doc, "Es", "Nodo");
		pthread_mutex_lock(&mutex);
		BSON_APPEND_INT32(doc, "ID Nodo", idNodoGlobal);
		send(socket, &idNodoGlobal, sizeof(int), 0);
		idNodoGlobal++;
		pthread_mutex_unlock(&mutex);
		BSON_APPEND_INT32(doc, "Socket", socket);
		BSON_APPEND_UTF8 (doc, "IP", ipyPuertoNodo.IP);
		BSON_APPEND_UTF8(doc, "PUERTO" , ipyPuertoNodo.PUERTO);
		BSON_APPEND_UTF8(doc, "Estado", "No Disponible");
		if (!mongoc_collection_insert (nodos, MONGOC_INSERT_NONE, doc, NULL, &error)) {
    	        log_error(logger, error.message);
		}
		bson_destroy (doc);
	}
	return NULL;
}

void aplicarNodoGlobalYponerNodosNoDisponible(){
	bson_t *query;
	bson_t *update;
	int cantidad,a;

	query = bson_new ();
	update = bson_new ();

	query = BCON_NEW("Es","Nodo");
	update = BCON_NEW ("$set", "{",
	                           "Estado", BCON_UTF8 ("No Disponible"),
	                       "}");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	bson_destroy (query);

	query = bson_new ();
	query = BCON_NEW("Estado","Disponible");
	for(a=0;a<=cantidad;a++){
		mongoc_collection_update(nodos, MONGOC_UPDATE_NONE, query, update, NULL, NULL);
	}
	idNodoGlobal = cantidad + 1;
	bson_destroy (query);
	bson_destroy (update);
}

void verificarEstadoFS(){
	bson_t *query;
	query = BCON_NEW("Estado","Disponible");
	nodosActivos = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	bson_destroy (query);
}

void escribirBloqueEnNodo (int socket, estructuraSetBloque estructura){
	int entero; //Para el handshake
	char *mensaje; // Para mandar mensajes serializados
	entero = 2;
	send(socket, &entero, sizeof(int), 0);
	estructura.tamanioData = sizeof(int) + sizeof(int) + strlen(estructura.data) + 1;
	send(socket, &estructura.tamanioData, sizeof(estructura.tamanioData), 0);
	mensaje = serializarParaGetBloque(&estructura);
	send(socket, mensaje, estructura.tamanioData, 0);
	liberarMensaje(&mensaje);

}

char *pedirContenidoBloqueA (int socket, int nroBloque){
	int entero; //Para el handshake
	int tamanioTotalMensaje;
	entero = 1;
	send(socket, &entero, sizeof(int), 0);
	entero = nroBloque;
	send(socket, &entero,sizeof(int), 0);
	recv(socket, &tamanioTotalMensaje, sizeof(int), 0);
	if(recive_y_deserializa_Contenido_Bloque(&infoBloque, socket, tamanioTotalMensaje)){
		return infoBloque.datos;
	}
	infoBloque.datos = "error";
	return infoBloque.datos;
}

int recive_y_deserializa_Contenido_Bloque(t_getBloque *bloque, int socket, uint32_t tamanioTotal){
	int status = 1;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	bloque->datos = malloc(tamanioDinamico);
	memcpy(bloque->datos, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

// Funciones de Archivos

void agregarCopia (bson_t *documento, char* numeroCopia, int idNodo, int bloque){
	bson_t *doc4;
	doc4 = bson_new();
	BSON_APPEND_INT32(doc4, "ID Nodo", idNodo);
	BSON_APPEND_INT32(doc4, "Bloque", bloque);
	BSON_APPEND_DOCUMENT(documento, numeroCopia , doc4);
	bson_destroy (doc4);
}

int insertarArchivoAMongoYAlMDFS (char* path){
	bson_t *doc;
	bson_t *doc2;
	bson_t *doc3;
	bson_error_t error;
	int fd, contadorBloque;
	struct stat mystat;
	int cantidadBloques;
	div_t restoDivision;
	int i;
	estructuraSetBloque escribirBloque;

	char *pmap;

	char *contenidoBloque;
	long long tamanioBloque = 20971520; // Tamanio 20 MB
	long long bloqueALeer;
	long long bloqueAnterior;
	long long tamanioRestanteDelArchivo;

	fd = open(path,O_RDWR);
	fstat(fd,&mystat);
	restoDivision = div(mystat.st_size,tamanioBloque);
	if(restoDivision.rem > 0){
		cantidadBloques = restoDivision.quot + 1;
	}else{
		cantidadBloques = restoDivision.quot;
	}

	pmap = mmap(0,mystat.st_size, PROT_READ|PROT_WRITE ,MAP_SHARED,fd,0);
	bloqueAnterior = 0;
	tamanioRestanteDelArchivo = mystat.st_size;

	doc = bson_new ();
	doc2 = bson_new ();

	for(contadorBloque=0;contadorBloque < cantidadBloques;contadorBloque++){

		if(tamanioRestanteDelArchivo >= tamanioBloque){
			bloqueALeer = tamanioBloque + bloqueAnterior;
			while(pmap[bloqueALeer] != '\n'){
				bloqueALeer = bloqueALeer - 1;
			}
		}else{
			bloqueALeer = mystat.st_size;
		}
		bloqueALeer = bloqueALeer - bloqueAnterior;
		contenidoBloque = malloc(bloqueALeer);
		memcpy(contenidoBloque,pmap+bloqueAnterior,bloqueALeer);
		bloqueAnterior = bloqueALeer + 1;
		tamanioRestanteDelArchivo = tamanioRestanteDelArchivo - bloqueALeer;

		escribirBloque.data = contenidoBloque;

		// FALTA SABER LOS 3 NODOS CON MAS BLOQUES LIBRES DISPONIBLES

		doc3 = bson_new ();

		i = socketNodoGlobal;
		escribirBloque.bloque = 40;
		escribirBloqueEnNodo(i,escribirBloque);
		agregarCopia(doc3, "3", 76, 40);

		BSON_APPEND_DOCUMENT(doc2, string_itoa(contadorBloque), doc3);
		bson_destroy (doc3);

		free(contenidoBloque);
	}

	BSON_APPEND_UTF8(doc, "Nombre", "Marceloasdasdasdasd");
	BSON_APPEND_INT32 (doc, "Tamanio", mystat.st_size);
	BSON_APPEND_INT32(doc, "Directorio Padre" , 53);
	BSON_APPEND_UTF8(doc, "Direccion Fisica", path);
	BSON_APPEND_UTF8(doc, "Estado", "Disponible");
	BSON_APPEND_INT32(doc, "Cantidad Bloques", cantidadBloques);
	BSON_APPEND_ARRAY(doc, "Bloques", doc2);

	if (!mongoc_collection_insert (archivos, MONGOC_INSERT_NONE, doc, NULL, &error)) {
	        log_error(logger, error.message);
	}

	bson_destroy (doc);
	bson_destroy (doc2);
	munmap(pmap,strlen(pmap));
	return 20;
}

t_copia infoBloqueyCopia(int nroBloque, int nroCopia, bson_t *doc4){

	t_copia info;
	bson_iter_t iter4;
	bson_iter_t subiter4;

	char *resultado;
	char *resultado2;
	char *resultado3;
	char *resultado4;

	asprintf(&resultado,"%s%i","Bloques.",nroBloque);
	asprintf(&resultado2,"%s%s",resultado,".");
	asprintf(&resultado3,"%s%i",resultado2,nroCopia);
	asprintf(&resultado4,"%s%s",resultado3,".ID Nodo");
	if (bson_iter_init (&iter4, doc4) && bson_iter_find_descendant (&iter4, resultado4 , &subiter4) && BSON_ITER_HOLDS_INT32 (&subiter4)) {
		info.id_nodo = bson_iter_int32 (&subiter4);
	}
	asprintf(&resultado,"%s%i","Bloques.",nroBloque);
	asprintf(&resultado2,"%s%s",resultado,".");
	asprintf(&resultado3,"%s%i",resultado2,nroCopia);
	asprintf(&resultado4,"%s%s",resultado3,".Bloque");
	if (bson_iter_init (&iter4, doc4) && bson_iter_find_descendant (&iter4, resultado4, &subiter4) && BSON_ITER_HOLDS_INT32 (&subiter4)) {
		info.bloque = bson_iter_int32 (&subiter4);
	}

	free(resultado);
	free(resultado2);
	free(resultado3);
	free(resultado4);

	return info;
}
