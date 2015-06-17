/*
 * manejoDeMemoria.c
 *
 *  Created on: 13/6/2015
 *      Author: utnso
 */
#include "funcionesParaEnviar.h"

int* crearArraySegunTamanioArchiboBin(char* archivo){

	int tamanioDelArchibo;
	int cantidadDeBloques;


	tamanioDelArchibo = strlen(archivo);
	cantidadDeBloques = tamanioDelArchibo % (20);

	int array[cantidadDeBloques];
	return array;
	}

char* mapearAMemoriaVirtual(){
	fd = open(archivo_bin,O_RDWR);
		if(fd == -1){
		printf("Error al leer el ARCHIBO_BIN\n");
		exit(1);
	}

	if(fstat(fd,&mystat) < 0){
		printf("Error al establecer fstat\n");
		close(fd);
		exit(1);
	}

	pmap = mmap(0,mystat.st_size, PROT_READ|PROT_WRITE ,MAP_SHARED,fd,0);
	if(pmap == MAP_FAILED){
		printf("Error al mapear a memoria\n");
		close(fd);
		exit(1);
	}
	return pmap;
}

