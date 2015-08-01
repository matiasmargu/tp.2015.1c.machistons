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

void aparear(char* file1,char* file2,char* file3){
	FILE* arch1=fopen(file1,"r");
	FILE* arch2=fopen(file2,"r");
	FILE* resul=fopen(file3,"w");

	char* str1=malloc(1024);
	char* str2=malloc(1024);

	fgets(str1,1024,arch1);
	fgets(str2,1024,arch2);

	//printf("hola\n");
	while((!feof(arch1))&&(!feof(arch2))){
		if((strcmp(str1,str2)<0)){
			fputs(str1,resul);
			fgets(str1,1024,arch1);
		}else{
			fputs(str2,resul);
			fgets(str2,1024,arch2);
		}
	}

	if((feof(arch1)) && (feof(arch2))){
		fclose(arch1);
		fclose(arch2);
		fclose(resul);
		return;
	}else{
		if(feof(arch1)){
			fputs(str2,resul);
			while(!feof(arch2)){
				fgets(str2,1024,arch2);
				fputs(str2,resul);
			}
		}else{
			fputs(str1,resul);
			while(!feof(arch1)){
				fgets(str1,1024,arch1);
				fputs(str1,resul);
			}
		}

		fclose(arch1);
		fclose(arch2);
		fclose(resul);
	}
	return;
}
char* aparearYelim(char*lista[1000],int cantArch,char* nombre){

	int a=0;
	char* primero;
	char *segundo;
	char* aux;
	char* resultado;

	cantArch--;

	asprintf(&resultado,"%s%s","/tmp/",nombre);
	printf("Este es el nombre: %s\n",nombre);
	printf("Esta es la cantidad: %i\n",cantArch);

	for(a=0;a<cantArch;a++){

		asprintf(&aux,"%s%s","/tmp/Red",string_itoa(a));

		if(a==0){
			asprintf(&primero,"%s%s","/tmp/",lista[a]);
			asprintf(&segundo,"%s%s","/tmp/",lista[a+1]);
		}else{
			asprintf(&primero,"%s%s","/tmp/Red",string_itoa(a-1));
			asprintf(&segundo,"%s%s","/tmp/",lista[a+1]);
		}

		aparear(primero,segundo,aux);
	}

	char *buf=mapearAMemoriaVirtual(aux);

	FILE* fp = fopen(resultado,"w");
	fputs(buf,fp);
	fclose(fp);

	FILE* hola = fopen("/tmp/gil.txt","w");
	fputs(buf,fp);
	fclose(fp);

	cantArch++;
	for(a=0;a<cantArch;a++){
		asprintf(&primero,"%s%s","/tmp/",lista[a]);
		asprintf(&aux,"%s%s","/tmp/Red",string_itoa(a));
		remove(primero);
		remove(aux);
	}

	return resultado;

}




