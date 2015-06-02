#include "functions.h"
#include <pthread.h>
#include <socket/socket.h>


int recieve_and_deserialize(struct t_Package *package, int socketCliente){

			int status;
			int buffer_size;
			char *buffer = malloc(buffer_size = sizeof(uint32_t));

			uint32_t username_long;
			status = recv(socketCliente, buffer, sizeof(package->username_long), 0);
			memcpy(&(username_long), buffer, buffer_size);
			if (!status) return 0;

			status = recv(socketCliente, package->username, username_long, 0);
			if (!status) return 0;

			uint32_t message_long;
			status = recv(socketCliente, buffer, sizeof(package->message_long), 0);
			memcpy(&(message_long), buffer, buffer_size);
			if (!status) return 0;

			status = recv(socketCliente, package->message, message_long, 0);
			if (!status) return 0;


			free(buffer);

			return status;
		}




