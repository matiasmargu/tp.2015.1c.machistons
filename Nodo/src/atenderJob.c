/*
 * atenderJob.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

void* atenderJob(void* arg){
	char modo[] = "0777";
	int socket = (int)arg;
	int comando=1;
	int tamanioScript;
	char* direccionAuxiliar=malloc(strlen(dir_temp));
	char* script_mapper;
	char* script_reducer;
	int i;

	pthread_t hiloMapper[1000];
	pthread_t hiloReducer;

	send(socket, &comando,sizeof(int),0);

	int cont1;

	while(1){

		//77 salio todo bien
		//66 salio todo mal

		recv(socket, &comando, sizeof(int),0 );
		switch(comando){
			case 1: //Este va a ser el map
				printf("Se levanto un hilo mapper\n");

				send(socket, &comando,sizeof(int),0);

				recv(socket,&tamanioScript,sizeof(int),0);
				send(socket, &comando,sizeof(int),0);


				script_mapper=malloc(tamanioScript);

				recv(socket,script_mapper,tamanioScript,0);

				escribirScript(script_mapper,1);

				i = strtol(modo, 0, 8);
				chmod("/tmp/mapper",modo);

				printf("se asignaron los permisos\n");

				pthread_create(&hiloMapper[cont1], NULL, (void*) mapper,(void*) socket);
				cont1++;
				break;

			case 2: //Este va a ser el reduce
				printf("Se levanto un hilo reducer.sh\n");

				send(socket, &comando,sizeof(int),0);

				recv(socket,&tamanioScript,sizeof(int),0);
				send(socket, &comando,sizeof(int),0);
				script_reducer=malloc(tamanioScript);

				recv(socket,script_reducer,tamanioScript,0);
				escribirScript(script_reducer,2);
				send(socket, &comando,sizeof(int),0);

				strcpy(direccionAuxiliar,dir_temp);
				strcat(direccionAuxiliar,"/reducer");

				chmod(direccionAuxiliar,modo);

			//	pthread_create(&hiloReducer, NULL,reducer,(void*) socket);
				break;
				}
		}
	free(direccionAuxiliar);
	return NULL;
}

