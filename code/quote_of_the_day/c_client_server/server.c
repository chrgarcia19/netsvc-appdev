#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
// System includes
#include <sys/time.h>
#include <sys/socket.h>
// Networking includes
#include <arpa/inet.h>
#include <netinet/in.h>
// Macros
#define DEBUG_STATEMENTS	1
#define START_PORT_RANGE	4200
#define END_PORT_RANGE		4300
#define DEFAULT_PORT		START_PORT_RANGE
#define MAX_CONNECTIONS		30
// Personal headers
#include "networking.h"

int main(int argc, char** argv){
	if(argc > 4){
		fprintf(stderr, "[ERROR] Invalid number of arguments!\n");
		if(DEBUG_STATEMENTS)
			printf("Usage: %s <port> <ip address> <options>\n", argv[0]);
	}

	int opt = 1;	// opt is true
	int port = (argc > 1 ? atoi(argv[1]) : 0);
	char * ip_addr = (argc > 2 ? argv[2] : NULL);
	char * options = (argc > 3 ? argv[3] : NULL);

	int connect_socket, new_socket, client_sockets[MAX_CONNECTIONS];
	struct sockaddr_in server_addr;
	socklen_t addr_length;
	fd_set readfds;
	int activity, max_sd;

	bzero(&server_addr, sizeof(server_addr));
	for(int i = 0; i < MAX_CONNECTIONS; i++)
		client_sockets[i] = 0;

	srand(time(NULL));

	connect_socket = socket_init();
	setsockopt(connect_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt);
	port = socket_address_config(&server_addr, port, ip_addr);

	bind(connect_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	listen(connect_socket, 3);	//max of 3 pending connections

	printf("[LOG] Host is waiting for connections on port %d\n", port);
	addr_length = sizeof(server_addr);

	while(1){
		FD_ZERO(&readfds);

		FD_SET(connect_socket, &readfds);
		max_sd = connect_socket;

		for(int i = 0; i < MAX_CONNECTIONS; i++){
			if(client_sockets[i] > 0)
				FD_SET(client_sockets[i], &readfds);
			max_sd = (client_sockets[i] > max_sd ? client_sockets[i] : max_sd);
		}

		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(connect_socket, &readfds)){
			new_socket = accept(
				connect_socket, 
				(struct sockaddr *)&server_addr,
				addr_length
			);
			printf(
				"[LOG] New socket fd is %d, IP : %s, Port : %d \n", 
				new_socket, 
				inet_ntoa(server_addr.sin_addr), 
				ntohs(server_addr.sin_port)
			);
			for(int i = 0; i < MAX_CONNECTIONS; i++){
				if(!client_sockets[i]){
					client_sockets[i] = new_socket;
					if(DEBUG_STATEMENTS)
						printf(
							"[DEBUG] Adding to sock_list at position %d\n", 
							i
						);
					break;
				}
			}
		}
		
		for(int i = 0; i < MAX_CONNECTIONS; i++){
			if(FD_ISSET(client_sockets[i], &readfds)){
				int 
			}
		}
	}

	// It's closing time...

	for(int i = 0; i < MAX_CONNECTIONS; i++)
		close(client_sockets[i]);
	
	close(connect_socket);

	return EXIT_SUCCESS;
}
