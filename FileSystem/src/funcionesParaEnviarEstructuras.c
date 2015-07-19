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

	memcpy(&bloque->tamanioArchivoDatos, buffer + offset, sizeof(int));
	offset += sizeof(int);

	free(buffer);
	return status;
}

char* recive_y_deserialisa_SOLOIP_Nodo(int socket, uint32_t tamanioTotal){
	char* IP;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;

	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	IP = malloc(tamanioDinamico);
	memcpy(IP, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return IP;
}


// Funciones de Nodos

void *agregoNodoaMongo (void*arg){
	int socket = (int)arg;
	int nodoNuevoOViejo;
	bson_t *doc;
	bson_t *doc2;
	bson_t *query;
	bson_t *update;
	bson_error_t error;
	int tamanioTotalMensaje;
	estructuraIPyNodo ipyPuertoNodo;
	div_t restoDivision;
	long long tamanioBloque = 20971520; // Tamanio 20 MB
	int cantidadBloques;
	int a;
	int idNodito;
	int IPNodito;
	a= 60;
	send(socket, &a, sizeof(int),0);
	recv(socket, &nodoNuevoOViejo, sizeof(int),0);

	switch(nodoNuevoOViejo){
	case 32: //Nodo Nuevo
		send(socket, &a, sizeof(int),0);
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
			restoDivision = div(ipyPuertoNodo.tamanioArchivoDatos,tamanioBloque);
			if(restoDivision.rem > 0){
				cantidadBloques = restoDivision.quot + 1;
			}else{
				cantidadBloques = restoDivision.quot;
			}
			BSON_APPEND_UTF8(doc, "Coneccion", "Conectado");
			BSON_APPEND_UTF8(doc, "Estado", "No Disponible");
			BSON_APPEND_INT32(doc, "Cantidad de Bloques Total", cantidadBloques);
			if (!mongoc_collection_insert (nodos, MONGOC_INSERT_NONE, doc, NULL, &error)) {
				log_error(logger, error.message);
			}
			bson_destroy (doc);
		}
		break;
	case 48: // Nodo Viejo
		send(socket, &a, sizeof(int),0);
		recv(socket, &idNodito, sizeof(int),0); // recibo el id Nodo
		send(socket, &a, sizeof(int),0);
		recv(socket, &tamanioTotalMensaje, sizeof(int), 0);
		send(socket, &a, sizeof(int),0);
		IPNodito = recive_y_deserialisa_SOLOIP_Nodo(socket, tamanioTotalMensaje);
		query = bson_new ();
		update = bson_new ();
		BSON_APPEND_INT32(query, "ID Nodo", idNodito);
		update = BCON_NEW ("$set", "{",
				"Estado", BCON_UTF8 ("No Disponible"),
				"Coneccion", BCON_UTF8 ("Conectado"),
				"Socket", BCON_INT32 (socket),
				"IP", BCON_UTF8 (IPNodito),
				"}");
		mongoc_collection_update(nodos, MONGOC_UPDATE_NONE, query, update, NULL, NULL);
		bson_destroy (query);
		bson_destroy (update);
		break;
	}
	return NULL;
}

void crearDirectorio(){
	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";
	char *separador2="\n"; // Los uso para separar el \n del nombre del archivo
	char **comandoSeparado2; // Los uso para separar el \n del nombre del archivo
	char *path;
	int tamanio;

	printf("Ingrese el directorio que desea crear\n");

	fgets(bufferComando,MAXSIZE_COMANDO, stdin);
	comandoSeparado=string_split(bufferComando, separator);
	comandoSeparado2=string_split(comandoSeparado[0], separador2);
	path = comandoSeparado[0];

	tamanio = strlen(path);
	printf("%i\n",tamanio);
	printf("%c\n",path[0]);
	//sprintf();

	bson_t *doc;
	doc = bson_new ();
	pthread_mutex_lock(&mutexParaIDDirectorio);
	BSON_APPEND_INT32 (doc, "Index", idDirectorioGlobal);
	idDirectorioGlobal++;
	pthread_mutex_unlock(&mutexParaIDDirectorio);
	BSON_APPEND_UTF8(doc, "Directorio", "temporal");
	BSON_APPEND_INT32(doc, "Padre", 0);

	if (!mongoc_collection_insert (directorios, MONGOC_INSERT_NONE, doc, NULL, NULL)) {
		log_error(logger, "Error al insertar nuevo directorio");
	}

	bson_destroy (doc);
	printf("Directorio creado correctamente");
}

