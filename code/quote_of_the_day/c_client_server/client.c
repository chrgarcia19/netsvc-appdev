#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
// Networking includes
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
// Macros
#define DEBUG_STATEMENTS	1
#define START_PORT_RANGE	4200
#define END_PORT_RANGE		4300
#define DEFAULT_PORT		START_PORT_RANGE
#define MAX_STR_SIZE		256
// Personal headers
#include "networking.h"

int main(int argc, char** argv){
	printf("%d\n", argc);
	if(argc > 4){
		fprintf(stderr, "[ERROR] Invalid number of arguments!\n");
		if(DEBUG_STATEMENTS)
			printf("Usage: %s <port> <ip address> [OPTIONS]\n", argv[0]);
	}

	int port = (argc > 1 ? atoi(argv[1]) : 0);
	char * ip_addr = (argc > 2 ? argv[2] : NULL);
	char * options = (argc > 3 ? argv[3] : NULL);

	int server_socket = 0, client_socket = 0;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_length;
	
	char * msg = calloc(MAX_STR_SIZE, sizeof(char));

	bzero(&server_addr, sizeof(server_addr));
	bzero(&client_addr, sizeof(client_addr));

	srand(time(NULL));

	client_socket = socket_init();

	socket_address_config(&server_addr, port, ip_addr);

	connect(
		client_socket, 
		(struct sockaddr *)&server_addr, 
		sizeof(server_addr)
	);

	if(strchr(options, 'q')){
		send(client_socket, "q", sizeof(char), 0);
		read(client_socket, msg, MAX_STR_SIZE);
		printf("Quote: %s\n", msg);
		memset(msg, 0, strlen(msg));
	}
	if(strchr(options, 'a')){
		send(client_socket, "a", sizeof(char), 0);
		read(client_socket, msg, MAX_STR_SIZE);
		printf("Author: %s\n", msg);
		memset(msg, 0, strlen(msg));
	}
	if(strchr(options, 'd')){
		send(client_socket, "d", sizeof(char), 0);
		read(client_socket, msg, MAX_STR_SIZE);
		printf("Date: %s\n", msg);
		memset(msg, 0, strlen(msg));
	}

	close(client_socket);
	close(server_socket);

	return EXIT_SUCCESS;
}