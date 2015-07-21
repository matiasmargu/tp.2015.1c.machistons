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

t_list *lista_nodos_estado;

typedef struct{
	int id_nodo;
	char *ip_nodo;
	char *puerto_nodo;
}t_nodo2;

typedef struct{
	int estado; // 0 = todavia no se mando a ejecutar; 1 = en ejecucion; 2 = Fin; 3 = Error
	int bloque_archivo;
	char *nombre_archivo_resultado;
	int id_nodo;
}t_tablaProcesos_porJob2;

typedef struct{
	int idJob;
	t_list *tabla_procesos;
}t_lista_job2;

typedef struct {
	int idNodo;
	char* puertoNodo;
	char * ipNodo;
	t_list* archivosAReducir;
    char* nombreArchivoResultado;
}t_archivosAReducirPorNodo;


int main(void){

	t_lista_job2 *tabla;
	t_lista_job2 *tabla2;
	t_lista_job2* tabla3;
	t_tablaProcesos_porJob2 *campoDeTabla;
	t_tablaProcesos_porJob2 *campoDeTabla2;
	t_tablaProcesos_porJob2 *campoDeTabla3;
	t_tablaProcesos_porJob2 *campoDeTabla4;
	t_tablaProcesos_porJob2 *campoDeTabla5;
	t_tablaProcesos_porJob2 *campoDeTabla6;
	t_list* lista_job_tabla;


		// para probar
        int tamanio23;
        char * lluo;
        lluo= "campoTabla1";
        tamanio23=sizeof(int)* 5 + 4 * strlen(lluo) + 1 ;
		tabla = malloc(tamanio23);
		tabla2 = malloc(tamanio23);
		tabla3 = malloc(tamanio23);
        campoDeTabla = malloc(tamanio23);
        campoDeTabla2 = malloc(tamanio23);
        campoDeTabla3 = malloc(tamanio23);
        campoDeTabla4 = malloc(tamanio23);
        campoDeTabla5 = malloc(tamanio23);
        campoDeTabla6 = malloc(tamanio23);
        tabla->idJob=4;
		tabla->tabla_procesos = list_create();

		tabla2->idJob=7854;
		tabla2->tabla_procesos = list_create();

		tabla3->idJob=5;

		tabla3->tabla_procesos = list_create();


        campoDeTabla->bloque_archivo = 5;
		campoDeTabla->estado = 3;
		campoDeTabla->id_nodo = 4;
		campoDeTabla->nombre_archivo_resultado = "campoTabla1";

		campoDeTabla2->bloque_archivo = 2;
		campoDeTabla2->estado = 0;
		campoDeTabla2->id_nodo = 2;
		campoDeTabla2->nombre_archivo_resultado = "campoTabla2";

		campoDeTabla3->bloque_archivo = 4;
		campoDeTabla3->estado = 1;
		campoDeTabla3->id_nodo = 7;
		campoDeTabla3->nombre_archivo_resultado = "campoTabla3";

	    campoDeTabla4->bloque_archivo = 0;
		campoDeTabla4->estado = 1;
		campoDeTabla4->id_nodo = 54;
		campoDeTabla4->nombre_archivo_resultado = "campoTabla4";

		campoDeTabla5->bloque_archivo = 14;
		campoDeTabla5->estado = 2;
		campoDeTabla5->id_nodo = 54;
		campoDeTabla5->nombre_archivo_resultado = "campoTabla5";

		campoDeTabla6->bloque_archivo = 21;
		campoDeTabla6->estado = 3;
		campoDeTabla6->id_nodo = 54;
		campoDeTabla6->nombre_archivo_resultado = "campoTabla6";


		list_add(tabla->tabla_procesos,campoDeTabla);
		list_add(tabla->tabla_procesos,campoDeTabla2);
		list_add(tabla2->tabla_procesos,campoDeTabla3);
		list_add(tabla2->tabla_procesos,campoDeTabla4);
		list_add(tabla3->tabla_procesos,campoDeTabla5);
		list_add(tabla3->tabla_procesos,campoDeTabla6);

		lista_job_tabla = list_create();
		list_add(lista_job_tabla,tabla);
		list_add(lista_job_tabla,tabla2);
		list_add(lista_job_tabla,tabla3);

		lista_nodos_estado = list_create();
		t_nodo2 *nodo1;
		t_nodo2 *nodo2;
		t_nodo2 *nodo3;
		t_nodo2 *nodo4;
		t_nodo2 *nodo5;
		t_nodo2 *nodo6;

		nodo1=malloc(tamanio23);
		nodo2=malloc(tamanio23);
		nodo3=malloc(tamanio23);
		nodo4=malloc(tamanio23);
		nodo5=malloc(tamanio23);
		nodo6=malloc(tamanio23);


		nodo1->id_nodo = 4;
		nodo1->ip_nodo = "44.251";
		nodo1->puerto_nodo = "887";
		nodo2->id_nodo = 2;
		nodo2->ip_nodo = "44.250";
		nodo2->puerto_nodo = "886";
		nodo3->id_nodo = 7;
		nodo3->ip_nodo = "44.252";
		nodo3->puerto_nodo = "885";
		nodo4->id_nodo = 54;
		nodo4->ip_nodo = "44.253";
		nodo4->puerto_nodo = "884";
		nodo5->id_nodo = 23;
		nodo5->ip_nodo = "44.251";
		nodo5->puerto_nodo = "883";
		nodo6->id_nodo = 44;
		nodo6->ip_nodo = "44.254";
		nodo6->puerto_nodo = "882";

		list_add(lista_nodos_estado,nodo1);
		list_add(lista_nodos_estado,nodo2);
		list_add(lista_nodos_estado,nodo3);
		list_add(lista_nodos_estado,nodo4);
		list_add(lista_nodos_estado,nodo5);
		list_add(lista_nodos_estado,nodo6);


		  int a,b,c,d,e,aux,aux3,variableParaResultadoReduce,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
				int tamanioListaDeNodos,auxla=0,tamanio;
			    t_lista_job2 *campoDeLaLista;
			    campoDeLaLista = malloc(tamanio23*10);
				t_tablaProcesos_porJob2 *campoDeLaListaTablaDeProcesos;
		        campoDeLaListaTablaDeProcesos = malloc(tamanio23*10);
			    t_list * lista_archivosAReducirPorNodo;
			    t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
		        campoArchivosAReducirPorNodo= malloc(tamanio23*10);
			    t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
		    campoAAgregarAListaReducirPorNodo = malloc(tamanio23*10);
			    t_nodo2 *campoDeListaDeNodo;
			    campoDeListaDeNodo = malloc(tamanio23*10);
				lista_archivosAReducirPorNodo = list_create();
				list_clean(lista_archivosAReducirPorNodo);

				// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
				//reducir
				tamanioListaJobTabla=list_size(lista_job_tabla);// es 3

				printf("\n\nLos campos de la lista que deberian estar son:\n");

				for(a=0;a<tamanioListaJobTabla;a++){
					campoDeLaLista=list_get(lista_job_tabla,a); // corrroborado que los saca bien
					tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos); // es dos

					for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
						campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
						tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
				    	if(tamanioListaArchivosAReducirPorNodo == 0){
				    		campoAAgregarAListaReducirPorNodo->idNodo =  campoDeLaListaTablaDeProcesos->id_nodo;
				    		campoAAgregarAListaReducirPorNodo->archivosAReducir = list_create();
				    		list_add(campoAAgregarAListaReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
				    		asprintf(&campoAAgregarAListaReducirPorNodo->nombreArchivoResultado,"%s%i","archivo",variableParaResultadoReduce);
				    		variableParaResultadoReduce ++;
				    		// buscamos el ip y el puerto nodo
				    		tamanioListaDeNodos =list_size(lista_nodos_estado);
				    		aux=0;
				    		for(d=0;d<tamanioListaDeNodos && aux==0;d++){
				    			campoDeListaDeNodo = list_get(lista_nodos_estado,d);
				    			if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
				    				campoAAgregarAListaReducirPorNodo->ipNodo =  campoDeListaDeNodo->ip_nodo;
				    				campoAAgregarAListaReducirPorNodo->puertoNodo =  campoDeListaDeNodo->puerto_nodo;
				    				aux=1;

				    			}
				    		}// agrega el campo a la lista

				    		list_add(lista_archivosAReducirPorNodo,campoAAgregarAListaReducirPorNodo);
				    		campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    		printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    		e++;
				    	}
				    	else{
				    	aux=0;
				    		for(c=0;c <tamanioListaArchivosAReducirPorNodo && aux ==0;c++){
				    			campoArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,c);
				    			if(campoArchivosAReducirPorNodo->idNodo == campoDeLaListaTablaDeProcesos->id_nodo){
				    				list_add(campoArchivosAReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
				    				aux =1;
				    			}
				    		}
				    			if(aux==0){//cuando no esta el id del nodo, entonces tiene que agregar todo directo
				    				printf("Aca deberia entrar 4 veces \n");
				    				campoAAgregarAListaReducirPorNodo->idNodo =  campoDeLaListaTablaDeProcesos->id_nodo;
				    				campoAAgregarAListaReducirPorNodo->archivosAReducir = list_create();
				    				list_add(campoAAgregarAListaReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);

				    				asprintf(&campoAAgregarAListaReducirPorNodo->nombreArchivoResultado,"%s%i","archivo",variableParaResultadoReduce);
				    				variableParaResultadoReduce ++;
				    				// buscamos el ip y el puerto nodo
				    				tamanioListaDeNodos =list_size(lista_nodos_estado);
				    				aux=0;
				    				for(d=0;d<tamanioListaDeNodos && aux==0;d++){
				    					campoDeListaDeNodo = list_get(lista_nodos_estado,d);
				    					if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
				    						campoAAgregarAListaReducirPorNodo->ipNodo =  campoDeListaDeNodo->ip_nodo;
				    						campoAAgregarAListaReducirPorNodo->puertoNodo =  campoDeListaDeNodo->puerto_nodo;
				    						aux=1;

				    					}
				    				}// agrega el campo a la lista
				    				list_add(lista_archivosAReducirPorNodo,campoAAgregarAListaReducirPorNodo);
				    				campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    				printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    				e++;

				    			}
				    	}
					}
					}



           tamanio = list_size(lista_archivosAReducirPorNodo);
           printf("\n\nEl tamanio final de la lista es: %i\n",tamanio);
           printf("Lo que imprime a fuera del for es:\n");
           for(e=0;e<tamanio;e++){
        	   campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
        	   printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
           }
         free(campoDeLaLista);
         free(campoDeLaListaTablaDeProcesos);
         free(campoArchivosAReducirPorNodo);
         free(campoDeListaDeNodo);
         /*
		//
	    int a,b,c,d,aux=0,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
		int tamanioListaDeNodos,variableDelRealloc=0,aux2,tamanioListaNueva;
		char* archivosAReducirTemporales;
	    t_lista_job2 *campoDeLaLista;
	    campoDeLaLista = malloc(tamanio23*5);
		t_tablaProcesos_porJob2 *campoDeLaListaTablaDeProcesos;
        campoDeLaListaTablaDeProcesos = malloc(tamanio23*5);
	    t_archivosAReducirPorNodo *lista_archivosAReducirPorNodoParcial;
	    t_archivosAReducirPorNodo *lista_archivosAReducirPorNodoFinal;
	    t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
        campoArchivosAReducirPorNodo= malloc(tamanio23*5);
	    t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
	    campoAAgregarAListaReducirPorNodo = malloc(tamanio23*5);
	    t_nodo2 *campoDeListaDeNodo;
	    campoDeListaDeNodo = malloc(tamanio23*5);
		lista_archivosAReducirPorNodoParcial = malloc(sizeof(t_archivosAReducirPorNodo));

		// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
		//reducir
		tamanioListaJobTabla=list_size(lista_job_tabla);
		printf("tamanioListaJobTabla %i\n",tamanioListaJobTabla);
		tamanioListaNueva=sizeof(int)*2 + sizeof(char)*4;

		for(a=0;a<tamanioListaJobTabla;a++){
			campoDeLaLista=list_get(lista_job_tabla,a);
			tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos);
			for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
				campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
				if(aux==0){ // si es la primera vez hay que hacer un malloc para inicializar la lista que vamos a hacer nosotros, e inicializar la de archivos a reducir
					lista_archivosAReducirPorNodoParcial = malloc(tamanioListaNueva);
					lista_archivosAReducirPorNodoParcial[0].idNodo =campoDeLaListaTablaDeProcesos->id_nodo;
					lista_archivosAReducirPorNodoParcial[0].archivosAReducir=malloc(sizeof(char));
					lista_archivosAReducirPorNodoParcial[0].archivosAReducir[0]=campoDeLaListaTablaDeProcesos->nombre_archivo_resultado;
					lista_archivosAReducirPorNodoParcial[0].cantidadDeArchivosAReducir=1;
					asprintf(&lista_archivosAReducirPorNodoParcial[0].nombreArchivoResultado,"%s%i","archivo",aux);
					tamanioListaDeNodos =list_size(lista_nodos_estado);
					aux2=0;
					for(d=0;d<tamanioListaDeNodos && aux2==0;d++){
						campoDeListaDeNodo = list_get(lista_nodos_estado,d);
						if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
							lista_archivosAReducirPorNodoParcial[0].ipNodo=  campoDeListaDeNodo->ip_nodo;
							lista_archivosAReducirPorNodoParcial[0].puertoNodo =  campoDeListaDeNodo->puerto_nodo;
							aux2=1;
						}
					}
					aux ++;
					printf("tiene que ser solo uno\n");
				}
				else{ // aca ya no es la primera, hay que ver si el id del nodo es el mismo o no a alguno, si es el mismo hay que hacer un realloc en los archivos a reducir
					//si no es el mismo hay que hacer un realloc en la lista que hacemos nosotros y hay que inicializar(hacer un malloc) en la lista de archivos a reducir
					aux2=0;
					for(c=0;c<aux && aux2==0;c++){// aux va a contener cuantas posiciones del vector hay
						if(lista_archivosAReducirPorNodoParcial[c].idNodo==campoDeListaDeNodo->id_nodo){
							// aca la va a recorrer y si encuentra un id que coincide ahi agrega el archivo a reducir
							lista_archivosAReducirPorNodoParcial[c].cantidadDeArchivosAReducir ++;
							archivosAReducirTemporales = realloc(lista_archivosAReducirPorNodoParcial[c].archivosAReducir, lista_archivosAReducirPorNodoParcial[c].cantidadDeArchivosAReducir * sizeof(char));
							/* Si hubo un error
							if (archivosAReducirTemporales == NULL)
							{
							printf("Error al reservar memoria\n");
							/* Liberamos la memoria del puntero
							y terminamos el programa
							free(lista_archivosAReducirPorNodoParcial[c].archivosAReducir);
							exit(1);
							}
							lista_archivosAReducirPorNodoParcial[c].archivosAReducir= archivosAReducirTemporales;
							lista_archivosAReducirPorNodoParcial[c].archivosAReducir[(lista_archivosAReducirPorNodoParcial[c].cantidadDeArchivosAReducir - 1 )] = campoDeLaListaTablaDeProcesos->nombre_archivo_resultado;
							aux2=1;
						}
					} si aux2 es igual a cero, significa que nunca entro por lo que ahi vamos a tener que hacer el realloc y el malloc
					if(aux2==0){
						lista_archivosAReducirPorNodoFinal = realloc(lista_archivosAReducirPorNodoParcial,(aux+1)*tamanioListaNueva);
						if (lista_archivosAReducirPorNodoFinal == NULL) {
							printf("Error al reservar memoria\n");
							/* Liberamos la memoria del puntero
													y terminamos el programa
							free(lista_archivosAReducirPorNodoFinal);
							exit(1);
						}

						lista_archivosAReducirPorNodoParcial[aux].idNodo =campoDeLaListaTablaDeProcesos->id_nodo;
						lista_archivosAReducirPorNodoParcial[aux].archivosAReducir=malloc(sizeof(char));
						lista_archivosAReducirPorNodoParcial[aux].archivosAReducir[0]=campoDeLaListaTablaDeProcesos->nombre_archivo_resultado;
						lista_archivosAReducirPorNodoParcial[aux].cantidadDeArchivosAReducir=1;
						asprintf(&lista_archivosAReducirPorNodoParcial[aux].nombreArchivoResultado,"%s%i","archivo",aux);
						tamanioListaDeNodos =list_size(lista_nodos_estado);
						aux2=0;
						for(d=0;d<tamanioListaDeNodos && aux2==0;d++){
							campoDeListaDeNodo = list_get(lista_nodos_estado,d);
							if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
								lista_archivosAReducirPorNodoParcial[aux].ipNodo=  campoDeListaDeNodo->ip_nodo;
								lista_archivosAReducirPorNodoParcial[aux].puertoNodo =  campoDeListaDeNodo->puerto_nodo;
								aux2=1;
							}
						}
						aux ++;
					}

				}
			}
		}

		for(d=0;d<aux;d++){
			printf("vector[%i]:\nidNodo %i\n ipNodo %c\n puertoNodo %c\n nombreArchivoResultado %c \n\n ",lista_archivosAReducirPorNodoFinal[aux].idNodo,lista_archivosAReducirPorNodoFinal[aux].ipNodo,lista_archivosAReducirPorNodoFinal[aux].puertoNodo,lista_archivosAReducirPorNodoFinal[aux].nombreArchivoResultado);
		}

		free(lista_archivosAReducirPorNodoParcial);

/*
	*/












	/*
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
		}*/
		return EXIT_SUCCESS;
}








