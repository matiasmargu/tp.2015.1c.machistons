/*
 ============================================================================
 Name        : Marta2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "funciones.h"

t_list *lista_nodos_estado;

typedef struct{
	int id_nodo;
	char *ip_nodo;
	char *puerto_nodo;
}t_nodo2;

typedef struct{
	int estado; // 0 = todavia no se mando a ejecutar; 1 = en ejecucion; 2 = Fin; 3 = Error
	int bloque_archivo;
	char *nombre_archivo_resultado;
	int id_nodo;
}t_tablaProcesos_porJob2;

typedef struct{
	int idJob;
	t_list *tabla_procesos;
}t_lista_job2;

typedef struct {
	int idNodo;
	char* puertoNodo;
	char * ipNodo;
	t_list* archivosAReducir;
    char* nombreArchivoResultado;
}t_archivosAReducirPorNodo;

typedef struct {
	int contador;
	int idNodo;
}t_contadorNodo;

typedef struct {
	char *puertoNodo;
	char *ipNodo;
	t_list * archivosAMover;
}t_moverArchivos;

typedef struct {
	char *puertoNodo;
	char *ipNodo;
	char * archivoAMover;
}t_serializarUnArchivoParaMover;

char* serializar_archivoAMover(t_serializarUnArchivoParaMover * archivoAMover,int tamanioSerializacionMover){
	char *serializedPackage = malloc(tamanioSerializacionMover);
	int offset = 0;
	int size_to_send;

	int tamanioNombre;
	tamanioNombre = strlen(archivoAMover->puertoNodo) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->puertoNodo) + 1;
	memcpy(serializedPackage + offset, archivoAMover->puertoNodo, size_to_send);
	offset += size_to_send;

	tamanioNombre = strlen(archivoAMover->ipNodo) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->ipNodo) + 1;
	memcpy(serializedPackage + offset, archivoAMover->ipNodo, size_to_send);
	offset += size_to_send;

	tamanioNombre = strlen(archivoAMover->archivoAMover) + 1;
	size_to_send = sizeof(int);
	memcpy(serializedPackage + offset, &tamanioNombre, size_to_send);
	offset += size_to_send;

	size_to_send =  strlen(archivoAMover->archivoAMover) + 1;
	memcpy(serializedPackage + offset, archivoAMover->archivoAMover, size_to_send);
	offset += size_to_send;

	return serializedPackage;
}


void planificarSincombiner(){

	t_lista_job2 *tabla;
	t_lista_job2 *tabla2;
	t_lista_job2* tabla3;
	t_tablaProcesos_porJob2 *campoDeTabla;
	t_tablaProcesos_porJob2 *campoDeTabla2;
	t_tablaProcesos_porJob2 *campoDeTabla3;
	t_tablaProcesos_porJob2 *campoDeTabla4;
	t_tablaProcesos_porJob2 *campoDeTabla5;
	t_tablaProcesos_porJob2 *campoDeTabla6;
	t_list* lista_job_tabla;
	//REDUCE FINAL
	t_job_marta_reduce job_marta;
	int contador2,r,l,tamanioArecibir,p,tamanioStruct, tamanioListaFinal,m;
	t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo;
	t_archivosAReducirPorNodo *campoDeListaArchivosAReducirPorNodo2;
	t_archivosAReducirPorNodo * campoDeListaArchivosAReducirPorNodo3;
	t_list* listaFinalDeArchivosAReducir;
	t_marta_job_reduce antesDeSerializar;
	char* structParaJob;
	char* archivoA;
	char* archivo;
	contador2 = 0;


	// para probar
	int tamanioTabla,tamanioCampoDeTabla,tamanioNodo, contador;
	tamanioTabla=sizeof(int);
	tamanioCampoDeTabla= sizeof(int) * 3 + sizeof(char);
	tamanioNodo = sizeof(char) * 3 + sizeof(int);

	tabla = malloc(tamanioTabla);
	tabla2 = malloc(tamanioTabla);
	tabla3 = malloc(tamanioTabla);
	campoDeTabla = malloc(tamanioCampoDeTabla);
	campoDeTabla2 = malloc(tamanioCampoDeTabla);
	campoDeTabla3 = malloc(tamanioCampoDeTabla);
	campoDeTabla4 = malloc(tamanioCampoDeTabla);
	campoDeTabla5 = malloc(tamanioCampoDeTabla);
	campoDeTabla6 = malloc(tamanioCampoDeTabla);
	tabla->idJob=4;
	tabla->tabla_procesos = list_create();

	tabla2->idJob=7854;
	tabla2->tabla_procesos = list_create();

	tabla3->idJob=5;

	tabla3->tabla_procesos = list_create();


	campoDeTabla->bloque_archivo = 5;
	campoDeTabla->estado = 3;
	campoDeTabla->id_nodo = 4;
	campoDeTabla->nombre_archivo_resultado = "campoTabla1";

	campoDeTabla2->bloque_archivo = 2;
	campoDeTabla2->estado = 0;
	campoDeTabla2->id_nodo = 2;
	campoDeTabla2->nombre_archivo_resultado = "campoTabla2";

	campoDeTabla3->bloque_archivo = 4;
	campoDeTabla3->estado = 1;
	campoDeTabla3->id_nodo = 7;
	campoDeTabla3->nombre_archivo_resultado = "campoTabla3";

	campoDeTabla4->bloque_archivo = 0;
	campoDeTabla4->estado = 1;
	campoDeTabla4->id_nodo = 54;
	campoDeTabla4->nombre_archivo_resultado = "campoTabla4";

	campoDeTabla5->bloque_archivo = 14;
	campoDeTabla5->estado = 2;
	campoDeTabla5->id_nodo = 54;
	campoDeTabla5->nombre_archivo_resultado = "campoTabla5";

	campoDeTabla6->bloque_archivo = 21;
	campoDeTabla6->estado = 3;
	campoDeTabla6->id_nodo = 54;
	campoDeTabla6->nombre_archivo_resultado = "campoTabla6";


	list_add(tabla->tabla_procesos,campoDeTabla);
	list_add(tabla->tabla_procesos,campoDeTabla2);
	list_add(tabla2->tabla_procesos,campoDeTabla3);
	list_add(tabla2->tabla_procesos,campoDeTabla4);
	list_add(tabla3->tabla_procesos,campoDeTabla5);
	list_add(tabla3->tabla_procesos,campoDeTabla6);

	lista_job_tabla = list_create();
	list_add(lista_job_tabla,tabla);
	list_add(lista_job_tabla,tabla2);
	list_add(lista_job_tabla,tabla3);

	lista_nodos_estado = list_create();
	t_nodo2 *nodo1;
	t_nodo2 *nodo2;
	t_nodo2 *nodo3;
	t_nodo2 *nodo4;
	t_nodo2 *nodo5;
	t_nodo2 *nodo6;

	nodo1=malloc(tamanioNodo);
	nodo2=malloc(tamanioNodo);
	nodo3=malloc(tamanioNodo);
	nodo4=malloc(tamanioNodo);
	nodo5=malloc(tamanioNodo);
	nodo6=malloc(tamanioNodo);


	nodo1->id_nodo = 4;
	nodo1->ip_nodo = "44.251";
	nodo1->puerto_nodo = "887";
	nodo2->id_nodo = 2;
	nodo2->ip_nodo = "44.250";
	nodo2->puerto_nodo = "886";
	nodo3->id_nodo = 7;
	nodo3->ip_nodo = "44.252";
	nodo3->puerto_nodo = "885";
	nodo4->id_nodo = 54;
	nodo4->ip_nodo = "44.253";
	nodo4->puerto_nodo = "884";
	nodo5->id_nodo = 23;
	nodo5->ip_nodo = "44.251";
	nodo5->puerto_nodo = "883";
	nodo6->id_nodo = 44;
	nodo6->ip_nodo = "44.254";
	nodo6->puerto_nodo = "882";

	list_add(lista_nodos_estado,nodo1);
	list_add(lista_nodos_estado,nodo2);
	list_add(lista_nodos_estado,nodo3);
	list_add(lista_nodos_estado,nodo4);
	list_add(lista_nodos_estado,nodo5);
	list_add(lista_nodos_estado,nodo6);

	int a,b,c,d,e,aux,variableParaResultadoReduce,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
	int tamanioListaDeNodos,tamanio,tamanioArchivosaReducir,tamanioListaContadoresNodo;
	t_archivosAReducirPorNodo *campoDeUnNodo;
	t_archivosAReducirPorNodo * campoParaLiberar;
	t_marta_job_reduce structAserializar;
	int handshakeJob, enteroPrueba,tamanioAenviar,tamanioTotalVector, socketJob;
	char* archivo1;
	char* structAEnviarAJob;
	char* archivosAMoverSerializados;
	char * archivoASerializar;
	t_lista_job2 *campoDeLaLista;
	campoDeLaLista = malloc(tamanioTabla);
	t_tablaProcesos_porJob2 *campoDeLaListaTablaDeProcesos;
	campoDeLaListaTablaDeProcesos = malloc(tamanioCampoDeTabla);
	t_list * lista_archivosAReducirPorNodo;
	t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
	campoArchivosAReducirPorNodo= malloc(tamanioCampoDeTabla);
	t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
	t_archivosAReducirPorNodo *campoParaAumentarContadores;
	t_contadorNodo * contadorNodo;
	t_list * listaDeContadoresNodo;
	listaDeContadoresNodo = list_create();
	t_nodo2 *campoDeListaDeNodo;
	campoDeListaDeNodo = malloc(tamanioNodo);
	lista_archivosAReducirPorNodo = list_create();
	list_clean(lista_archivosAReducirPorNodo);
	t_moverArchivos * moverArchivos;
	moverArchivos = malloc(sizeof(char));
	moverArchivos->archivosAMover = list_create();
	t_serializarUnArchivoParaMover * archivoAMover;
	archivoAMover = malloc(sizeof(char) * 3);

	tamanioListaJobTabla = list_size(lista_job_tabla);

	for(a=0;a<tamanioListaJobTabla;a++){
		campoDeLaLista=list_get(lista_job_tabla,a);
		tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos);

		for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
			campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
			tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
			if(tamanioListaArchivosAReducirPorNodo == 0){
				campoAAgregarAListaReducirPorNodo = malloc(tamanioCampoDeTabla);
				campoAAgregarAListaReducirPorNodo->idNodo =  campoDeLaListaTablaDeProcesos->id_nodo;
				campoAAgregarAListaReducirPorNodo->archivosAReducir = list_create();
				list_add(campoAAgregarAListaReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
				asprintf(&campoAAgregarAListaReducirPorNodo->nombreArchivoResultado,"%s%i","archivo",variableParaResultadoReduce);
				variableParaResultadoReduce ++;
				// buscamos el ip y el puerto nodo
				tamanioListaDeNodos =list_size(lista_nodos_estado);
				aux=0;
				for(d=0;d<tamanioListaDeNodos && aux==0;d++){
					campoDeListaDeNodo = list_get(lista_nodos_estado,d);
					if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
						campoAAgregarAListaReducirPorNodo->ipNodo =  campoDeListaDeNodo->ip_nodo;
						campoAAgregarAListaReducirPorNodo->puertoNodo =  campoDeListaDeNodo->puerto_nodo;
						aux=1;

					}
				}// agrega el campo a la lista

				list_add(lista_archivosAReducirPorNodo,campoAAgregarAListaReducirPorNodo);
				campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				e++;
			}
			else{
				aux=0;
				for(c=0;c <tamanioListaArchivosAReducirPorNodo && aux ==0;c++){
					campoArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,c);
					if(campoArchivosAReducirPorNodo->idNodo == campoDeLaListaTablaDeProcesos->id_nodo){
						list_add(campoArchivosAReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
						aux =1;
					}
				}
				if(aux==0){//cuando no esta el id del nodo, entonces tiene que agregar todo directo
					printf("Aca deberia entrar 4 veces \n");
					campoAAgregarAListaReducirPorNodo = malloc(tamanioCampoDeTabla);
					campoAAgregarAListaReducirPorNodo->idNodo =  campoDeLaListaTablaDeProcesos->id_nodo;
					campoAAgregarAListaReducirPorNodo->archivosAReducir = list_create();
					list_add(campoAAgregarAListaReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);

					asprintf(&campoAAgregarAListaReducirPorNodo->nombreArchivoResultado,"%s%i","archivo",variableParaResultadoReduce);
					variableParaResultadoReduce ++;
					// buscamos el ip y el puerto nodo
					tamanioListaDeNodos =list_size(lista_nodos_estado);
					aux=0;
					for(d=0;d<tamanioListaDeNodos && aux==0;d++){
						campoDeListaDeNodo = list_get(lista_nodos_estado,d);
						if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
							campoAAgregarAListaReducirPorNodo->ipNodo =  campoDeListaDeNodo->ip_nodo;
							campoAAgregarAListaReducirPorNodo->puertoNodo =  campoDeListaDeNodo->puerto_nodo;
							aux=1;

						}
					}// agrega el campo a la lista
					list_add(lista_archivosAReducirPorNodo,campoAAgregarAListaReducirPorNodo);
					campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
					printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
					e++;

				}
			}
		}
	}

	     tamanio = list_size(lista_archivosAReducirPorNodo);
		 contadorNodo = malloc(sizeof(int) * 2 );
		 contadorNodo->contador = 0;
		 for(e=0;e<tamanio;e++){
			 campoParaAumentarContadores = list_get(lista_archivosAReducirPorNodo,e);
			 a=list_size(campoParaAumentarContadores->archivosAReducir);
			 // va agregando los archivos a mover
			 for(b=0;b<a;b++){
				 archivo = list_get(campoParaAumentarContadores->archivosAReducir,b);
				 list_add(moverArchivos->archivosAMover,archivo);
			 }

		     // la finalidad de este if que quede en el id el nodo con mas archivos a reducir
			 if (a > contadorNodo->contador) {
				 contadorNodo->idNodo = campoParaAumentarContadores->idNodo;
			 }

		 }

		 printf("\n%i\n",contadorNodo->contador);

		 // con este for ya vamos a tener el puerto el ip y la lista de archivos a mover
		 aux=0;
		 for(d=0;d<tamanioListaDeNodos && aux==0;d++){
			 campoDeListaDeNodo = list_get(lista_nodos_estado,d);
			 if(contadorNodo->idNodo==campoDeListaDeNodo->id_nodo){
				 moverArchivos->ipNodo =  campoDeListaDeNodo->ip_nodo;
				 moverArchivos->puertoNodo = campoDeListaDeNodo->puerto_nodo;
				 aux=1;

			 	 }
		 }

		 // ahora hay que decirle al nodo que los mueva

		 int tamanioSerializacionMover;
		 tamanioSerializacionMover = malloc(sizeof(char) * 3 );
		 handshakeJob = 22;
		 send(socketJob, &handshakeJob, sizeof(int),0);
		 recv(socketJob, &enteroPrueba, sizeof(int),0);
		 tamanio = list_size(moverArchivos->archivosAMover);
		 for(a=0;a < tamanio;a++){
			 archivoAMover->ipNodo =moverArchivos->ipNodo;
			 archivoAMover->puertoNodo = moverArchivos->puertoNodo;
			 archivo = list_get(moverArchivos->archivosAMover, a);
			 archivoAMover->archivoAMover = archivo;
			 archivoASerializar = serializar_archivoAMover(&archivoAMover,tamanioSerializacionMover);
			 send(socketJob, &tamanioSerializacionMover, sizeof(int),0);
			 recv(socketJob, &enteroPrueba, sizeof(int),0);
			 send(socketJob,archivoASerializar,tamanioSerializacionMover,0);
			 recv(socketJob,&enteroPrueba,sizeof(int),0);
		 }


		 // ahora hay que decirle al nodo que haga el reduce final sobre los archivos mapeados

		 handshakeJob = 34;

		 send(socketJob, &handshakeJob, sizeof(int),0);
		 recv(socketJob, &enteroPrueba, sizeof(int),0);

		 //	antesDeSerializar.archivoResultadoReduce =  aca va el qe pasa job al principio hay que ver si viene como parametro o lo tenemos como global
		 antesDeSerializar.cantidadArchivos = list_size(moverArchivos->archivosAMover);
		 antesDeSerializar.idNodo = contadorNodo->idNodo;
		 antesDeSerializar.ipNodo = moverArchivos->ipNodo;
		 antesDeSerializar.puertoNodo = moverArchivos->puertoNodo;
		 list_add_all(antesDeSerializar.listaArchivosTemporales, moverArchivos->archivosAMover);

		 for(m=0; m< tamanioArchivosaReducir; m++){
			 archivoA =	list_get( listaFinalDeArchivosAReducir,m);
			 tamanioListaFinal += strlen(archivoA);
			 tamanioListaFinal += 1;
		 }

		 tamanioStruct = (sizeof(int)*2)+strlen(antesDeSerializar.archivoResultadoReduce)+1 + strlen(antesDeSerializar.puertoNodo)+1 + strlen(antesDeSerializar.ipNodo) +1 + tamanioListaFinal;
		 send(socketJob, &tamanioStruct , sizeof(int),0);
		 recv(socketJob, &enteroPrueba, sizeof(int),0);
		 structParaJob = serializar_marta_job_reduce(&antesDeSerializar, tamanioStruct);
		 send(socketJob, structParaJob, tamanioStruct, 0);


/*

int main(void){

	t_lista_job2 *tabla;
	t_lista_job2 *tabla2;
	t_lista_job2* tabla3;
	t_tablaProcesos_porJob2 *campoDeTabla;
	t_tablaProcesos_porJob2 *campoDeTabla2;
	t_tablaProcesos_porJob2 *campoDeTabla3;
	t_tablaProcesos_porJob2 *campoDeTabla4;
	t_tablaProcesos_porJob2 *campoDeTabla5;
	t_tablaProcesos_porJob2 *campoDeTabla6;
	t_list* lista_job_tabla;


		// para probar
        int tamanio23;
        char * lluo,temp;
        lluo= "campoTabla1";
        tamanio23=sizeof(int)* 5 + 4 * strlen(lluo) + 1 ;
		tabla = malloc(tamanio23);
		tabla2 = malloc(tamanio23);
		tabla3 = malloc(tamanio23);
        campoDeTabla = malloc(tamanio23);
        campoDeTabla2 = malloc(tamanio23);
        campoDeTabla3 = malloc(tamanio23);
        campoDeTabla4 = malloc(tamanio23);
        campoDeTabla5 = malloc(tamanio23);
        campoDeTabla6 = malloc(tamanio23);
        tabla->idJob=4;
		tabla->tabla_procesos = list_create();

		tabla2->idJob=7854;
		tabla2->tabla_procesos = list_create();

		tabla3->idJob=5;

		tabla3->tabla_procesos = list_create();


        campoDeTabla->bloque_archivo = 5;
		campoDeTabla->estado = 3;
		campoDeTabla->id_nodo = 4;
		campoDeTabla->nombre_archivo_resultado = "campoTabla1";

		campoDeTabla2->bloque_archivo = 2;
		campoDeTabla2->estado = 0;
		campoDeTabla2->id_nodo = 2;
		campoDeTabla2->nombre_archivo_resultado = "campoTabla2";

		campoDeTabla3->bloque_archivo = 4;
		campoDeTabla3->estado = 1;
		campoDeTabla3->id_nodo = 7;
		campoDeTabla3->nombre_archivo_resultado = "campoTabla3";

	    campoDeTabla4->bloque_archivo = 0;
		campoDeTabla4->estado = 1;
		campoDeTabla4->id_nodo = 54;
		campoDeTabla4->nombre_archivo_resultado = "campoTabla4";

		campoDeTabla5->bloque_archivo = 14;
		campoDeTabla5->estado = 2;
		campoDeTabla5->id_nodo = 54;
		campoDeTabla5->nombre_archivo_resultado = "campoTabla5";

		campoDeTabla6->bloque_archivo = 21;
		campoDeTabla6->estado = 3;
		campoDeTabla6->id_nodo = 54;
		campoDeTabla6->nombre_archivo_resultado = "campoTabla6";


		list_add(tabla->tabla_procesos,campoDeTabla);
		list_add(tabla->tabla_procesos,campoDeTabla2);
		list_add(tabla2->tabla_procesos,campoDeTabla3);
		list_add(tabla2->tabla_procesos,campoDeTabla4);
		list_add(tabla3->tabla_procesos,campoDeTabla5);
		list_add(tabla3->tabla_procesos,campoDeTabla6);

		lista_job_tabla = list_create();
		list_add(lista_job_tabla,tabla);
		list_add(lista_job_tabla,tabla2);
		list_add(lista_job_tabla,tabla3);

		lista_nodos_estado = list_create();
		t_nodo2 *nodo1;
		t_nodo2 *nodo2;
		t_nodo2 *nodo3;
		t_nodo2 *nodo4;
		t_nodo2 *nodo5;
		t_nodo2 *nodo6;

		nodo1=malloc(tamanio23);
		nodo2=malloc(tamanio23);
		nodo3=malloc(tamanio23);
		nodo4=malloc(tamanio23);
		nodo5=malloc(tamanio23);
		nodo6=malloc(tamanio23);


		nodo1->id_nodo = 4;
		nodo1->ip_nodo = "44.251";
		nodo1->puerto_nodo = "887";
		nodo2->id_nodo = 2;
		nodo2->ip_nodo = "44.250";
		nodo2->puerto_nodo = "886";
		nodo3->id_nodo = 7;
		nodo3->ip_nodo = "44.252";
		nodo3->puerto_nodo = "885";
		nodo4->id_nodo = 54;
		nodo4->ip_nodo = "44.253";
		nodo4->puerto_nodo = "884";
		nodo5->id_nodo = 23;
		nodo5->ip_nodo = "44.251";
		nodo5->puerto_nodo = "883";
		nodo6->id_nodo = 44;
		nodo6->ip_nodo = "44.254";
		nodo6->puerto_nodo = "882";

		list_add(lista_nodos_estado,nodo1);
		list_add(lista_nodos_estado,nodo2);
		list_add(lista_nodos_estado,nodo3);
		list_add(lista_nodos_estado,nodo4);
		list_add(lista_nodos_estado,nodo5);
		list_add(lista_nodos_estado,nodo6);


		  int a,b,c,d,e,aux,aux3,variableParaResultadoReduce,tamanioListaJobTabla,tamanioListaTablaDeProcesosPorJob,tamanioListaArchivosAReducirPorNodo;
				int tamanioListaDeNodos,auxla=0,tamanio;
			    t_lista_job2 *campoDeLaLista;
			    campoDeLaLista = malloc(tamanio23*10);
				t_tablaProcesos_porJob2 *campoDeLaListaTablaDeProcesos;
		        campoDeLaListaTablaDeProcesos = malloc(tamanio23*10);
			    t_list * lista_archivosAReducirPorNodo;
			    t_archivosAReducirPorNodo *campoArchivosAReducirPorNodo;
		        campoArchivosAReducirPorNodo= malloc(tamanio23*10);
			    t_archivosAReducirPorNodo *campoAAgregarAListaReducirPorNodo;
		    campoAAgregarAListaReducirPorNodo = malloc(tamanio23*10);
			    t_nodo2 *campoDeListaDeNodo;
			    campoDeListaDeNodo = malloc(tamanio23*10);
				lista_archivosAReducirPorNodo = list_create();
				list_clean(lista_archivosAReducirPorNodo);

				// lo que buscamos es recorrer cada campo de la lista para ordenar por id de nodo la lista de archivos a
				//reducir
				tamanioListaJobTabla=list_size(lista_job_tabla);// es 3

				printf("\n\nLos campos de la lista que deberian estar son:\n");

				for(a=0;a<tamanioListaJobTabla;a++){
					campoDeLaLista=list_get(lista_job_tabla,a); // corrroborado que los saca bien
					tamanioListaTablaDeProcesosPorJob = list_size(campoDeLaLista->tabla_procesos); // es dos

					for(b=0;b<tamanioListaTablaDeProcesosPorJob;b++){
						campoDeLaListaTablaDeProcesos = list_get(campoDeLaLista->tabla_procesos,b);
						tamanioListaArchivosAReducirPorNodo = list_size(lista_archivosAReducirPorNodo);
				    	if(tamanioListaArchivosAReducirPorNodo == 0){
				    		campoAAgregarAListaReducirPorNodo = malloc(tamanio23);
				    		campoAAgregarAListaReducirPorNodo->idNodo =  campoDeLaListaTablaDeProcesos->id_nodo;
				    		campoAAgregarAListaReducirPorNodo->archivosAReducir = list_create();
				    		list_add(campoAAgregarAListaReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
				    		asprintf(&campoAAgregarAListaReducirPorNodo->nombreArchivoResultado,"%s%i","archivo",variableParaResultadoReduce);
				    		variableParaResultadoReduce ++;
				    		// buscamos el ip y el puerto nodo
				    		tamanioListaDeNodos =list_size(lista_nodos_estado);
				    		aux=0;
				    		for(d=0;d<tamanioListaDeNodos && aux==0;d++){
				    			campoDeListaDeNodo = list_get(lista_nodos_estado,d);
				    			if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
				    				campoAAgregarAListaReducirPorNodo->ipNodo =  campoDeListaDeNodo->ip_nodo;
				    				campoAAgregarAListaReducirPorNodo->puertoNodo =  campoDeListaDeNodo->puerto_nodo;
				    				aux=1;

				    			}
				    		}// agrega el campo a la lista

				    		list_add(lista_archivosAReducirPorNodo,campoAAgregarAListaReducirPorNodo);
				    		campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    		printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    		e++;
				    	}
				    	else{
				    	aux=0;
				    		for(c=0;c <tamanioListaArchivosAReducirPorNodo && aux ==0;c++){
				    			campoArchivosAReducirPorNodo = list_get(lista_archivosAReducirPorNodo,c);
				    			if(campoArchivosAReducirPorNodo->idNodo == campoDeLaListaTablaDeProcesos->id_nodo){
				    				list_add(campoArchivosAReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);
				    				aux =1;
				    			}
				    		}
				    			if(aux==0){//cuando no esta el id del nodo, entonces tiene que agregar todo directo
				    				printf("Aca deberia entrar 4 veces \n");
				    				campoAAgregarAListaReducirPorNodo = malloc(tamanio23);
				    				campoAAgregarAListaReducirPorNodo->idNodo =  campoDeLaListaTablaDeProcesos->id_nodo;
				    				campoAAgregarAListaReducirPorNodo->archivosAReducir = list_create();
				    				list_add(campoAAgregarAListaReducirPorNodo->archivosAReducir,campoDeLaListaTablaDeProcesos->nombre_archivo_resultado);

				    				asprintf(&campoAAgregarAListaReducirPorNodo->nombreArchivoResultado,"%s%i","archivo",variableParaResultadoReduce);
				    				variableParaResultadoReduce ++;
				    				// buscamos el ip y el puerto nodo
				    				tamanioListaDeNodos =list_size(lista_nodos_estado);
				    				aux=0;
				    				for(d=0;d<tamanioListaDeNodos && aux==0;d++){
				    					campoDeListaDeNodo = list_get(lista_nodos_estado,d);
				    					if(campoDeListaDeNodo->id_nodo==campoDeLaListaTablaDeProcesos->id_nodo){
				    						campoAAgregarAListaReducirPorNodo->ipNodo =  campoDeListaDeNodo->ip_nodo;
				    						campoAAgregarAListaReducirPorNodo->puertoNodo =  campoDeListaDeNodo->puerto_nodo;
				    						aux=1;

				    					}
				    				}// agrega el campo a la lista
				    				list_add(lista_archivosAReducirPorNodo,campoAAgregarAListaReducirPorNodo);
				    				campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
				    				printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
				    				e++;

				    			}
				    	}
					}
					}



           tamanio = list_size(lista_archivosAReducirPorNodo);
           printf("\n\nEl tamanio final de la lista es: %i\n",tamanio);
           printf("Lo que imprime a fuera del for es:\n");
           for(e=0;e<tamanio;e++){
        	   campoAAgregarAListaReducirPorNodo = list_get(lista_archivosAReducirPorNodo,e);
        	   printf("idnodo %i ipnodo %s puertonodo %s nombreDeArchivoResultado %s\n",campoAAgregarAListaReducirPorNodo->idNodo,campoAAgregarAListaReducirPorNodo->ipNodo,campoAAgregarAListaReducirPorNodo->puertoNodo,campoAAgregarAListaReducirPorNodo->nombreArchivoResultado);
        	   a=list_size(campoAAgregarAListaReducirPorNodo->archivosAReducir);
        	   printf("%i\n",a);
          	   for(b=0;b<a;b++){
        		   printf("Los campos de la lista de archivos a reducir son: %s \n",list_get(campoAAgregarAListaReducirPorNodo->archivosAReducir,b));
        	   }
        	   }

         free(campoDeLaLista);
         free(campoDeLaListaTablaDeProcesos);
         free(campoArchivosAReducirPorNodo);
         free(campoDeListaDeNodo);
return EXIT_SUCCESS;

}
*/
}






