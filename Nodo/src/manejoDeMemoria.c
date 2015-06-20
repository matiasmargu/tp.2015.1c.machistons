/*
 * manejoDeMemoria.c
 *
 *  Created on: 13/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

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

char* mapeoDeArchivo(int fdr){
	char* archivoMap;
	struct stat mystat;

	if(fstat(fdr,&mystat) < 0){
		printf("Error al establecer fstat\n");
		close(fd);
		exit(1);
	}

	archivoMap = mmap(0,mystat.st_size, PROT_READ|PROT_WRITE ,MAP_SHARED,fdr,0);
	if(pmap == MAP_FAILED){
		printf("Error al mapear a memoria\n");
		close(fd);
		exit(1);
	}
	return archivoMap;
}

int conseguirIntegerDelRegistro(FILE* fds,int nroDelBloque){
	int n1;
	int n2;
	int resultado;

	fseek(fds, nroDelBloque * (20*1024*1024), SEEK_SET);
	n1 = fgetc(fd);
	if(n1 == 0){
		fseek(fd,1, SEEK_SET );
		n2 = fgetc(fd);
		resultado = n2;
	}else{
		fseek( fd, 1, SEEK_SET );
		n2 = fgetc(fd);
		strcat(n1,n2);
		resultado = n1;
	}
	fseek(fds, 0, SEEK_SET);
	return resultado;
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


