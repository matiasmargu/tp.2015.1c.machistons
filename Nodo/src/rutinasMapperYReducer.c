/*
 * rutinasMapperYReducer.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"


#define SIZE 20*1024*1024

void mapper(t_mapper* arg){

	char* buffer=malloc(SIZE);
	char* bloque=malloc(SIZE);

	char *resultado;
	char* resultado_aux;

	variableDatos=1;


	t_getBloque infoBloque;

	pid_t pid;
	int status;

	printf("el argumento: %i %i %s\n",arg->bloque_map,arg->socket,arg->resultado);

//***************************************************

	infoBloque=getBloque(arg->bloque_map);

	strip(infoBloque.contenido);

	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);

//*******************************************


	 if ( (pid=fork()) == 0 )
	  { // hijo

		dup2(pipe_padreAHijo[0],STDIN_FILENO);
		dup2(pipe_hijoAPadre[1],STDOUT_FILENO);

		close( pipe_padreAHijo[1] ); /* cerramos el lado de escritura del pipe */
		close( pipe_hijoAPadre[0] ); /* cerramos el lado de lectura del pipe */
		close( pipe_hijoAPadre[1]);
		close( pipe_padreAHijo[0]);

		execve(arg->mapper,NULL,NULL);
		//system("/tmp/mapper");
		//exit(1);

	  }
	  else
	  { // padre
	    close( pipe_padreAHijo[0] ); /* cerramos el lado de lectura del pipe */
	    close( pipe_hijoAPadre[1] ); /* cerramos el lado de escritura del pipe */

	    //Aca escribe en hijo
	    write( pipe_padreAHijo[1], infoBloque.contenido, infoBloque.tamanio );
	    //write( pipe_padreAHijo[1], "hola faknflanflfan", strlen("hola faknflanflfan") );
	    close( pipe_padreAHijo[1]);

	    waitpid(pid,&status,0);

	    //Aca leo del hijo
	    read( pipe_hijoAPadre[0], buffer, SIZE );
	    close( pipe_hijoAPadre[0]);

	 }

	asprintf(&resultado_aux,"%s%s","/tmp/resultadoDelMapPorOrdenar",string_itoa(arg->socket));

	FILE* fdMapeo = fopen(resultado_aux,"w");
	fputs(buffer,fdMapeo);
	fclose(fdMapeo);

	asprintf(&resultado,"%s%s","/tmp/",arg->resultado);
	printf("Aca esta el temporal: %s\n",resultado);

	ordernarAlfabeticamente(resultado,resultado_aux);
	remove(arg->mapper);

	int entero = 42;
	send(arg->socket,&entero,sizeof(int),0);

	variableDatos=0;
	free(buffer);
	free(bloque);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void reducer(void* arg){

#define SIZE 20*1024*1024


	pid_t pid;
	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);

	int comando,tamanioDeLaEstructura;
	t_job_nodo_reduce red;
	int indice = 0;
	char* bufferProv;
	char* buffer;

	if(recv(socket,&tamanioDeLaEstructura,sizeof(int),0)<0){
		return;
	}
	send(socket, &comando,sizeof(int),0);
	recive_y_deserializa_EST_REDUCE(&red,socket,tamanioDeLaEstructura);



	while(!list_is_empty(red.archivosAreducir)){
		char* nombre=list_remove(red.archivosAreducir,indice);
		bufferProv = mapearAMemoriaVirtual(nombre);
		if(indice == 0){
			buffer = bufferProv;
		}else{
			string_append(buffer,bufferProv);
		}
		indice++;

	}


	if((pid = fork()) == -1){
		printf("Error en el fork");
		exit(1);
	}


	 if ( (pid=fork()) == 0 )
	  { // hijo
	    close( pipe_padreAHijo[1] ); /* cerramos el lado de escritura del pipe */
	    close( pipe_hijoAPadre[0] ); /* cerramos el lado de lectura del pipe */

	    dup2(pipe_padreAHijo[0],STDIN_FILENO);
	    dup2(pipe_hijoAPadre[1],STDOUT_FILENO);

	    execv("/tmp/reduce",NULL);
	  }
	  else
	  { // padre
	    close( pipe_padreAHijo[0] ); /* cerramos el lado de lectura del pipe */
	    close( pipe_hijoAPadre[1] ); /* cerramos el lado de escritura del pipe */

		    //Aca escribe en hijo
	    write( pipe_padreAHijo[1], buffer, strlen( buffer ) );
	    close( pipe_padreAHijo[1]);
		    //Aca leo del hijo
	   read( pipe_hijoAPadre[0], buffer, SIZE );

	   char* tmp = "/tmp/";
	   string_append(&tmp,red.nombreArchivoResultado);
	   FILE* fdRed = fopen(tmp,"w");
	   fputs(buffer,fdRed);
	   fclose(fdRed);

	   char* archivoResultado = mapearAMemoriaVirtual(tmp);

	   //ordernarAlfabeticamente(red.nombreArchivoResultado,fdRed,sizeof(archivoResultado));

	    //close( pipe_hijoAPadre[0]);
	  }
	  waitpid( pid, NULL, 0 );
	//  free(buffer);
	//  free(bloque);
}

