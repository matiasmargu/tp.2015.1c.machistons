/*
 * functionsMarta.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#include "funcionesMarta.h"


int recive_y_deserialisa(t_charpuntero* nombre, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	nombre->archivo = malloc(tamanioDinamico);
	memcpy(nombre->archivo, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}


int recive_y_deserialisa_job(t_job_marta* job_marta, int socket,uint32_t tamanioTotal ){

	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	memcpy(&job_marta->numeroBloque, buffer + offset, sizeof(int));
	offset += sizeof(int);

	memcpy(&job_marta->resultado, buffer + offset, sizeof(int));
	offset += sizeof(int);

	memcpy(&job_marta->rutina, buffer + offset, sizeof(int));
	offset += sizeof(int);

	free(buffer);
	return status;


}

// ACA NECESITO QUE EL FS MANDE EL NOMBRE, LA CANT DE BLOQUES, Y LA UBICACION DE LAS COPIAS
int recive_y_guarda_estructura(t_archivo arch, int socket, uint32_t tamanioTotal){

	char *buffer = malloc(tamanioTotal);
	int offset = 0;
	recv(socket, buffer, tamanioTotal, 0);
	//LO QUE RECIBO TIENE ESTA ESTRCTURA: [TAM_ESTRUC][TAM_NOM][NOMBRE][CANT_BLOQ]   [NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ][ID_NODO][NUMERO_BLOQ]

/*	// COPIO EL NOMBRE DEL ARCHIVO
	int tamanioNombre;
	offset += sizeof(int);
	memcpy(&tamanioNombre, buffer+offset, sizeof(int));
	offset += sizeof(int);
	memcpy(&arch.nombre, buffer+offset, tamanioNombre);
	offset += tamanioNombre;
*/
	// COPIO LA CANT_BLOQUES DEL ARCHIVO
	memcpy(&arch.cantidadDeBloques, buffer+offset, sizeof(int));
	offset += sizeof(int);

	// COPIO LOS BLOQUES QUE CONTIENE EL ARCHIVO A UN ESTRUCTURA DE LISTA
	arch.bloques = list_create();
	int i;
	for(i=0; i< arch.cantidadDeBloques; i++){

		t_bloque bloque;
		memcpy(&bloque.NumeroBloque, buffer+offset, sizeof(int));
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

char* serializar_charpuntero(t_charpuntero *nombre, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

			int tamanioNombre = strlen(nombre->archivo) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(nombre->archivo) + 1;
			memcpy(serializedPackage + offset, nombre->archivo, size_to_send);
			offset += size_to_send;

			return serializedPackage;
		}

//FUNCION QUE EJECUTA EL HILO QUE ATIENDE A CADA JOB
void  *conectarseAlJob(void*arg){

	int socket = (int)arg;
	int saludo = 9 ;
	int tamanioTotal;
	char* listaArchivosJob;
	char* combiner;
	int tamanioCombiner;

	//MANDAMOS HANDSHAKE A JOB
	send(socket,&saludo,sizeof(int),0);

	//ACA RECIBIMOS LA LISTA DE ARCHIVOS DE JOB COMO UN CHAR*
   	recv(socket, &tamanioTotal, sizeof(int),0);
   	int estado2 = 1; // Estructura que manjea el status de los recieve.
   	listaArchivosJob = malloc(tamanioTotal);
   	estado2 = recive_y_deserialisa(&listaArchivosJob, socket, tamanioTotal);
   	if(estado2){
                printf("el string es %s\n",listaArchivosJob);
   	}

   	//ACA RECIBIMOS LA INFORMACION SOBRE EL COMBINER(SI TIENE O NO)
   	recv(socket, &tamanioCombiner, sizeof(int),0);
  	int estadoCombiner = 1; // Estructura que manjea el status de los recieve.
   	combiner = malloc(tamanioCombiner);
   	estadoCombiner = recive_y_deserialisa(&combiner, socket, tamanioCombiner);
   	if(estadoCombiner){
   		printf("el combiner es %s\n",combiner);
   	}

   	//ACA SE CONECTA CON FS
   	char* puerto_fs, ip_fs ;
   	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";
   	t_config* archivoConfiguracion;
   	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
   	puerto_fs = config_get_string_value(archivoConfiguracion, "PUERTO_FS") ;
   	ip_fs = config_get_string_value(archivoConfiguracion, "IP_FS");
   	int socketFS,handshakeFS;
   	socketFS = crearCliente (ip_fs, puerto_fs);
   	handshakeFS = 25;
   	send(socketFS,&handshakeFS,sizeof(int),0);


   	char* archivoAEnviar;
   	t_charpuntero nombre;
   	t_archivo archivo;
   	char **archivos_separados = string_get_string_as_array(listaArchivosJob); //CONVERTIMOS EL STRING A UN ARRAY (Esto te devuelve algo asi: ["a", "b", "c"])

   	int s = 0;
   	//CONTAMOS LA CANTIDAD DE ARCHIVOS Y SE LA MANDAMOS A FS
   	int cantidad = 0;
   	while(archivos_separados[s] != NULL){
   		cantidad = cantidad + 1;	//cantidad = TAMANIO LISTA DE ARCHIVOS JOB
   		s = s+1;
   	}
   	printf("cantidad%i\n,",cantidad);

   	if(list_is_empty(lista_archivos)){
   		pthread_mutex_lock(&mutex);
   		lista_archivos =	list_create();
   		pthread_mutex_unlock(&mutex);
   	}
   	//MANDAMOS CADA ARCHIVO POR SEPARADO A FS(SOLO LOS QUE NO TENEMOS GUARDADOS) Y RECIBIMOS LA MATRIZ DE CADA ARCHIVO
   	int a;
   	for(a = 0 ; a < cantidad; a++){
   		nombre.archivo = archivos_separados[a];
   		if(!(list_any_satisfy(lista_archivos, (nombre.archivo == archivo.nombre)))){
   			// Le pide al FS que le envie los datos del archivo
   			tamanioTotal = sizeof(int)+ strlen(nombre.archivo)+1;
   			send(socketFS, &tamanioTotal, sizeof(int),0);
   			archivoAEnviar =  serializar_charpuntero( &nombre, tamanioTotal);
   			send(socketFS,archivoAEnviar,tamanioTotal,0);

   			//DATOS QUE ME ENVIA EL FS
   			t_archivo archivo;
   			recv(socketFS, &tamanioTotal, sizeof(int),0);
   			recive_y_guarda_estructura(archivo, socketFS, tamanioTotal);

   			pthread_mutex_lock(&mutex);
   			list_add(lista_archivos,&archivo);
   			pthread_mutex_unlock(&mutex);

   		}
   	}

   	//planificarMap();


   	close(socketFS);
   	return NULL;

}

/*
int calcularCantidadDeBloques(t_list* lista_archivos){
	int cantidad,j,h,k,p;
	t_archivo* archivo;
	t_list* listaDeBloques;
	listaDeBloques = list_create();
	for(j=0 ; j< list_size(lista_archivos);j++){
		archivo = list_get(lista_archivos,j);
		for(h=0;h < archivo->cantidadDeBloques;h++){
				t_bloque* bloque = list_get(archivo->bloques,h);

				for(k=0;k < 3; k++){
					for(p=0;p < list_size(listaDeBloques); p++){
						//ESTO ESTA MAL , HAY QUE VER COMO SE PUEDE ARREGLAR
						int elemento = list_get_element(listaDeBloques, p);
					if(list_any_satisfy(listaDeBloques,(elemento == bloque->NumeroBloque))){
				}
					else{list_add(listaDeBloques , bloque->NumeroBloque);} //ACA LO AGREGO P VECES (ESTO HAY QUE ARREGLARLO)
		}
					for(p=1;p < list_size(listaDeBloques); p++){
						list_remove(listaDeBloques , bloque->NumeroBloque);} //ACA LO SACO P VECES -1
					}
	}
	cantidad = list_size(listaDeBloques);
	return cantidad;
}
}
*/

void inicializarBitarray(t_bitarray *bitmap, int tamanio){
	int i;
	for(i=0;i < tamanio;i++){
		 bitarray_set_bit(bitmap, i);
	}
}

int buscarPorNodo(int idNodo, int nodos_activos[], int pos){
	return 1;
}

t_cargaBitarray_aux *armarVectorDeBitarray(int cantidadDeNodos, int nodos_activos[], int sub_indice){

	int f;
	//int cantidadDeBloquesTotales = calcularCantidadDeBloques(lista_archivos);
	t_cargaBitarray_aux vectorDeBitArrays[] = malloc(sizeof(t_cargaBitarray_aux));
	t_archivo *un_archivo;
	// OJO CON ESTO, NO ESTOY SEGURO SI NO HAY QUE PASARLO CARGADO CON 0 PREVIAMENTE
	char* un_nombre = malloc(cantidadDeNodos * sizeof(char));

	//seteo los bitarrays
	int j,h, k;
	sub_indice = 0;
	for(j=0 ; j< list_size(lista_archivos);j++){
		un_archivo = list_get(lista_archivos,j);
		for(h=0;h < un_archivo->cantidadDeBloques;h++){
			t_bloque* bloque = list_get(un_archivo->bloques,h);

			vectorDeBitArrays = realloc(vectorDeBitArrays,sizeof(t_cargaBitarray_aux)*(sub_indice+1));
			vectorDeBitArrays[sub_indice].nombre_arch = un_archivo->nombre;
			vectorDeBitArrays[sub_indice].bloque_arch = bloque->NumeroBloque;
			vectorDeBitArrays[sub_indice].bitmap = bitarray_create(&un_nombre, cantidadDeNodos);
			inicializarBitarray(vectorDeBitArrays[sub_indice].bitmap, cantidadDeNodos);

				for(k=0;k < 3; k++){
					int pos;
					if(buscarPorNodo(bloque->copias[k]->idNodo, nodos_activos, pos)) bitarray_set_bit(vectorDeBitArrays[sub_indice].bitmap, pos);
				}
			sub_indice ++;
		}
	}

	free(un_nombre);
	return vectorDeBitArrays; //LIBERAR ESTA MEMORIA DESP Y ELIMINAR CADA BITARRAY TMB

}



void planificarMap(){
	if(list_is_empty(lista_nodos_estado)){
		pthread_mutex_lock(&mutex_nodos);
		lista_nodos_estado = list_create();
		pthread_mutex_unlock(&mutex_nodos);

		// PARA PLANIFICAR NECESITO SABER LOS NODOS ACTIVOS. PARA ESO SE LO PIDO AL FS
		int cantidad_nodos_activos = 4;	// ESTO ME LO MANDA EL FS, JUNTO CON LOS NODOS_ACTIVOS
		int nodos_activos[cantidad_nodos_activos]; //LOS ID DE LOS NODOS ACTIVOS NECESITO QUE ME LOS MANDES ASI GASTON: [1,14,22,31] ORDENADOS DE MENOR A MAYOR

		int tamanio;
		t_cargaBitarray_aux *bitmap = armarVectorDeBitarray(cantidad_nodos_activos, nodos_activos, tamanio);
		int division = tamanio/cantidad_nodos_activos;
		int resto_division = tamanio%cantidad_nodos_activos;
		if(resto_division == 0) division--;

		int j;
		for(j=0;j <= division;j++){
			int k = 0;
			int bloques_alineados = division;
			if((resto_division != 0) && (j == division)) bloques_alineados = resto_division-1;
			while(k <= bloques_alineados){
				//ACA APLICO EL ALGORITMO DE PLANIFICACION PARA CADA BLOQUE
				k++;
			}
		}
	}

}

int recive_y_deserialisa_paquete_nodos(t_charpuntero* ip, t_charpuntero* puerto, int ipnodo, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);

	ip->archivo = malloc(tamanioDinamico);
	memcpy(ip->archivo, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	puerto->archivo = malloc(tamanioDinamico);
	memcpy(puerto->archivo, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}



char* serializar_estructura_t_marta_a_job(t_marta_a_job estructura_t_marta_a_job, int tamanioTotal){
			char *serializedPackage = malloc(tamanioTotal);

			int offset = 0;
			int size_to_send;

            // estructura_t_marta_a_job->ip
			int tamanioNombre = strlen(estructura_t_marta_a_job.ip) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(estructura_t_marta_a_job.ip) + 1;
			memcpy(serializedPackage + offset, estructura_t_marta_a_job.ip, size_to_send);
			offset += size_to_send;

			// estructura_t_marta_a_job->puerto_nodo
			tamanioNombre = strlen(estructura_t_marta_a_job.puerto_nodos) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(estructura_t_marta_a_job.puerto_nodos) + 1;
			memcpy(serializedPackage + offset, estructura_t_marta_a_job.puerto_nodos, size_to_send);
			offset += size_to_send;

			// el map o reduce
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &estructura_t_marta_a_job.map_o_reduce, size_to_send);
			offset += size_to_send;


		    // El vector_bloque_nodos
		    // El estructura_t_marta_a_job->vector_bloque_nodos->nombre_arch
			 tamanioNombre = strlen(estructura_t_marta_a_job.vector_bloque_nodos.nombre_arch) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(estructura_t_marta_a_job.vector_bloque_nodos.nombre_arch) + 1;
			memcpy(serializedPackage + offset, estructura_t_marta_a_job.vector_bloque_nodos.nombre_arch , size_to_send);
			offset += size_to_send;

			// El estructura_t_marta_a_job->vector_bloque_nodos->bloque_arch
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &estructura_t_marta_a_job.vector_bloque_nodos.bloque_arch, size_to_send);
			offset += size_to_send;


		    //El bitmap

			tamanioNombre = strlen(estructura_t_marta_a_job.vector_bloque_nodos.bitmap->bitarray) + 1;
			size_to_send = sizeof(int);
			memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
			offset += size_to_send;

			size_to_send =  strlen(estructura_t_marta_a_job.vector_bloque_nodos.bitmap->bitarray) + 1;
			memcpy(serializedPackage + offset, estructura_t_marta_a_job.vector_bloque_nodos.bitmap->bitarray , size_to_send);
			offset += size_to_send;


			//El size_t size del bitmap
			size_to_send = sizeof(int);
		    memcpy(serializedPackage + offset, &estructura_t_marta_a_job.vector_bloque_nodos.bitmap->size, size_to_send);
			offset += size_to_send;

			return serializedPackage;
}


