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

	if(recv(socket, buffer, tamanioTotal, 0)<0) return -1;

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

	if(recv(socket, buffer, tamanioTotal, 0)<0) return "error";

	int tamanioDinamico;

	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	IP = malloc(tamanioDinamico);
	memcpy(IP, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return IP;
}

void *agregoNodoaMongo (void*arg){
	int socket = (int)arg;
	int nodoNuevoOViejo;
	bson_t *doc;
	bson_t *doc2;
	bson_t *doc3;
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
	if(recv(socket, &nodoNuevoOViejo, sizeof(int),0)<0) return NULL;

	switch(nodoNuevoOViejo){
	case 32: //Nodo Nuevo
		send(socket, &a, sizeof(int),0);
		if(recv(socket, &tamanioTotalMensaje, sizeof(int), 0)<0) return NULL;
		if(recive_y_deserialisa_IPyPUERTO_Nodo(&ipyPuertoNodo, socket, tamanioTotalMensaje)){
			doc = bson_new ();
			doc3 = bson_new ();
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
			cantidadBloques = restoDivision.quot;
			BSON_APPEND_UTF8(doc, "Conexion", "Conectado");
			BSON_APPEND_UTF8(doc, "Estado", "No Disponible");
			BSON_APPEND_INT32(doc, "Cantidad de Bloques Total", cantidadBloques);

			doc2 = bson_new ();
			for(a=0;a<cantidadBloques;a++){
				BSON_APPEND_INT32(doc2, string_itoa(a), a);
			}
			BSON_APPEND_ARRAY(doc, "Bloques Libres", doc2);
			bson_destroy (doc2);

			doc2 = bson_new ();
			BSON_APPEND_ARRAY(doc, "Bloques Ocupados", doc2);
			bson_destroy (doc2);

			pthread_mutex_lock(&mutex);
			if (!mongoc_collection_insert (nodos, MONGOC_INSERT_NONE, doc, NULL, &error)) {
				log_error(logger, error.message);
			}
			pthread_mutex_unlock(&mutex);
			bson_destroy (doc);
			bson_destroy (doc3);
		}
		break;
	case 48: // Nodo Viejo
		send(socket, &a, sizeof(int),0);
		if(recv(socket, &idNodito, sizeof(int),0)<0) return NULL; // recibo el id Nodo
		send(socket, &a, sizeof(int),0);
		if(recv(socket, &tamanioTotalMensaje, sizeof(int), 0)<0) return NULL;
		send(socket, &a, sizeof(int),0);
		IPNodito = recive_y_deserialisa_SOLOIP_Nodo(socket, tamanioTotalMensaje);
		query = bson_new ();
		update = bson_new ();
		BSON_APPEND_INT32(query, "ID Nodo", idNodito);
		update = BCON_NEW ("$set", "{",
				"Estado", BCON_UTF8 ("No Disponible"),
				"Conexion", BCON_UTF8 ("Conectado"),
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
	char **comandoSeparado2;
	char *separador="\n";
	char *separator=" ";
	char *path;
	bson_t *doc;

	printf("Ingrese el directorio que desea crear\n"
			"Debe comenzar con '/'\n"
			"Ejemplo: /user/juan/datos\n");

	fgets(bufferComando,MAXSIZE_COMANDO, stdin);
	comandoSeparado=string_split(bufferComando, separator);
	comandoSeparado2=string_split(comandoSeparado[0], separador);
	path = comandoSeparado2[0];

	if(path[0]!='/'){
		printf("Direccion mal cargada\n"
				"Por favor, escribir la '/' al inicio\n");
		return ;
	}

	doc = bson_new ();
	pthread_mutex_lock(&mutexParaIDDirectorio);
	BSON_APPEND_UTF8(doc, "Es" , "Directorio");
	BSON_APPEND_INT32 (doc, "Index", idDirectorioGlobal);
	BSON_APPEND_UTF8(doc, "Directorio", path);
	idDirectorioGlobal++;
	if (!mongoc_collection_insert (directorios, MONGOC_INSERT_NONE, doc, NULL, NULL)) {
		printf("Error al insertar el directorio en la base de datos.\n"
				"Por favor, reintentelo\n");
		return ;
	}
	pthread_mutex_unlock(&mutexParaIDDirectorio);
	bson_destroy (doc);

	printf("El directorio %s ha sido creado correctamente\n",path);
	free(path);
}

int elegirDirectorioParaArchivo(){

	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";
	bson_t *doc;
	bson_t *query;
	int cantidad;
	int idDirectorio;
	const char* directorio;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;
	int index;

	index = -1;
	query = bson_new ();
	BSON_APPEND_UTF8(query, "Es" , "Directorio");
	cantidad = mongoc_collection_count(directorios, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (directorios, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		printf("\n""Los directorios que se encuentran disponibles son los siguientes:\n");
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "Index"))idDirectorio = bson_iter_int32(&iter);
				if(bson_iter_find (&iter, "Directorio"))directorio = bson_iter_utf8(&iter,NULL);
				printf(">> Index: %i, Directorio: %s\n",idDirectorio,directorio);
			}
		}
		bson_destroy (query);
		printf("\n""Ingrese el Index del directorio en el que desee agregar al archivo\n");
		fgets(bufferComando,MAXSIZE_COMANDO, stdin);
		comandoSeparado=string_split(bufferComando, separator);
		index = atoi(comandoSeparado[0]);
		return index;
	}else{
		printf("No hay directorios creados\n");
	}
	return index;
}

