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

	typedef struct{
	t_marta_job Marta_Job;
	int	socketMarta;
	int	numeroDeBloque;
	}t_conectarseAlNodo;


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





int socketMarta = crearCliente (ip_marta, puerto_marta);


 Job_Marta_Inicio.lista_archivos =  lista_archivos;
  Job_Marta_Inicio.combiner = combiner;

  serializadorJob_Marta_Inicio();


	send(socketMarta,&Job_Marta_Inicio,sizeof(struct job_marta_inicio),0);





while(((recv(socketMarta, &Marta_Job, ((sizeof(int)+sizeof(int)+sizeof(int)+sizeof(int)+(strlen(Marta_Job.ip_nodo)+1)+(strlen(Marta_Job.nombreNodo)+1+(strlen(Marta_Job.nombre_archivo_resultado)+1)+strlen(Marta_Job.puerto)+1))),0)) != 0 )){


	int socketNodo = crearCliente (Marta_Job.ip_nodo, Marta_Job.puerto);

	if(Marta_Job.rutina == 1){
	char* mensajeMapper = serializarMapper(&mapper);

	send(socketNodo,mensajeMapper,strlen(mensajeMapper)+1,0);
	}else{
		char* mensajeReduce = serializarMapper(&reduce);

			send(socketNodo,mensajeReduce,strlen(mensajeReduce)+1,0);
	}

	for(int i = 0; i< Marta_Job.cantidadDeBloques; i++){

            int numeroDeBloque = &((Marta_Job.ListaDeBloques)[i]);
            pthread_t (hiloNodo_i);

            t_conectarseAlNodo CAN;
            CAN.Marta_Job = Marta_Job;
            CAN.numeroDeBloque = numeroDeBloque;
            CAN.socketMarta = socketMarta;

			pthread_create(&hiloNodo_i, NULL, (void*) conectarseAlNodo, &CAN);


                                                 }


}




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
