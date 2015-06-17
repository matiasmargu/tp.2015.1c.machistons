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
	recv(socket, &comando, sizeof(int),0 );

	pthread_t hiloMapper;
	pthread_t hiloReducer;


	switch(comando){
	case 1: //Este va a ser el map
		pthread_create(&hiloMapper, NULL,&mapper, NULL);
		break;
	case 2: //Este va a ser el reduce
		pthread_create(&hiloReducer, NULL,&reducer, NULL);
		break;
	}

	return NULL;
}