void eliminarDirectorio(){

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

void agregarNodo(){

	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";
	bson_t *doc;
	bson_t *query;
	bson_t *update;
	int cantidad;
	int idNodo;
	const char* IPNodo;
	const char* PUERTONodo;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;

	query = bson_new ();
	BSON_APPEND_UTF8(query, "Estado", "No Disponible");
	BSON_APPEND_UTF8 (query, "Coneccion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

		printf("Los nodos que se pueden agregar son los siguientes:\n");
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "ID Nodo"))idNodo = bson_iter_int32(&iter);
				if(bson_iter_find (&iter, "IP"))IPNodo = bson_iter_utf8(&iter,NULL);
				if(bson_iter_find (&iter, "PUERTO"))PUERTONodo = bson_iter_utf8(&iter,NULL);
				printf(">> ID Nodo: %i, IP: %s, Puerto: %s\n",idNodo,IPNodo,PUERTONodo);
			}
		}
		bson_destroy (query);
		query = bson_new ();
		printf("Ingrese el ID del nodo que desee agregar\n");
		fgets(bufferComando,MAXSIZE_COMANDO, stdin);
		comandoSeparado=string_split(bufferComando, separator);
		BSON_APPEND_INT32(query, "ID Nodo" , atoi(comandoSeparado[0]));
		update = BCON_NEW ("$set", "{",
				"Estado", BCON_UTF8 ("Disponible"),
				"}");
		mongoc_collection_update(nodos, MONGOC_UPDATE_NONE, query, update, NULL, NULL);
		bson_destroy (query);
		printf("Se ha agregado el nodo %i correctamente\n"
				"Ingrese 0 para imprimir el menu\n", atoi(comandoSeparado[0]));
		verificarEstadoFS();
	}else{
		printf("No hay nodos conectados para agregar\n"
				"Ingrese 0 para imprimir el menu\n");
	}
}

void darDeBajaElNodo(int socket){
	bson_t *query;
	bson_t *update;
	query = bson_new ();
	update = bson_new ();
	BSON_APPEND_INT32(query, "Socket", socket);
	BSON_APPEND_UTF8 (query, "Coneccion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	update = BCON_NEW ("$set", "{",
		                           "Estado", BCON_UTF8 ("No Disponible"),
								   "Coneccion", BCON_UTF8 ("No Conectado"),
								   "Socket", BCON_INT32 (0),
		                       "}");
	mongoc_collection_update(nodos, MONGOC_UPDATE_NONE, query, update, NULL, NULL);
	bson_destroy (query);
	bson_destroy (update);
}

void verificarEstadoFS(){
	bson_t *query;
	query = bson_new ();
	BSON_APPEND_UTF8 (query, "Estado","Disponible");
	BSON_APPEND_UTF8 (query, "Coneccion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	nodosActivos = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	bson_destroy (query);
}

int escribirBloqueEnNodo (int socket, estructuraSetBloque estructura){

	int entero; //Para el handshake

	entero = 2;
	send(socket, &entero, sizeof(int), 0);
	if(recv(socket, &entero, sizeof(int), 0)<0) return -1; // Entero para que no se boludee

	entero = 65;
	estructura.tamanioData = strlen(estructura.data);

	send(socket, &estructura.bloque, sizeof(int), 0); // Envio el numero de bloque a escribir
	if(recv(socket, &entero, sizeof(int), 0)<0) return -1; // Entero para que no se boludee

	send(socket, &estructura.tamanioData, sizeof(int), 0);
	if(recv(socket, &entero, sizeof(int), 0)<0) return -1; // Entero para que no se boludee

	send(socket, estructura.data, estructura.tamanioData,0);

	return 90;
}

char *pedirContenidoBloqueA (int socket, int nroBloque){  // Devuelve 90 si esta bien o -1 si se callo el bloque
	int entero;
	int variableDelPaquetito = 64*1024;
	int tamanioBloque;
	char *bufferSet;
	int offset;
	char *paquetito;
	int tamanioDelPaquetito;
	int tamanioReal;

	entero = 1;
	send(socket, &entero, sizeof(int), 0);
	if(recv(socket, &entero, sizeof(int), 0)<0) return "error"; // Entero para que no se boludee

	entero = nroBloque;
	send(socket, &entero,sizeof(int), 0);

	if(recv(socket,&tamanioBloque,sizeof(int),0)<0) return "error";
	printf("El tamaño del buffer: %i\n",tamanioBloque);

	bufferSet=malloc(tamanioBloque);
	send(socket,&entero,sizeof(int),0); //Basura
	offset = 0;
	while(tamanioBloque != offset){
		if((tamanioBloque-offset)<variableDelPaquetito){
			tamanioDelPaquetito = tamanioBloque-offset;
		}else{
			tamanioDelPaquetito = variableDelPaquetito;
		}
		paquetito = malloc(tamanioDelPaquetito);
		tamanioReal = recv(socket,paquetito,tamanioDelPaquetito,0);
		memcpy(bufferSet + offset,paquetito,tamanioReal);
		offset += tamanioReal;
		liberarMensaje(&paquetito);
		printf("offset: %i, tamanio Paquete: %i, tamanioReal: %i\n",offset, tamanioDelPaquetito, tamanioReal);
	}
	return bufferSet;
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

	cantidadBloques = 1;

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
		escribirBloque.bloque = 0;

		escribirBloqueEnNodo(i,escribirBloque);

		agregarCopia(doc3, "1", 21, 30);
		agregarCopia(doc3, "2", 58, 10);
		agregarCopia(doc3, "3", 76, 40);

		BSON_APPEND_DOCUMENT(doc2, string_itoa(contadorBloque), doc3);
		bson_destroy (doc3);

		free(contenidoBloque);
	}

	BSON_APPEND_UTF8(doc, "Nombre", strrchr(path, '/')+1);
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
