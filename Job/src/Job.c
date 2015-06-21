/*
 ============================================================================
 Name        : Job.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "funcionesJob.h"

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
	char* lista_archivos;
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


//LEEMOS TODOS LOS ARCHIVOS DE LA CONFIGURACION
archivoConfiguracion = config_create(rutaArchivoConfiguracion);
puerto_marta = config_get_string_value(archivoConfiguracion, "PUERTO_MARTA");
ip_marta = config_get_string_value(archivoConfiguracion, "IP_MARTA");
mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
reduce = config_get_string_value(archivoConfiguracion, "REDUCE");
combiner = config_get_string_value(archivoConfiguracion, "COMBINER");
lista_archivos = config_get_string_value(archivoConfiguracion, "ARCHIVOS");
archivo_resultado = config_get_string_value(archivoConfiguracion, "RESULTADO");

//NOS CONECTAMOS CON MARTA
int socketMarta = crearCliente (ip_marta, puerto_marta);
handshakeMarta = 9;
send(socketMarta,&handshakeMarta,sizeof(int),0);
recv(socketMarta, &saludo, sizeof(int),0);
log_info(logger,"Conexion establecida con proceso Marta");
printf("Conexion establecida con proceso Marta:%i \n",saludo);

//ACA LE MANDAMOS A MARTA LA LISTA DE ARCHIVOS COMO UN CHAR*
char* archivoAEnviar;
nombre.archivo = lista_archivos;
tamanioTotal = sizeof(int)+ strlen(nombre.archivo)+1;
send(socketMarta, &tamanioTotal, sizeof(int),0);
archivoAEnviar =  serializar_charpuntero( &nombre, tamanioTotal);
send(socketMarta,archivoAEnviar,tamanioTotal,0);

//MANDAMOS A MARTA SI TIENE O NO COMBINER
char* combinerAEnviar;
int tamanioCombiner;
tamanioCombiner = sizeof(int) + strlen(combiner)+1;
send(socketMarta, &tamanioCombiner, sizeof(int),0);
structCombiner.archivo = combiner;
combinerAEnviar =  serializar_charpuntero(&structCombiner, tamanioCombiner);
send(socketMarta,combinerAEnviar,tamanioCombiner,0);


/*



//DE ACA PARA ABAJO NO BORRAR HAY QUE HACER MODIFICACIONES

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

//ACA LE MANDA LA RUTINA MAPPER AL NODO
Job_Nodo.tipoRutina = 1;
Job_Nodo.rutinaEjecutable = mapper;
tamanioTotal = sizeof(int) + sizeof(int) + strlen(mapper)+1;
send(socketNodo,&tamanioTotal,sizeof(int),0);
char* mensajeMapper = serializarRutina(Job_Nodo, tamanioTotal);
send(socketNodo,mensajeMapper,tamanioTotal,0);
//liberarMensaje(mensajeMapper);

//ACA LE MANDA LA RUTINA REDUCE AL NODO
Job_Nodo.tipoRutina = 2;
Job_Nodo.rutinaEjecutable = reduce;
tamanioTotal = sizeof(int) + sizeof(int) + strlen(reduce)+1;
send(socketNodo,&tamanioTotal,sizeof(int),0);
char* mensajeReduce = serializarRutina(Job_Nodo, tamanioTotal);
send(socketNodo,mensajeReduce,tamanioTotal,0);
//liberarMensaje(mensajeReduce);



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

			asprintf(&resultado , "%s%i", "hiloNodo",i);
			pthread_create(&resultado, NULL, (void*) conectarseAlNodo, &CAN);
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