void eliminarDirectorio(){
	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";
	bson_t *doc;
	bson_t *query;
	int cantidad;
	int idDirectorio;
	const char* directorio;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;

	query = bson_new ();
	BSON_APPEND_UTF8(query, "Es" , "Directorio");
	cantidad = mongoc_collection_count(directorios, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (directorios, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		printf("\n""Los directorios que se encuentran disponibles son los siguientes:\n");
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "Index"))idDirectorio = bson_iter_int32(&iter);
				if(bson_iter_find (&iter, "Directorio"))directorio = bson_iter_utf8(&iter,NULL);
				printf(">> Index: %i, Directorio: %s\n",idDirectorio,directorio);
			}
		}
		bson_destroy (query);
		printf("\n""Ingrese el Index del directorio en el que desee eliminar al archivo\n");
		fgets(bufferComando,MAXSIZE_COMANDO, stdin);
		comandoSeparado=string_split(bufferComando, separator);
		query = bson_new ();
		BSON_APPEND_UTF8(query, "Es" , "Directorio");
		BSON_APPEND_INT32(query, "Index", atoi(comandoSeparado[0]));
		mongoc_collection_remove(directorios, MONGOC_REMOVE_NONE, query, NULL, NULL);
		printf("El directorio se ha eliminado correctamente\n");
	}else{
			printf("No hay directorios creados\n");
		}
}

