/*
 ============================================================================
 Name        : Marta.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "functions.h"


int main(void) {


	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	char* puerto_fs ;
	char* ip_fs ;
	t_charpuntero archivoAFS;
	char* archivoAFSAEnviar;
	char* combiner;
	int tamanioTotalAFS;
	int tamanioCombiner;
	int socketFS;
	int d;
	int handshakeFS;
	int puerto_job;
	int entero;
	int socketjob ;


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

	// Termina Configuracion



/*

//ACA RECIBIMOS DE JOB SI TIENE O NO COMBINER

   	recv(socketjob, &tamanioCombiner, sizeof(int),0);
   	int estadoCombiner = 1; // Estructura que manjea el status de los recieve.
  	combiner = malloc(tamanioCombiner);
   	estadoCombiner = recive_y_deserialisa(&combiner, socketjob, tamanioCombiner);
   	if(estadoCombiner){

   		//ACA NOS CONECTAMOS CON EL FILE SYSTEM

   		printf("el combiner es %s\n",combiner);
   		/*
   		socketFS = crearCliente (ip_fs, puerto_fs);
   		handshakeFS = 25;

   		printf("%i\n",cantidad);

   		send(socketFS,&handshakeFS,sizeof(int),0);
   		printf("mando a fs %i\n",handshakeFS);

   		printf("%i\n",cantidad);

   		send(socketFS,&cantidad,sizeof(int),0);



   		for(d = 0 ; d < cantidad; d++){
   			archivoAFS.archivo = listaDeArchivos[d];
   			tamanioTotalAFS = sizeof(int) + strlen(archivoAFS.archivo)+1;
   			send(socketFS, &tamanioTotalAFS, sizeof(int),0);
   			archivoAFSAEnviar = serializar_charpuntero(&archivoAFS, tamanioTotalAFS);
   			send(socketFS,archivoAFSAEnviar,tamanioTotalAFS,0);

   		}



   	int columnas;
   	int filas;
   	int f;
   	int e;
   	//PRUEBA MATRIZ CON JOB
   		recv(socketjob,&columnas,sizeof(int),0);
   		printf("columnas %i\n ",columnas);
   		recv(socketjob,&filas,sizeof(int),0);
   		printf("filas %i\n ",columnas);
   		char* matriz[filas][columnas];
   		for(f = 0 ; f < filas; f++){
   			for(e = 0 ; e < columnas; e++){
   			 recv(socketjob, &tamanioTotal, sizeof(int),0);
   			   		 int estado = 1; // Estructura que manjea el status de los recieve.
   			   		 archivo= malloc(tamanioTotal);
   			   		 estado = recive_y_deserialisa(&archivo, socketjob, tamanioTotal);
   			   		 if(estado){
   			   			printf("el archivo es %s\n ",archivo);
   			   			matriz[f][e] = archivo;
   			   		 }
   			   		free(archivo);
   			}
   		}

 //PRUEBA PARA STEAR EL CONTADOR DE LOS NODOS EN 0

typedef struct{
   	char* nodo;
   	int contador_mapper;
} nodo_y_contador;
int x;
char* y;
char* resultado;
nodo_y_contador nodo;

   	for(x = 0; x< 20; x++){
   		y = "nodo";
   		asprintf(&resultado,"%s%i",y,x);
   		nodo.nodo = resultado;
   		nodo.contador_mapper = 0;
   		printf("nombre: %s, contador:%i\n",nodo.nodo,nodo.contador_mapper);

   	}

   	}
*/

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
   		    				case 9: // Este es JOB
   		    					socketjob = i;
   		    					pthread_create(&hilo_job, NULL, conectarseAlJob,(void *)socketjob);

   		    					break;
   		    			case 2: // Este es Nodo

   		    					break;
   		    				}
   		    			}
   		    	}
   		    }
   		}
   		}



	//free(combiner);
	close(socketFS);
	return EXIT_SUCCESS;
}













