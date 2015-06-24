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

	int i;
	t_archivo archivoNuevo;
	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";

	long long tamanioBloque = 20971520; // Tamanio 20 MB

	FILE *archivoNuevoALeer;
	char str[tamanioBloque];

	imprimirMenu();
	while(1){
			fgets(bufferComando,MAXSIZE_COMANDO, stdin);
			comandoSeparado=string_split(bufferComando, separator);
			switch(atoi(comandoSeparado[0])){
				case Imprimir_Menu: // 0
					imprimirMenu();
					break;
				case Formatear: // 1
					i = socketNodoGlobal;
					entero = 4;
					send(i,&entero,sizeof(int),0);
					formatear();
					break;
				case Eliminar_Arch: // 2
					break;
				case Renombrar_Arch: // 3
					break;
				case Mover_Arch: // 4
					break;
				case Crear_Directorio: // 5
					break;
				case Eliminar_Directorio: // 6
					break;
				case Renombrar_Directorio: // 7
					   archivoNuevoALeer = fopen("/home/utnso/Escritorio/201303hourly.txt" , "r");
					   if(archivoNuevoALeer == NULL)
					   {
					      printf("Mal direccion del archivo\n");
					   }
					   if( fgets(str, tamanioBloque, archivoNuevoALeer)!=NULL )
					   {
					      printf("%s\n",str);
					   }
					   fclose(archivoNuevoALeer);
					break;
				case Mover_Directorio: // 8
					break;
				case Ver_Bloque_Arch: // 9
					break;
				case Borrar_Bloque_Arch: // 10
					break;
				case Copiar_Bloque_Arch: // 11
					break;
				case Agregar_Nodo: // 12
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
				case Copiar_Arch_Al_FSLocal: // 15
					archivoNuevo.nombre = "CarlosJSFNASFN.txt";
					archivoNuevo.tamanio = 123;
					archivoNuevo.directorioPadre = 1;
					archivoNuevo.estado = 0;
					archivoNuevo.cantidadBloque = 2;
					archivoNuevo.path = "/home/utnso/Escritorio/Carlos.txt";
					insertarArchivoAMongo(archivoNuevo);
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
	bson_t *doc;
	bson_error_t error;
	doc = bson_new ();
	if (!mongoc_collection_remove (nodos, MONGOC_DELETE_NONE, doc, NULL, &error)) {
	        printf ("Delete failed: %s\n", error.message);
	}
	if (!mongoc_collection_remove (archivos, MONGOC_DELETE_NONE, doc, NULL, &error)) {
		        printf ("Delete failed: %s\n", error.message);
	}
	if (!mongoc_collection_remove (directorios, MONGOC_DELETE_NONE, doc, NULL, &error)) {
			        printf ("Delete failed: %s\n", error.message);
	}
	bson_destroy (doc);
}
