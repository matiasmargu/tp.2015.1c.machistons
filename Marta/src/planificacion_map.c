/*
 * planificacion_map.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"
/*
int calcularCantidadDeBloques(t_list* lista_archivos){
	int cantidad,j,h,k,p;
	t_archivo* archivo;
	t_list* listaDeBloques;
	listaDeBloques = list_create();
	int elemento;
	for(j=0 ; j< list_size(lista_archivos);j++){
		archivo = list_get(lista_archivos,j);
		for(h=0;h < archivo->cantidadDeBloques;h++){
				t_bloque* bloque = list_get(archivo->bloques,h);
				for(k=0;k < 3; k++){
					for(p=0;p < list_size(listaDeBloques); p++){
						//ESTO ESTA MAL , HAY QUE VER COMO SE PUEDE ARREGLAR
						elemento = list_get(listaDeBloques, p);
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

void inicializarBitarray(t_bitarray *bitmap, int tamanio){
	int i;
	for(i=0;i < tamanio;i++){
		 bitarray_clean_bit(bitmap, i);
	}
}

int get_nodo_pos(int nodos_activos[], int id){
	int i,pos;
	//printf("ID DEL NODO QUE ME LLEGA: %i\n",id);
	for(i=0;i<cant_nodos;i++){
		if(nodos_activos[i] == id) pos = i;
		//printf("NODO ACTIVO: %i\n",nodos_activos[i]);
	}
	return pos;
}

/*
t_cargaBitarray_aux *armarVectorDeBitarray(t_cargaBitarray_aux *vectorDeBitArrays, int cantidadDeNodos, int nodos_activos[], int *sub_indice){
	//int cantidadDeBloquesTotales = calcularCantidadDeBloques(lista_archivos);
	t_archivo *un_archivo = malloc(sizeof(t_archivo));
	t_bloque *bloque = malloc(sizeof(t_bloque));
	char *un_nombre;
	// OJO CON ESTO, NO ESTOY SEGURO SI NO HAY QUE PASARLO CARGADO CON 0 PREVIAMENTE
	//seteo los bitarrays
	int j,h,k,pos,nodo_id;
	(*sub_indice) = 0;
	printf("-------------------------Estructura del bitarray--------------------------------\n");
	printf("CANTIDAD DE ARCHIVOS: %i\n",list_size(lista_archivos));
	for(j=0 ; j< list_size(lista_archivos);j++){
		//printf("nodos control 3\n");
		printf("LIST ARCHIVOS SIZE: %i\n\n",list_size(lista_archivos));
		un_archivo = list_get(lista_archivos,j);
		vectorDeBitArrays = malloc(sizeof(t_cargaBitarray_aux)*list_size(un_archivo->bloques));
		//printf("nodos control 4\n");
		for(h=0;h < un_archivo->cantidadDeBloques;h++){
			bloque = list_get(un_archivo->bloques,h);
			//printf("ID DEL BLOQUE DENTRO DEL BITARRAY: %i\n",bloque->NumeroBloque);
			//cargo t_bitarray_aux
			//vectorDeBitArrays = malloc(sizeof(t_cargaBitarray_aux)*((*sub_indice)+1));
			vectorDeBitArrays[(*sub_indice)].nombre_arch = un_archivo->nombre;
			//printf("NOMBRE DEL ARCHIVO: %s \n", vectorDeBitArrays[(*sub_indice)].nombre_arch);
			vectorDeBitArrays[(*sub_indice)].bloque_arch = bloque->NumeroBloque;
			//printf("ID DEL BLOQUE DENTRO DEL BITARRAY: %i\n\n", vectorDeBitArrays[(*sub_indice)].bloque_arch);
			//printf("CANTIDAD DE NODOS: %i\n",cantidadDeNodos);
			un_nombre = malloc(cantidadDeNodos*sizeof(char));
			//printf("TAMAÑO DEL CHAR*: %i\n",cantidadDeNodos*sizeof(char));
			vectorDeBitArrays[(*sub_indice)].bitmap = bitarray_create(un_nombre, cantidadDeNodos);
			printf("TAMAÑO DEL BITMAP ADENTRO: %i\n",vectorDeBitArrays[(*sub_indice)].bitmap->size);
			inicializarBitarray(vectorDeBitArrays[(*sub_indice)].bitmap, cantidadDeNodos);
			//Cargo el bitarray
			nodo_id = bloque->copia1_idnodo;
			pos = get_nodo_pos(nodos_activos, nodo_id);
			//printf("POSICION: %i\n",pos);
			bitarray_set_bit(vectorDeBitArrays[(*sub_indice)].bitmap, pos);

			nodo_id = bloque->copia2_idnodo;
			pos = get_nodo_pos(nodos_activos, nodo_id);
			//printf("POSICION: %i\n",pos);
			bitarray_set_bit(vectorDeBitArrays[(*sub_indice)].bitmap, pos);

			nodo_id = bloque->copia3_idnodo;
			pos = get_nodo_pos(nodos_activos, nodo_id);
			//printf("POSICION: %i\n",pos);
			bitarray_set_bit(vectorDeBitArrays[(*sub_indice)].bitmap, pos);

			(*sub_indice) ++;

			/*
			printf("---------------------BITMAP-----------------------\n");
			for(k=0;k<cant_nodos;k++){
				if(bitarray_test_bit(vectorDeBitArrays[(*sub_indice)].bitmap, k)) printf("    1    |");
				else printf("    0    |");
			}
			printf("\n");
			*/
