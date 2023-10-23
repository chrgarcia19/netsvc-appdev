#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
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
#define MAX_MESSAGES		10
#define FIB_INDEX_MAX		30
// Personal headers
#include "networking.h"

int main(int argc, char** argv){
	if(argc > 4){
		fprintf(stderr, "[ERROR] Invalid number of arguments!\n");
		if(DEBUG_STATEMENTS)
			printf("Usage: %s <port> <ip address> <options>\n", argv[0]);
	}

	int port = (argc > 1 ? atoi(argv[1]) : 0);
	char * ip_addr = (argc > 2 ? argv[2] : NULL);
	char * options = (argc > 3 ? argv[3] : NULL);

	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_length;
	pthread_t thread;

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

	close(client_socket);
	close(server_socket);

	return EXIT_SUCCESS;
}
