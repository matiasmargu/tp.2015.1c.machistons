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
#include <../commons/config.h>
#include <../commons/log.h>
#include <../commons/string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <socket/socket.h>
#include <unistd.h>
#include "funciones.h"
#include <pthread.h>

t_log* logger; // Log Global


int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";

	t_config* archivoConfiguracion;

	logger = log_create("LOG_JOB", "log_job" ,false, LOG_LEVEL_INFO);
    int status;
    int entero; // Lo uso para el handshake
	char* puerto_marta;
	char* ip_marta;
	char* mapper;
	char* reduce;
	char* combiner;
	char** lista_archivos;
	char* archivo_resultado;
	int ID;
	int operacionAnterior;



    struct job_marta_inicio Job_Marta_Inicio;
    struct marta_job Marta_Job;
    struct job_marta_resultado Job_Marta_Resultado;
    struct nodo_job Nodo_Job;
    struct job_nodo Job_Nodo;
    char* lista_nodos;
    char* lista_archivos_a_reducir;
    lista_nodos = malloc(sizeof(char*));
    lista_archivos_a_reducir  = malloc(sizeof(char*));
    pthread_mutex_t mutex;

	//Marta_Job.ipNodo = "aaa";

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
	ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
	mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
	reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
	combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
	lista_archivos = config_get_array_value(archivoConfiguracion, "ARCHIVOS");
	archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");



	//printf("%i\n\n",puerto_marta);

	// PROBANDO HILOS - COMIENZO
/*
	pthread_t hiloA;
		pthread_create(&hiloA, NULL, (void*) decrementar, NULL);

		pthread_t hiloB;
		pthread_create(&hiloB, NULL, (void*) incrementar , NULL);

		pthread_join(hiloA, NULL);
		pthread_join(hiloB, NULL);

// PROBANDO HILOS- FIN */


	int socketMarta = crearCliente (ip_marta, puerto_marta);

	Job_Marta_Inicio.operacionID = 150;
	Job_Marta_Inicio.lista_archivos = lista_archivos;



	send(socketMarta,&Job_Marta_Inicio,sizeof(struct job_marta_inicio),0);

	recv(socketMarta, &Marta_Job, sizeof(struct marta_job),0);

//	printf("recibo la operacion %i\n",Marta_Job.operacionID);

    int socketNodo = crearCliente (Marta_Job.ipNodo, Marta_Job.puertoNodo);

    operacionAnterior = 0;

	while((strcmp("mapper",Marta_Job.rutina) == 0) && (Marta_Job.operacionID != operacionAnterior) ){//PROBLEMA EN LA COMPARACION DE STRINGS

			  pthread_t hiloA;
		      pthread_create(&hiloA, NULL, (void*) conectarseAlNodoMapper,(socketNodo, &Marta_Job,socketMarta));
              operacionAnterior = Marta_Job.operacionID;

		      recv(socketMarta, &Marta_Job, sizeof(struct marta_job),0);

		      }

    while(strcmp("reduce",Marta_Job.rutina) == 0 && (Marta_Job.operacionID != operacionAnterior)){

		    	  pthread_t hiloB;
			     pthread_create(&hiloB, NULL, (void*) conectarseAlNodoReducer,(socketNodo, &Marta_Job, socketMarta));
			     operacionAnterior = Marta_Job.operacionID;

			     recv(socketMarta, &Marta_Job, sizeof(struct marta_job),0);
		      }




	close(socketNodo);
	close(socketMarta);
	log_destroy(logger);
	free(lista_nodos);
	free(lista_archivos_a_reducir);
	free(mapper);
	free(reduce);
	free(ip_marta);
	free(combiner);
	return EXIT_SUCCESS;

}
