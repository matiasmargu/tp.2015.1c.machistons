/*
 * consola.c
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#include "librerias_y_estructuras.h"

void *atenderConsola(void*arg) {

	enum {Imprimir_Menu, Formatear, Eliminar_Arch, Renombrar_Arch, Mover_Arch, Crear_Directorio, Eliminar_Directorio,
		Renombrar_Directorio, Mover_Directorio, Ver_Bloque_Arch, Borrar_Bloque_Arch, Copiar_Bloque_Arch, Agregar_Nodo, Eliminar_Nodo,
		Copiar_Arch_Al_MDFS, Copiar_Arch_Al_FSLocal, Solicitar_MD5, Salir};

	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char **comandoSeparado2;
	char *separador2="\n";
	char *separator=" ";

	int a;
	bson_t *doc;
	bson_t *doc2;
	bson_t *doc3;

	int *prueba;

	char *mensaje; // Para mandar mensajes serializados

	imprimirMenu();
	while(1){
			fgets(bufferComando,MAXSIZE_COMANDO, stdin);
			comandoSeparado=string_split(bufferComando, separator);
			switch(atoi(comandoSeparado[0])){
				case Imprimir_Menu: // 0
					imprimirMenu();
					break;
				case Formatear: // 1
					formatear();
					printf("Ingrese 0 para imprimir el menu\n");
					break;
				case Eliminar_Arch: // 2
					if(nodosActivos >= nodosNecesarios){

					}else{
						mensajeEstadoInactivoFS();
					}
					break;
				case Renombrar_Arch: // 3
					doc = bson_new ();
					doc3 = bson_new ();
					BSON_APPEND_UTF8(doc, "Es", "Nodo");
					pthread_mutex_lock(&mutex);
					BSON_APPEND_INT32(doc, "ID Nodo", idNodoGlobal);
					idNodoGlobal++;
					pthread_mutex_unlock(&mutex);
					BSON_APPEND_INT32(doc, "Socket", 7);
					BSON_APPEND_UTF8 (doc, "IP", "1212312312");
					BSON_APPEND_UTF8(doc, "PUERTO" , "666");
					BSON_APPEND_UTF8(doc, "Conexion", "Conectado");
					BSON_APPEND_UTF8(doc, "Estado", "Disponible");
					BSON_APPEND_INT32(doc, "Cantidad de Bloques Total", 70);

					doc2 = bson_new ();
					for(a=0;a<70;a++){
						BSON_APPEND_INT32(doc2, string_itoa(a), a);
					}
					BSON_APPEND_ARRAY(doc, "Bloques Libres", doc2);
					bson_destroy (doc2);

					doc2 = bson_new ();
					BSON_APPEND_ARRAY(doc, "Bloques Ocupados", doc2);
					bson_destroy (doc2);

					pthread_mutex_lock(&mutex);
					if (!mongoc_collection_insert (nodos, MONGOC_INSERT_NONE, doc, NULL, NULL)) {

					}
					pthread_mutex_unlock(&mutex);
					bson_destroy (doc);
					bson_destroy (doc3);



					break;
				case Mover_Arch: // 4
					break;
				case Crear_Directorio: // 5
					if(nodosActivos >= nodosNecesarios){
						crearDirectorio();
						printf("Ingrese 0 para imprimir el menu\n");
					}else{
						mensajeEstadoInactivoFS();
					}
					break;
				case Eliminar_Directorio: // 6
					if(nodosActivos >= nodosNecesarios){
						eliminarDirectorio();
					}else{
						mensajeEstadoInactivoFS();
					}
					break;
				case Renombrar_Directorio: // 7
					mensaje = calcularCombinacionesDeAsignacion(81);
					break;
				case Mover_Directorio: // 8
					printf("%i\n",formatearBloque(atoi(comandoSeparado[1]),atoi(comandoSeparado[2])));
					break;
				case Ver_Bloque_Arch: // 9
					mensaje = pedirContenidoBloqueA(atoi(comandoSeparado[1]), 0);
					printf("%s\n",mensaje);
					break;
				case Borrar_Bloque_Arch: // 10
					break;
				case Copiar_Bloque_Arch: // 11

					break;
				case Agregar_Nodo: // 12
					agregarNodo();
					break;
				case Eliminar_Nodo: // 13
					verificarEstadoFS();
					break;
				case Copiar_Arch_Al_MDFS: // 14
					comandoSeparado2=string_split(comandoSeparado[1], separador2);
					if(insertarArchivoAMongoYAlMDFS(comandoSeparado2[0])== 20){
						printf("Se agrego correctamente el archivo al MDFS\n"
								"Ingrese 0 para imprimir el menu\n");
					}else{
						printf("No hay espacio libre para almacenar el archivo"
								"Ingrese 0 para imprimir el menu\n");
					}
					break;
				case Copiar_Arch_Al_FSLocal: // 15
					break;
				case Solicitar_MD5: // 16
					break;
				case Salir: // 17
					break;
				default:
					{
						printf("Nro. de comando mal ingresado. Ingrese 0 para imprimir menu. \n");
						log_error(logger,"Opcion mal ingresada por teclado en la consola");
					}
			}
	}
	return NULL;
}

void imprimirMenu(void){

	printf("  Los comandos se ingresan con su numero \n"
			" 		COMANDOS \n"
			"	Formatear el MDFS: 1 \n"
			"	Operaciones sobre Archivos  \n"
			"	  Eliminar: 2 \n"
			"	  Renonmbrar: 3 \n"
			"	  Mover: 4 \n"
			"	Operaciones sobre Directorios  \n"
			"	  Crear: 5 \n"
			"	  Eliminar: 6 \n"
			"	  Renombrar: 7 \n"
			"	  Mover: 8 \n"
			"	Operaciones sobre bloques de un archivo \n"
			" 	  Ver: 9 \n"
			" 	  Borrar: 10 \n"
			" 	  Copiar: 11 \n"
			" 	Operaciones sobre nodos de datos:\n"
			"	  Agregar: 12 \n"
			"	  Eliminar: 13 \n"
			"	Copiar un archivo local al MDFS: 14 DireccionFisicaDelArchivo \n"
			"	Copiar un archivo del MDFS al filesystem local: 15 \n"
			"	Solicitar el MD5 de un archivo en MDFS: 16 \n"
			"	SALIR 17 \n");
	return ;
}

void mensajeEstadoInactivoFS(){
	printf("El FileSystem se encuentra inactivo debido a que no posee los suficientes nodos conectado\n"
			"Por favor, agregar los nodos necesarios para que el FileSystem se encuentre en estado Operativo\n"
			"Cantidad de Nodos Activos: %i\n"
			"Cantidad de Nodos Necesarios: %i\n"
			"Ingrese 0 para imprimir el menu\n", nodosActivos, nodosNecesarios);
}

void formatear(){
	bson_t *doc;
	bson_t *query;
	int cantidad;
	bson_error_t error;
	int entero; //Para el handshake
	int socketNodo;
	bson_iter_t iter;
	mongoc_cursor_t *cursor;
	entero = 4;
	int cantidadBloques;
	int a;

	doc = bson_new ();
   	query = bson_new ();
	BSON_APPEND_UTF8 (query, "Conexion", "Conectado");
	BSON_APPEND_UTF8(query, "Es" , "Nodo");
	cantidad = mongoc_collection_count(nodos, MONGOC_QUERY_NONE, query,0,0,NULL,NULL);
	if(cantidad > 0){
		cursor = mongoc_collection_find (nodos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
		while (mongoc_cursor_next (cursor, &doc)) {
			if (bson_iter_init (&iter, doc)) {
				if(bson_iter_find (&iter, "Socket"))socketNodo = bson_iter_int32(&iter);
				if(bson_iter_find (&iter, "Cantidad de Bloques Total"))cantidadBloques = bson_iter_int32(&iter);
				send(socketNodo,&entero,sizeof(int),0);
				for(a=0;a<cantidadBloques;a++){
					elBloqueDelNodoSeLibero(socketNodo,a);
				}
			}
		}
	}else{
		printf("No hay nodos conectados para formatear \n");
	}
	bson_destroy (query);
	doc = bson_new ();
//	if (!mongoc_collection_remove (nodos, MONGOC_DELETE_NONE, doc, NULL, &error)) {
//		printf ("Delete failed: %s\n", error.message);
//	}
	if (!mongoc_collection_remove (archivos, MONGOC_DELETE_NONE, doc, NULL, &error)) {
		        printf ("Delete failed: %s\n", error.message);
	}
	if (!mongoc_collection_remove (directorios, MONGOC_DELETE_NONE, doc, NULL, &error)) {
			        printf ("Delete failed: %s\n", error.message);
	}
	idDirectorioGlobal = 1;
	bson_destroy (doc);
	printf("El MDFS ha sido formateado correctamente \n");
}
