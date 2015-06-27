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
}

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
	char* ip=obtenerIP();
	fputs(ip,nuevoArchivo);
	fputc('\n',nuevoArchivo);

	fputs("PUERTO_NODO=6000\n",nuevoArchivo);
	fclose(nuevoArchivo);

	archivoConfiguracion = config_create("/home/utnso/git/tp-2015-1c-machistons/Configuracion/nodo.conf");
}

char* obtenerIP(){
	int fd;
	struct ifreq ifr;
	char* ip;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);
	ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

	return ip;
	close(fd);
}
