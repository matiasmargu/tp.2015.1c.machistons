/*
 * manejoDeMemoria.c
 *
 *  Created on: 13/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"
#include <errno.h>

char* mapearAMemoriaVirtual(char* archivo){
	char* pmap;
	int fd;
	struct stat mystat;

	fd = open(archivo,O_RDWR);
	if(fd==-1){
		return NULL;
	}

	if(fstat(fd,&mystat) < 0){
		printf("Error al establecer fstat\n");
		close(fd);
		exit(1);
	}

	pmap = mmap(0,mystat.st_size, PROT_READ|PROT_WRITE ,MAP_SHARED,fd,0);
	if(pmap == MAP_FAILED){
		printf("Error al mapear a memoria: %s\n",strerror(errno));
		close(fd);
		exit(1);
	}
	return pmap;
}

char* mapearDeFD_charp(int fd){
	//Con open(nombre del archivo (path),O_RDWR);
	struct stat mystat;
	char* pmap;

	if(fstat(fd,&mystat) < 0){
		printf("Error al establecer fstat\n");
		close(fd);
		exit(1);
	}

	pmap = mmap(0,mystat.st_size, PROT_READ|PROT_WRITE ,MAP_SHARED|MAP_NORESERVE,fd,0);
	if(pmap == MAP_FAILED){
		printf("Error al mapear a memoria\n");
		close(fd);
		exit(1);
	}
	return pmap;
}


int tamanioEspecifico(char* pmap,int nroDelBloque){
	int i,contador=0,b=0;

	for(i=nroDelBloque*20*1024*1024;b==0;i++){

		contador++;
		if(pmap[i]=='/'){
			b=1;
		}

		if(contador >= 20*1024*1024){
			b=1;
		}
	}
	return (contador-1);
}

int tamanioEspecificoInversa(char* pmap, int nroDelBloque){
	int contador,i,b;

	contador = 0;
	b=0;
	for(i=(((nroDelBloque+1)*20*1024*1024)-1);b==0;i--){
		contador ++;
		if(pmap[i]!='/'){
			b=1;
		}
		if(contador == 20*1024*1024){
			b=1;
		}
	}

	contador--;
	if(contador >= 20*1024*1024){
		contador = 20*1024*1024;
	}else{
		contador = (20*1024*1024) - contador;
	}
	return contador;
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

void liberar(char **paquete){
	free(*paquete);
}

t_getBloque getBloque(int nroDelBloque){

	t_getBloque infoBloque;
	int tamanioBloque, tamanioBloqueExacto;
	char *bloque;
	char* pmap;


	pmap = mapearAMemoriaVirtual(archivo_bin);
	tamanioBloque=tamanioEspecifico(pmap,nroDelBloque);

	bloque=malloc(tamanioBloque);

	tamanioBloqueExacto = nroDelBloque * 1024 * 1024* 20;
	memcpy(bloque,pmap + tamanioBloqueExacto,tamanioBloque);

	munmap(pmap,tamanioArchivo_BIN);

	infoBloque.tamanio = tamanioBloque;
	infoBloque.contenido = bloque;
	return infoBloque;
}

void formatearArchivo(char* pmap){
	int n,i;

	n = tamanioArchivo_BIN;
	printf("tamanioDelArchivo: %i\n",n);
	for(i=0;i<n;i++){
		pmap[i]='/';
	}
	msync(pmap,n,0);
}

void formatearBloque(char* pmap,int nroDeBloque){
	int n,m,i;
	n = tamanioArchivo_BIN;
	m = tamanioEspecifico(pmap,nroDeBloque);

	printf("%i\n",m);

	for(i=0;i<m;i++){
		pmap[i+(nroDeBloque*20*1024*1024)]='/';
	}

	msync(pmap,n,0);
}

int tamanioDelArchivoBIN(){
	int tamanioArchivo_BIN;
	FILE* fdAux = fopen(archivo_bin,"r");
	fseek(fdAux, 0L, SEEK_SET);
	fseek(fdAux, 0L, SEEK_END);

	tamanioArchivo_BIN = ftell(fdAux);
return tamanioArchivo_BIN;
}


