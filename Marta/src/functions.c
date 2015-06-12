/*
 * functions.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#include "functions.h"

#include <pthread.h>
#include <socket/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int recive_y_deserialisa(char *archivo, int socket, uint32_t tamanioTotal){
		int status;
		char *buffer = malloc(tamanioTotal);
		int offset=0;

		recv(socket, buffer, tamanioTotal, 0);


		int tamanioDinamico;
		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		archivo = malloc(tamanioDinamico);
		memcpy(archivo, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;


		free(buffer);
		return status;

}

/*
int recieve_and_deserialize(struct job_marta_inicio *package, int socketCliente){

			int status;
			int buffer_size;
			int buffer_size2;
			int buffer_size3;
			char *buffer = malloc(buffer_size = sizeof(char**));
			char *buffer2 = malloc(buffer_size2 = sizeof(char*));
			int *buffer3 = malloc(buffer_size3 = sizeof(int*));

			int operacion;

			status = recv(socketCliente, buffer3, sizeof(package->operacionID), 0);
			memcpy(&(operacion), buffer3, buffer_size3);
			if (!status) return 0;

			status = recv(socketCliente, package->operacionID, 6, 0);
				if (!status) return 0;

			char** lista_archivos;

			status = recv(socketCliente, buffer, sizeof(package->lista_archivos), 0);
			memcpy(&(lista_archivos), buffer, buffer_size);
			if (!status) return 0;

			status = recv(socketCliente, package->lista_archivos, lista_archivos, 0);
							if (!status) return 0;
			/*char *combiner;
			status = recv(socketCliente, buffer2, sizeof(package->combiner), 0);
			memcpy(&(combiner), buffer2, buffer_size2);
			if (!status) return 0;




			free(buffer);
			free(buffer2);
			free(buffer3);

			return status;
		}

//Programa que guarda una matriz de tamanio mxn, usando memoria dinamica
/*

void getDatos(int* rows, int* cols);
int** getMatrix(int rows,int cols);
void fillMatrix(int** matrix,int rows,int cols);
void printMatrix(int** matrix, int rows, int cols);
void freeMemory(int** matrix,int rows);

int main(void){
   int rows=0,cols=0;
   int** matrix=NULL;

   getDatos(&rows,&cols);
   matrix=getMatrix(rows,cols);
   fillMatrix(matrix,rows,cols);
   printMatrix(matrix,rows,cols);
   freeMemory(matrix,rows);
return 0;
}

void getDatos(int* rows,int* cols){
   printf("Renglones=");
   scanf("%d",rows);
   printf("Columnas=");
   scanf("%d",cols);
}

int** getMatrix(int rows,int cols){
   int i;
   int** matrix=NULL;
   matrix=(int**)malloc(sizeof(int*)*rows);
   for(i=0;i<rows;i++){
      *(matrix+i)=(int*)malloc(sizeof(int)*cols);
   }
   return matrix;
}

void fillMatrix(int** matrix,int rows,int cols){
   int i,j;
   for(i=0;i<rows;i++){
      for(j=0;j<cols;j++){
         *(*(matrix+i)+j)=i+j;
      }
   }
}

void printMatrix(int** matrix,int rows,int cols){
   int i,j;
   for(i=0;i<rows;i++){
      for(j=0;j<cols;j++){
         printf("\t%d",*(*(matrix+i)+j));
      }
      printf("\n");
   }
}

void freeMemory(int** matrix,int rows){
   int i;
   for(i=0;i<rows;i++){
      free(*(matrix+i));
   }
   free(matrix);
}


*/
/*
typedef struct{
	struct nodo* siguiente;
	char* nombre;
} nodo;
nodo* primer = NULL;
nodo* ultimo = NULL;

void agregar(nodo* _nodo){
	_nodo -> siguiente = NULL;
	if(primer == NULL){
		primer = _nodo;
		ultimo = _nodo;
	}else{
		ultimo -> siguiente = _nodo;
		ultimo = _nodo;
	};
};
*/
/*
int main(){
	nodo* primerNodo = malloc(sizeof(nodo));
	primerNodo -> nombre = "Primer Elemento";
	nodo* segundoNodo = malloc(sizeof(nodo));
	segundoNodo -> nombre = "Segundo Elemento";
	nodo* tercerNodo = malloc(sizeof(nodo));
	tercerNodo -> nombre = "Tercer Elemento";
	agregar(primerNodo);
	agregar(segundoNodo);
	agregar(tercerNodo);
	nodo* i = primerNodo;
	while(i != NULL){
		printf("%s\n", i -> nombre);
		i = i -> siguiente;
	};
	return 0;
};



*/