/*
		}
		printf("TAMAÑO DEL BITMAP: %i\n",vectorDeBitArrays[2].bitmap->size);
		for(k=0;k<=cantidadDeNodos;k++){
			if(bitarray_test_bit(vectorDeBitArrays[2].bitmap, k) == true) printf("BIT DE TESTEO N° %i: 1\n",k);
			else printf("BIT DE TESTEO N° %i: 0\n",k);
		}
		printf("dsadjsadsakj\n");
		printf("TAMAÑO DEL BITMAP %i\n",bitarray_get_max_bit(vectorDeBitArrays[2].bitmap));
	}
	//free(un_nombre);
	printf("nodos control 5\n");
	return vectorDeBitArrays; //LIBERAR ESTA MEMORIA DESP Y ELIMINAR CADA BITARRAY TMB
}
*/
/*
void buscarVictimasPorBloque(t_cargaBitarray_aux bitmap[], int tamanio, int *vector_contador, int cant_nodos, int *bloqueArch_pos, int *nodo){
	int min = bitarray_get_max_bit(bitmap[0].bitmap);
	int min_pos = 0;
	int i;
	// busco el menor
	for(i=1;i<=tamanio;i++){
		if((bitarray_get_max_bit(bitmap[i].bitmap)) < min){
			min = bitarray_get_max_bit(bitmap[i].bitmap);
			min_pos = i;
		}
	}
	//Del mas chico que encontre, defino donde se va a asignar por contadores de nodos
	nodo = buscarVictimasPorContadores(bitmap[min_pos], vector_contador, cant_nodos);
	bloqueArch_pos = min_pos;
}

int buscarVictimasPorContadores(t_cargaBitarray_aux bitmap, int *vector_contador, int cant_nodos){
	int i;
	int min = 10000;
	int nodo = 0;
	for(i=0;i<cant_nodos;i++){
		if(bitarray_test_bit(bitmap.bitmap,i)){
			if(vector_contador[i] < min){
				min = vector_contador[i];
				nodo = i;
			}
		}
	}
	return nodo;
}

void cargar_nodos_en_array(int *nodos){
	int i;
	t_nodo *nodo = malloc(sizeof(t_nodo));
	for(i=0;i < list_size(lista_nodos_estado);i++){
		nodo = list_get(lista_nodos_estado,i);
		nodos[i] = nodo->id_nodo;
	}
}
*/
int buscarPorContadores(t_bloque *bloque){
	int i;
	int min = contadores_nodos[0];
	int pos_id_nodo;
	int id_nodo = 555;
	bool flag = false;
	t_nodo *data_nodo = malloc(sizeof(t_nodo));
	do
	{
	//---------------------------------------BUSCO EL MAS CHICO DE LOS CONTADORES--------------------------------
		for(i=1;i<cant_nodos;i++){
			if(contadores_nodos[i] < min){
				min = contadores_nodos[i];
				pos_id_nodo = i;
			}
		}
		data_nodo = list_get(lista_nodos_estado,pos_id_nodo);
	//-------------------------------------BUSCO SI LA COPIA ESTA EN EL NODO MAS CHICO---------------------------
		for(i=0;i<3;i++){
			if(data_nodo->id_nodo == bloque->copias[i].idNodo){
				id_nodo = data_nodo->id_nodo;
				flag = true;
				break;
			}
		}
		if(flag == false) contadores_nodos[pos_id_nodo] = 4444;

	} while(flag == false);

	return id_nodo;
}