void aplicarNodoGlobalYponerNodosNoDisponible(){
	bson_t *query;
	bson_t *update;
	int cantidad,a,cantidadD;

	query = bson_new ();
	update = bson_new ();

	query = BCON_NEW("Es","Nodo");
	update = BCON_NEW ("$set", "{",
	                           "Estado", BCON_UTF8 ("No Disponible"),
							   "Conexion", BCON_UTF8 ("No Conectado"),
							   "Socket", BCON_INT32 (0),
	                       "}");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	bson_destroy (query);

	query = bson_new ();
	query = BCON_NEW("Es","Directorio");
	cantidadD = mongoc_collection_count(directorios, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	bson_destroy (query);

	query = bson_new ();
	query = BCON_NEW("Estado","Disponible");
	for(a=0;a<=cantidad;a++){
		mongoc_collection_update(nodos, MONGOC_UPDATE_NONE, query, update, NULL, NULL);
	}
	idNodoGlobal = cantidad + 1;
	idDirectorioGlobal = cantidadD + 1;
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
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
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

void eliminarNodo(){
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
	BSON_APPEND_UTF8(query, "Estado", "Disponible");
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

		printf("Los nodos que se pueden eliminar son los siguientes:\n");
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
		printf("Ingrese el ID del nodo que desee eliminar\n");
		fgets(bufferComando,MAXSIZE_COMANDO, stdin);
		comandoSeparado=string_split(bufferComando, separator);
		BSON_APPEND_INT32(query, "ID Nodo" , atoi(comandoSeparado[0]));
		update = BCON_NEW ("$set", "{",
				"Estado", BCON_UTF8 ("No Disponible"),
				"}");
		mongoc_collection_update(nodos, MONGOC_UPDATE_NONE, query, update, NULL, NULL);
		bson_destroy (query);
		printf("Se ha eliminado el nodo %i correctamente\n"
				"Ingrese 0 para imprimir el menu\n", atoi(comandoSeparado[0]));
		verificarEstadoFS();
	}else{
		printf("No hay nodos conectados para eliminar\n"
				"Ingrese 0 para imprimir el menu\n");
	}
}

void darDeBajaElNodo(int socket){
	bson_t *query;
	bson_t *update;
	query = bson_new ();
	update = bson_new ();
	BSON_APPEND_INT32(query, "Socket", socket);
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	update = BCON_NEW ("$set", "{",
		                           "Estado", BCON_UTF8 ("No Disponible"),
								   "Conexion", BCON_UTF8 ("No Conectado"),
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
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	nodosActivos = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	bson_destroy (query);
}

void *escribirBloqueEnNodo (t_escribirBloque *estructura){

	int entero; //Para el handshake

	entero = 2;
	send(estructura->socket, &entero, sizeof(int), 0);
	if(recv(estructura->socket, &entero, sizeof(int), 0)<0) return NULL; // Entero para que no se boludee

	entero = 65;

	send(estructura->socket, &estructura->bloque, sizeof(int), 0); // Envio el numero de bloque a escribir
	if(recv(estructura->socket, &entero, sizeof(int), 0)<0) return NULL; // Entero para que no se boludee

	send(estructura->socket, &estructura->tamanioData, sizeof(int), 0);
	if(recv(estructura->socket, &entero, sizeof(int), 0)<0) return NULL; // Entero para que no se boludee

	send(estructura->socket, estructura->data, estructura->tamanioData,0);

	free(estructura);

	return NULL;
}

int formatearBloque(int socket, int nroBloque){ // Devuelve -1 si hay error
	int entero;
	entero = 5;
	send(socket, &entero, sizeof(int), 0);
	if(recv(socket, &entero, sizeof(int), 0)<0){ // basura
		return -1;
	}
	send(socket, &nroBloque, sizeof(int),0);
	return 10;
}

char *pedirContenidoBloqueA (int socket, int nroBloque){
	int entero;
	int variableDelPaquetito = 128*1024;
	int tamanioBloque;
	char *bufferSet;
	int offset;
	char *paquetito;
	int tamanioReal;

	entero = 1;
	send(socket, &entero, sizeof(int), 0);  // Envio codigo para get bloque
	if(recv(socket, &entero, sizeof(int), 0)<0) return "error"; // Entero para que no se boludee

	entero = nroBloque;
	send(socket, &entero,sizeof(int), 0);  // envio numero del bloque

	if(recv(socket,&tamanioBloque,sizeof(int),0)<0) return "error";  // recibo tamanio del bloque
	send(socket,&entero,sizeof(int),0); //Basura

	bufferSet=malloc(tamanioBloque);

	variableDatos = 1;
	offset = 0;
	paquetito = malloc(variableDelPaquetito);
	while(tamanioBloque != offset){
		tamanioReal = recv(socket, paquetito, variableDelPaquetito, 0); // recibo los paquetes
		if(tamanioReal < 0) return "error";
		memcpy(bufferSet + offset, paquetito, tamanioReal);
		offset += tamanioReal;
	}
	free(paquetito);
	variableDatos = 0;
	return bufferSet;
}

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
	int a,b,z,offset;
	t_escribirBloque *escribirBloque;
	t_matrix datosMatrisAsignacion;

	int socketNodoCopia1, socketNodoCopia2, socketNodoCopia3;
	int bloqueNodoCopia1, bloqueNodoCopia2, bloqueNodoCopia3;
	int contC1,contC2,contC3;
	pthread_t hiloNodoCopia1[1000];
	pthread_t hiloNodoCopia2[1000];
	pthread_t hiloNodoCopia3[1000];

	char *pmap;

	char *contenidoBloque;
	long long tamanioBloque = 20971520; // Tamanio 20 MB
	long long bloqueALeer;
	long long bloqueAnterior;
	long long tamanioRestanteDelArchivo;
	int directorio;

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

	// Combinaciones
	datosMatrisAsignacion = calcularCombinacionesDeAsignacion(cantidadBloques);
	if(datosMatrisAsignacion.tamanio == -1){
		printf("No hay espacio suficiente para almacenar el archivo\n");
		return -1;
	}
	offset = 0;
	int matris[cantidadBloques][3];
	for(a=0;a<cantidadBloques;a++){
		for(b=0;b<3;b++){
			memcpy(&matris[a][b], datosMatrisAsignacion.datosDeMatrix + offset, sizeof(int));
			offset += sizeof(int);
		}
	}
	printf("\n");
	printf("La asignacion de bloques es la siguiente:\n\n");
	printf("Nro Bloque | Copia 1 | Copia 2 | Copia 3 |\n");
	for(z=0; z<cantidadBloques; z++){
		printf("    %i      |    %i    |    %i    |    %i    |\n",z,matris[z][0],matris[z][1],matris[z][2]);
	}
	//
	directorio = elegirDirectorioParaArchivo();
	if(directorio == -1) return -1;

	//
	printf("Espere por favor...\n");
	for(contadorBloque=0;contadorBloque < cantidadBloques;contadorBloque++){
		printf("Copiando el bloque %i...\n",contadorBloque);
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
		bloqueAnterior += bloqueALeer;
		tamanioRestanteDelArchivo = tamanioRestanteDelArchivo - bloqueALeer;

		socketNodoCopia1 = socketNodo(matris[contadorBloque][0]);
		socketNodoCopia2 = socketNodo(matris[contadorBloque][1]);
		socketNodoCopia3 = socketNodo(matris[contadorBloque][2]);

		bloqueNodoCopia1 = primerBloqueLibre(matris[contadorBloque][0]);
		bloqueNodoCopia2 = primerBloqueLibre(matris[contadorBloque][1]);
		bloqueNodoCopia3 = primerBloqueLibre(matris[contadorBloque][2]);

		elBloqueDelNodoSeOcupo(socketNodoCopia1,bloqueNodoCopia1);
		elBloqueDelNodoSeOcupo(socketNodoCopia2,bloqueNodoCopia2);
		elBloqueDelNodoSeOcupo(socketNodoCopia3,bloqueNodoCopia3);

		escribirBloque = malloc(sizeof(t_escribirBloque));
		escribirBloque->data = contenidoBloque;
		escribirBloque->tamanioData = strlen(escribirBloque->data);
		escribirBloque->socket = socketNodoCopia1;
		escribirBloque->bloque = bloqueNodoCopia1;
		pthread_create(&hiloNodoCopia1[contC1], NULL, (void *)escribirBloqueEnNodo, (void *)escribirBloque);
		escribirBloque = malloc(sizeof(t_escribirBloque));
		escribirBloque->data = contenidoBloque;
		escribirBloque->tamanioData = strlen(escribirBloque->data);
		escribirBloque->socket = socketNodoCopia2;
		escribirBloque->bloque = bloqueNodoCopia2;
		pthread_create(&hiloNodoCopia2[contC2], NULL, (void *)escribirBloqueEnNodo, (void *)escribirBloque);
		escribirBloque = malloc(sizeof(t_escribirBloque));
		escribirBloque->data = contenidoBloque;
		escribirBloque->tamanioData = strlen(escribirBloque->data);
		escribirBloque->socket = socketNodoCopia3;
		escribirBloque->bloque = bloqueNodoCopia3;
		pthread_create(&hiloNodoCopia3[contC3], NULL, (void *)escribirBloqueEnNodo, (void *)escribirBloque);

		pthread_join(hiloNodoCopia1[contC1],NULL);
		pthread_join(hiloNodoCopia2[contC2],NULL);
		pthread_join(hiloNodoCopia3[contC3],NULL);

		contC1++;
		contC2++;
		contC3++;

		doc3 = bson_new ();
		agregarCopia(doc3, "1", matris[contadorBloque][0], bloqueNodoCopia1);
		agregarCopia(doc3, "2", matris[contadorBloque][1], bloqueNodoCopia2);
		agregarCopia(doc3, "3", matris[contadorBloque][2], bloqueNodoCopia3);

		BSON_APPEND_DOCUMENT(doc2, string_itoa(contadorBloque), doc3);
		bson_destroy (doc3);
		free(contenidoBloque);
	}
	//

	BSON_APPEND_UTF8(doc, "Es", "Archivo");
	BSON_APPEND_UTF8(doc, "Nombre", strrchr(path, '/')+1);
	BSON_APPEND_INT32 (doc, "Tamanio", mystat.st_size);
	BSON_APPEND_INT32(doc, "Directorio Padre" , directorio);
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

int socketNodo(int idNodo){
	bson_t *doc;
	bson_t *query;
	int cantidad;
	int socketNodo;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;

	query = bson_new ();
	BSON_APPEND_UTF8(query, "Estado", "Disponible");
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_INT32 (query, "ID Nodo", idNodo);
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "Socket"))socketNodo = bson_iter_int32(&iter);
			}
		}
	}else{
		printf("No encontro el nodo\n");
		return -1;
	}
	return socketNodo;
}

