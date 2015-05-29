#include "Funciones.h"
#include <pthread.h>
#include <socket/socket.h>
#include "Marta.c"

struct job_nodo Job_Nodo;
char* handshake = "conexion establecida";
struct job_marta_resultado Job_Marta_Resultado;
struct nodo_job Nodo_Job;



char* rutaArchivoConfiguracion = "/home/utnso/git/tp-2015-1c-machistons/Configuracion/marta.conf";


t_log* logger; // Log Global



archivoConfiguracion = config_create(rutaArchivoConfiguracion);
mapper = config_get_string_value(archivoConfiguracion, "MAPPER");
reduce = config_get_string_value(archivoConfiguracion, "REDUCE");


