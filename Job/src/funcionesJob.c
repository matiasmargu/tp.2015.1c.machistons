/*
 * funciones.c
 *
 *  Created on: 9/5/2015
 *      Author: utnso
 */

#include "funcionesJob.h"


int resultado;


t_job_nodo Job_Nodo;

t_conectarseAlNodo CAN;

void conectarseAlNodo(t_conectarseAlNodo CAN){
	printf("a/n");

	int tamanioReduce;
	int socketNodo = crearCliente (CAN.Marta_Job.ip_nodo, CAN.Marta_Job.puerto);
	int tamanioNodo;
	Job_Nodo.NumerobloqueDeDAtos = CAN.numeroDeBloque;
    Job_Nodo.nombreRutina = CAN.Marta_Job.rutina;
    Job_Nodo.resultado = CAN.Marta_Job.nombre_archivo_resultado;
    char * archivoResultadoReduce;

   switch(CAN.Marta_Job.rutina ){
   case 1:
	   //Manda la estructura job_nodo al NOdo

	   tamanioNodo = sizeof(int)+ (strlen(Job_Nodo.resultado)+1) + sizeof(int) + sizeof(int);
	   send(socketNodo,&tamanioNodo,sizeof(int),0);
	   char* archivoANodo = serializarJob_Nodo(&Job_Nodo, tamanioNodo);
	   send(socketNodo,archivoANodo,tamanioNodo,0);

	   //RECIBE UN ENTERO QUE ES EL RESULTADO EXITOSO O FALLIDO
	   recv(socketNodo, &resultado, sizeof(int),0);


	      //MANDA A MARTA EL RESULTADO
	   	  int tamanioTotal = sizeof(int)+sizeof(int)+sizeof(int)+sizeof(int)+sizeof(int);
	      t_job_marta *job_marta;
	      job_marta->numeroBloque = CAN.numeroDeBloque;
	      job_marta->rutina = CAN.Marta_Job.rutina;
	      job_marta->resultado = resultado;
	      job_marta->idNodo = CAN.idNodo;
	      send(CAN.socketMarta, &tamanioTotal, sizeof(int),0);
	      char* archivoResultado =  serializar_job_marta(&job_marta, tamanioTotal);
	      send(CAN.socketMarta, &archivoResultado, tamanioTotal,0);

	   break;
   case 2:
	   // HAY QUE VER EN QUE CAMBIA SI RECIBE REDUCE

	  	//SERIALIZAR

	 //  send(socketNodo,&Job_Nodo_Reduce,sizeof(struct job_nodo),0);

	   tamanioReduce = sizeof(int)+sizeof(int)+sizeof(int)+sizeof(int)+sizeof(int);
	  			t_job_marta *job_marta_reduce;
	  		      job_marta_reduce->numeroBloque = CAN.numeroDeBloque;
	  		      job_marta_reduce->rutina = CAN.Marta_Job.rutina;
	  		      job_marta_reduce->resultado = resultado;
	  		      send(CAN.socketMarta, &tamanioTotal, sizeof(int),0);
	  		      archivoResultadoReduce =  serializar_job_marta(&job_marta_reduce, tamanioReduce);
	  		      send(CAN.socketMarta, &archivoResultadoReduce, tamanioTotal,0);



			   break;
   }



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








char* serializar_charpuntero(t_charpuntero *nombre, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

			int tamanioNombre = strlen(nombre->archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(nombre->archivo) + 1;
			memcpy(serializedPackage + offset, nombre->archivo, size_to_send);
			offset += size_to_send;

			return serializedPackage;
		}

char* serializar_job_marta(t_job_marta *job_marta, int tamanioTotal){
	char *serializedPackage = malloc(tamanioTotal);
	int offset = 0;
	int size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->numeroBloque, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->resultado, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->rutina, size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &job_marta->idNodo, size_to_send);
	offset += size_to_send;

	offset += size_to_send;
	return serializedPackage;

}

	char* serializarJob_Nodo(t_job_nodo *job_nodo, int tamanioTotal){
		char *serializedPackage = malloc(tamanioTotal);

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



	int recive_y_deserialisa_marta_job(t_marta_job *bloque, int socket, uint32_t tamanioTotal){
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



		bloque->puerto = malloc(tamanioDinamico);
		memcpy(bloque->puerto, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;


		memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
		offset += sizeof(int);

		bloque->nombre_archivo_resultado = malloc(tamanioDinamico);
		memcpy(bloque->nombre_archivo_resultado, buffer + offset, tamanioDinamico);
		offset += tamanioDinamico;


		//Falta poner en donde copia el char* de los bloques o vector de bloques

		memcpy(&(bloque->idNodo), buffer + offset, sizeof(bloque->idNodo));
		offset += sizeof(bloque->idNodo);

		free(buffer);
		return status;
	}


void liberarMensaje(char *package){
	free(*package);
}

void *mandarPruebaAMarta(void*arg){
	int socketMarta =(int)arg;
    int prueba;
	recv(socketMarta,&prueba,sizeof(int),0);
    printf("%i",prueba);
    return 0;
}

void *mandarPrueba2ANodo(void*arg){

	char * ip_nodo,puerto_nodo; // cuando se ponga el ip y el puerto se setean
	int socketNodo = crearCliente (ip_nodo/* poner ip*/, puerto_nodo /* poner puerto*/);
    int prueba2 = (int)arg;

    send(socketNodo,prueba2,sizeof(int),0);

    return 0;
}
