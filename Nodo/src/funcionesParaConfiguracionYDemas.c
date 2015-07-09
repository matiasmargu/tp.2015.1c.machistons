/*
 * funcionesParaConfiguracionYDemas.c
 *
 *  Created on: 26/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"


void leerRutaDeConfiguracion(){


	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);


	archivo_bin = config_get_string_value(archivoConfiguracion, "ARCHIVO_BIN");
	dir_temp = config_get_string_value(archivoConfiguracion, "DIR_TEMP");
	nodo_nuevo = config_get_string_value(archivoConfiguracion, "NODO_NUEVO");
	ip_nodo = config_get_string_value(archivoConfiguracion, "IP_NODO");
	puerto_nodo = config_get_int_value(archivoConfiguracion, "PUERTO_NODO");
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS");

	if(nodo_nuevo=="NO"){
		id_nodo = config_get_string_value(archivoConfiguracion, "ID_NODO");
	}


	config_destroy(archivoConfiguracion);
}

void leerID_NODO(){
	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf";
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);

	id_nodo = config_get_string_value(archivoConfiguracion, "ID_NODO");

	config_destroy(archivoConfiguracion);
}

//Despreciable
void crearNuevaConfiguracion(){

	nuevoArchivo = fopen("/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf","w");

	fputs("PUERTO_FS=3000\n",nuevoArchivo);

	fputs("IP_FS=",nuevoArchivo);
	fputs(ip_fs,nuevoArchivo);
	fputc('\n',nuevoArchivo);

	fputs("ARCHIVO_BIN=",nuevoArchivo);
	fputs(archivo_bin,nuevoArchivo);
	fputc('\n',nuevoArchivo);

	fputs("DIR_TEMP=",nuevoArchivo);
	fputs(dir_temp,nuevoArchivo);
	fputc('\n',nuevoArchivo);

	fputs("NODO_NUEVO=NO\n",nuevoArchivo);
	nodo_nuevo = "NO";

	fputs("IP_NODO=",nuevoArchivo);
	fputs(ip_nodo,nuevoArchivo);
	fputc('\n',nuevoArchivo);

	fputs("PUERTO_NODO=6000\n",nuevoArchivo);
	fclose(nuevoArchivo);

	fputs("ID_NODO=",nuevoArchivo);
	fputs(id_nodo,nuevoArchivo);
	fputc('\n',nuevoArchivo);
}
