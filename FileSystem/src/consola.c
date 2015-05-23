/*
 * consola.c
 *
 *  Created on: 6/5/2015
 *      Author: gcrivelli
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "consola.h"
#include <commons/string.h>

void *atenderConsola(void*arg) {

	enum {Formatear, Eliminar_Arch, Renombrar_Arch, Mover_Arch, Crear_Directorio, Eliminar_Directorio,
		Renombrar_Directorio, Mover_Directorio, Ver_Bloque_Arch, Borrar_Bloque_Arch, Copiar_Bloque_Arch, Agregar_Nodo, Eliminar_Nodo,
		Copiar_Arch_Al_MDFS, Copiar_Arch_Al_FSLocal, Solicitar_MD5, Salir};

	char bufferComando[MAXSIZE_COMANDO];
	//char bufferPorTeclado[MAXBUFERTECLADO];
	char **comandoSeparado;
	char *separator=" ";
	//char *bufferLectura;
	//int i;

	while(1){
			imprimirMenu();
			fgets(bufferComando,MAXSIZE_COMANDO, stdin);
			comandoSeparado=string_split(bufferComando, separator);
			switch(atoi(comandoSeparado[0])){
				case Formatear:
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
				case Agregar_Nodo:
					break;
				case Eliminar_Nodo:
					break;
				case Copiar_Arch_Al_MDFS:
					break;
				case Copiar_Arch_Al_FSLocal:
					break;
				case Solicitar_MD5:
					break;
				case Salir:
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
