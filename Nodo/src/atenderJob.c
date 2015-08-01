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

	char* listaAux[1000];
	char* paquete;
	int tamanioDinamico;
	int bloque_map;
	int offset;

	char* resultado_map;
	char* resultado_script;

	char* dir_map;
	char* dir_red;
	int cantidadArch;

	int a;
	char* pruebita;


	t_para_nodo* comb;
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

		if(recv(socket, &comando, sizeof(int),0)<0)return NULL;


		//printf("Esto me llega una vez: %i\n",comando);
		switch(comando){
			case 1: //Este va a ser el map
				printf("Se levanto un hilo mapper\n");
				offset=0;
				tamanioDinamico=0;

				send(socket, &comando,sizeof(int),0);

				if(recv(socket,&tamanioScript,sizeof(int),0)<0) return NULL;
				paquete = malloc(tamanioScript);//Hago el espacio para el paquete

				send(socket, &comando,sizeof(int),0);


				if(recv(socket,paquete,tamanioScript,0)<0)return NULL;

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

				if((recv(socket,&tamanioScript,sizeof(int),0)<0)) return NULL;
				printf("tam: %i\n",tamanioScript);
				send(socket, &comando,sizeof(int),0);

				// paquete a deserializar [tamanioResultado][Resultado][tamanioScript][scrip][cantidadDeArchivos]([tamanioNombreArchivo][Nombre])*
				// * bucle para archivos
				paquete=malloc(tamanioScript);
				if(recv(socket,paquete,tamanioScript,0)<0)return NULL;

				memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
				offset += sizeof(int);

				resultado_script = malloc(tamanioDinamico);
				memcpy(resultado_script, paquete + offset, tamanioDinamico);
				offset += tamanioDinamico;
				//printf("res: %s\n",resultado_script);

				memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
				offset += sizeof(int);

				script_reducer = malloc(tamanioDinamico);
				memcpy(script_reducer, paquete + offset, tamanioDinamico);
				offset += tamanioDinamico;
				//printf("scr: %s\n",script_reducer);

				memcpy(&cantidadArch,paquete + offset, sizeof(int));
				offset += sizeof(int);
				//printf("cant: %i\n",cantidadArch);

				//deserializa la lista
				//listaArchivos = list_create();
				for(a=0; a<cantidadArch ; a++){
					//printf("o: %i\n",offset);
					memcpy(&tamanioDinamico, paquete + offset, sizeof(int));
					offset += sizeof(int);
					//printf("o dsp: %i\n",offset);
					//printf("tam din: %i\n",tamanioDinamico);

					pruebita = malloc(tamanioDinamico);
					memcpy(pruebita, paquete + offset, tamanioDinamico);
					offset += tamanioDinamico;
					//printf("el archivo: %s\n",pruebita);
					//list_add(listaArchivos,archivo);
					listaAux[a]=pruebita;
				}


				est_red=malloc(sizeof(t_reduce));

				est_red->socket=socket;
				est_red->resultado=resultado_script;

				//printf("socket: %i\nres: %s\n",est_red->socket,est_red->resultado);

				int contaE=0;
				while(contaE<cantidadArch){
					est_red->lista[contaE]=listaAux[contaE];
					//printf("arch: %s\n",est_red->lista[contaE]);
					contaE++;
				}
				est_red->cant=cantidadArch;

				dir_red=escribirScript(script_reducer,2,est_red->socket);

				if(chmod(dir_red,0777)<0){
					printf("No se le pudieron agregar los permisos\n");
				}

				est_red->reducer=dir_red;


				printf("Este es el archivo resultado: %s\n",est_red->resultado);

				printf("se asignaron los permisos\n");

				pthread_create(&hiloReducer[cont2], NULL,(void*)reducer,(void*) est_red);
				cont2++;
				break;
			case 3:
<<<<<<< HEAD
				pthread_mutex_lock(&mutex_reduce);
=======
				//pthread_mutex_lock(&mutexNodo);
>>>>>>> 03f844682401e2b9f554468ec2cb877360f738f3
				printf("Se levanto un movimiento de archivos\n");

				pthread_mutex_lock(&mutexNodo);
				send(socket, &comando,sizeof(int),0);

				if(recv(socket,&tamanioTotalIP_P,sizeof(int),0)<0) return NULL;
				send(socket, &comando,sizeof(int),0);
				//printf("tam: %i\n",tamanioTotalIP_P);
				comb=malloc(sizeof(t_para_nodo));
				recive_y_deserializa_NODO_C(comb,socket,tamanioTotalIP_P);


				//printf("Este es el IP: %s\nEste es el PUERTO: %s\nEste es el nombre del arch: %s\n",comb->ip,comb->puerto,comb->archivo);

				int socket_nodo = crearCliente(comb->ip,comb->puerto);
				comando=7;
				send(socket_nodo,&comando,sizeof(int),0);
				printf("IP: %s\n PUERTO: %s\n",comb->ip,comb->puerto);
				recv(socket_nodo,&comando,sizeof(int),0);
				comando=3;

				send(socket_nodo,&comando,sizeof(int),0);
				pthread_mutex_unlock(&mutexNodo);

				pedirContenidoDeUnArchivo(comb->archivo,socket_nodo);

				pthread_mutex_lock(&mutexNodo);
				send(socket, &comando,sizeof(int),0);
<<<<<<< HEAD
				pthread_mutex_unlock(&mutex_reduce);
=======
				pthread_mutex_unlock(&mutexNodo);
>>>>>>> 03f844682401e2b9f554468ec2cb877360f738f3
				break;
				}
		}
}

