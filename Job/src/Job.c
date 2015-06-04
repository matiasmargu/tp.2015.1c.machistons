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

    int entero; // Lo uso para el handshake
	char* puerto_marta;
	char* ip_marta;
	FILE* mapper;
	FILE* reduce;
	char* combiner;
	char** lista_archivos;
	FILE* archivo_resultado;




    t_marta_job Marta_Job;
    t_job_marta_inicio Job_Marta_Inicio;






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


//	pthread_t* hiloNodo;

int socketMarta = crearCliente (ip_marta, puerto_marta);


 Job_Marta_Inicio.lista_archivos =  lista_archivos;
  Job_Marta_Inicio.combiner = combiner;

  serializadorJob_Marta_Inicio();


	send(socketMarta,&Job_Marta_Inicio,sizeof(struct job_marta_inicio),0);


/*
	// PRUEBA DE CONEXION CON NODO
int enter = 8;
int caca = 9;

int socketNodo = crearCliente("192.168.3.99","6000");


	 send(socketNodo,&enter,sizeof(int),0);
	 send(socketNodo,&caca,sizeof(int),0);
	printf("%i\n\n",55);
	 close(socketNodo);

*/





while(((recv(socketMarta, &Marta_Job, sizeof(struct marta_job),0)) != 0 )){


	for(int i = 0; i< sizeof(Marta_Job.ListaDeBloques); i++){

            int numeroDeBloque = (Marta_Job.ListaDeBloques)[i];

			pthread_create(&hiloNodo+i, NULL, (void*) conectarseAlNodo,( Marta_Job,socketMarta, numeroDeBloque));


                                                 }


}









	//close(socketMarta);

	log_destroy(logger);
	free(mapper);
	free(lista_archivos);
	free(archivo_resultado);
	free(reduce);
	free(ip_marta);
	free(combiner);

	return EXIT_SUCCESS;
}
