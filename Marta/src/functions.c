#include "functions.h"
#include <pthread.h>
#include <socket/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int recieve_and_deserialize(struct job_marta_inicio *package, int socketCliente){

			int status;
			int buffer_size;
			int buffer_size2;
			int buffer_size3;
			char *buffer = malloc(buffer_size = sizeof(char**));
			char *buffer2 = malloc(buffer_size2 = sizeof(char*));
			int *buffer3 = malloc(buffer_size3 = sizeof(int*));

			int operacion;

			status = recv(socketCliente, buffer3, sizeof(package->operacionID), 0);
			memcpy(&(operacion), buffer3, buffer_size3);
			if (!status) return 0;

			status = recv(socketCliente, package->operacionID, 6, 0);
				if (!status) return 0;

			char** lista_archivos;

			status = recv(socketCliente, buffer, sizeof(package->lista_archivos), 0);
			memcpy(&(lista_archivos), buffer, buffer_size);
			if (!status) return 0;

			status = recv(socketCliente, package->lista_archivos, lista_archivos, 0);
							if (!status) return 0;
			/*char *combiner;
			status = recv(socketCliente, buffer2, sizeof(package->combiner), 0);
			memcpy(&(combiner), buffer2, buffer_size2);
			if (!status) return 0;
*/



			free(buffer);
			free(buffer2);
			free(buffer3);

			return status;
		}




