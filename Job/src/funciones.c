/*
 * funciones.c
 *
 *  Created on: 9/5/2015
 *      Author: utnso
 */

#include "funciones.h"
#include <pthread.h>
#include <socket/socket.h>
#include <sys/socket.h>



int handshake = 8;

typedef struct{
	t_marta_job Marta_Job;
	int	socketMarta;
	int	numeroDeBloque;
	}t_conectarseAlNodo;


char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";
t_job_nodo_mapper Job_Nodo_Mapper;

t_log* logger; // Log Global


archivoConfiguracion = config_create(rutaArchivoConfiguracion);
mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
reduce = config_get_string_value(archivoConfiguracion, "REDUCE");

t_conectarseAlNodo CAN;

void conectarseAlNodo(t_conectarseAlNodo CAN){

	int socketNodo = crearCliente (CAN.Marta_Job.ip_nodo, CAN.Marta_Job.puerto);

	send(socketNodo,&handshake,sizeof(int),0);

   switch(CAN.Marta_Job.rutina ){
   case 1:



	   Job_Nodo_Mapper.NumerobloqueDeDAtos = CAN.numeroDeBloque;
	   Job_Nodo_Mapper.nombreRutina = CAN.Marta_Job.rutina;
	   Job_Nodo_Mapper.resultado = CAN.Marta_Job.nombre_archivo_resultado;

	   serializarJob_Nodo_Mapper(Job_Nodo_Mapper);

	   send(socketNodo,&Job_Nodo_Mapper,(sizeof(int)+sizeof(int)+strlen(CAN.Marta_Job.nombre_archivo_resultado)+1),0);
			   break;

   case 2:


	    Job_Nodo_Mapper.NumerobloqueDeDAtos = CAN.numeroDeBloque;
	  	Job_Nodo_Mapper.nombreRutina = CAN.Marta_Job.rutina;
	  	Job_Nodo_Mapper.resultado = CAN.Marta_Job.nombre_archivo_resultado;;

	  	//SERIALIZAR

	   send(socketNodo,&Job_Nodo_Reduce,sizeof(struct job_nodo),0);
			   break;
   }

   recv(socketNodo, &Nodo_Job, sizeof(struct nodo_job),0);
//DESSERIALIZAR

   send(socketMarta, &Nodo_Job.resultado, strlen(Nodo_Job.resultado)+1),0);

   close(socketNodo);
}







 ///AGREGAR INT PARA DECIRLE QE ES MAP AL NODO
	char* serializarMapper(FILE *mapper){
		char *serializedPackage = malloc(sizeof(FILE));

		int offset = 0;
		int size_to_send;

		size_to_send =  sizeof(FILE);
		memcpy(serializedPackage + offset, &(mapper), size_to_send);
		offset += size_to_send;

		return serializedPackage;
	}





	char* serializarJob_Nodo_Mapper(t_job_nodo_mapper *job_nodo){
		char *serializedPackage = malloc((strlen(job_nodo->resultado)+1)+ (sizeof(int))+(sizeof(int)));

		int offset = 0;
		int size_to_send;


		int tamanioResultado = strlen(job_nodo->resultado) + 1;
		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &tamanioResultado, size_to_send);
		offset += size_to_send;

		size_to_send =  strlen(job_nodo->resultado) + 1;
		memcpy(serializedPackage + offset, job_nodo->resultado, size_to_send);
		offset += size_to_send;

		size_to_send =  sizeof(job_nodo->NumerobloqueDeDAtos);
		memcpy(serializedPackage + offset, &(job_nodo->NumerobloqueDeDAtos), size_to_send);
		offset += size_to_send;

		size_to_send =  sizeof(job_nodo->nombreRutina);
		memcpy(serializedPackage + offset, &(job_nodo->nombreRutina), size_to_send);
		offset += size_to_send;

		return serializedPackage;
	}





void liberarMensaje(char **package){
	free(*package);
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

