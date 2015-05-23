/*
 * funciones.c
 *
 *  Created on: 9/5/2015
 *      Author: utnso
 */

#include "funciones.h"
#include <pthread.h>
#include <socket/socket.h>


void conectarseAlNodoMapper(int socketNodo,struct job_nodo Job_Nodo){
	send(socketNodo,&Job_Nodo,sizeof(struct job_nodo),0);

}

void conectarseAlNodoReducer(int socketNodo,struct job_nodo Job_Nodo){
	send(socketNodo,&Job_Nodo,sizeof(struct job_nodo),0);

}

//void conectarseAlNodoReducer();FALTA HACER


/* PROBANDO HILOS INICIO
int valor = 9000;
pthread_mutex_t mutex;

void decrementar(){

	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < 4000 ; i++ ){
		valor-- ;

	}
	pthread_mutex_unlock(&mutex);

	printf("%d\n",valor);
	}


void incrementar(){

	int i;

	pthread_mutex_lock(&mutex);
	for(i = 0; i < 3000 ; i++ ){
		valor++ ;
	}
	pthread_mutex_unlock(&mutex);

		printf("%d\n",valor);

	}

void imprimir(){
	while(1)
	printf("hola como estas");

}
void imprimir2(){
    while(1)
	printf("me llamo pepe");
}
*/ //PROBANDO HILOS FIN