int indexDelDirectorio(char* directorio){
	int index;
	bson_t *doc;
	bson_t *query;
	int cantidad;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;

	index = -1;
	query = bson_new ();
	BSON_APPEND_UTF8(query, "Es" , "Directorio");
	BSON_APPEND_UTF8(query, "Directorio" , directorio);
	cantidad = mongoc_collection_count(directorios, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (directorios, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "Index"))index = bson_iter_int32(&iter);
			}
		}
		bson_destroy (query);
		return index;
	}else{

	}
	return index;
}

int primerBloqueLibre(int idNodo){
	bson_t *doc;
	bson_t *query;
	int cantidad;
	bson_iter_t iter;
	bson_iter_t iter2;
	mongoc_cursor_t *cursor;
	int tamanioArray;
	int flag;
	int bloque;

	doc = bson_new ();
	query = bson_new ();
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	BSON_APPEND_UTF8(query, "Estado", "Disponible");
	BSON_APPEND_INT32(query, "ID Nodo", idNodo);
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	tamanioArray=0;
	flag=1;
	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find(&iter, "Bloques Libres") && BSON_ITER_HOLDS_ARRAY(&iter)){
					if(bson_iter_recurse(&iter,&iter2)){
						if(bson_iter_find (&iter2, string_itoa(tamanioArray))){
							bloque = bson_iter_int32(&iter2);
							tamanioArray++;
						}else{
							flag=2;
						}
					}
				}
			}
		}
	}else{
			return -1;
		}
	bson_destroy (query);
	return bloque;
}

void eliminarArchivo(){
	t_archivo archivo;
	bson_iter_t iter;
	t_copia info;
	int nroBloque;
	int nroCopia;
	int cantidad;
	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";
	char **comandoSeparado2;
	char *separador="\n";
	int socketNodito;

	const char *nombre;
	bson_t *doc;
	bson_t *query;
	mongoc_cursor_t *cursor;

	query = bson_new ();
	doc = bson_new ();
	BSON_APPEND_UTF8(query, "Es", "Archivo");
	cantidad = mongoc_collection_count(archivos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (archivos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		printf("\n""Los archivos cargados en el MDFS son los siguientes:\n");
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "Nombre"))nombre = bson_iter_utf8(&iter,NULL);
				printf(">> Archivo: %s\n",nombre);
			}
		}
		bson_destroy (query);
		//bson_destroy (doc);
	}else{
		printf("No hay archivos para eliminar\n");
		return;
	}
	printf("\n""Ingrese el nombre del Archivo que desee eliminar\n");
	fgets(bufferComando,MAXSIZE_COMANDO, stdin);
	comandoSeparado=string_split(bufferComando, separator);
	comandoSeparado2=string_split(comandoSeparado[0], separador);

	query = bson_new ();
	doc = bson_new ();
	BSON_APPEND_UTF8(query, "Es", "Archivo");
	BSON_APPEND_UTF8(query, "Nombre", comandoSeparado2[0]);
	cursor = mongoc_collection_find (archivos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	while (mongoc_cursor_next (cursor, &doc)) {
		if (bson_iter_init (&iter, doc)) {
			if(bson_iter_find (&iter, "Cantidad Bloques"))archivo.cantidadBloque = bson_iter_int32(&iter);
			for(nroBloque=0;nroBloque<archivo.cantidadBloque;nroBloque++){
				for(nroCopia=1;nroCopia<=3;nroCopia++){
					info = infoBloqueyCopia(nroBloque, nroCopia, doc);
					socketNodito = socketNodo(info.id_nodo);
					formatearBloque(socketNodito,info.bloque);
					elBloqueDelNodoSeLibero(socketNodito,info.bloque);
				}
			}
		}
	}
	mongoc_collection_remove(archivos,MONGOC_REMOVE_NONE,query,NULL,NULL);
	printf("Se ha eliminado correctamente el archivo %s del MDFS\n",comandoSeparado2[0]);
}

int primerBloqueOcupado(int idNodo){
	bson_t *doc;
	bson_t *query;
	int cantidad;
	bson_iter_t iter;
	bson_iter_t iter2;
	mongoc_cursor_t *cursor;
	int tamanioArray;
	int flag;
	int bloque;

	doc = bson_new ();
	query = bson_new ();
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	BSON_APPEND_UTF8(query, "Estado", "Disponible");
	BSON_APPEND_INT32(query, "ID Nodo", idNodo);
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	tamanioArray=0;
	flag=1;
	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find(&iter, "Bloques Ocupados") && BSON_ITER_HOLDS_ARRAY(&iter)){
					if(bson_iter_recurse(&iter,&iter2)){
						if(bson_iter_find (&iter2, string_itoa(tamanioArray))){
							bloque = bson_iter_int32(&iter2);
							tamanioArray++;
						}else{
							flag=2;
						}
					}
				}
			}
		}
	}else{
			return -1;
		}
	bson_destroy (query);
	return bloque;
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

