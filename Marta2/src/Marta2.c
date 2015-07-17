/*
 ============================================================================
 Name        : Marta2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "funciones.h"

int main(void){
	char* rutaArchivoConfiguracion;
	t_config* archivoConfiguracion;
	int fdmax, listener, newfd, yes = 1, addrlen, i,puerto_job,socketjob,entero;
	int martafd, entero2, entero3; //Socket de coneccion con Marta
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	fd_set master;
	fd_set read_fds;
	pthread_t hilo_job;
	t_respuesta_reduce respuestaReduce;

	listaJobs = list_create();

	rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_job = config_get_int_value(archivoConfiguracion, "PUERTO_MARTA") ;

	listaArchivos = list_create();

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	listener = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(puerto_job);
	memset(&(serveraddr.sin_zero), '\0', 8);
	bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	listen(listener, 10);
	FD_SET(listener, &master);

	fdmax = listener;
	logger = log_create("LOG_MARTA", "log_marta" ,false, LOG_LEVEL_INFO);

	for (;;){
	read_fds = master;
	select(fdmax+1, &read_fds, NULL, NULL, NULL);
	printf("select activo\n");
	for(i = 0; i <= fdmax; i++)
	{
	    if(FD_ISSET(i, &read_fds))
	    {
	    	if(i == listener)
	    	{
				addrlen = sizeof(clientaddr);
				if((newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen)) == -1)
				{
				}
				else
				{
					FD_SET(newfd, &master);
					if(newfd > fdmax)
					{
						fdmax = newfd;
					}
				}
	    	}
	    	else
	    	{
	    		if((recv(i, &entero, sizeof(int),0 )) <= 0)
	    		{
	    			if (i == martafd){
	    				log_info(logger,"Se ha perdido la coneccion con Job");
	    				//se callo el job
	    			}
	    			else{

	    				// se callo un nodo y tengo que ponerlo como no disponible
	    			}
	    			close(i); // Coneccion perdida
	    			FD_CLR(i, &master);
	    		}
	    		else{
	    			switch(entero){
	    				case 1: // ACA EL JOB LE PASA LISTA DE ARCHIVOS Y EL COMBINER
	    					socketjob = i;
	    					printf("se conecto el job\n");

	    					pthread_create(&hilo_job, NULL, recibirListaDeArchivos,(void *)socketjob);
	    					break;
	    				case 2: //ACA EL JOB LE PASA EL RESULTADO DEL MAP
	    					//socketJob = i;
	    					printf("aca recibe resultado map\n");
	    				//	pthread_create(&hilo_job, NULL, planificarReduce, (void*) socketJob);

	    					break;

	    				case 3://ACA EL JOB LE PASA EL RESULTADO DEL REDUCE

	    					//pthread_create(&hilo_job, NULL, recibirResultadoReduce,(void *)socketjob);
	    					break;

	    			}
	    		}
	    		}
	    	}
		}
		}
		return EXIT_SUCCESS;
}








