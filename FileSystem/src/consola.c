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

	char bufferComando[MAXSIZE_COMANDO];
	char **comandoSeparado;
	char *separator=" ";
	char *separador2="\n"; // Los uso para separar el \n del nombre del archivo
	char *separador3="/";
	char **comandoSeparado2; // Los uso para separar el \n del nombre del archivo

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
					break;
				case Eliminar_Arch: // 2
					if(nodosActivos >= nodosNecesarios){

					}else{
						mensajeEstadoInactivoFS();
					}
					break;
				case Renombrar_Arch: // 3
					break;
				case Mover_Arch: // 4
					break;
				case Crear_Directorio: // 5
					if(nodosActivos >= nodosNecesarios){
						crearDirectorio();
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
					break;
				case Mover_Directorio: // 8
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
					//comandoSeparado2=string_split(comandoSeparado[1], separador2);
					// comandoSeparado2[0]
					// verificar si hay espacio para este archivo
					//insertarArchivoAMongoYAlMDFS("/home/utnso/Escritorio/201303hourly.txt");
					if(insertarArchivoAMongoYAlMDFS("/home/utnso/Escritorio/201303hourly.txt")== 20){
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
	bson_error_t error;
	int entero; //Para el handshake

	entero = 4;
	send(socketNodoGlobal,&entero,sizeof(int),0);

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
