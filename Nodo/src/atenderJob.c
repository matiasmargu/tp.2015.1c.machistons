/*
 * atenderJob.c
 *
 *  Created on: 17/6/2015
 *      Author: utnso
 */
#include "variablesGlobales.h"

void* atenderJob(void* arg){
	char modo[] = "0777";
	int socket = (int)arg;
	int comando=1;
	int tamanioScript;
	char* direccionAuxiliar=malloc(strlen(dir_temp));
	char* script_mapper;
	char* script_reducer;
	int i;
	int tamanioTotalIP_P;

	t_para_nodo comb;

	pthread_t hiloMapper[1000];
	pthread_t hiloReducer[1000];

	send(socket, &comando,sizeof(int),0);

	int cont1=0;
	int cont2=0;

	char* puertoN_aConectar;

	while(1){

		//77 salio todo bien
		//66 salio todo mal

		recv(socket, &comando, sizeof(int),0 );
		switch(comando){
			case 1: //Este va a ser el map
				printf("Se levanto un hilo mapper\n");

				send(socket, &comando,sizeof(int),0);

				recv(socket,&tamanioScript,sizeof(int),0);
				send(socket, &comando,sizeof(int),0);


				script_mapper=malloc(tamanioScript);

				recv(socket,script_mapper,tamanioScript,0);

				escribirScript(script_mapper,1);

				i = strtol(modo, 0, 8);
				chmod("/tmp/mapper",modo);

				printf("se asignaron los permisos\n");

				pthread_create(&hiloMapper[cont1], NULL, (void*) mapper,(void*) socket);
				cont1++;
				break;

			case 2: //Este va a ser el reduce
				printf("Se levanto un hilo reduce\n");

				send(socket, &comando,sizeof(int),0);

				recv(socket,&tamanioScript,sizeof(int),0);
				send(socket, &comando,sizeof(int),0);
				script_reducer=malloc(tamanioScript);

				recv(socket,script_reducer,tamanioScript,0);
				escribirScript(script_reducer,2);
				send(socket, &comando,sizeof(int),0);


				chmod(direccionAuxiliar,modo);


				pthread_create(&hiloReducer[cont2], NULL,(void*)reducer,(void*) socket);
				cont2++;
				break;
			case 3:
				printf("Se levanto un movimiento de archivos\n");

				send(socket, &comando,sizeof(int),0);

				recv(socket,&tamanioTotalIP_P,sizeof(int),0);
				recive_y_deserializa_NODO_C(&comb,socket,tamanioTotalIP_P);

				int socket_nodo = crearCliente(comb.ip,comb.puerto);

				comando=3;

				send(socket_nodo,&comando,sizeof(int),0);
				pedirContenidoDeUnArchivo(comb.archivo,socket_nodo);
				break;
				}
		}
	free(direccionAuxiliar);
	return NULL;
}

