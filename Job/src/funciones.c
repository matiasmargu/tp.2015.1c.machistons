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
int resultado;

typedef struct{
	t_marta_job2 Marta_Job;
	int	socketMarta;
	int	numeroDeBloque;
	t_job_nodo Job_Nodo;
	}t_conectarseAlNodo;


char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";
t_job_nodo_mapper Job_Nodo_Mapper;
t_job_nodo Job_Nodo;

t_log* logger; // Log Global


archivoConfiguracion = config_create(rutaArchivoConfiguracion);
mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
reduce = config_get_string_value(archivoConfiguracion, "REDUCE");

t_conectarseAlNodo CAN;

void conectarseAlNodo(t_conectarseAlNodo CAN){

	int socketNodo = crearCliente (CAN.Marta_Job.ip_nodo, CAN.Marta_Job.puerto);

	send(socketNodo,&handshake,sizeof(int),0);

	Job_Nodo.rutinaEjecutable = CAN.Job_Nodo.rutinaEjecutable;
	Job_Nodo.tipoRutina = CAN.Job_Nodo.tipoRutina;

	serializarMapper(Job_Nodo);

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

	 //  send(socketNodo,&Job_Nodo_Reduce,sizeof(struct job_nodo),0);
			   break;
   }

   recv(socketNodo, &resultado, sizeof(int),0);


   send(CAN.socketMarta, &resultado, sizeof(int),0);

   close(socketNodo);

   switch(CAN.Marta_Job.rutina ){
      case 1:
    	  if(resultado == 1){
   log_info(logger,"Finalizo el hilo mapper de forma exitosa ");
   			printf("Finalizo hilo mapper de forma exitosa");
    	  }else{log_info(logger,"Finalizo el hilo mapper de forma no esperada ");
 			printf("Finalizo hilo mapper de forma no esperada");}break;

      case 2:
    	  if(resultado == 1){
    	     log_info(logger,"Finalizo el hilo reducer de forma exitosa ");
    	     			printf("Finalizo hilo reducer de forma exitosa");
    	      	  }else{log_info(logger,"Finalizo el hilo reducer de forma no esperada ");
    	   			printf("Finalizo hilo reducer de forma no esperada");}break;
   }
}







	char* serializarMapper(t_job_nodo *jn){
		char *serializedPackage = malloc(sizeof(FILE));

		int offset = 0;
		int size_to_send;

		size_to_send =  sizeof(FILE);
		memcpy(serializedPackage + offset, &(jn->rutinaEjecutable), size_to_send);
		offset += size_to_send;

		size_to_send =  sizeof(jn->tipoRutina);
		memcpy(serializedPackage + offset, &(jn->tipoRutina), size_to_send);
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


	int recive_y_deserialisa(t_marta_job2 *bloque, int socket, uint32_t tamanioTotal){
		int status;
		char *buffer = malloc(tamanioTotal);
		int offset=0;

		recv(socket, buffer, tamanioTotal, 0);

		memcpy(&(bloque->rutina), buffer + offset, sizeof(bloque->rutina));
		offset += sizeof(bloque->rutina);

		int tamanioDinamico;
		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->ip_nodo = malloc(tamanioDinamico);
		memcpy(bloque->ip_nodo, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;

		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->nombreNodo = malloc(tamanioDinamico);
		memcpy(bloque->nombreNodo, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;

		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->puerto = malloc(tamanioDinamico);
		memcpy(bloque->puerto, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;

		memcpy(&(bloque->cantidadDeBloques), buffer + offset, sizeof(bloque->rutina));
		offset += sizeof(bloque->cantidadDeBloques);

		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->nombre_archivo_resultado = malloc(tamanioDinamico);
		memcpy(bloque->nombre_archivo_resultado, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;



		free(buffer);
		return status;
	}



void liberarMensaje(char *package){
	free(*package);
}

