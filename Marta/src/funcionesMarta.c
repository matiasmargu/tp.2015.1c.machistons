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


char* serializar_aplicarReduce(t_mandarAlHilo *estructura, int tamanioTotal){

	char *serializedPackage = malloc(tamanioTotal);
		int offset = 0;
		int size_to_send;

		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &estructura->cantidadArchivosTemporales, size_to_send);
		offset += size_to_send;

		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &estructura->idNodo, size_to_send);
		offset += size_to_send;

		int tamanioArchivoReduce = strlen(estructura->archivoResultadoReduce) + 1;
				size_to_send = sizeof(int);
				memcpy(serializedPackage + offset, &tamanioArchivoReduce, size_to_send);
				offset += size_to_send;

				size_to_send =  strlen(estructura->archivoResultadoReduce) + 1;
				memcpy(serializedPackage + offset, estructura->archivoResultadoReduce, size_to_send);
				offset += size_to_send;

				int tamanioLista = list_size(estructura->vectorArchivosTemporales);
				int a, numero;

		for(a=0;a< tamanioLista; a++){
			numero = list_get(estructura->vectorArchivosTemporales, a);
			size_to_send = sizeof(int);
					memcpy(serializedPackage + offset, &numero, size_to_send);
					offset += size_to_send;
		}

				return serializedPackage;



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

// ACA NECESITO QUE EL FS MANDE EL NOMBRE, LA CANT DE BLOQUES, Y LA UBICACIOsN DE LAS COPIAS
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
void *conectarseAlJob(void*arg){

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
   		char* presenciaCombiner = combiner;
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
   			tamanioTotal = sizeof(int)+ string_length(nombre.archivo)+1;
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
		 bitarray_clean_bit(bitmap, i);
	}
}

int buscarPorNodo(int idNodo, int nodos_activos[], int pos){
	return 1;
}

t_cargaBitarray_aux *armarVectorDeBitarray(t_cargaBitarray_aux *vectorDeBitArrays, int cantidadDeNodos, int nodos_activos[], int *sub_indice){

	//int cantidadDeBloquesTotales = calcularCantidadDeBloques(lista_archivos);
	t_archivo *un_archivo;
	// OJO CON ESTO, NO ESTOY SEGURO SI NO HAY QUE PASARLO CARGADO CON 0 PREVIAMENTE
	char *un_nombre = malloc(cantidadDeNodos * sizeof(char));

	//seteo los bitarrays
	int j,h, k;
	(*sub_indice) = 0;
	for(j=0 ; j< list_size(lista_archivos);j++){
		un_archivo = list_get(lista_archivos,j);
		for(h=0;h < un_archivo->cantidadDeBloques;h++){
			t_bloque* bloque = list_get(un_archivo->bloques,h);

			vectorDeBitArrays = realloc(vectorDeBitArrays,sizeof(t_cargaBitarray_aux)*((*sub_indice)+1));
			vectorDeBitArrays[(*sub_indice)].nombre_arch = un_archivo->nombre;
			vectorDeBitArrays[(*sub_indice)].bloque_arch = bloque->NumeroBloque;
			vectorDeBitArrays[(*sub_indice)].bitmap = bitarray_create(un_nombre, cantidadDeNodos);
			inicializarBitarray(vectorDeBitArrays[(*sub_indice)].bitmap, cantidadDeNodos);

				for(k=0;k < 3; k++){
					int pos;
					if(buscarPorNodo(bloque->copias[k]->idNodo, nodos_activos, pos)) bitarray_set_bit(vectorDeBitArrays[(*sub_indice)].bitmap, pos);
				}
			(*sub_indice) ++;
		}
	}

	free(un_nombre);
	return vectorDeBitArrays; //LIBERAR ESTA MEMORIA DESP Y ELIMINAR CADA BITARRAY TMB

}


