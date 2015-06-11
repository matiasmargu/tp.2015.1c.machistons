/*
 ============================================================================
 Name        : Job.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <./commons/config.h>
#include <./commons/log.h>
#include <./commons/string.h>
#include <./commons/collections/list.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "funciones.h"
#include <pthread.h>
#include <socket/socket.h>

t_log* logger; // Log Global

//FALTA PONER EN LOG Cabeceras de mensajes enviados y recibidos de cada hilo/proceso

int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";

	t_config* archivoConfiguracion;


	logger = log_create("LOG_JOB", "log_job" ,false, LOG_LEVEL_INFO);

	char* puerto_marta;
	char* ip_marta;
	char* mapper;
	char* reduce;
	char* combiner;
	char** lista_archivos;
	char* archivo_resultado;
	int i;
	int c;
	int tamanioTotal;
	int numero;
	int saludo;
	int handshakeMarta;




    t_marta_job2 Marta_Job;
    t_job_nodo Job_Nodo;





	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
	ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
	mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
	reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
	combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
	lista_archivos = config_get_array_value(archivoConfiguracion, "ARCHIVOS");
	archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");





int socketMarta = crearCliente (ip_marta, puerto_marta);

handshakeMarta = 9;

send(socketMarta,&handshakeMarta,sizeof(int),0);

recv(socketMarta, &saludo, sizeof(int),0);


log_info(logger,"Conexion establecida con proceso Marta");
printf("Conexion establecida con proceso Marta: %i\n",saludo);

char* l;
int cantidad = 0;
int s = 0;
l = lista_archivos[s];

while(l != NULL){
	cantidad += 1;     //cantidad = TAMANIO LISTA DE ARCHIVOS
	s = s+1;
	l = lista_archivos[s];
}


 send(socketMarta,&cantidad,sizeof(int),0);

 //HASTA ACA ESTA PROBADO


int a;

for(a = 0 ; a <= cantidad; a++){

	char *archivo;
	archivo = lista_archivos[a];

	send(socketMarta,&archivo,strlen(archivo)+1,0);

}


send(socketMarta,&combiner,strlen(combiner)+1,0);


recv(socketMarta, &tamanioTotal, sizeof(int),0);

int status = 1; // Estructura que manjea el status de los recieve.

status = recive_y_deserialisa(&Marta_Job, socketMarta, tamanioTotal);


while(status){


int listaDeBloques[Marta_Job.cantidadDeBloques];

	for(c= 0; c<= Marta_Job.cantidadDeBloques; c++ ){

	recv(socketMarta,&numero, sizeof(int),0);

	listaDeBloques[c] = numero;

											}

int socketNodo = crearCliente (Marta_Job.ip_nodo, Marta_Job.puerto);

if(Marta_Job.rutina == 1){
Job_Nodo.tipoRutina = 1;
Job_Nodo.rutinaEjecutable = mapper;

char* mensajeMapper = serializarMapper(Job_Nodo);

	send(socketNodo,mensajeMapper,strlen(mensajeMapper)+1,0);
	liberarMensaje(mensajeMapper);

	}else{
		/*char* mensajeReduce = serializarReducer(&reduce);
		Job_Nodo.tipoRutina = 1;
		Job_Nodo.rutinaEjecutable = mapper;

			send(socketNodo,mensajeReduce,strlen(mensajeReduce)+1,0);
			liberarMensaje(mensajeReduce);
	}

	for(i = 0; i<= Marta_Job.cantidadDeBloques; i++){

            int numeroDeBloque = listaDeBloques[i] ;

            pthread_t (hiloNodo_i);

            t_conectarseAlNodo CAN;
            CAN.Marta_Job = Marta_Job;
            CAN.numeroDeBloque = numeroDeBloque;
            CAN.socketMarta = socketMarta;
            CAN.Job_Nodo = Job_Nodo;
            CAN.logger = logger;
            if(Marta_Job.rutina == 1){
            	Job_Nodo.rutinaEjecutable = mapper;
            	Job_Nodo.tipoRutina = 1;
            }else{Job_Nodo.rutinaEjecutable = reduce;
        	Job_Nodo.tipoRutina = 2;}

			pthread_create(&hiloNodo_i, NULL, (void*) conectarseAlNodo, &CAN);
			if(Marta_Job.rutina == 1){
			log_info(logger,"Se creo un hilo mapper  "); //AGREGAR PARAMETROS RECIBIDOS
			printf("Se creo un hilo mapper");
			}
			else{log_info(logger,"Se creo un hilo reducer "); //AGREGAR PARAMETROS RECIBIDOS
			printf("Se creo un hilo reducer");
			}



                                                 }


}




*/

	close(socketMarta);

	log_destroy(logger);
	free(mapper);
	free(lista_archivos);
	free(archivo_resultado);
	free(reduce);
	free(ip_marta);
	free(combiner);

	return EXIT_SUCCESS;
}


