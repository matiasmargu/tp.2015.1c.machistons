/*
 * funciones.c
 *
 *  Created on: 9/5/2015
 *      Author: utnso
 */

#include "funciones.h"
#include <pthread.h>
#include <socket/socket.h>

/*


void conectarseAlNodo(struct marta_job Marta_Job, int socketMarta){

	int socketNodo = crearCliente (Marta_Job.ipNodo, Marta_Job.puertoNodo);

//	send(socketNodo,&handshake,sizeof(),0);

   switch(Marta_Job.rutina ){
   case 1:

	   send(socketNodo,&Job_Nodo//MAAPP,sizeof(struct job_nodo),0);

   case 2:

	   send(socketNodo,&Job_Nodo//MAAPP,sizeof(struct job_nodo),0);

   }

   recv(socketNodo, &Nodo_Job, sizeof(struct nodo_job),0);

   send(socketMarta, &Job_Marta_Resultado, sizeof(struct job_marta_resultado),0);

/*

	Job_Nodo.NumerobloqueDeDAtos = Marta_Job.NumeroBloqueDeDatos; //HACER ASI CON TODO LO QUE LE PASO AL JOB


	send(socketNodo,&Job_Nodo,sizeof(struct job_nodo),0);
	 recv(socketNodo, &Nodo_Job, sizeof(struct nodo_job),0);

	 Job_Marta_Resultado.archivo_resultado = Nodo_Job.archivo_resultado; //AGREGAR LAS DEMAS COSAS QUE LE MANDO A MARTA

   send(socketMarta, &Job_Marta_Resultado, sizeof(struct job_marta_resultado),0);


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

