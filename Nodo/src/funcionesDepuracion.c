/*
 * funcionesDepuracion.c
 *
 *  Created on: 17/7/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"


//Elimina los enters de un buffer
void eliminarEnters(char* buffer, int tamanio){
	int a;

	for(a=0;a<tamanio;a++){
		char exami= buffer[a];

		if(exami == '\n'){
			buffer[a]='\0';
		}

	}

}

void ordernarAlfabeticamente(char* nombreDelArchivoResultado,FILE *fdMape,int tamanioArchivo){
	pid_t pid;

	char* buffer=malloc(tamanioArchivo);


	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);



	if((pid = fork()) == -1){
		printf("Error en el fork");
		exit(1);
	}


	 if ( (pid=fork()) == 0 )
	  { // hijo

		 close( pipe_padreAHijo[1] ); /* cerramos el lado de escritura del pipe */
		 close( pipe_hijoAPadre[0] ); /* cerramos el lado de lectura del pipe */

		//dup2(pipe_padreAHijo[0],STDIN_FILENO);
		dup2(pipe_hijoAPadre[1],STDOUT_FILENO);

		execv("sort",fdMape);
	  }
	  else
	  { // padre
	    close( pipe_padreAHijo[0] ); // cerramos el lado de lectura del pipe
	    close( pipe_hijoAPadre[1] ); //cerramos el lado de escritura del pipe
	    //Aca escribe en hijo

		//Aca leo del hijo
		read( pipe_hijoAPadre[0], buffer, tamanioArchivo );

		FILE* fdCompletado = fopen(nombreDelArchivoResultado,"w");
		fputs(buffer,tamanioArchivo);
		fclose(buffer);

		close( pipe_hijoAPadre[0]);
		  }
	  waitpid( pid, NULL, 0 );
}
