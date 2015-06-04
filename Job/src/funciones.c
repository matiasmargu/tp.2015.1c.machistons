/*
 * funciones.c
 *
 *  Created on: 9/5/2015
 *      Author: utnso
 */

#include "funciones.h"
#include <pthread.h>
#include <socket/socket.h>


struct job_nodo Job_Nodo;
char* handshake = "Se conecto el Job";




char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";


t_log* logger; // Log Global


archivoConfiguracion = config_create(rutaArchivoConfiguracion);
mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
reduce = config_get_string_value(archivoConfiguracion, "REDUCE");

t_conectarseAlNodo CAN;

void conectarseAlNodo(t_conectarseAlNodo CAN){

	int socketNodo = crearCliente (CAN.Marta_Job.ipNodo, CAN.Marta_Job.puertoNodo);

	send(socketNodo,&handshake,sizeof(char*),0);

   switch(CAN.Marta_Job.rutina ){
   case 1:

	   Job_Nodo.CANrutina = mapper;
	   Job_Nodo.NumerobloqueDeDAtos = CAN.numeroDeBloque;


	   send(socketNodo,&Job_Nodo,sizeof(struct job_nodo),0);
			   break;

   case 2:

	   Job_Nodo.rutina = reduce;
	   Job_Nodo.NumerobloqueDeDAtos = CAN.numeroDeBloque;

	   send(socketNodo,&Job_Nodo,sizeof(struct job_nodo),0);
			   break;
   }

   recv(socketNodo, &Nodo_Job, sizeof(struct nodo_job),0);

   send(socketMarta, &Job_Marta_Resultado, sizeof(struct job_marta_resultado),0);

   close(socketNodo);
}

void serializadorMapper(){
	char* serializarMapper(FILE *mapper){
		char *serializedPackage = malloc(sizeof(FILE));

		int offset = 0;
		int size_to_send;

		size_to_send =  sizeof(FILE);
		memcpy(serializedPackage + offset, &(mapper), size_to_send);
		offset += size_to_send;

		return serializedPackage;
	}


void serializadorJob_Marta_Inicio() {


	char* serializarInicio(t_job_marta_inicio *inicio){
		char *serializedPackage = malloc((strlen(inicio->combiner)+1)+ (strlen(inicio->lista_archivos)+1));

		int offset = 0;
		int size_to_send;


		int tamanioCombiner = strlen(inicio->combiner) + 1;
		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &tamanioCombiner, size_to_send);
		offset += size_to_send;

		size_to_send =  strlen(inicio->combiner) + 1;
		memcpy(serializedPackage + offset, inicio->combiner, size_to_send);
		offset += size_to_send;

		int tamanioLista = strlen(inicio->lista_archivos) + 1;
		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &tamanioLista, size_to_send);
		offset += size_to_send;

		size_to_send =  strlen(inicio->lista_archivos) + 1;
		memcpy(serializedPackage + offset, inicio->lista_archivos, size_to_send);
		offset += size_to_send;

		return serializedPackage;
	}


}


void liberarMensaje(char **package){
	free(*package);
}
}





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