void elBloqueDelNodoSeOcupo(int socketNodo, int nroBloque){

	bson_t *doc2;
	bson_t *doc3;
	bson_t *query2;
	bson_t *update;

	query2 = bson_new ();
	BSON_APPEND_UTF8 (query2, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query2, "Es" , "Nodo");
	BSON_APPEND_INT32(query2, "Socket", socketNodo);

	update = bson_new ();
	doc2 = bson_new ();
	doc3 = bson_new ();
	update = BCON_NEW ("$addToSet", "{",
			"Bloques Ocupados", BCON_INT32(nroBloque),
			"}");
	bson_destroy (doc2);
	bson_destroy (doc3);

	mongoc_collection_update(nodos,MONGOC_UPDATE_NONE, query2, update,NULL, NULL);
	bson_destroy (update);

	update = bson_new ();
	doc2 = bson_new ();
	doc3 = bson_new ();
	update = BCON_NEW ("$pull", "{",
			"Bloques Libres", BCON_INT32(nroBloque),
			"}");
	bson_destroy (doc2);
	bson_destroy (doc3);

	mongoc_collection_update(nodos,MONGOC_UPDATE_NONE, query2, update,NULL, NULL);
	bson_destroy (update);

	return;
}

void elBloqueDelNodoSeLibero(int socketNodo, int nroBloque){

	bson_t *doc2;
	bson_t *doc3;
	bson_t *query2;
	bson_t *update;

	query2 = bson_new ();
	BSON_APPEND_UTF8 (query2, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query2, "Es" , "Nodo");
	BSON_APPEND_INT32(query2, "Socket", socketNodo);

	update = bson_new ();
	doc2 = bson_new ();
	doc3 = bson_new ();
	update = BCON_NEW ("$pull", "{",
			"Bloques Ocupados", BCON_INT32(nroBloque),
			"}");
	bson_destroy (doc2);
	bson_destroy (doc3);

	mongoc_collection_update(nodos,MONGOC_UPDATE_NONE, query2, update,NULL, NULL);
	bson_destroy (update);

	update = bson_new ();
	doc2 = bson_new ();
	doc3 = bson_new ();
	update = BCON_NEW ("$addToSet", "{",
			"Bloques Libres", BCON_INT32(nroBloque),
			"}");
	bson_destroy (doc2);
	bson_destroy (doc3);

	mongoc_collection_update(nodos,MONGOC_UPDATE_NONE, query2, update,NULL, NULL);
	bson_destroy (update);

	return;
}

int cantidadBloquesLibres(int idNodo){ // Devuelve -1 si no existe el nodo
	bson_t *doc;
	bson_t *query;
	int cantidad;
	bson_iter_t iter;
	bson_iter_t iter2;
	mongoc_cursor_t *cursor;
	int tamanioArray;
	int flag;

	doc = bson_new ();
	query = bson_new ();
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	BSON_APPEND_UTF8(query, "Estado", "Disponible");
	BSON_APPEND_INT32(query, "ID Nodo", idNodo);
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find(&iter, "Bloques Libres") && BSON_ITER_HOLDS_ARRAY(&iter)){
					if(bson_iter_recurse(&iter,&iter2)){
						tamanioArray=0;
						flag=1;
						while(flag!=2){
							if(bson_iter_find (&iter2, string_itoa(tamanioArray))){
								tamanioArray++;
							}else{
								flag=2;
							}
						}
					}
				}
			}
		}
	}else{
			return -1;
		}
	bson_destroy (query);
	return tamanioArray;
}

t_matrix calcularCombinacionesDeAsignacion(int cantidadBloquesArch){
	bson_t *doc;
	bson_t *query;
	int cantidad;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;
	int contNodos;
	int mayor,a,z,nodoARestar;
	t_matrix matris_a_enviar;
	int offset;
	int size_to_send;
	int idCopia1,idCopia2,idCopia3;
	int b;

	doc = bson_new ();
	query = bson_new ();
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	BSON_APPEND_UTF8(query, "Estado", "Disponible");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);

	int matrizDeAsignacion[cantidadBloquesArch][3];

	t_nodoVector vectorBloquesNodos[cantidad];
	contNodos = 0;

	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "ID Nodo")) vectorBloquesNodos[contNodos].idNodo = bson_iter_int32(&iter);
				vectorBloquesNodos[contNodos].bloquesLibres = cantidadBloquesLibres(vectorBloquesNodos[contNodos].idNodo);
				contNodos++;
			}
		}
	}else{
		matris_a_enviar.tamanio = -1;
		matris_a_enviar.datosDeMatrix = "error";
		return matris_a_enviar;
	}

	for(a=0; a<cantidadBloquesArch ; a++){

		// Asigno el ID de la primer Copia
		mayor = 0;
		for(z=0; z<cantidad; z++){
			if(vectorBloquesNodos[z].bloquesLibres > mayor){
				mayor = vectorBloquesNodos[z].bloquesLibres;
				idCopia1 = vectorBloquesNodos[z].idNodo;
				nodoARestar = z;
			}
		}
		vectorBloquesNodos[nodoARestar].bloquesLibres--;

		// Asigno el ID de la segunda Copia
		mayor= 0;
		for(z=0; z<cantidad; z++){
			if(vectorBloquesNodos[z].bloquesLibres > mayor){
				if(vectorBloquesNodos[z].idNodo != idCopia1){
					mayor = vectorBloquesNodos[z].bloquesLibres;
					idCopia2 = vectorBloquesNodos[z].idNodo;
					nodoARestar = z;
				}
			}
		}
		vectorBloquesNodos[nodoARestar].bloquesLibres--;

		// Asigno el ID de la tercer Copia
		mayor= 0;
		for(z=0; z<cantidad; z++){
			if(vectorBloquesNodos[z].bloquesLibres > mayor){
				if((vectorBloquesNodos[z].idNodo != idCopia1) && (vectorBloquesNodos[z].idNodo != idCopia2)){
						mayor = vectorBloquesNodos[z].bloquesLibres;
						idCopia3 = vectorBloquesNodos[z].idNodo;
						nodoARestar = z;
				}
			}
		}
		vectorBloquesNodos[nodoARestar].bloquesLibres--;

		matrizDeAsignacion[a][0] = idCopia1;
		matrizDeAsignacion[a][1] = idCopia2;
		matrizDeAsignacion[a][2] = idCopia3;
	}

	matris_a_enviar.tamanio = cantidadBloquesArch * 3 * sizeof(int);
	matris_a_enviar.datosDeMatrix = malloc(matris_a_enviar.tamanio);

	offset = 0;

	for(a=0;a<cantidadBloquesArch;a++){
		for(b=0;b<3;b++){
			size_to_send =  sizeof(matrizDeAsignacion[a][b]);
			memcpy(matris_a_enviar.datosDeMatrix + offset, &(matrizDeAsignacion[a][b]), size_to_send);
			offset += size_to_send;
		}
	}

	return matris_a_enviar;
}

