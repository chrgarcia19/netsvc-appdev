#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Networking includes
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
// Macros
#define EXIT_SUCCESS		0
#define EXIT_FAILURE		1
#define DEBUG_STATEMENTS	1
#define START_PORT_RANGE	4200
#define END_PORT_RANGE		4300
#define DEFAULT_PORT		START_PORT_RANGE
#define MAX_STR_SIZE		256
// Personal headers
#include "networking.h"

void error(char * msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
	if(DEBUG_STATEMENTS){
		printf("[DEBUG] Number of args: %d\n", argc);
	}
	
	if(argc < 4){
		printf("[ERROR] Invalid number of arguments!\n");
		printf("Usage: %s <port> <ip address> [OPTIONS]\n", argv[0]);
		return EXIT_FAILURE;
	}

	int port = (argc > 1 ? atoi(argv[1]) : 0);
	char * ip_addr = (argc > 2 ? argv[2] : NULL);
	char * options = (argc > 3 ? argv[3] : NULL);

	int client_socket, errval;
	struct sockaddr_in server_addr;
	char * msg = (char *)malloc(MAX_STR_SIZE);

	memset(&server_addr, 0, sizeof(server_addr));
	memset(msg, 0, MAX_STR_SIZE);

	srand(time(NULL));

	client_socket = socket_init();
	socket_config(&client_socket);
	port = address_config(&server_addr, port, ip_addr);

	errval = connect(
		client_socket, 
		(struct sockaddr *)&server_addr, 
		sizeof(server_addr)
	);

	if(errval < 0)
		error("[ERROR] Connecting to the server");

	if(strchr(options, 'q')){
		errval = send(client_socket, "q", sizeof(char), 0);

		if(errval < 0)
			error("[ERROR] Sending character q");

		errval = read(client_socket, msg, MAX_STR_SIZE);

		if(errval < 0)
			error("[ERROR] Reading quote");

		printf("Quote: %s\n", msg);
		memset(msg, 0, MAX_STR_SIZE);
	}
	if(strchr(options, 'a')){
		errval = send(client_socket, "a", sizeof(char), 0);

		if(errval < 0)
			error("[ERROR] Sending character a");

		errval = read(client_socket, msg, MAX_STR_SIZE);

		if(errval < 0)
			error("[ERROR] Reading author");

		printf("Author: %s\n", msg);
		memset(msg, 0, MAX_STR_SIZE);
	}
	if(strchr(options, 'd')){
		errval = send(client_socket, "d", sizeof(char), 0);
		
		if(errval < 0)
			error("[ERROR] Sending character d");

		errval = read(client_socket, msg, MAX_STR_SIZE);

		if(errval < 0)
			error("[ERROR] Reading date");

		printf("Date: %s\n", msg);
		memset(msg, 0, MAX_STR_SIZE);
	}
	if(strchr(options, 'e')){
		errval = send(client_socket, "e", sizeof(char), 0);

		if(errval < 0)
			error("[ERROR] Sending character e");
	}

	close(client_socket);

	return EXIT_SUCCESS;
}
