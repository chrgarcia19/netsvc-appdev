#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
// System includes
#include <sys/time.h>
#include <sys/socket.h>
// Networking includes
#include <arpa/inet.h>
#include <netinet/in.h>
// Macros
#define EXIT_SUCCESS		0
#define EXIT_FAILURE		1
#define DEBUG_STATEMENTS	1
#define START_PORT_RANGE	4200
#define END_PORT_RANGE		4300
#define DEFAULT_PORT		START_PORT_RANGE
#define MAX_CONNECTIONS		30
// Personal headers
#include "networking.h"

int close_server = 0;

void error(char * msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

void nice_close(int sig){
	close_server = 1;
	signal(sig, SIG_IGN);
}

int main(int argc, char** argv){
	signal(SIGINT, nice_close);
	if(argc > 3){
		printf("[ERROR] Invalid number of arguments!\n");
		if(DEBUG_STATEMENTS)
			printf("Usage: %s <port> <ip address>\n", argv[0]);
		
		return EXIT_FAILURE;
	}

	int port = (argc > 1 ? atoi(argv[1]) : 0);
	char * ip_addr = (argc > 2 ? argv[2] : NULL);

	int server_socket, new_client_socket, errval;
	struct sockaddr_in server_addr, client_addr;
	socklen_t server_addr_len, client_addr_len;
	pid_t client_pid;

	char incoming_char = 0;
	char * msg = (char *)malloc(MAX_MSG_SIZE);

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));
	memset(&msg, 0, sizeof(MAX_MSG_SIZE));

	srand(time(NULL));

	server_socket = udp_socket_init();
	socket_config(&server_socket);
	port = address_config(&server_addr, port, ip_addr);

	server_addr_len = sizeof(server_addr);
	client_addr_len = sizeof(client_addr);

	errval = bind(
		server_socket, 
		(struct sockaddr *)&server_addr, 
		server_addr_len
	);

	if(errval < 0)
		error("[ERROR] Binding the socket and server");

	printf("[LOG] Host is waiting for packets on port %d\n", port);
	
	while(!close_server){
		
	}
	
	close(server_socket);

	return EXIT_SUCCESS;
}