int CalcFileMD5(char *file_name, char *md5_sum)
{
    #define MD5SUM_CMD_FMT "md5sum %." STR(PATH_LEN) "s 2>/dev/null"
    char cmd[PATH_LEN + sizeof (MD5SUM_CMD_FMT)];
    sprintf(cmd, MD5SUM_CMD_FMT, file_name);
    #undef MD5SUM_CMD_FMT

    FILE *p = popen(cmd, "r");
    if (p == NULL) return 0;
    printf("Calculando MD5....\n");
    int i, ch;
    for (i = 0; i < MD5_LEN && isxdigit(ch = fgetc(p)); i++) {
        *md5_sum++ = ch;
    }

    *md5_sum = '\0';
    pclose(p);
    return i == MD5_LEN;
}

char *pedirArchivoA(int socket, char *nombre){
	char *contenidoArchivo;
	char *contenidoArchivoAux;
	int entero,tamanioReal,tamanioLeido;

	entero = 3;
	send(socket,&entero,sizeof(int),0);
	if(recv(socket,&entero,sizeof(int),0)<0) return "error"; // basura
	entero = strlen(nombre);
	send(socket,&entero,sizeof(int),0); // tamanio nombre
	if(recv(socket,&entero,sizeof(int),0)<0) return "error"; // basura
	send(socket,nombre,strlen(nombre),0); // nombre
	if(recv(socket,&entero,sizeof(int),0)<0) return "error"; // tamanio contenido
	contenidoArchivo = malloc(entero);
	send(socket,&entero,sizeof(int),0); // basura
	tamanioLeido = 0;
	while(tamanioLeido != entero){
		contenidoArchivoAux = malloc(64*1024);
		tamanioReal = recv(socket,contenidoArchivoAux,64*1024,0);
		if(tamanioReal < 0) return "error";
		memcpy(contenidoArchivo+tamanioLeido,contenidoArchivoAux,tamanioReal);
		tamanioLeido += tamanioReal;
	}
	return contenidoArchivo;
}

int espacioLibre(){
	bson_t *doc;
	bson_t *query;
	int cantidad;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;
	int contNodos;
	int mayor,a,z,nodoARestar,idCopia1,idCopia2,idCopia3;
	int cantidadBloquesArch = 1;

	doc = bson_new ();
	query = bson_new ();
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	BSON_APPEND_UTF8(query, "Estado", "Disponible");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);

	t_nodoVector vectorBloquesNodos[cantidad];
	t_nodoVector vectorBloquesNodosAux[cantidad];
	contNodos = 0;

	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "ID Nodo")) vectorBloquesNodos[contNodos].idNodo = bson_iter_int32(&iter);
				vectorBloquesNodos[contNodos].bloquesLibres = cantidadBloquesLibres(vectorBloquesNodos[contNodos].idNodo);
				contNodos++;
			}
		}
	}else{
		return -1;
	}

	for(a=0; a<cantidad;a++){
		vectorBloquesNodosAux[a].idNodo = vectorBloquesNodos[a].idNodo;
		vectorBloquesNodosAux[a].bloquesLibres = vectorBloquesNodos[a].bloquesLibres;
	}

	while(1){

		for(a=0; a<cantidad;a++){
				vectorBloquesNodos[a].idNodo = vectorBloquesNodosAux[a].idNodo;
				vectorBloquesNodos[a].bloquesLibres = vectorBloquesNodosAux[a].bloquesLibres;
		}

		for(a=0; a<cantidadBloquesArch ; a++){

			// Asigno el ID de la primer Copia
			mayor = 0;
			for(z=0; z<cantidad; z++){
				if(vectorBloquesNodos[z].bloquesLibres > mayor){
					mayor = vectorBloquesNodos[z].bloquesLibres;
					idCopia1 = vectorBloquesNodos[z].idNodo;
					nodoARestar = z;
				}
			}
			vectorBloquesNodos[nodoARestar].bloquesLibres--;

			// Asigno el ID de la segunda Copia
			mayor= 0;
			for(z=0; z<cantidad; z++){
				if(vectorBloquesNodos[z].bloquesLibres > mayor){
					if(vectorBloquesNodos[z].idNodo != idCopia1){
						mayor = vectorBloquesNodos[z].bloquesLibres;
						idCopia2 = vectorBloquesNodos[z].idNodo;
						nodoARestar = z;
					}
				}
			}
			vectorBloquesNodos[nodoARestar].bloquesLibres--;

			// Asigno el ID de la tercer Copia
			mayor= 0;
			for(z=0; z<cantidad; z++){
				if(vectorBloquesNodos[z].bloquesLibres > mayor){
					if((vectorBloquesNodos[z].idNodo != idCopia1) && (vectorBloquesNodos[z].idNodo != idCopia2)){
						mayor = vectorBloquesNodos[z].bloquesLibres;
						idCopia3 = vectorBloquesNodos[z].idNodo;
						nodoARestar = z;
					}
				}
			}
			vectorBloquesNodos[nodoARestar].bloquesLibres--;
		}

		for(a=0; a<cantidad;a++){
			if(vectorBloquesNodos[a].bloquesLibres < 0){
				return cantidadBloquesArch -1;
			}
		}
		cantidadBloquesArch++;
	}
	return cantidadBloquesArch;
}

