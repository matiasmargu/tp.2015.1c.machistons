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
#include <socket/socket.h>
#include <unistd.h>
#include "funciones.h"
#include <pthread.h>

t_log* logger; // Log Global



int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";

	t_config* archivoConfiguracion;

	logger = log_create("LOG_JOB", "log_job" ,false, LOG_LEVEL_INFO);

	char* puerto_marta;
	char* ip_marta;
	FILE* mapper;
	FILE* reduce;
	char* combiner;
	t_list lista_archivos;
	FILE* archivo_resultado;

	typedef struct{
	t_marta_job Marta_Job;
	int	socketMarta;
	int	numeroDeBloque;
	}t_conectarseAlNodo;


    t_marta_job Marta_Job;







	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
	ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
	mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
	reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
	combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
	lista_archivos = config_get_array_value(archivoConfiguracion, "ARCHIVOS");
	archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");





int socketMarta = crearCliente (ip_marta, puerto_marta);





  int cantidad = 10; //TAMANIO LISTA DE ARCHIVOS
  send(socketMarta,&cantidad,sizeof(int),0);
  int a;

for(a = 0 ; a <= cantidad; a++){

	char *archivo = lista_archivos[a];

	send(socketMarta,&archivo_Inicio,strlen(archivo)+1,0);

}


send(socketMarta,&combiner,strlen(combiner)+1,0);





while(((recv(socketMarta, &Marta_Job, ((sizeof(int)+sizeof(int)+sizeof(int)+sizeof(int)+(strlen(Marta_Job.ip_nodo)+1)+(strlen(Marta_Job.nombreNodo)+1+(strlen(Marta_Job.nombre_archivo_resultado)+1)+strlen(Marta_Job.puerto)+1))),0)) != 0 )){

for(int c; c<= //cantidad de bloques qe recibo de marta; c++ ){

recv(socketMarta,&numero, sizeof(int))



}

	int socketNodo = crearCliente (Marta_Job.ip_nodo, Marta_Job.puerto);

	if(Marta_Job.rutina == 1){
	char* mensajeMapper = serializarMapper(&mapper);

	send(socketNodo,mensajeMapper,strlen(mensajeMapper)+1,0);
	}else{
		char* mensajeReduce = serializarMapper(&reduce);

			send(socketNodo,mensajeReduce,strlen(mensajeReduce)+1,0);
	}

	for(int i = 0; i< Marta_Job.cantidadDeBloques; i++){

            int numeroDeBloque = list_get(t_list *self, int index);
            pthread_t (hiloNodo_i);

            t_conectarseAlNodo CAN;
            CAN.Marta_Job = Marta_Job;
            CAN.numeroDeBloque = numeroDeBloque;
            CAN.socketMarta = socketMarta;

			pthread_create(&hiloNodo_i, NULL, (void*) conectarseAlNodo, &CAN);


                                                 }


}




	// PRUEBA DE CONEXION CON NODO
int entero = 8;


int socketNodo = crearCliente("192.168.3.78","6000");


	 send(socketNodo,&entero,sizeof(int),0);

	 recv(socketNodo, &entero,sizeof(int),0);

	printf("%i\n\n",55);
	printf("%i\n",entero);
	 close(socketNodo);



// escribir en el LOG log_info(logger,"lo que va en el archivo log");


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
