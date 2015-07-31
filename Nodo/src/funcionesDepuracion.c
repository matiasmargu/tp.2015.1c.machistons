/*
 * funcionesDepuracion.c
 *
 *  Created on: 17/7/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"
#define SIZE 20*1024*1024

//Elimina los enters de un buffer
void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
    	if(*s != '\t' && *s != '\n') {
    		*p2++ = *s++;
    	} else {
    		++s;
    	}
    }
    *p2 = '\0';
}


int contarENT(char*buffer,int tamanio){
	int a,bre=1,cont=0;
	a=0;
	while((a<tamanio)&&(bre!=0)){
		if((buffer[a]=='\n')&&(bre!=0)){
			cont++;
		}
		if(a==tamanio){
			bre =1;
		}
		a++;
	}
	return cont;
}



void ordernarAlfabeticamente(char* nombreDelArchivoResultado,char* resultado_aux){

	pid_t pid;
	int status;

	char* bufferAUX=malloc(SIZE);
	char* test;


	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);

	asprintf(&test,"%s%s","sort ",resultado_aux);
	printf("%s\n",test);

	if ( (pid=fork()) == 0 )
		  { // hijo

			//dup2(pipe_padreAHijo[0],STDIN_FILENO);
			dup2(pipe_hijoAPadre[1],STDOUT_FILENO);

			close( pipe_padreAHijo[1] ); /* cerramos el lado de escritura del pipe */
			close( pipe_hijoAPadre[0] ); /* cerramos el lado de lectura del pipe */
			close( pipe_hijoAPadre[1]);
			close( pipe_padreAHijo[0]);

			system(test);
			exit(1);
			//execv(test,NULL);
		  }
		  else
		  { // padre
		    close( pipe_padreAHijo[0] ); /* cerramos el lado de lectura del pipe */
		    close( pipe_hijoAPadre[1] ); /* cerramos el lado de escritura del pipe */

		    //Aca escribe en hijo
		   // write( pipe_padreAHijo[1],mapRes,strlen(mapRes));
		    //write( pipe_padreAHijo[1], "hola faknflanflfan", strlen("hola faknflanflfan") );
		    close( pipe_padreAHijo[1]);

		    waitpid(pid,&status,0);

		    //Aca leo del hijo
		    read( pipe_hijoAPadre[0], bufferAUX, SIZE );
		    close( pipe_hijoAPadre[0]);

		  }

		printf("Este es el resultado del sort: %s\n",bufferAUX);

		FILE* fdCompletado = fopen(nombreDelArchivoResultado,"w");
		fputs(bufferAUX,fdCompletado);
		fclose(fdCompletado);

		remove(resultado_aux);
		free(bufferAUX);
		return;
}
void ordenar(char* buffer,char* nombre){

	pid_t pid;
	int status;

	char* bufferAUX=malloc(SIZE);
	char* test;

	int pipe_padreAHijo[2];
	int pipe_hijoAPadre[2];

	pipe(pipe_padreAHijo);
	pipe(pipe_hijoAPadre);

	FILE* ff=fopen(nombre,"w");
	fputs(buffer,ff);
	fclose(ff);

	asprintf(&test,"%s%s","sort ",nombre);
	printf("%s\n",test);

	if ( (pid=fork()) == 0 )
		  { // hijo

			//dup2(pipe_padreAHijo[0],STDIN_FILENO);
			dup2(pipe_hijoAPadre[1],STDOUT_FILENO);

			close( pipe_padreAHijo[1] ); /* cerramos el lado de escritura del pipe */
			close( pipe_hijoAPadre[0] ); /* cerramos el lado de lectura del pipe */
			close( pipe_hijoAPadre[1]);
			close( pipe_padreAHijo[0]);

			system(test);
			exit(1);
			//execv(test,NULL);
		  }
		  else
		  { // padre
		    close( pipe_padreAHijo[0] ); /* cerramos el lado de lectura del pipe */
		    close( pipe_hijoAPadre[1] ); /* cerramos el lado de escritura del pipe */

		    //Aca escribe en hijo
		   // write( pipe_padreAHijo[1],mapRes,strlen(mapRes));
		    //write( pipe_padreAHijo[1], "hola faknflanflfan", strlen("hola faknflanflfan") );
		    close( pipe_padreAHijo[1]);

		    waitpid(pid,&status,0);

		    //Aca leo del hijo
		    read( pipe_hijoAPadre[0], bufferAUX, SIZE );
		    close( pipe_hijoAPadre[0]);

		  }

	ff=fopen(nombre,"w");
	fputs(bufferAUX,ff);
	fclose(ff);

}



