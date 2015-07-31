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
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void reducer(t_reduce* arg){
	pid_t pid;
	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);

	int indice = 0;
	int status;
	char* bufferProv;
	char* buffer;

	char*resultado;
	char*resultado_aux;
	char* elim;
	char* bufferRed;


	while(!list_is_empty(arg->lista)){
		char* nombre=list_remove(arg->lista,indice);
		bufferProv = mapearAMemoriaVirtual(nombre);
		if(indice == 0){
			buffer = bufferProv;
		}else{
			string_append(buffer,bufferProv);
		}
		indice++;
		asprintf(&elim,"%s%s","/tmp/",nombre);
		remove(elim);
	}


	 if ( (pid=fork()) == 0 )
		  { // hijo

			dup2(pipe_padreAHijo[0],STDIN_FILENO);
			dup2(pipe_hijoAPadre[1],STDOUT_FILENO);

			close( pipe_padreAHijo[1] ); /* cerramos el lado de escritura del pipe */
			close( pipe_hijoAPadre[0] ); /* cerramos el lado de lectura del pipe */
			close( pipe_hijoAPadre[1]);
			close( pipe_padreAHijo[0]);

			execve(arg->reducer,NULL,NULL);
			//system("/tmp/mapper");
			//exit(1);

		  }
		  else
		  { // padre
		    close( pipe_padreAHijo[0] ); /* cerramos el lado de lectura del pipe */
		    close( pipe_hijoAPadre[1] ); /* cerramos el lado de escritura del pipe */

		    //Aca escribe en hijo
		    write( pipe_padreAHijo[1], buffer, strlen(buffer) );
		    //write( pipe_padreAHijo[1], "hola faknflanflfan", strlen("hola faknflanflfan") );
		    close( pipe_padreAHijo[1]);

		    waitpid(pid,&status,0);

		    //Aca leo del hijo
		    read( pipe_hijoAPadre[0], bufferRed, SIZE );
		    close( pipe_hijoAPadre[0]);

		 }
	asprintf(&resultado_aux,"%s%s","/tmp/resultadoDelReducePorOrdenar",string_itoa(arg->socket));

	FILE* fdRed = fopen(resultado_aux,"w");
	fputs(bufferRed,fdRed);
	fclose(fdRed);

	asprintf(&resultado,"%s%s","/tmp/",arg->resultado);
	printf("Aca esta el temporal: %s\n",resultado);


	ordernarAlfabeticamente(resultado,resultado_aux);
	free(bufferRed);
	free(buffer);

	int entero = 42;
	send(arg->socket,&entero,sizeof(int),0);
	return;

}

