/*
 * consola.c
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#include "consola.h"
#include "librerias_y_estructuras.h"

void *atenderConsola(void*arg) {

	enum {Imprimir_Menu, Formatear, Eliminar_Arch, Renombrar_Arch, Mover_Arch, Crear_Directorio, Eliminar_Directorio,
		Renombrar_Directorio, Mover_Directorio, Ver_Bloque_Arch, Borrar_Bloque_Arch, Copiar_Bloque_Arch, Agregar_Nodo, Eliminar_Nodo,
		Copiar_Arch_Al_MDFS, Copiar_Arch_Al_FSLocal, Solicitar_MD5, Salir};

	int i;
	t_archivo archivoNuevo;
	char bufferComando[MAXSIZE_COMANDO];
	//char bufferPorTeclado[MAXBUFERTECLADO];
	char **comandoSeparado;
	char *separator=" ";
	//char *bufferLectura;
	//int i;
	imprimirMenu();


	bson_iter_t iter;
	int estado,tamanio,directorioP;
	const char *nombre;
	const char *path;

	while(1){
			fgets(bufferComando,MAXSIZE_COMANDO, stdin);
			comandoSeparado=string_split(bufferComando, separator);
			switch(atoi(comandoSeparado[0])){
				case Imprimir_Menu:
					imprimirMenu();
					break;
				case Formatear:
					i = socketNodoGlobal;
					entero = 4;
					send(i,&entero,sizeof(int),0);
					formatear();
					break;
				case Eliminar_Arch:
					break;
				case Renombrar_Arch:
					break;
				case Mover_Arch:
					break;
				case Crear_Directorio:
					break;
				case Eliminar_Directorio:
					break;
				case Renombrar_Directorio:
					break;
				case Mover_Directorio:
					break;
				case Ver_Bloque_Arch:
					break;
				case Borrar_Bloque_Arch:
					break;
				case Copiar_Bloque_Arch:
					break;
				case Agregar_Nodo: // 12
					query = BCON_NEW("Nombre","CarlosJSFNASFN.txt");
					cursor = mongoc_collection_find (archivos, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

					while (mongoc_cursor_next (cursor, &doc)) {
					        if (bson_iter_init (&iter, doc)) {
									while (bson_iter_next(&iter)) {
										if(bson_iter_find (&iter, "Nombre"))nombre = bson_iter_utf8(&iter,NULL);
										if(bson_iter_find (&iter, "Tamanio"))tamanio = bson_iter_int32(&iter);
										if(bson_iter_find (&iter, "Directorio Padre"))directorioP = bson_iter_int32(&iter);
										if(bson_iter_find (&iter, "Direccion Fisica"))path = bson_iter_utf8(&iter,NULL);
										if(bson_iter_find (&iter, "Estado"))estado = bson_iter_int32(&iter);

										printf("%s\n%i\n%i\n%s\n%i\n",nombre,tamanio,directorioP,path,estado);
									}
							}
					}
					break;
				case Eliminar_Nodo: // 13
					i = socketNodoGlobal;
					mensaje = pedirContenidoBloqueA(i, 10);
					printf("%s\n",mensaje);
					break;
				case Copiar_Arch_Al_MDFS: // 14
					i = socketNodoGlobal;
					mensaje = "MATIASSSASAS";
					escribirBloque.bloque = 100;
					escribirBloque.data = mensaje;
					escribirBloqueEnNodo(i,escribirBloque);
					break;
				case Copiar_Arch_Al_FSLocal:
					archivoNuevo.name = "CarlosJSFNASFN.txt";
					archivoNuevo.size = 123;
					archivoNuevo.parent_directory = 1;
					archivoNuevo.status = 0;
					archivoNuevo.path = "/home/utnso/Escritorio/Carlos.txt";
					insertarArchivoAMongo(archivoNuevo);
					break;
				case Solicitar_MD5:
					break;
				case Salir:
					apagarFS = 1;
					return NULL;
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

	printf("> Los comandos se ingresan con su numero \n"
			"> 		COMANDOS \n"
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
			"	Copiar un archivo local al MDFS: 14 \n"
			"	Copiar un archivo del MDFS al filesystem local: 15 \n"
			"	Solicitar el MD5 de un archivo en MDFS: 16 \n"
			"	SALIR 17 \n");
	return ;
}

void formatear(){
	doc = bson_new ();
	if (!mongoc_collection_delete (nodos, MONGOC_DELETE_NONE, doc, NULL, &error)) {
	        printf ("Delete failed: %s\n", error.message);
	}
	if (!mongoc_collection_delete (archivos, MONGOC_DELETE_NONE, doc, NULL, &error)) {
		        printf ("Delete failed: %s\n", error.message);
	}
	if (!mongoc_collection_delete (directorios, MONGOC_DELETE_NONE, doc, NULL, &error)) {
			        printf ("Delete failed: %s\n", error.message);
	}
	bson_destroy (doc);
}
