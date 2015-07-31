/*
 ============================================================================
 Name        : Marta.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "librerias_y_estructuras.h"


int main(void) {

	contador_cant_job = 0;
	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	char* archivoAFSAEnviar;
	char *combiner;
	int tamanioCombiner,d, puerto_job, entero, socketjob,tamanioTotalAFS,prueba;
	t_config* archivoConfiguracion;
	archivoConfiguracion = config_create(rutaArchivoConfiguracion);

	puerto_job = config_get_int_value(archivoConfiguracion, "PUERTO_MARTA") ;
   	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");


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

	t_job_procesos *job_procesos;
	t_tablaProcesos_porJob *proceso;
	t_infoJob *job;
	int tamanio_total,offset, estado, id_proceso, id_job;
	char *buffer;
	offset = 0;


	/*
	   	t_archivo archivo_prueba;
	   	archivo_prueba.bloques = list_create();

	   	recive_y_guarda_estructura(archivo_prueba,socketFS,tamanio_total);
	   	printf("dsadasdas\n");
	   	t_bloque *bloque = list_get(archivo_prueba.bloques,14);
	   	t_copia *copia = list_get(bloque->copias,1);
	   	printf("idNodo: %i\n numBloque: %i\n", copia->idNodo, copia->Numerobloque);


	   	printf("gaston traga penes\n");

		*/
	int contador = 0;



	// Termina Configuracion



   	// Empieza el select
	lista_tabla_procesos = list_create();
	lista_jobs = list_create();

   	for (;;){
   		read_fds = master;
   		select(fdmax+1, &read_fds, NULL, NULL, NULL);
   		//printf("select activo\n");
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
   		    			printf("se callo el socket %i\n",i);
   		    			close(i); // Coneccion perdida
   		    			FD_CLR(i, &master);
   		    		}
   		    		else{
   		    			switch(entero){ // HANDSHAKE
   		    				case 1: // ACA EL JOB LE PASA LISTA DE ARCHIVOS Y EL COMBINER
   		    					socketjob = i;
   		    					pthread_create(&hilo_job, NULL, (void *)atenderJob,(void *)socketjob);//mitrar

   		    					//recv(socketjob,&prueba,sizeof(int),0);
   		    					//printf("%i/n",prueba);
   		    					//send(socketjob,prueba,sizeof(int),0);// este para el entero
   		    					//send(socketjob,2,sizeof(int),0);
   		    					break;
   		    				case 42: //ACA EL JOB LE PASA EL RESULTADO DEL MAP
   		    					//pthread_mutex_lock(&mutex_socket_job);
   		    					estado = 25;
   		    					id_proceso = 555;
   		    					id_job = 666;
   		    					offset = 0;
   		    					socketjob = i;
   		    					//pthread_create(&hilo_job, NULL, recibirResultadoMap,(void *)socketjob);
   		    					prueba = 100;
   		    					send(socketjob,&prueba,sizeof(int),0);
   		    					recv(socketjob,&tamanio_total,sizeof(int),0);
   		    					buffer = malloc(tamanio_total);
   		    					send(socketjob,&prueba,sizeof(int),0);
   		    					recv(socketjob,buffer,tamanio_total,0);

   		    					memcpy(&estado,buffer+offset,sizeof(int));
   		    					offset += sizeof(int);
   		    					memcpy(&id_proceso,buffer+offset,sizeof(int));
   		    					offset += sizeof(int);
   		    					memcpy(&id_job,buffer+offset,sizeof(int));
   		    					offset += sizeof(int);

   		    					job = malloc(sizeof(t_infoJob));
   		    					job = list_get(lista_jobs,id_job);
   		    					job_procesos = malloc(sizeof(t_job_procesos));
   		    					job_procesos = list_get(lista_tabla_procesos, id_job);

   		    					proceso = malloc(sizeof(t_tablaProcesos_porJob));
   		    					proceso = list_get(job_procesos->tabla_procesos,id_proceso);
   		    					proceso->estado = estado;
   		    					pthread_mutex_lock(&mutex_lista_procesos);
   		    					list_replace(job_procesos->tabla_procesos,id_proceso,proceso);
   		    					pthread_mutex_unlock(&mutex_lista_procesos);

   		    					//printf("PROCESO: %i\n",id_proceso);
   		    					free(buffer);
   		    					contador ++;
   		    					//pthread_mutex_unlock(&mutex_socket_job);
   		    					printf("CONTADOR: %i\n",contador);

   		    				    break;
   		    				case 25: //ACA EL JOB LE PASA EL RESULTADO DEL REDUCE
   		    					socketjob = i;/*
   		    					pthread_create(&hilo_job, NULL, recibirResultadoReduce,(void *)socketjob);
                                */
   		    					recv(socketjob,&prueba,sizeof(int),0);
   		    					printf("%i/n",prueba);


   		    					break;
   		    				case 69: //Aca recibo la info de los nodos cada vez que cambian.
   		    					break;
   		    				}
   		    			}
   		    	}
   		    }
   		}
   		}

	return EXIT_SUCCESS;
}
