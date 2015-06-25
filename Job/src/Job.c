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
    t_marta_job Marta_Job;
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






recv(socketMarta, &tamanioTotal, sizeof(int),0);

int status = 1; // Estructura que manjea el status de los recieve.

status = recive_y_deserialisa_marta_job(&Marta_Job, socketMarta, tamanioTotal);


while(status){



int socketNodo = crearCliente (Marta_Job.ip_nodo, Marta_Job.puerto);
int handshakeNodo = 8;
send(socketNodo,&handshakeNodo,sizeof(int),0);
int numero = 3;
send(socketNodo,&numero,sizeof(int),0);


//ACA LE MANDA LA RUTINA MAPPER AL NODO
int numeroRutina = 1; // LE AVISA QUE MANDA UNA RUTINA MAP
send(socketNodo,&numeroRutina,sizeof(int),0);
t_charpuntero rutinaMapper;
char* rutinaAEnviarNodo;
rutinaMapper.archivo = mapper;
int tamanioTotalMapper = sizeof(int)+ strlen(rutinaMapper.archivo)+1;
send(socketNodo, &tamanioTotal, sizeof(int),0);
rutinaAEnviarNodo =  serializar_charpuntero(&rutinaMapper, tamanioTotalMapper);
send(socketNodo,rutinaAEnviarNodo,tamanioTotal,0);

//ACA LE MANDA LA RUTINA REDUCE AL NODO
numeroRutina = 2; //Le avisa que manda reduce
send(socketNodo,&numeroRutina,sizeof(int),0);
t_charpuntero rutinaReduce;
char* rutinaReduceAEnviar;
rutinaMapper.archivo = reduce;
int tamanioTotalReduce = sizeof(int)+ strlen(rutinaReduce.archivo)+1;
send(socketNodo, &tamanioTotalReduce, sizeof(int),0);
rutinaReduceAEnviar =  serializar_charpuntero(&rutinaReduce, tamanioTotalReduce);
send(socketNodo,rutinaReduceAEnviar,tamanioTotal,0);

/*
//DE ACA PARA ABAJO NO BORRAR HAY QUE HACER MODIFICACIONES

	for(i = 0; i<= Marta_Job.cantidadDeBloques; i++){

            int numeroDeBloque = listaDeBloques[i] ;
			asprintf(&resultado , "%s%i", "hiloNodo",i);
            pthread_t (resultado);

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
}


