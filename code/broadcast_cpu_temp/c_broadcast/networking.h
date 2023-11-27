#include <netinet/in.h>
#include <sys/socket.h>

#ifndef NETWORKING_H
#define NETWORKING_H

int tcp_socket_init(void);
int udp_socket_init(void);
int address_config(struct sockaddr_in * const addr, const int PORT, const char * const IP);
int socket_config(int * socket);
int read_from_tcp_socket(int * const socket, void * data, size_t data_size);
int read_from_udp_socket(int * const socket, void * data, size_t data_size, struct sockaddr_in * address, socklen_t * addr_len);
int write_to_tcp_socket (int * const socket, void * data, size_t data_size);
int write_to_udp_socket (int * const socket, void * data, size_t data_size, struct sockaddr_in * address);
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

#define MAX_UDP_PACKET_SIZE 512

#ifndef MAX_MSG_SIZE
#define MAX_MSG_SIZE MAX_UDP_PACKET_SIZE
#endif

int tcp_socket_init(void){
	return socket(AF_INET, SOCK_STREAM, 0);
}

int udp_socket_init(void){
	return socket(AF_INET, SOCK_DGRAM, 0);
}

int address_config(struct sockaddr_in * const addr, const int PORT, const char * const IP){
	if(!addr)
		return -1;	//return if addr is NULL
	bzero(addr, sizeof(*addr));
	
	int using_port = (PORT ? PORT : DEFAULT_PORT);	//if port is non-zero, use port
	addr->sin_port = htons(using_port);
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = (IP ? inet_addr(IP) : htonl(INADDR_ANY));

	return using_port;	//returns the port number used
}

int socket_config(int * socket){
	int opt = 1;
	int err = setsockopt(
		*socket,
		SOL_SOCKET,
		SO_REUSEADDR | SO_REUSEPORT,
		&opt,
		sizeof(opt)
	);

	if(err)
		return -1;
	else
		return *socket;
}

int read_from_tcp_socket(int * const socket, void * data, size_t data_size){
	int errval;

	if(!data)
		data = malloc(data_size);
	if(data_size <= 4){	//treat as uint32 and convert as such
		void * const read_data = malloc(data_size);
		int * converted_data = malloc(sizeof(int));

		errval = recv(
			*socket,
			data,
			data_size,
			0
		);

		*converted_data = ntohl(*(uint32_t *)read_data);

		data = converted_data;
	}
	else{
		memset(data, 0, data_size);
		errval = read(*socket, data, data_size);
	}

	return errval;
}

int read_from_udp_socket(int * const socket, void * data, size_t data_size, struct sockaddr_in * address, socklen_t * addr_len){
	int errval;

	if(!data)
		data = malloc(data_size);
	memset(data, 0, data_size);
	errval = recvfrom(
		*socket, 
		data, 
		data_size,
		MSG_WAITALL,
		(struct sockaddr *)address,
		addr_len
	);

	return errval;
}

int write_to_tcp_socket (int * const socket, void * data, size_t data_size){
	int errval;

	if(data_size <= 4){ //treat as uint32 and convert as such
		int send_data = htonl(*(uint32_t *)data);
		errval = write(*socket, &send_data, data_size);
	}
	else{
		errval = write(*socket, data, data_size);
	}

	return errval;
}

int write_to_udp_socket (int * const socket, void * data, size_t data_size, struct sockaddr_in * address){
	int errval;

	errval = sendto(
		*socket,
		data,
		data_size,
		MSG_CONFIRM,
		(const struct sockaddr *)address,
		sizeof(*address)
	);

	return errval;
}

#endif