void muestroEspacioLibre(){
	long long bloques;
	long long tamAux;
	long long tamanioBloque = 20 *1024 *1024;

	bloques =espacioLibre();
	tamAux = (tamanioBloque * bloques);
	if((tamAux*bloques) > (1024 * 1024)){
		tamAux = (tamanioBloque * bloques) / (1024 * 1024);
		printf("Tamanio Libre del FileSystem %d MegaBytes \n",tamAux);
		log_info(logger,"Tamanio Libre del FileSystem %d MegaBytes \n",tamAux);
	}else{
		tamAux = (tamanioBloque * bloques);
		printf("Tamanio Libre del FileSystem %d Bytes \n",tamAux);
		log_info(logger,"Tamanio Libre del FileSystem %d Bytes \n",tamAux);
	}
}

void agregarArchivoResultadoAMongo(int idNodo,char* nombre){
	int socket;
	char *mensaje;
	char *rutaDelArch;
	int directorio;
	char *directorioBuscado;
	int a = strlen(nombre);
	while(nombre[a]!='/'){
		a--;
	}
	directorioBuscado = malloc(a);
	memcpy(directorioBuscado,nombre+0,a);
	*(directorioBuscado + a) = '\0';

	directorio = indexDelDirectorio(directorioBuscado);

	asprintf(&rutaDelArch,"%s%s","/tmp/",strrchr(nombre, '/')+1);

	socket = socketNodo(idNodo);
	mensaje = pedirArchivoA(socket,nombre);
	FILE *fd = fopen(rutaDelArch,"w");
	fputs(mensaje,fd);
	fclose(fd);
	printf("Se procede a agregar el archivo %s al MDFS \n",strrchr(nombre, '/')+1);
	insertarArchivoAMongoYAlMDFSPARARESULTADOS(rutaDelArch,directorio);
	printf("El archivo %s, se ha agregado correctamente\n",strrchr(nombre, '/')+1);
	remove(rutaDelArch);
}

