/*
 * rutinasMapperYReducer.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"

#define NUM_PIPES 2

#define READ_FD  0
#define WRITE_FD 1

#define PARENT_WRITE_PIPE  0
#define PARENT_READ_PIPE   1

#define PARENT_READ_FD  (pipes[1][0])
#define PARENT_WRITE_FD (pipes[0][1])

#define CHILD_READ_FD   (pipes[0][0])
#define CHILD_WRITE_FD  (pipes[1][1])

void *mapper(void* arg){
	int socket = (int)arg;
	int pipes[NUM_PIPES][2];

	return NULL;
}

void *reducer(void* arg){
	int socket = (int)arg;
	int pipes[NUM_PIPES][2];

	return NULL;
}
