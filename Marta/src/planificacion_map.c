/*
 * planificacion_map.c
 *
 *  Created on: 15/7/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

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
				//	if(buscarPorNodo(list_get(bloque->)->idNodo, nodos_activos, pos)) bitarray_set_bit(vectorDeBitArrays[(*sub_indice)].bitmap, pos);
				}
			(*sub_indice) ++;
		}
	}
	free(un_nombre);
	return vectorDeBitArrays; //LIBERAR ESTA MEMORIA DESP Y ELIMINAR CADA BITARRAY TMB
}

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

void planificarMap(){
	// PARA PLANIFICAR NECESITO SABER LOS NODOS ACTIVOS. PARA ESO SE LO PIDO AL FS
	int cantidad_nodos_activos = 4;	// ESTO ME LO MANDA EL FS, JUNTO CON LOS NODOS_ACTIVOS
	int nodos_activos[cantidad_nodos_activos]; //LOS ID DE LOS NODOS ACTIVOS NECESITO QUE ME LOS MANDES ASI GASTON: [1,14,22,31] ORDENADOS DE MENOR A MAYOR
	t_cargaBitarray_aux bitmapAuxiliar[cantidad_nodos_activos];
    int victim_pos;
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
	}

}

