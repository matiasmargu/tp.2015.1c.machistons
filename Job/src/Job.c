/*
 ============================================================================
 Name        : Job.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "funciones.h"

//FALTA PONER EN LOG Cabeceras de mensajes enviados y recibidos de cada hilo/proceso

int main(void) {

	char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/job.conf";

	t_config* archivoConfiguracion;


	logger = log_create("LOG_JOB", "log_job" ,false, LOG_LEVEL_INFO);

	char* puerto_marta;
	char* ip_marta;
	char* mapper;
	char* reduce;
	char* combiner;
	char** lista_archivos;
	char* archivo_resultado;
	int i;
	int c;
	int tamanioTotal;
	int numero;
	int saludo;
	int handshakeMarta;
	t_charpuntero structCombiner;

	t_charpuntero nombre;



    t_marta_job2 Marta_Job;
    t_job_nodo Job_Nodo;





	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
	ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
	mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
	reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
	combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
	lista_archivos = config_get_array_value(archivoConfiguracion, "ARCHIVOS");
	archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");

int socketMarta = crearCliente (ip_marta, puerto_marta);

handshakeMarta = 9;

send(socketMarta,&handshakeMarta,sizeof(int),0);

recv(socketMarta, &saludo, sizeof(int),0);


log_info(logger,"Conexion establecida con proceso Marta");
printf("Conexion establecida con proceso Marta: %i\n",saludo);

char* l;
int cantidad = 0;
int s = 0;
l = lista_archivos[s];

//ACA CALCULAMOS EL TAMANIO DE LA LISTA DE ARCHIVOS QUE ESTA EN LA CONFIGURACION
while(l != NULL){
	cantidad = cantidad + 1;     //cantidad = TAMANIO LISTA DE ARCHIVOS
	s = s+1;
	l = lista_archivos[s];
}
send(socketMarta,&cantidad,sizeof(int),0);

char* stringArchivos;




int a;
char *archivo;
char* archivoAEnviar;
char* combinerAEnviar;
int tamanioCombiner;



//ACA LE MANDAMOS A MARTA CADA ARCHIVO DE LA LISTA QUE ESTA EN LA CONFIGURACION
for(a = 0 ; a < cantidad; a++){
	nombre.archivo = lista_archivos[a];
	printf("el archivo es %s\n",nombre.archivo);
	tamanioTotal = sizeof(int)+ strlen(nombre.archivo)+1;
	send(socketMarta, &tamanioTotal, sizeof(int),0);
	archivoAEnviar = serializar_charpuntero(&nombre, tamanioTotal);
	send(socketMarta,archivoAEnviar,tamanioTotal,0);

}


//MANDAMOS A MARTA SI TIENE O NO COMBINER
tamanioCombiner = sizeof(int) + strlen(combiner)+1;
send(socketMarta, &tamanioCombiner, sizeof(int),0);
structCombiner.archivo = combiner;
combinerAEnviar =  serializar_charpuntero(&structCombiner, tamanioCombiner);
send(socketMarta,combinerAEnviar,tamanioCombiner,0);


// Aca hago como si le mandase a job una matriz

int columnas = 3;
int filas = 3;

send(socketMarta,&filas,sizeof(int),0);
send(socketMarta,&columnas,sizeof(int),0);

char* matriz[filas][columnas];

int cont, cont1, cont2, cont3;

for(cont=0; cont<filas ;cont++ ){
	for(cont1=0; cont1 < columnas; cont1++){
		matriz[cont][cont1] = "juan";
	}
}
for(cont2= 0 ; cont2< filas; cont2++){
	for(cont3=0;cont3< columnas; cont3++){
	nombre.archivo = matriz[cont2][cont3];
	printf("el archivo es %s\n",nombre.archivo);
	tamanioTotal = sizeof(int)+ strlen(nombre.archivo)+1;
	send(socketMarta, &tamanioTotal, sizeof(int),0);
	archivoAEnviar = serializar_charpuntero(&nombre, tamanioTotal);
	send(socketMarta,archivoAEnviar,tamanioTotal,0);
	}
}

// a partir de aca haria como si formase un vector de estructuras que tengan el nodo y el contador
// que la funcion de ese contador va a ser mostrar cuantos hilos mapper se mandaron a ese nodo para
// asi ver a cual nodo mandarle el hilo mapper a realizar

typedef struct{
	char* nodo;
	int contador_mapper;
} nodo_y_contador;

int cantidad_de_nodos = 3*3; // aca pongo la cantidad de nodos, que me la tendria que mandar alguien o la tendria que sacar de algun lado
                           // pero ahora le pongo 3*3 que es la mtriz que armamos.

nodo_y_contador v[cantidad_de_nodos]; // vector de estructuras

int aux = 0; // variable auxiliar que me va a decir si ya se cargo o no
int posicion_del_vector_de_nodos = 0;


for(cont1=0;cont1 < filas; cont1 ++){
	for(cont2=0; cont2 < columnas; cont2 ++){ // con estos for recorro la matriz

		for(cont3=0; cont3 < cantidad_de_nodos && aux == 0; cont3 ++){ // con este for recorro el vector de nodos y me fijo si el nodo ya se cargo

			if(matriz[filas][columnas] == v[cont3].nodo ){
					aux = 1; // si algun nodo coincide es xq ya fue cargado
			}
		}
		if(aux == 0){ // significa que el nodo no fue cargado al vector de estructuras

		v[posicion_del_vector_de_nodos].nodo = matriz[filas][columnas];
		v[posicion_del_vector_de_nodos].contador_mapper = 0;

		posicion_del_vector_de_nodos ++; // aumentamos la posicion del vector de nodos

		}
	}
}






/*




recv(socketMarta, &tamanioTotal, sizeof(int),0);

int status = 1; // Estructura que manjea el status de los recieve.

status = recive_y_deserialisa(&Marta_Job, socketMarta, tamanioTotal);


while(status){


int listaDeBloques[Marta_Job.cantidadDeBloques];

	for(c= 0; c<= Marta_Job.cantidadDeBloques; c++ ){

	recv(socketMarta,&numero, sizeof(int),0);

	listaDeBloques[c] = numero;

											}

int socketNodo = crearCliente (Marta_Job.ip_nodo, Marta_Job.puerto);

if(Marta_Job.rutina == 1){
Job_Nodo.tipoRutina = 1;
Job_Nodo.rutinaEjecutable = mapper;

char* mensajeMapper = serializarMapper(Job_Nodo);

	send(socketNodo,mensajeMapper,strlen(mensajeMapper)+1,0);
	liberarMensaje(mensajeMapper);

	}else{
		char* mensajeReduce = serializarReducer(&reduce);
		Job_Nodo.tipoRutina = 1;
		Job_Nodo.rutinaEjecutable = mapper;

			send(socketNodo,mensajeReduce,strlen(mensajeReduce)+1,0);
			liberarMensaje(mensajeReduce);
	}

	for(i = 0; i<= Marta_Job.cantidadDeBloques; i++){

            int numeroDeBloque = listaDeBloques[i] ;

            pthread_t (hiloNodo_i);

            t_conectarseAlNodo CAN;
            CAN.Marta_Job = Marta_Job;
            CAN.numeroDeBloque = numeroDeBloque;
            CAN.socketMarta = socketMarta;
            CAN.Job_Nodo = Job_Nodo;

            if(Marta_Job.rutina == 1){
            	Job_Nodo.rutinaEjecutable = mapper;
            	Job_Nodo.tipoRutina = 1;
            }else{Job_Nodo.rutinaEjecutable = reduce;
        	Job_Nodo.tipoRutina = 2;}

			pthread_create(&hiloNodo_i, NULL, (void*) conectarseAlNodo, &CAN);
			if(Marta_Job.rutina == 1){
			log_info(logger,"Se creo un hilo mapper  "); //AGREGAR PARAMETROS RECIBIDOS
			printf("Se creo un hilo mapper");
			}
			else{log_info(logger,"Se creo un hilo reducer "); //AGREGAR PARAMETROS RECIBIDOS
			printf("Se creo un hilo reducer");
			}



                                                 }


}




*/

	close(socketMarta);

	log_destroy(logger);
	free(mapper);
	free(lista_archivos);
	free(archivo_resultado);
	free(reduce);
	free(ip_marta);
	free(combiner);

	return EXIT_SUCCESS;
}


