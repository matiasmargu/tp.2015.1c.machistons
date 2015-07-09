/*
 ============================================================================
 Name        : Marta.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "funcionesMarta.h"


int main(void) {


	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	t_charpuntero archivoAFS;
	char* archivoAFSAEnviar, combiner;
	int tamanioCombiner,d, puerto_job, entero, socketjob,tamanioTotalAFS;
	t_config* archivoConfiguracion;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);

	puerto_job = config_get_int_value(archivoConfiguracion, "PUERTO_MARTA") ;



	pthread_t hilo_job;

	// Configuracion Select

	fd_set master;
		fd_set read_fds;

		pthread_t hiloConsola;
		pthread_t hiloMarta;

		struct sockaddr_in serveraddr;
		struct sockaddr_in clientaddr;

		int fdmax, listener, newfd, yes = 1, addrlen, i;

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

	// Termina Configuracion



   	// Empieza el select



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

   		    			close(i); // Coneccion perdida
   		    			FD_CLR(i, &master);
   		    		}
   		    		else{
   		    			switch(entero){ // HANDSHAKE
   		    				case 72: // ACA EL JOB LE PASA LISTA DE ARCHIVOS Y EL COMBINER
   		    					socketjob = i;
   		    					pthread_create(&hilo_job, NULL, conectarseAlJob,(void *)socketjob);//mitrar

   		    					break;
   		    				case 42: //ACA EL JOB LE PASA EL RESULTADO DEL MAP
   		    					socketjob = i;
   		    					pthread_create(&hilo_job, NULL, recibirResultadoMap,(void *)socketjob);

   		    					break;
   		    				case 25: //ACA EL JOB LE PASA EL RESULTADO DEL REDUCE
   		    					socketjob = i;
   		    					pthread_create(&hilo_job, NULL, recibirResultadoReduce,(void *)socketjob);

   		    					break;
   		    				}
   		    			}
   		    	}
   		    }
   		}
   		}

	return EXIT_SUCCESS;
}













