
#include "funciones.h"



char* serializar_marta_job_map(t_marta_job_map *bloque, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

			size_to_send =  sizeof(bloque->cantidadBloques);
			memcpy(serializedPackage + offset, &(bloque->cantidadBloques), size_to_send);
			offset += size_to_send;

			int tamanioNombre = strlen(bloque->ip_nodo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->ip_nodo) + 1;
			memcpy(serializedPackage + offset, bloque->ip_nodo, size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->puerto) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->puerto) + 1;
			memcpy(serializedPackage + offset, bloque->puerto, size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->nombre_archivo_resultado) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->nombre_archivo_resultado) + 1;
			memcpy(serializedPackage + offset, bloque->nombre_archivo_resultado, size_to_send);
			offset += size_to_send;

			size_to_send =  sizeof(bloque->idNodo);
			memcpy(serializedPackage + offset, &(bloque->idNodo), size_to_send);
			offset += size_to_send;



			//LISTA
			int tamanioLista = list_size(bloque->bloques);
			int a, numero;


			for(a=0;a< tamanioLista; a++){
			numero = list_get(bloque->bloques, a);
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &numero, size_to_send);
			offset += size_to_send;
					}




			return serializedPackage;
		}

char* serializar_marta_job_reduce(t_marta_job_reduce *bloque, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

			size_to_send =  sizeof(bloque->cantidadArchivos);
			memcpy(serializedPackage + offset, &(bloque->cantidadArchivos), size_to_send);
			offset += size_to_send;

			int tamanioNombre = strlen(bloque->ipNodo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->ipNodo) + 1;
			memcpy(serializedPackage + offset, bloque->ipNodo, size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->puertoNodo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->puertoNodo) + 1;
			memcpy(serializedPackage + offset, bloque->puertoNodo, size_to_send);
			offset += size_to_send;


			size_to_send =  sizeof(bloque->idNodo);
			memcpy(serializedPackage + offset, &(bloque->idNodo), size_to_send);
			offset += size_to_send;

			tamanioNombre = strlen(bloque->archivoResultadoReduce) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(bloque->archivoResultadoReduce) + 1;
			memcpy(serializedPackage + offset, bloque->archivoResultadoReduce, size_to_send);
			offset += size_to_send;



			//LISTA
			int tamanioLista = list_size(bloque->listaArchivosTemporales);
			int a;
			char* archivo;

			for(a=0;a< tamanioLista; a++){
			archivo = list_get(bloque->listaArchivosTemporales, a);
			tamanioNombre = strlen(archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(archivo) + 1;
			memcpy(serializedPackage + offset, archivo, size_to_send);
			offset += size_to_send;
					}




			return serializedPackage;
		}

int cantidadArchivosEnArray(char** strings) {
	int cantidadArchivosEnArray = 0;
	while (*strings != NULL) {
		cantidadArchivosEnArray += 1;
		strings++;
	}
	return cantidadArchivosEnArray;
}


char* guardarPrimeroDelArray(char** strings) {
	char* archivo;
	if (*strings != NULL) {
		archivo = *strings;

	}

	return archivo;
}

char** eliminarPrimeroDelArray(char** strings){
	char** array;
	if (*strings != NULL) {
	strings++;

	}
	array = strings;
	return array;

}


bool estaEnLaLista(t_list* lista, char* archivo){
	int a,b;
	t_archivo_job* archivo2;
	b = 0;
	for(a=0; a < list_size(lista);a++){
		archivo2 = list_get(lista,a);
		if(archivo2->nombre == archivo){ b = 1;}
	}
	if(b == 1){return true;}else{return false;}
}

// ACA NECESITO QUE EL FS MANDE EL NOMBRE, LA CANT DE BLOQUES, Y LA UBICACIOsN DE LAS COPIAS
int recive_y_guarda_estructura(t_archivo arch, int socket, uint32_t tamanioTotal){

	char *buffer = malloc(tamanioTotal);
	int offset = 0;
	recv(socket, buffer, tamanioTotal, 0);
	//LO QUE RECIBO TIENE ESTA ESTRCTURA: [TAM_ESTRUC][TAM_NOM][NOMBRE][CANT_BLOQ]   [NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ]



	// COPIO LA CANT_BLOQUES DEL ARCHIVO
	memcpy(&(arch.cantidadDeBloques), buffer+offset, sizeof(int));
	offset += sizeof(int);

	// COPIO LOS BLOQUES QUE CONTIENE EL ARCHIVO A UN ESTRUCTURA DE LISTA
	arch.bloques = list_create();
	int i;
	for(i=0; i< arch.cantidadDeBloques; i++){

		t_bloque bloque;
		memcpy(&(bloque.NumeroBloque), buffer+offset, sizeof(int));
		offset += sizeof(int);

		int j;
		t_copia copias[3];
		for(j=0; j<3; j++){

			memcpy(&(copias[j].idNodo), buffer+offset, sizeof(int));
			offset += sizeof(int);
			memcpy(&(copias[j].Numerobloque), buffer+offset, sizeof(int));
			offset += sizeof(int);
		}

		bloque.copias = copias;
		list_add_in_index(arch.bloques, bloque.NumeroBloque, &bloque);
	}

	return 0;

}
void * recibirListaDeArchivos(void*arg){

	int socketJob = (int)arg;
	int tamanioTotal,i,tamanioCombiner,socketFS,handshakeFS,a,tamanioAEnviar,numero,estado,  tamanioArchivoResultado;
	int idJob =0;
	char* listaArchivosJob;
	t_archivo unArchivo;
	char** arrayDeArchivos;
	char* archivo;
	char** arrayModificado;
	char* combiner;
	char* puerto_fs;
	char* archivoAenviar;
	char* ip_fs ;
	char* rutaArchivoConfiguracion;
	char* nombreArchivoResultadoFinal;
	t_config* archivoConfiguracion;
	char* archivoAEnviar;
	char* structASerializar;
	t_archivo_job* archivoEnPosicionA;
	t_job job;
	int enteroPrueba;
	enteroPrueba = 60;
	send(socketJob, &enteroPrueba, sizeof(int),0);
	//ACA RECIBIMOS LA INFORMACION SOBRE EL COMBINER(SI TIENE O NO)
	recv(socketJob, &tamanioCombiner, sizeof(int),0);
	int estadoCombiner = 1; // Estructura que manjea el status de los recieve.
	combiner = malloc(tamanioCombiner);
	send(socketJob, &enteroPrueba, sizeof(int),0);
	recv(socketJob, combiner, tamanioCombiner,0);

		if(string_equals_ignore_case(combiner ,"SI")){
			job.combiner = 1;
		}else{ job.combiner = 0;}
			send(socketJob, &enteroPrueba, sizeof(int),0);
	//ACA RECIBIMOS LA LISTA DE ARCHIVOS DE JOB COMO UN CHAR*
	 		recv(socketJob, &tamanioTotal, sizeof(int),0);
	 		int estado2 = 1; // Estructura que manjea el status de los recieve.
	 		listaArchivosJob = malloc(tamanioTotal);
	 		send(socketJob, &enteroPrueba, sizeof(int),0);
	 		recv(socketJob, listaArchivosJob, tamanioTotal,0);
	 			char* archivos = listaArchivosJob;
	 			arrayDeArchivos = string_get_string_as_array(archivos);
	 			job.cantidadDeArchivos = cantidadArchivosEnArray(arrayDeArchivos);
	 			printf("%i\n",job.cantidadDeArchivos);
	 			job.idJob = idJob;
	 			idJob ++;

	 //ACA RECIBIMOS EL ARCHIVO DONDE VA EL RESULTADO FINAL DEL JOB
	 			send(socketJob, &enteroPrueba, sizeof(int),0);
	 			recv(socketJob, &tamanioArchivoResultado, sizeof(int),0);
	 			send(socketJob, &enteroPrueba, sizeof(int),0);
	 			nombreArchivoResultadoFinal = malloc(tamanioArchivoResultado);
	 			recv(socketJob, nombreArchivoResultadoFinal, tamanioArchivoResultado,0);

	 			printf("el archivo final es %s\n",nombreArchivoResultadoFinal);





//ACA ARRANCA CONEXION CON FS
	   			rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
	   			archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	   		   	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
	   		   	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");

	   		   //	socketFS = crearCliente (ip_fs, puerto_fs);

	   		   	handshakeFS = 25;
	   		 //  	send(socketFS,&handshakeFS,sizeof(int),0);

	   		// recv(socketFS, &enteroPrueba, sizeof(int),0);
	   	//MANDAMOS LOS NOMBRES DE LOS ARCHIVOS Y RECIBIMOS LOS DATOS

	   		arrayModificado = arrayDeArchivos;

	   		    	for(a = 0 ; a < job.cantidadDeArchivos; a++){
	   		    		 nombre =  guardarPrimeroDelArray(arrayModificado);
	   		    		arrayModificado =  eliminarPrimeroDelArray(arrayModificado);
	   		    		if(!(estaEnLaLista(listaArchivos, nombre))){
	   		    			// Le pide al FS que le envie los datos del archivo
	   		    			tamanioTotal =  strlen(nombre)+1;
	   		    			printf("AASASASASASAS %s\n",nombre);
	   		    			send(socketFS, &tamanioTotal, sizeof(int),0);
	   		    			recv(socketFS, &enteroPrueba, sizeof(int),0);
	   		    			send(socketFS,nombre,tamanioTotal,0);
	   		    			//GUARDO NOMBRE Y JOB EN LA LISTA DE ARCHIVOS GLOBAL
	   		    			unArchivo.nombre = nombre;
	   		    			unArchivo.idJob = job.idJob;

	   		    			//DATOS QUE ME ENVIA EL FS
	   		    			recv(socketFS, &tamanioTotal, sizeof(int),0);
	   		    			estado = recive_y_guarda_estructura(unArchivo, socketFS, tamanioTotal);
	   		    			if(estado){
	   		    			pthread_mutex_lock(&mutex);
	   		    			list_add(listaArchivos,&unArchivo);
	   		    			pthread_mutex_unlock(&mutex);
	   		    		}
	   		    		}

	   		    	}





	   		   	//PROBANDO MANDAR A MAPEAR AL JOB
	   		  	int pruebaaa;
	   		   	pruebaaa = 30;
	   		 send(socketJob,&pruebaaa,sizeof(int),0);
	   		recv(socketJob, &enteroPrueba, sizeof(int),0);
	   		t_marta_job_map structPrueba;
	   		structPrueba.ip_nodo = "192.168.3.80";
	   		structPrueba.puerto = "6000";
	   		structPrueba.idNodo = 3;
	   		structPrueba.cantidadBloques = 5;
	   		structPrueba.nombre_archivo_resultado = "pepitoloco";
	   		structPrueba.bloques = list_create();
	   		list_add(structPrueba.bloques,4);
	   		list_add(structPrueba.bloques,3);
	   		list_add(structPrueba.bloques,45);
	   		list_add(structPrueba.bloques,98);
	   		list_add(structPrueba.bloques,7);
	   		int tamanioStructPrueba = (sizeof(int)*10)+ strlen(structPrueba.ip_nodo) +1 +strlen(structPrueba.puerto) +1 + strlen(structPrueba.nombre_archivo_resultado) +1;
	   		send(socketJob,&tamanioStructPrueba,sizeof(int),0);
	   		recv(socketJob, &enteroPrueba, sizeof(int),0);
	   		char* structAEnviar =  serializar_marta_job_map(&structPrueba, tamanioStructPrueba);
			send(socketJob,structAEnviar,tamanioStructPrueba,0);
/*
		 	//PROBANDO MANDAR A MAPEAR AL JOB
   		   	pruebaaa = 30;
   		 send(socketJob,&pruebaaa,sizeof(int),0);
   		recv(socketJob, &enteroPrueba, sizeof(int),0);
   		t_marta_job_map structPruebaa;
   		structPruebaa.ip_nodo = "192.168.3.80";
   		structPruebaa.puerto = "6000";
   		structPruebaa.idNodo = 3;
   		structPruebaa.cantidadBloques = 2;
   		structPruebaa.nombre_archivo_resultado = "peco";
   		structPruebaa.bloques = list_create();
   		list_add(structPruebaa.bloques,4);
   		list_add(structPruebaa.bloques,1);
   		list_add(structPruebaa.bloques,5);
   		int tamanioStructPruebaa = (sizeof(int)*8)+ strlen(structPruebaa.ip_nodo) +1 +strlen(structPruebaa.puerto) +1 + strlen(structPruebaa.nombre_archivo_resultado) +1;
   		send(socketJob,&tamanioStructPruebaa,sizeof(int),0);
   		recv(socketJob, &enteroPrueba, sizeof(int),0);
   		char* structAEnviaar =  serializar_marta_job_map(&structPruebaa, tamanioStructPruebaa);
		send(socketJob,structAEnviaar,tamanioStructPruebaa,0);


				//PROBANDO MANDAR A  REDUCIR AL JOB (NO BORRAR)
	   		 int c, tamanioLista;
	   		 char* archivoo;
	   		 int prueba5 = 34;
	   		 send(socketJob,&prueba5,sizeof(int),0);
	   		 recv(socketJob, &enteroPrueba, sizeof(int),0);
	   		 t_marta_job_reduce structPrueba2;
	   		 structPrueba2.ipNodo = "192.168.0.112";
 	   		 structPrueba2.puertoNodo = "5000";
 	   		 structPrueba2.idNodo = 88;
 	   		 structPrueba2.cantidadArchivos = 5;
	   		 structPrueba2.archivoResultadoReduce = "reducimeesta";
	   		 structPrueba2.listaArchivosTemporales = list_create();
	   		 list_add(structPrueba2.listaArchivosTemporales,"hola");
	   		 list_add(structPrueba2.listaArchivosTemporales,"rodolfo");
	   		 list_add(structPrueba2.listaArchivosTemporales,"riber");
	    	 list_add(structPrueba2.listaArchivosTemporales,"sos");
   	   		 list_add(structPrueba2.listaArchivosTemporales,"cagon");
   	   		 for(c = 0; c < structPrueba2.cantidadArchivos ; c++){
   	   			 archivoo = list_get(structPrueba2.listaArchivosTemporales, c);
   	   			 tamanioLista += strlen(archivoo);
   	   			 tamanioLista += 1;

   	   		 }
  	   		 int tamanioStructPrueba2 = (sizeof(int)*10)+ strlen(structPrueba2.ipNodo) +1 +strlen(structPrueba2.puertoNodo) +1 + strlen(structPrueba2.archivoResultadoReduce) +1 + tamanioLista;
	   		 send(socketJob,&tamanioStructPrueba2,sizeof(int),0);
	   		 recv(socketJob, &enteroPrueba, sizeof(int),0);
	   		char* structAEnviar2 =  serializar_marta_job_reduce(&structPrueba2, tamanioStructPrueba2);
	   		send(socketJob,structAEnviar2,tamanioStructPrueba2,0);

*/

	   	}