int buscarVictimasPorBloque(t_cargaBitarray_aux bitmap[], int tamanio, t_cargaBitarray_aux *vectorVictimas){
	int min = bitarray_get_max_bit(bitmap[0].bitmap);
	int i;
	bool flag = false;
	// busco el menor
	for(i=1;i<=tamanio;i++){
		if((bitarray_get_max_bit(bitmap[i].bitmap)) <= min){
			bitarray_set_bit(vectorVictimas->bitmap,i);
			min = bitarray_get_max_bit(bitmap[i].bitmap);
			flag = true;
		}
	}
	// si flag es false, el primero es el elegido
	if(flag == false) bitarray_set_bit(vectorVictimas->bitmap,0);
	// 0 = no pudo encontrar
	// 1 = encontro 1 victima
	// 2 = encontro 2 o mas victimas
	if(bitarray_get_max_bit(vectorVictimas->bitmap) == 0) return 0;
	else{
		if(bitarray_get_max_bit(vectorVictimas->bitmap) == 1) return 1; else return 2;
	}

}

buscarVictimasPorContadores(t_cargaBitarray_aux bitmap[], int tamanio, t_cargaBitarray_aux *vectorVictimas, int *vector_contador){

	int i, victim;
	bool flag = false;
	for(i=0;i<tamanio;i++){
		if(bitarray_test_bit(vectorVictimas->bitmap,i) == 1 && flag == false){
			victim = i;
			flag = true;
		}
		else if(bitarray_test_bit(vectorVictimas->bitmap,i) == 1 && flag == true){
			if(vector_contador[i] < vector_contador[victim]){
				victim = i;
			}
		}
	}
	//limpio el vector victimas, dejando solo a la victima elegida
	for(i=0;i<tamanio;i++) if(i == victim) bitarray_set_bit(vectorVictimas->bitmap,i);
}


algoritmoMap(t_cargaBitarray_aux *bitmapAuxiliar, int *vector_contador, t_cargaBitarray_aux *vectorVictimas, int cant){



	int r = buscarVictimasPorBloque(bitmapAuxiliar, cant, vectorVictimas);
	if(r == 2){
		r = buscarVictimasPorNodo();
		if(r == 2){
			r = buscarVictimasPorContadores(bitmapAuxiliar, cant, vectorVictimas, vector_contador);
			if (r == false){
				;
			}
		}
	}
	// busco el bloque que se eligio y lo devuelvo
	for(r=0;r<cant;r++){
		if(bitarray_test_bit(vectorVictimas->bitmap,r) == 1) ;
	}

}





