/*
 * rutinasMapperYReducer.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"

#define NUM_PIPES 2

#define LEER_FD  0
#define ESCRIBIR_FD 1

#define PARENT_ESCRIBE_EN_CHILD_PIPE  0
#define PARENT_LEE_EN_CHILD_PIPE   1

#define PARENT_LEE_FD  (pipes[PARENT_LEE_EN_CHILD_PIPE][LEER_FD])
#define PARENT_ESCRIBE_FD (pipes[PARENT_ESCRIBE_EN_CHILD_PIPE][ESCRIBIR_FD])

#define CHILD_LEE_FD   (pipes[PARENT_ESCRIBE_EN_CHILD_PIPE][LEER_FD])
#define CHILD_ESCRIBE_FD  (pipes[PARENT_LEE_EN_CHILD_PIPE][ESCRIBIR_FD])

void *mapper(void* arg){
	int socket = (int) arg;
	int pipes[NUM_PIPES][2];

	pipe(pipes[PARENT_LEE_EN_CHILD_PIPE ]);
	pipe(pipes[PARENT_ESCRIBE_EN_CHILD_PIPE ]);

		if(!fork()) {
			//Con esto establezco que el child lea del parent
			char* script[]={"/tmp/map.sh","-q",0};
			dup2(CHILD_LEE_FD, STDIN_FILENO);
			dup2(CHILD_ESCRIBE_FD, STDOUT_FILENO);

			close(CHILD_LEE_FD);
			close(CHILD_ESCRIBE_FD);
			close(PARENT_LEE_FD);
			close(PARENT_ESCRIBE_FD);

			execv(script[0], script);

		}else{

			int tamanioBloque, tamanioBloqueExacto,nroDelBloque;

			close(CHILD_LEE_FD);
			close(CHILD_ESCRIBE_FD);

			char* pmap = mapearAMemoriaVirtual(archivo_bin);

			recv(socket,&nroDelBloque,sizeof(int),0);

			tamanioBloque=tamanioEspecifico(pmap,nroDelBloque);

			printf("%i\n",tamanioBloque);
			char* bloque=malloc(tamanioBloque);

			tamanioBloqueExacto = nroDelBloque /* * 1024 * 1024* 20*/;
			memcpy(bloque,pmap + tamanioBloqueExacto,tamanioBloque);

			write(PARENT_ESCRIBE_FD,bloque,strlen(bloque));

			//Ahora deberia leer y ver que nombre le ponemos a los archivos
		}

	return NULL;
}

void *reducer(void* arg){
	int pipes[NUM_PIPES][2];

	pipe(pipes[PARENT_LEE_EN_CHILD_PIPE ]);
	pipe(pipes[PARENT_ESCRIBE_EN_CHILD_PIPE ]);

	if(!fork()) {
	//Con esto establezco que el child lea del parent
		char* script[]={"/tmp/reduce.sh","-q",0};
		dup2(CHILD_LEE_FD, STDIN_FILENO);
		dup2(CHILD_ESCRIBE_FD, STDOUT_FILENO);

		close(CHILD_LEE_FD);
		close(CHILD_ESCRIBE_FD);
		close(PARENT_LEE_FD);
		close(PARENT_ESCRIBE_FD);

		execv(script[0], script);

		}else{

		close(CHILD_LEE_FD);
		close(CHILD_ESCRIBE_FD);

		//A definir que le escribimos y que leo (nombre del archivo);



	}
	return NULL;
}
