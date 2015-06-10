/*
 * funcionesParaEnviarEstructuras.c
 *
 *  Created on: 6/6/2015
 *      Author: utnso
 */

#include "librerias_y_estructuras.h"

char* serializarParaGetBloque(estructuraSetBloque *bloque){

	char *serializedPackage = malloc(bloque->tamanioData);

	int offset = 0;
	int size_to_send;

	size_to_send =  sizeof(bloque->bloque);
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

void liberarMensaje(char **package){
	free(*package);
}

int recive_y_deserialisa_IPyPUERTO_Nodo(estructuraIPyNodo *bloque, int socket, uint32_t tamanioTotal){
	int status;
	char *buffer = malloc(tamanioTotal);
	int offset=0;

	recv(socket, buffer, tamanioTotal, 0);

	int tamanioDinamico;
	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	bloque->IP = malloc(tamanioDinamico);
	memcpy(bloque->IP, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	memcpy(&tamanioDinamico, buffer + offset, sizeof(int));
	offset += sizeof(int);
	bloque->PUERTO = malloc(tamanioDinamico);
	memcpy(bloque->PUERTO, buffer + offset, tamanioDinamico);
	offset += tamanioDinamico;

	free(buffer);
	return status;
}

t_archivo* mapBsonToFile(bson_t* document){

	t_archivo* file = malloc(sizeof(t_archivo));
	file->blocks = list_create();

	t_archivo_bloque* block = malloc(sizeof(t_archivo_bloque));
	block->copies = list_create();

	t_archivo_copias* copies = malloc(sizeof(t_archivo_copias));

	bson_iter_t iter;
	bson_iter_t subiter;
	bson_iter_t sub_block_iter;
	bson_iter_t sub_cpy_iter;
	bson_iter_t sub_obj_iter;

	if (bson_iter_init (&iter, document)) {
		while (bson_iter_next (&iter)) {
			if(bson_iter_find (&iter, "name"))file->name = bson_iter_utf8(&iter,NULL);
			if(bson_iter_find (&iter, "size"))file->size = bson_iter_int32(&iter);
			if(bson_iter_find (&iter, "parentDirectory"))file->parent_directory = bson_iter_int32(&iter);
			if(bson_iter_find (&iter, "path"))file->path = bson_iter_utf8(&iter,NULL);
			if(bson_iter_find (&iter, "status"))file->status = bson_iter_int32(&iter);

			if (bson_iter_init_find (&iter, document, "blocks") &&
			    (BSON_ITER_HOLDS_DOCUMENT (&iter) ||
			     BSON_ITER_HOLDS_ARRAY (&iter)) &&
			    bson_iter_recurse (&iter, &subiter)) {

				while (bson_iter_next (&subiter)) {

			      if(bson_iter_recurse(&subiter,&sub_block_iter)) {

			    	  //A nivel de cada bloque
			    	  while(bson_iter_next(&sub_block_iter)) {

			    		  if(string_equals_ignore_case(bson_iter_key (&sub_block_iter),"id")) {
			    			  block->id = bson_iter_int32(&sub_block_iter);
			    		  }

			    		  if (string_equals_ignore_case(bson_iter_key (&sub_block_iter),"copies") &&
			    						  bson_iter_recurse (&sub_block_iter, &sub_cpy_iter)) {

			    			  //A nivel de cada copia
			    			  while (bson_iter_next (&sub_cpy_iter)) {

			    				  if(bson_iter_recurse(&sub_cpy_iter,&sub_obj_iter)) {

			    					  while(bson_iter_next(&sub_obj_iter)) {

			    						  if(string_equals_ignore_case(bson_iter_key (&sub_obj_iter),"copy")) {
			    							  copies->copy = bson_iter_int32(&sub_obj_iter);
			    						  }
			    						  if(string_equals_ignore_case(bson_iter_key (&sub_obj_iter),"content")) {
			    							  copies->content = bson_iter_utf8(&sub_obj_iter, NULL);
			    						  }
			    						  if(string_equals_ignore_case(bson_iter_key (&sub_obj_iter),"node")) {
			    							  copies->node = bson_iter_int32(&sub_obj_iter);
			    						  }
			    						  if(string_equals_ignore_case(bson_iter_key (&sub_obj_iter),"block")) {
			    							  copies->node_block = bson_iter_int32(&sub_obj_iter);
			    						  }
			    					  }
			    					  list_add_in_index(block->copies,copies->copy,copies);
			    					  			    				  }
			    			  }//fin copia
			    		  }

			    	  } //fin bloque
			    	  list_add_in_index(file->blocks,block->id,block);
			      }
			   }
			}
		}
	}
	bson_destroy (document);
	free(copies);
	free(block);
	return file;
}
