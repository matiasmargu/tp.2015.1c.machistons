/*
 * variablesGlobales.h
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#ifndef VARIABLESGLOBALES_H_
#define VARIABLESGLOBALES_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <./commons/collections/list.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


FILE* nuevoArchivo;

int tamanioArchivo_BIN;
int variableDatos;

char *ip_nodo;
int puerto_nodo;
char *archivo_bin;
char *dir_temp;
char *nodo_nuevo;
char *ip_fs;
char *puerto_fs;
int id_nodo;

char* rutaArchivoConfiguracion;

t_log* logger; // Log Global
t_config* archivoConfiguracion;


//////////////////////////ESTRUCTURAS/////////////

typedef struct{
	int bloque_map;
	int socket;
	char* resultado;
}t_mapper;

typedef struct{
	int tamanio;
	char *contenido;
}t_getBloque;


//Funciones de mapper y reducer
void mapper(t_mapper* arg);
void reducer(void* arg);

//Funciones de memoria
char* mapearAMemoriaVirtual(char* archivo_bin);
int tamanioEspecifico(char* pmap,int nroDelBloque);
void formateoElRegistro(FILE* fdf);
char* mapearDeFD_charp(int fd);
void liberar(char **paquete);
int tamanioEspecificoInversa(char* pmap, int nroDelBloque);
int tamanioDelArchivoBIN();

//Funciones de serializacion y deserializacion
int recive_y_deserialisa_CHARp(char *script, int socket, uint32_t tamanioTotal);
int recive_y_deserialisa_SET_BLOQUE(estructuraSetBloque *bloque, int socket, uint32_t tamanioTotal);
int recive_y_deserializa_EST_REDUCE(t_job_nodo_reduce *bloque, int socket, uint32_t tamanioTotal);
int recive_y_deserializa_NODO_C(t_para_nodo *bloque, int socket, uint32_t tamanioTotal);
char* serializarIP_PUERTO(char* ip_fs,char* puerto_fs, int tamanioData);
char* serializarCHARp(char* Aserializar, int tamanioData);
char* serializarBloqueDeDatos(char* bloque, int tamanioData);

//Funciones de handshake
void handshakeConFS ();
void handshakeConJob(int socket_job);

//Funciones de atender
void *atenderNFS(void*arg);
void *atenderJob(void* arg);

//Funciones para escribir archivos
void escribirScript(char* script_virtual, int comando);

//Para laburar con archivos de configuracion
void crearNuevaConfiguracion();
void leerRutaDeConfiguracion();
void leerID_NODO();

//Funciones para la depuracion
void eliminarEnters(char* buffer, int tamanio);
void formatearArchivo(char* pmap);
void formatearBloque(char* pmap,int nroDeBloque);
void ordernarAlfabeticamente(char* nombreDelArchivoResultado,FILE *fdMape,int tamanioArchivo);
int contarENT(char*buffer,int tamanio);

//Funciones esenciales
t_getBloque getBloque(int nroDelBloque);
void getFileContent(int socket);
void pedirContenidoDeUnArchivo(char* nombre,int socket);

#endif /* VARIABLESGLOBALES_H_ */