void planificarMap(){
	// PARA PLANIFICAR NECESITO SABER LOS NODOS ACTIVOS. PARA ESO SE LO PIDO AL FS
	int cantidad_nodos_activos = 4;	// ESTO ME LO MANDA EL FS, JUNTO CON LOS NODOS_ACTIVOS
	int nodos_activos[cantidad_nodos_activos]; //LOS ID DE LOS NODOS ACTIVOS NECESITO QUE ME LOS MANDES ASI GASTON: [1,14,22,31] ORDENADOS DE MENOR A MAYOR
	t_cargaBitarray_aux bitmapAuxiliar[cantidad_nodos_activos];

	if(list_is_empty(lista_nodos_estado)){
		pthread_mutex_lock(&mutex_nodos);
		lista_nodos_estado = list_create();
		pthread_mutex_unlock(&mutex_nodos);


		int *tamanio = malloc(sizeof(int));
		t_cargaBitarray_aux *bitmap = malloc(sizeof(t_cargaBitarray_aux));
		armarVectorDeBitarray(bitmap, cantidad_nodos_activos, nodos_activos, tamanio);
		int division = (*tamanio)/cantidad_nodos_activos;
		int resto_division = (*tamanio)%cantidad_nodos_activos;
		if(resto_division == 0) division--;
		// vector_contador = vector de contadores para planificar (esto no cambia hasta que termina el planificarMap)
		int *vector_contador = malloc(sizeof(int)*cantidad_nodos_activos);
		int j;
		// inicializo vector_contador
		for(j=0;j<cantidad_nodos_activos;j++) vector_contador[j] = 0;

		for(j=0;j <= division;j++){
			int k = 0;
			int bloques_alineados = division;
			if((resto_division != 0) && (j == division)) bloques_alineados = resto_division-1;

			while(k <= bloques_alineados){
				// tengo que cargar el auxiliar con los bloques que va a planificar el algoritmo de map
				// ej: si es la primera pasada, auxiliar va a tener la cantidad de bloques
				// si es la 2da pasada, auxiliar va a tener la cantidad de bloques menos el que ya se eligio antes
				cargarBitmapAuxiliar(bitmapAuxiliar, bitmap, bloques_alineados);

				char *vectorVictimas_str = malloc(sizeof(char) * cantidad_nodos_activos);
				t_cargaBitarray_aux *vectorVictimas = malloc(sizeof(t_cargaBitarray_aux));
				vectorVictimas->bitmap = bitarray_create(vectorVictimas_str, cantidad_nodos_activos);
				inicializarBitarray(vectorVictimas->bitmap, cantidad_nodos_activos);

				algoritmoMap(bitmapAuxiliar, vector_contador, vectorVictimas, bloques_alineados);
				//aca hay que recorrer el vector victimas y buscar el que tiene el bit en 1.
				// cuando lo encontramos vamos a buscar el bloque qe se asigno y lo eliminamos para la proxima pasada
				vector_contador[victim_pos] ++;
				if (j == 0){
					//si j==0 quiere decir que la alineacion de vectorVictimas es pura
					//si j > 0 quiere decir que la alineacion de vectorVictimas es k*cantidad_nodos_activos
					// alineacion = posicion del bloque dentro del bitmap grande
				}
				k++;
			}
		}
		free(vector_contador);
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


/*
char* serializar_estructura_t_marta_a_job(t_marta_job estructura_t_marta_a_job, int tamanioTotal){
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
*/


//ESTO SE USA EN EL REDUCE , NO BORRAR

/*
struct linkelement{
		t_nodos* data;
		struct linkelement *next;
	};
typedef struct linkelement tlinkelement;
typedef struct {
		tlinkelement *head;
		int elements_count;
	} t_lista;
	static tlinkelement* list_create_element(t_nodos* data) {
		tlinkelement* element = malloc(sizeof(tlinkelement));
		element->data = data;
		element->next = NULL;
		return element;
	}
	static tlinkelement* list_get_element(t_lista* self, int index) {
		int cont = 0;
		if ((self->elements_count > index) && (index >= 0)) {
			tlinkelement *element = self->head;
			while (cont < index) {
				element = element->next;
				cont++;
			}
			return element;
		}
		return NULL;
	}
	static void list_link_element(tlinkelement* previous, tlinkelement* next) {
		if (previous != NULL) {
			previous->next = next;
		}
	}
	int lista_add(t_lista *self, t_nodos *data) {
		tlinkelement *new_element = list_create_element(data);
		if (self->elements_count == 0) {
			self->head = new_element;
		} else {
			list_link_element(list_get_element(self, self->elements_count - 1), new_element);
		}
		self->elements_count++;
		return self->elements_count - 1;
	}
*/

//HASTA ACA ES LO DEL REDUCE QUE NO HAY QE BORRAR

serializar_nodo_a_mapear(t_nodos nodo_a_mapear,int tamanioTotal){

	    char *serializedPackage = malloc(tamanioTotal);

		int offset = 0;
		int size_to_send;

		size_to_send =  sizeof(nodo_a_mapear);
		memcpy(serializedPackage + offset, &(bloque->bloque), size_to_send);
		offset += size_to_send;

		int tamanioNombre = strlen(bloque->data) + 1;
		size_to_send = sizeof(int);
		memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
		offset += size_to_send;

		size_to_send =  strlen(bloque->data) + 1;
		memcpy(serializedPackage + offset, bloque->data, size_to_send);
		offset += size_to_send;

		return serializedPackage;
}
		}
}



void planificarReduce(char* nombreArchivo, int cantidadPosicionesVectorNodo, t_nodoPorArchivo nodoPorArchivo, int socketjob){
	int i;
	char* resultado;
	for(i=0; i< cantidadPosicionesVectorNodo; i++){
	pthread_t hilo_reduce;
	t_aplicarReduce structAEnviar;
	structAEnviar.idNodo = nodoPorArchivo.idNodo;
	structAEnviar.socketJob = socketjob;
	structAEnviar.vectorArchivosTemporales = nodoPorArchivo.vectorArchivosTemporales;
	structAEnviar.cantidadArchivosTemporales = nodoPorArchivo.cantidadArchivosTemporales;
	asprintf(&resultado,"%s%i","ArchivoTemporalNumero",i);
	structAEnviar.archivoResultadoReduce = resultado;

	pthread_create(&hilo_reduce, NULL, aplicarReduce, (void*)&structAEnviar);
	return;

	}
}