int insertarArchivoAMongoYAlMDFSPARARESULTADOS (char* path, int directorio){
	bson_t *doc;
	bson_t *doc2;
	bson_t *doc3;
	bson_error_t error;
	int fd, contadorBloque;
	struct stat mystat;
	int cantidadBloques;
	div_t restoDivision;
	int a,b,z,offset;
	t_escribirBloque *escribirBloque;
	t_matrix datosMatrisAsignacion;

	int socketNodoCopia1, socketNodoCopia2, socketNodoCopia3;
	int bloqueNodoCopia1, bloqueNodoCopia2, bloqueNodoCopia3;
	int contC1,contC2,contC3;
	pthread_t hiloNodoCopia1[1000];
	pthread_t hiloNodoCopia2[1000];
	pthread_t hiloNodoCopia3[1000];

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

	// Combinaciones
	datosMatrisAsignacion = calcularCombinacionesDeAsignacion(cantidadBloques);
	if(datosMatrisAsignacion.tamanio == -1){
		printf("No hay espacio suficiente para almacenar el archivo\n");
		return -1;
	}
	offset = 0;
	int matris[cantidadBloques][3];
	for(a=0;a<cantidadBloques;a++){
		for(b=0;b<3;b++){
			memcpy(&matris[a][b], datosMatrisAsignacion.datosDeMatrix + offset, sizeof(int));
			offset += sizeof(int);
		}
	}
	printf("\n");
	printf("La asignacion de bloques es la siguiente:\n\n");
	printf("Nro Bloque | Copia 1 | Copia 2 | Copia 3 |\n");
	for(z=0; z<cantidadBloques; z++){
		printf("    %i      |    %i    |    %i    |    %i    |\n",z,matris[z][0],matris[z][1],matris[z][2]);
	}
	//
	printf("Espere por favor...\n");
	for(contadorBloque=0;contadorBloque < cantidadBloques;contadorBloque++){
		printf("Copiando el bloque %i...\n",contadorBloque);
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
		bloqueAnterior += bloqueALeer;
		tamanioRestanteDelArchivo = tamanioRestanteDelArchivo - bloqueALeer;

		socketNodoCopia1 = socketNodo(matris[contadorBloque][0]);
		socketNodoCopia2 = socketNodo(matris[contadorBloque][1]);
		socketNodoCopia3 = socketNodo(matris[contadorBloque][2]);

		bloqueNodoCopia1 = primerBloqueLibre(matris[contadorBloque][0]);
		bloqueNodoCopia2 = primerBloqueLibre(matris[contadorBloque][1]);
		bloqueNodoCopia3 = primerBloqueLibre(matris[contadorBloque][2]);

		elBloqueDelNodoSeOcupo(socketNodoCopia1,bloqueNodoCopia1);
		elBloqueDelNodoSeOcupo(socketNodoCopia2,bloqueNodoCopia2);
		elBloqueDelNodoSeOcupo(socketNodoCopia3,bloqueNodoCopia3);

		escribirBloque = malloc(sizeof(t_escribirBloque));
		escribirBloque->data = contenidoBloque;
		escribirBloque->tamanioData = strlen(escribirBloque->data);
		escribirBloque->socket = socketNodoCopia1;
		escribirBloque->bloque = bloqueNodoCopia1;
		pthread_create(&hiloNodoCopia1[contC1], NULL, (void *)escribirBloqueEnNodo, (void *)escribirBloque);
		escribirBloque = malloc(sizeof(t_escribirBloque));
		escribirBloque->data = contenidoBloque;
		escribirBloque->tamanioData = strlen(escribirBloque->data);
		escribirBloque->socket = socketNodoCopia2;
		escribirBloque->bloque = bloqueNodoCopia2;
		pthread_create(&hiloNodoCopia2[contC2], NULL, (void *)escribirBloqueEnNodo, (void *)escribirBloque);
		escribirBloque = malloc(sizeof(t_escribirBloque));
		escribirBloque->data = contenidoBloque;
		escribirBloque->tamanioData = strlen(escribirBloque->data);
		escribirBloque->socket = socketNodoCopia3;
		escribirBloque->bloque = bloqueNodoCopia3;
		pthread_create(&hiloNodoCopia3[contC3], NULL, (void *)escribirBloqueEnNodo, (void *)escribirBloque);

		pthread_join(hiloNodoCopia1[contC1],NULL);
		pthread_join(hiloNodoCopia2[contC2],NULL);
		pthread_join(hiloNodoCopia3[contC3],NULL);

		contC1++;
		contC2++;
		contC3++;

		doc3 = bson_new ();
		agregarCopia(doc3, "1", matris[contadorBloque][0], bloqueNodoCopia1);
		agregarCopia(doc3, "2", matris[contadorBloque][1], bloqueNodoCopia2);
		agregarCopia(doc3, "3", matris[contadorBloque][2], bloqueNodoCopia3);

		BSON_APPEND_DOCUMENT(doc2, string_itoa(contadorBloque), doc3);
		bson_destroy (doc3);
		free(contenidoBloque);
	}
	//

	BSON_APPEND_UTF8(doc, "Es", "Archivo");
	BSON_APPEND_UTF8(doc, "Nombre", strrchr(path, '/')+1);
	BSON_APPEND_INT32 (doc, "Tamanio", mystat.st_size);
	BSON_APPEND_INT32(doc, "Directorio Padre" , directorio);
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

char *rearmarArchivo(){
	char* contenido;
	t_archivo archivo;
	bson_iter_t iter;
	t_copia info;
	int nroBloque;
	int nroCopia;
	int cantidad;
	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";
	char **comandoSeparado2;
	char *separador="\n";
	int socketNodito;
	char* contenidoAux;
	int idDirectorio,tamanioDinamico,offset;
	char *separador2="\n";
	char *contenidoArchivo;

	const char *nombre;
	bson_t *doc;
	bson_t *query;
	mongoc_cursor_t *cursor;

	query = bson_new ();
	doc = bson_new ();
	BSON_APPEND_UTF8(query, "Es", "Archivo");
	cantidad = mongoc_collection_count(archivos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (archivos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		printf("\n""Los archivos cargados en el MDFS son los siguientes:\n");
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "Nombre"))nombre = bson_iter_utf8(&iter,NULL);
				printf(">> Archivo: %s\n",nombre);
				}
		}
		bson_destroy (query);
		//bson_destroy (doc);
	}else{
		printf("No hay archivos para copiar\n");
		return "error";
	}
	printf("\n""Ingrese el nombre del Archivo que desee copiar\n");
	fgets(bufferComando,MAXSIZE_COMANDO, stdin);
	comandoSeparado=string_split(bufferComando, separator);
	comandoSeparado2=string_split(comandoSeparado[0], separador);

	offset = 0;

	query = bson_new ();
	doc = bson_new ();
	BSON_APPEND_UTF8(query, "Es", "Archivo");
	BSON_APPEND_UTF8(query, "Nombre", comandoSeparado2[0]);
	cursor = mongoc_collection_find (archivos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	while (mongoc_cursor_next (cursor, &doc)) {
		if (bson_iter_init (&iter, doc)) {
			if(bson_iter_find (&iter, "Cantidad Bloques"))archivo.cantidadBloque = bson_iter_int32(&iter);
			if(bson_iter_find (&iter, "Tamanio"))archivo.tamanio = bson_iter_int32(&iter);
			contenidoArchivo = malloc(archivo.tamanio);
			for(nroBloque=0;nroBloque<archivo.cantidadBloque;nroBloque++){
					nroCopia = 1;
					info = infoBloqueyCopia(nroBloque, nroCopia, doc);
					socketNodito = socketNodo(info.id_nodo);
					if(socketNodito<0){
						nroCopia = 2;
						info = infoBloqueyCopia(nroBloque, nroCopia, doc);
						socketNodito = socketNodo(info.id_nodo);
						if(socketNodito<0){
							nroCopia = 3;
							info = infoBloqueyCopia(nroBloque, nroCopia, doc);
							socketNodito = socketNodo(info.id_nodo);
							if(socketNodito<0){
								return "error";
							}
						}
					}
					contenidoAux = pedirContenidoBloqueA(socketNodito,nroBloque);
					tamanioDinamico = strlen(contenidoAux);
					memcpy(contenidoArchivo+offset,contenidoAux,tamanioDinamico);
					offset += tamanioDinamico;
				}
			}
		}
	printf("Ingrese la direccion fisica en donde desea guardar el archivo:\n"
			"Ejemplo: /tmp/%s\n",comandoSeparado2[0]);
	fgets(bufferComando,MAXSIZE_COMANDO, stdin);
	comandoSeparado=string_split(bufferComando, separator);
	comandoSeparado2=string_split(comandoSeparado[0], separador2);
	FILE *fd = fopen(comandoSeparado[0],"w");
	fputs(contenidoArchivo,fd);
	fclose(fd);

	return contenidoArchivo;
}
