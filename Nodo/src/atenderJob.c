/*
 * atenderJob.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#include "atenderJob.h"
#include "rutinasMapperYReducer.h"


void* atenderJob(void* arg){
	int socket= (int)arg;
	int comando;
	int tamanioScript;
	recv(socket, &comando, sizeof(int),0 );

	pthread_t hiloMapper;
	pthread_t hiloReducer;

	if(strlen(script_mapper)==0){
		recv(socket,&tamanioScript,sizeof(int),0);
		recive_y_deserialisa_SCRIPT(script_mapper, socket, tamanioScript);
	}

	if(strlen(script_reducer)==0){
		recv(socket,&tamanioScript,sizeof(int),0);
		recive_y_deserialisa_SCRIPT(script_reducer, socket, tamanioScript);
	}

	switch(comando){
	case 1: //Este va a ser el map
		pthread_create(&hiloMapper, NULL,&mapper,(void*) socket);
		break;
	case 2: //Este va a ser el reduce
		pthread_create(&hiloReducer, NULL,&reducer,(void*) socket);
		break;
	}

	return NULL;
}

