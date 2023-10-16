#include <netinet/in.h>
#include <sys/socket.h>

#ifndef NETWORKING_H
#define NETWORKING_H

int socket_init(void);
int socket_address_config(struct sockaddr_in * const socket_addr, const int PORT, const char * const IP);
void read_from_socket(int * const socket, void * data, size_t data_size);
void write_to_socket (int * const socket, void * data, size_t data_size);
#endif

#ifndef NETWORKING_SRC
#define NETWORKING_SRC

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 4200
#endif

int socket_init(void){
	return socket(AF_INET, SOCK_STREAM, 0);
}

int socket_address_config(struct sockaddr_in * const socket_addr, const int PORT, const char * const IP){
	if(!socket_addr)
		return -1;	//return if socket_addr is NULL
	bzero(socket_addr, sizeof(*socket_addr));
	
	int using_port = (PORT ? PORT : DEFAULT_PORT);	//if port is non-zero, use port
	socket_addr->sin_port = htons(using_port);
	socket_addr->sin_family = AF_INET;
	socket_addr->sin_addr.s_addr = (IP ? inet_addr(IP) : htonl(INADDR_ANY));

	return using_port;	//returns the port number used
}

void read_from_socket(int * const socket, void * data, size_t data_size){
	if(data_size <= 4){
		void * const read_data = malloc(data_size);
		int* converted_data = malloc(sizeof(int));

		read(*socket, read_data, data_size);
		*converted_data = ntohl(*(uint32_t *)read_data);

		data = converted_data;
	}
	else{
		printf("[ERROR] data_size passed to read_from_socket was larger than an int!\n");
	}
}

void write_to_socket (int * const socket, void * data, size_t data_size){
	if(data_size > 4)
		return;	//currently this is unsupported
	int send_data = htonl(*(uint32_t *)data);
	write(*socket, &send_data, data_size);
}

#endif