void aplicarReduce(t_aplicarReduce structRecibido){



	t_mandarAlHilo mandarAlHilo;
	mandarAlHilo.cantidadArchivosTemporales = structRecibido.cantidadArchivosTemporales;
	mandarAlHilo.idNodo = structRecibido.idNodo;
	mandarAlHilo.archivoResultadoReduce = structRecibido.archivoResultadoReduce;
	mandarAlHilo.vectorArchivosTemporales = structRecibido.vectorArchivosTemporales;
	int tamanioTotal = sizeof(int)+sizeof(int)+sizeof(int)+strlen(structRecibido.archivoResultadoReduce)+1+ (list_size(structRecibido.vectorArchivosTemporales)*sizeof(int));
	send(structRecibido.socketJob, &tamanioTotal, sizeof(int),0);
	char* archivoAenviar = serializar_aplicarReduce(&mandarAlHilo, tamanioTotal);
	send(structRecibido.socketJob,archivoAenviar,tamanioTotal,0);

}




















/*
//marta tiene que verificar previo a esta funcion que llega un hilo mapper
planificarReduce(int socketJob, int accionATomar,  char* archivoTemporalAAlmacenar, t_lista vectorNodos, int cantidadNodos, int idNodo, char* ip, char* puerto){
int aux = 0,cont,accionQueElJobDeberaTomar;
t_nodos nodo_a_mapear;
char* archivo_serializado;
int numero = 0;
int aux2 = 0;
t_nodos nodo;
           if(presenciaCombiner == "SI"){
              switch(accionATomar){
                    case 1: // aca nos llega que almacenemos un archivo temporal
	                       for(cont=0; cont < cantidadNodos; aux ++){
		                       while (aux2 != NULL){
			                        if(vectorNodos.head->data->idNodo == idNodo){
				                       vectorNodos.head->data->vector_archivos_temporales = archivoTemporalAAlmacenar; //Agregarlo al vector
				                       numero = 1;
		                            }
			                        aux2 = vectorNodos.elements_count;
		                       }
	 	                       if(numero ==0){
	 	                    	   nodo.idNodo = idNodo;
			                     lista_add(&vectorNodos,);
		                       }
	                        }
	                break;
                    case 2: // aca nos llega que un nodo esta mapeado seria el idNodo con su ip y puerto
                    	nodo_a_mapear = list_find(*vectorNodos,vectorNodos.head.data.idNodo == idNodo);
                    	int tamanioTotal = sizeof(nodo_a_mapear.idNodo) + sizeof(nodo_a_mapear.ipNodo) + sizeof(nodo_a_mapear.puertoNodo) + sizeof(nodo_a_mapear.cantidadArchivosTemporales) + sizeof(nodo_a_mapear.vector_archivos_temporales) + sizeof(nodo_a_mapear.archivoResultadoReduce);
                    	archivo_serializado = serializar_nodo_a_mapear(nodo_a_mapear,tamanioTotal);
                    	send(socketJob, &accionQueElJobDeberaTomar, sizeof(int),0);
                    	send(socketJob, &tamanioTotal, sizeof(int),0);
                    	send(socketJob, &archivo_serializado, sizeof(int),0);
              }
}
	t_tamanio tamanioTotal;
// el requisito aca es que todos los nodos tengan todos los reduce hechos, desp cuando esten todos hechos
	                           // ahi recien vamos a poder decirle a un nodo (hay uqe ver el criterio para elegirlo) que haga todos los reduce
	t_job_marta Job_Marta;
	t_matriz matrizMapper[cantidadDeArchivos][cantidadDeNodos];
	int cont2,cont3;
	for(cont2=0;cont2<cantidadDeArchivos;cont2++){
		for(cont3=0;cont3<cantidadDeNodos;cont3++){
			matrizMapper[cont2][cont3].bloques_mapeados = 0 ;
			// inicializar el nombre de archivo, que hay que ir buscando los archivos que tenemos e ir poniendolos
			// inicializar t_bloque_mapeado que es una lista de bloques, hay que ir buscando por nodo si esta el bloque o no
			// en el nodo y ahi sumarlo a ese vector
		}
	}
	t_marta_job_archivo_reduce Marta_Job;
	Marta_Job.lista_nombres_archivos_resultado = list_create(); // esto si son varios o no depende de lo mismo que arriba
	int tamanioTotal,respuestaReduce/* si es uno se hizo bien y si es 0 mal*/;
	/*int i ,k,j,aux, h,cont,contador, cantidadBloquesPresentes, contadorFinal;
	int reduceRealizado[cantidadDeNodos];
	//HAY QUE VER SI EL RECV SE HACE ANTES DEL FOR
	// Es jodido el tema, no se xq pones hasta la cant de nodos... el recv que hagamos puede ser de cualquier nodo, no necesariamente va a haber
	//una cierta cantidad de recv, ni necesariamente van a venir ordenados
	cont = 0;
	while(cont < cantidadDeNodos - 1){ // este ciclo es para cuando todavia queden recv para hacer, cuando no queden mas, el cont del ciclo for de
		                               // abajo va a quedar igual a cantidadDeNoddos - 1
		recv(socketJob, &tamanioTotal, sizeof(int),0);
	    int estado = 1;
	   estado =  recive_y_deserialisa_job(&Job_Marta, socketJob, tamanioTotal);
	     if(estado){
	    	 contador = 0;
	         cantidadBloquesPresentes = 0;
     		 contadorFinal = 0;
	    	if(Job_Marta.resultado ==1){
	    	 matrizMapper[Job_Marta.numeroBloque][Job_Marta.idNodo].resultadoMap = Job_Marta.resultado;
	    	 for(j=0;j<cantidadDeBloques ;j++){
		         if((matrizMapper[j][Job_Marta.idNodo].presencia) == 1){
		             cantidadBloquesPresentes ++;
		             if(matrizMapper[j][Job_Marta.idNodo].resultadoMap == 1){
		            	contador ++;
		            	list_add(Marta_Job.lista_nombres_archivos_resultado ,Job_Marta.nombreArchivo);
     			     }
		         }
	    	 }
		      if(contador == cantidadBloquesPresentes){
		    	  Marta_Job.rutina =2;
		    	  Marta_Job.idNodo = Job_Marta.idNodo;
		    	  Marta_Job.ip_nodo = 3 ;// ACA HAY Q BUSAR COMO PONER LA IP
		    	  Marta_Job.puerto = 3 ;// ACA HAY Q BUSCAR EL PUERTO
		    	  //serializar y mandar el reduce al job
		    	  // hay que ver que estructura va a tener
	 	     		//	send(socketJob, ) para el reduce
		}else{
			//aca significa que no se pudo hacer el map bien y entonces hay que mandar de vuelta lo que nos vino a la funcion  planificar map
		}
	  }
	}
   }
}
// aca hay que verificar que nos llega un hilo reduce
rePlanificar_y_planificar_reduce_general(int socketJob, int cantidadDeNodos, int cantidadDeBloques, char* presenciaCombiner){
	int respuestaReduce/* si es uno se hizo bien y si es 0 mal*/;
/*	int cont,reduceRealizado[cantidadDeNodos],aux;
	recv(socketJob, &respuestaReduce, sizeof(int),0);
	if(respuestaReduce == 1){
	   aux = 0;
	   for(cont=0;cont < cantidadDeNodos && aux ==0;cont++){
		   if(reduceRealizado[cont]== 0){
		      reduceRealizado[cont] = 1;
			  aux = 1; // aca para que salga cuando lo encuentre, asi queda cont == cantidadDeNodos cuando sea el ultimo nada mas
		   }
	   }
		   if(cont == cantidadDeNodos - 1){ // si el cont es = a la cantidadDeNodos entonces ya se lleno la ultima posicion
			 // aca hay que mandar el reduce general hay que tener una lista de los archivos donde se hizo el reduce y hay que elegir el nodo
			 //donde se va a hacer el reduce gral
		   }
    }else {
    	// de vuelta llamar a la funcion de planificar reduce
    }
}
*/
