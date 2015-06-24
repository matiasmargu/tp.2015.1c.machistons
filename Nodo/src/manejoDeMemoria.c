/*
 * manejoDeMemoria.c
 *
 *  Created on: 13/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

char* mapearAMemoriaVirtual(char* archivo_bin){
	char* pmap;
	int fd;
	struct stat mystat;

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
int tamanioEspecifico(char* pmap,int nroDelBloque){
	int i;
	int contador=0;
	int centinela=0;

	for(i=pmap[nroDelBloque*20*1024*1024];centinela==0;i++){
		contador++;
		if(pmap[i]=='/'){
			centinela = 1;
		}

	}

	return (contador - 1);
}



void escribeEnArchivoSegunNroDeBloque(FILE* fd,int nroDelBloque,int tamanio){
	fseek(fd, nroDelBloque * (20*1024*1024), SEEK_SET);
	fputc(tamanio,fd);
	fseek(fd, 0, SEEK_SET);
}

void formateoElRegistro(FILE* fdf){
	int i = 1;

	while(i != 0){
		fputc(0,fdf);
	    if( feof(fdf)){
	         i = 0;
	    }
	}
}


