/*
 * atenderJob.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

void* atenderJob(void* arg){
	int socket= (int)arg;
	int estado = 1;
	int comando;
	int comando2;
	int tamanioScript;
	recv(socket, &comando, sizeof(int),0 );

	pthread_t hiloMapper;
	pthread_t hiloReducer;

	while(1){
		recv(socket, &comando, sizeof(int),0 );
		switch(comando){
			case 1: //Este va a ser el map
				printf("Se levanto un hilo mapper\n");
				pthread_create(&hiloMapper, NULL,&mapper,(void*) socket);
				break;

			case 2: //Este va a ser el reduce
				printf("Se levanto un hilo reducer\n");
				pthread_create(&hiloReducer, NULL,&reducer,(void*) socket);
				break;

			case 3: //Aca me escribe las rutinas
				while(estado == 1){
				recv(socket, &comando, sizeof(int),0 );
				switch(comando2){
					case 1: //Me escribe una rutina mapper
						recv(socket,&tamanioScript,sizeof(int),0);
						recive_y_deserialisa_CHARp(script_mapper, socket, tamanioScript);
						escribirScript(script_mapper,dir_temp,comando2);
						break;

					case 2: //Me escribe una rutina reducer
						recv(socket,&tamanioScript,sizeof(int),0);
						recive_y_deserialisa_CHARp(script_reducer, socket, tamanioScript);
						escribirScript(script_reducer,dir_temp,comando2);
						break;
					case 3:
						estado = 0;
						break;
					}
				}
		}
	}
	return NULL;
}