void planificarMap(int id_job){
	/*
	// PARA PLANIFICAR NECESITO SABER LOS NODOS ACTIVOS. PARA ESO SE LO PIDO AL FS
	int cantidad_nodos_activos = list_size(lista_nodos_estado);	// ESTO ME LO MANDA EL FS, JUNTO CON LOS NODOS_ACTIVOS
	int *nodos_activos = malloc(sizeof(int)*cantidad_nodos_activos);
	cargar_nodos_en_array(nodos_activos);//LOS ID DE LOS NODOS ACTIVOS NECESITO QUE ME LOS MANDES ASI GASTON: [1,14,22,31] ORDENADOS DE MENOR A MAYOR
	t_cargaBitarray_aux bitmapAuxiliar[cantidad_nodos_activos];
	int i;
	for(i=0;i<cantidad_nodos_activos;i++) printf("IDs DEL NODO: %i\n",nodos_activos[i]);
	//int victim_pos;
    //pthread_mutex_lock(&mutex_nodos);
    //lista_nodos_estado = list_create();
    //pthread_mutex_unlock(&mutex_nodos);


    int *tamanio = malloc(sizeof(int));
    t_cargaBitarray_aux *bitmap = malloc(sizeof(t_cargaBitarray_aux));
    armarVectorDeBitarray(bitmap, cantidad_nodos_activos, nodos_activos, tamanio);
    /*
    int division = (*tamanio)/cantidad_nodos_activos;
    int resto_division = (*tamanio)%cantidad_nodos_activos;
    if(resto_division == 0) division--;
    // vector_contador = vector de contadores para planificar (esto no cambia hasta que termina el planificarMap)
    int *vector_contador = malloc(sizeof(int)*cantidad_nodos_activos);
    int j, bloqueArch_pos,bloque,nodo;
    // inicializo vector_contador
    for(j=0;j<cantidad_nodos_activos;j++) vector_contador[j] = 0;
    //
    for(j=0;j <= division;j++){
    	int k = 0;
    	int tamanio_bitmapAux = division;
    	if((resto_division != 0) && (j == division)) tamanio_bitmapAux = resto_division-1;
    	// tengo que cargar el auxiliar con los bloques que va a planificar el algoritmo de map
    	// ej: si es la primera pasada, auxiliar va a tener la cantidad de bloques
    	// si es la 2da pasada, auxiliar va a tener la cantidad de bloques menos el que ya se eligio antes
    	//cargarBitmapAuxiliar(bitmapAuxiliar, bitmap, tamanio_bitmapAux);
    	while(k <= tamanio_bitmapAux){
    		/*
				char *vectorVictimas_str = malloc(sizeof(char) * cantidad_nodos_activos);
				t_cargaBitarray_aux *vectorVictimas = malloc(sizeof(t_cargaBitarray_aux));
				vectorVictimas->bitmap = bitarray_create(vectorVictimas_str, cantidad_nodos_activos);
				inicializarBitarray(vectorVictimas->bitmap, cantidad_nodos_activos);
				*/
	/*
    		buscarVictimasPorBloque(bitmapAuxiliar, tamanio_bitmapAux, vector_contador, cantidad_nodos_activos, bloqueArch_pos, nodo);
    		bloque = bitmapAuxiliar[bloqueArch_pos].bloque_arch;
    		//eliminarBloque(bitmapAuxiliar, bloqueArch_pos, tamanio_bitmapAux);

    		if (j == 0){
    			//si j==0 quiere decir que la alineacion de vectorVictimas es pura
    			//si j > 0 quiere decir que la alineacion de vectorVictimas es k*cantidad_nodos_activos
    			// alineacion = posicion del bloque dentro del bitmap grande
					}
    		k++;
			}
		}
    free(vector_contador);
    */

    //---------------------OBTENGO LA INFO DEL JOB--------------------------------------
    t_infoJob *job = malloc(sizeof(t_infoJob));
    job = list_get(lista_jobs,id_job);
    //----------------------------------------------------------------------------------

    t_job_procesos *tabla_procesos_job = malloc(sizeof(t_job_procesos));
    tabla_procesos_job = list_get(lista_tabla_procesos,id_job);
    t_tablaProcesos_porJob *tabla_procesos;
    int j,i;
    int id_nodo = 0;
    t_archivo *un_archivo = malloc(sizeof(t_archivo));
    t_bloque *un_bloque = malloc(sizeof(t_bloque));

    printf("       es        |        id      |        bq       |        nm       |\n");
    for(i=0;i<list_size(job->archivos_job);i++){
    	un_archivo = list_get(lista_archivos,i);
    	for(j=0;j<list_size(un_archivo->bloques);j++){
    		un_bloque = list_get(un_archivo->bloques,j);
    		id_nodo = buscarPorContadores(un_bloque);
    		tabla_procesos = malloc(sizeof(t_tablaProcesos_porJob));
    		tabla_procesos->estado = 0; //Estado 0 = espera por enviarse
    		tabla_procesos->id_nodo = id_nodo;
    		tabla_procesos->bloque_archivo = un_bloque->NumeroBloque;
    		tabla_procesos->nombre_archivo = un_archivo->nombre;
    		list_add(tabla_procesos_job->tabla_procesos,tabla_procesos);
    		printf("       %i        |        %i      |        %i       |        %s       |\n",tabla_procesos->estado,tabla_procesos->id_nodo,tabla_procesos->bloque_archivo,tabla_procesos->nombre_archivo);

    	}
    }

    free(un_archivo);
    free(un_bloque);
}
