/*
 * atenderJob.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

void* atenderJob(void* arg){
	int socket = (int)arg;
	int comando=1;
	int tamanioScript;
	char* script_mapper;
	char* script_reducer;
	int tamanioTotalIP_P;

	char* paquete;
	int tamanioDinamico;
	int bloque_map;
	int offset;

	char* resultado_map;
	char* resultado_script;

	char* dir_map;
	char* dir_red;
	int cantidadArch;


	t_para_nodo comb;
	t_mapper* est_map;
	t_reduce* est_red;
	t_list* listaArchivos;

	pthread_t hiloMapper[1000];
	pthread_t hiloReducer[1000];


	int cont1=0;
	int cont2=0;

	variableDatos=0;

	send(socket, &comando,sizeof(int),0);

	while(1){

		if(recv(socket, &comando, sizeof(int),0)<0){
			printf("Se callo un job\n");
			return NULL;
		}


		//printf("Esto me llega una vez: %i\n",comando);
		switch(comando){
			case 1: //Este va a ser el map
				printf("Se levanto un hilo mapper\n");
				offset=0;
				tamanioDinamico=0;

				send(socket, &comando,sizeof(int),0);

				if(recv(socket,&tamanioScript,sizeof(int),0)<0){
					printf("Se callo un job\n");
					return NULL;
				}
				paquete = malloc(tamanioScript);//Hago el espacio para el paquete

				send(socket, &comando,sizeof(int),0);


				if(recv(socket,paquete,tamanioScript,0)<0){//recivo el paquete
					printf("Se callo un job\n");
					return NULL;
				}

				memcpy(&(bloque_map), paquete + offset, sizeof(int));//guarto el bloque en bloque_map
				offset += sizeof(int);

				memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
				offset += sizeof(int);

				script_mapper = malloc(tamanioDinamico);
				memcpy(script_mapper, paquete + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
				offset += sizeof(int);

				resultado_map = malloc(tamanioDinamico);
				memcpy(resultado_map, paquete + offset, tamanioDinamico);
				offset += tamanioDinamico;

				est_map=malloc(sizeof(t_mapper));

				est_map->bloque_map = bloque_map;
				est_map->socket = socket;
				est_map->resultado=resultado_map;

				printf("En el bloque %i hago un map, y me da %s\n",est_map->bloque_map,est_map->resultado);

				dir_map=escribirScript(script_mapper,1,est_map->socket);

				if(chmod(dir_map,0777)<0){
					printf("No se le pudieron agregar los permisos\n");
				}

				est_map->mapper=dir_map;

				printf("Termino la asignacion de permisos\n");

				pthread_create(&hiloMapper[cont1], NULL, (void*) mapper, (void*) est_map);
				cont1++;
				break;

			case 2: //Este va a ser el reduce
				printf("Se levanto un hilo reduce\n");
				tamanioDinamico=0;
				offset=0;

				send(socket, &comando,sizeof(int),0);

				recv(socket,&tamanioScript,sizeof(int),0);
				send(socket, &comando,sizeof(int),0);
				script_reducer=malloc(tamanioScript);

				recv(socket,script_reducer,tamanioScript,0);
				send(socket, &comando,sizeof(int),0);

				// paquete a deserializar [tamanioResultado][Resultado][tamanioScript][scrip][cantidadDeArchivos]([tamanioNombreArchivo][Nombre])*
				// * bucle para archivos
				paquete=malloc(tamanioScript);

				memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
				offset += sizeof(int);

				resultado_script = malloc(tamanioDinamico);
				memcpy(resultado_map, paquete + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
				offset += sizeof(int);

				script_reducer = malloc(tamanioDinamico);
				memcpy(script_mapper, paquete + offset, tamanioDinamico);
				offset += tamanioDinamico;

				memcpy(&cantidadArch,paquete + offset, sizeof(cantidadArch));
				offset += sizeof(cantidadArch);

				int a;
				char* archivo;

				//deserializa la lista
				listaArchivos = list_create();
				for(a=0;a< cantidadArch ; a++){
					memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
					offset += sizeof(int);
					archivo = malloc(tamanioDinamico);
					memcpy(archivo, paquete + offset, tamanioDinamico);
					offset += tamanioDinamico;
					printf("el archivooo %s \n\n",archivo);
					list_add(listaArchivos,archivo);
				}


				est_red=malloc(sizeof(t_reduce));

				est_red->socket=socket;
				est_red->resultado=resultado_script;
				est_red->lista=listaArchivos;

				dir_red=escribirScript(script_reducer,2,est_red->socket);

				if(chmod(dir_red,0777)<0){
					printf("No se le pudieron agregar los permisos\n");
				}


				est_red->reducer=dir_red;

				printf("se asignaron los permisos\n");

				pthread_create(&hiloReducer[cont2], NULL,(void*)reducer,(void*) est_red);
				cont2++;
				break;
			case 3:
				printf("Se levanto un movimiento de archivos\n");

				send(socket, &comando,sizeof(int),0);

				recv(socket,&tamanioTotalIP_P,sizeof(int),0);
				send(socket, &comando,sizeof(int),0);
				recive_y_deserializa_NODO_C(&comb,socket,tamanioTotalIP_P);
				send(socket, &comando,sizeof(int),0);

				int socket_nodo = crearCliente(comb.ip,comb.puerto);

				comando=3;

				send(socket_nodo,&comando,sizeof(int),0);
				pedirContenidoDeUnArchivo(comb.archivo,socket_nodo);
				break;
				}
		}

	return NULL;
}

