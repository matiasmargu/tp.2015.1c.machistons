/*
 * rutinasMapperYReducer.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */

#include "variablesGlobales.h"


void mapper(void* arg){

#define SIZE 512

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

	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);

//*******************************************

	if((pid = fork()) == -1){
		printf("Error en el fork");
		exit(1);
	}


	 if ( (pid=fork()) == 0 )
	  { // hijo


		dup2(pipe_padreAHijo[0],STDIN_FILENO);
		dup2(pipe_hijoAPadre[1],STDOUT_FILENO);

		execv("/tmp/mapper",NULL);

	  }
	  else
	  { // padre
	    close( pipe_padreAHijo[0] ); /* cerramos el lado de lectura del pipe */
	    close( pipe_hijoAPadre[1] ); /* cerramos el lado de escritura del pipe */

	    //Aca escribe en hijo
	    write( pipe_padreAHijo[1], bloque, tamanioDelBloque );
	    close( pipe_padreAHijo[1]);

	    //Aca leo del hijo
	    //while( (readbytes=read( pipe_hijoAPadre[0], buffer, SIZE )) > 0)
	    //close( pipe_hijoAPadre[0]);
	  }
	  waitpid( pid, NULL, 0 );
}

void reducer(void* arg){
	pid_t pid;
	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);

	//*******************************************

	if((pid = fork()) == -1){
		printf("Error en el fork");
		exit(1);
	}


	 if ( (pid=fork()) == 0 )
	  { // hijo
	    close( pipe_padreAHijo[1] ); /* cerramos el lado de escritura del pipe */
	    close( pipe_hijoAPadre[0] ); /* cerramos el lado de lectura del pipe */
		    //Aca leo del padre
	    //while( (readbytes=read( a[0], buffer, SIZE ) ) > 0)
	    close( pipe_padreAHijo[0] );
		    //Aca escribo en el padre
	 //   write( pipe_hijoAPadre[1], buffer, strlen( buffer ) );
	    close( pipe_hijoAPadre[1] );
	  }
	  else
	  { // padre
	    close( pipe_padreAHijo[0] ); /* cerramos el lado de lectura del pipe */
	    close( pipe_hijoAPadre[1] ); /* cerramos el lado de escritura del pipe */
		    //Aca escribe en hijo
	    //write( pipe_padreAHijo[1], buffer, strlen( buffer ) );
	    close( pipe_padreAHijo[1]);
		    //Aca leo del hijo
	   // while( (readbytes=read( pipe_hijoAPadre[0], buffer, SIZE )) > 0)
	    //close( pipe_hijoAPadre[0]);
	  }
	  waitpid( pid, NULL, 0 );
}


