/*
 * rutinasMapperYReducer.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"


void mapper(void* arg){

	FILE* fdAux;

	char* bloque;
	char* nombreDelArchivo;

	int comando =1;
	int socket = (int) arg;

	pid_t pid;

//***************************************************

	send(socket, &comando,sizeof(int),0);

	//Deveria recivir el nro del bloque
	int nroDeBloque=0;
	int tamanioDelBloque=getBloque(nroDeBloque, bloque);

	FILE* fdArch2 = fopen("/tmp/aMappear.txt","w+");
	fputs(bloque, fdArch2);
	fclose(fdArch2);

	int pipes[2];
	int fdMap = dup(1);

	pipe(pipes);

//*******************************************

	if((pid = fork()) == -1){
		printf("Error en el fork");
		exit(1);
	}


	if(pid == 0) {

		printf("Por aca todo bien");

		close(1);//cierro stdout
		//Ahora tengo que estableces el nuevo stdout
		dup(pipes[1]);

		close(pipes[0]);
		execlp("cat","cat","/tmp/aMappear.txt", NULL);

	}else{

		close(0);
		dup(pipes[0]);
		close(pipes[1]);

		FILE * stdout=fopen("/tmp/resultadotemporal.tmp", "w");
		dup2(fileno(stdout), 1);
		system("./tmp/mapper");
	}

	close(1);
	dup(fdMap);
	close(fdMap);

	free(nombreDelArchivo);
	free(bloque);
	fclose(fdAux);
}
/*
void *reducer(void* arg){
	int tamanioDeLaEstructura;
	int socket = (void*) arg;
	int pipes[NUM_PIPES][2];
	int comando=2;
	t_job_nodo_reduce red;

	pipe(pipes[PARENT_LEE_EN_CHILD_PIPE ]);
	pipe(pipes[PARENT_ESCRIBE_EN_CHILD_PIPE ]);


	if(!fork()) {
	//Con esto establezco que el child lea del parent
		char* script[]={"/tmp/reducer","-q",0};
		dup2(CHILD_LEE_FD, STDIN_FILENO);
		dup2(CHILD_ESCRIBE_FD, STDOUT_FILENO);

		close(CHILD_LEE_FD);
		close(CHILD_ESCRIBE_FD);
		close(PARENT_LEE_FD);
		close(PARENT_ESCRIBE_FD);

		printf("Esta por ejecutar\n");

		execv(script[0], script);

	}else{

		char* bloqueReduce[1024*1024*20];

		close(CHILD_LEE_FD);
		close(CHILD_ESCRIBE_FD);

		recv(socket,&tamanioDeLaEstructura,sizeof(int),0);
		send(socket, &comando,sizeof(int),0);
		recive_y_deserializa_EST_REDUCE(&red,socket,tamanioDeLaEstructura);

		//Escribo en child
		write(PARENT_ESCRIBE_FD,red.archivosAreducir,strlen(red.archivosAreducir));

		//Leo y escribo el nuevo archivo
		read(PARENT_LEE_FD, bloqueReduce ,sizeof(bloqueReduce)-1);

		FILE* fdAux = fopen(red.nombreArchivoResultado,"w");
		fputs(bloqueReduce,fdAux);
		fclose(fdAux);



	}
	printf("Termino el mapper\n");
	return NULL;
}*/
