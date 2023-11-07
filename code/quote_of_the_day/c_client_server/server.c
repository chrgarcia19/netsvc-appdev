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
#define EXIT_SUCCESS		0
#define EXIT_FAILURE		1
#define DEBUG_STATEMENTS	1
#define START_PORT_RANGE	4200
#define END_PORT_RANGE		4300
#define DEFAULT_PORT		START_PORT_RANGE
#define MAX_CONNECTIONS		30
// 60 sec/min * 60 min/hr * 24 hr/day
#define SECS_IN_DAY			60 * 60 * 24
// Personal headers
#include "networking.h"
#include "csv.h"

typedef enum {
	AUTHOR,
	QUOTE,
	DATE,
	NUM_COLS
} CSV_STRUCTURE;

void error(char * msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

char * format_author(char * author){
	return strtok(author, ",");
}

char * format_date(char * date){
	return strtok(date, ",");
}

int main(int argc, char** argv){
	if(argc > 3){
		printf("[ERROR] Invalid number of arguments!\n");
		if(DEBUG_STATEMENTS)
			printf("Usage: %s <port> <ip address>\n", argv[0]);
		
		return EXIT_FAILURE;
	}

	int port = (argc > 1 ? atoi(argv[1]) : 0);
	char * ip_addr = (argc > 2 ? argv[2] : NULL);

	int close_server = 0;

	int server_socket, new_client_socket, errval;
	struct sockaddr_in server_addr, client_addr, new_addr;
	socklen_t addr_len, new_addr_len;
	pid_t client_pid;

	char incoming_char = 0;
	char * msg = (char *)malloc(MAX_STR_SIZE);

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));
	memset(&new_addr, 0, sizeof(new_addr));
	memset(&msg, 0, sizeof(MAX_STR_SIZE));

	srand(time(NULL));

	csv_t quotes_csv;
	csv_init(&quotes_csv, "new_quotes.csv", '"');

	int qotd = ((time(NULL) / SECS_IN_DAY) % quotes_csv.rows);

	server_socket = socket_init();
	socket_config(&server_socket);
	port = address_config(&server_addr, port, ip_addr);
	addr_len = sizeof(server_addr);
	new_addr_len = sizeof(new_addr);

	bind(server_socket, (struct sockaddr *)&server_addr, addr_len);
	errval = listen(server_socket, MAX_CONNECTIONS);

	if(errval < 0)
		error("[ERROR] Listening for incoming connections");

	printf("[LOG] Host is waiting for connections on port %d\n", port);
	
	while(!close_server){
		new_client_socket = accept(
			server_socket, 
			(struct sockaddr *)&new_addr, 
			&new_addr_len
		);
		
		if(new_client_socket < 0)
			error("[ERROR] Server failed to accept a client");
		
		printf(
			"[LOG] Connection accepted from %s on port %d\n",
			inet_ntoa(new_addr.sin_addr),
			ntohs(new_addr.sin_port)
		);

		client_pid = fork();

		if(client_pid < 0)
			error("[ERROR] Forking the process");

		if(client_pid == 0){
			close(server_socket);

			char ** strarr = (char **)malloc(MAX_STR_SIZE * quotes_csv.cols);
			while(incoming_char != 'e'){
				strarr = read_row_from_csv(&quotes_csv, qotd);
				errval = read_from_socket(
					&new_client_socket,
					&incoming_char,
					sizeof(char)
				);

				if(errval < 0)
					error("[ERROR] Read from client");

				switch(incoming_char){
					case 'q':{
						errval = write_to_socket(
							&new_client_socket, 
							strarr[QUOTE], 
							MAX_STR_SIZE
						);

						if(errval < 0)
							error("[ERROR] Writing quote to client");
						
						printf(
							"[LOG] Sending quote \"%s\" to client...\n",
							strarr[QUOTE]
						);
						break;
					}
					case 'a':{
						errval = write_to_socket(
							&new_client_socket, 
							format_author(strarr[AUTHOR]), 
							MAX_STR_SIZE
						);

						if(errval < 0)
							error("[ERROR] Writing author to client");
						
						printf(
							"[LOG] Sending author %s to client...\n",
							format_author(strarr[AUTHOR])
						);
						break;
					}
					case 'd':{
						errval = write_to_socket(
							&new_client_socket, 
							format_date(strarr[DATE]),
							MAX_STR_SIZE
						);

						if(errval < 0)
							error("[ERROR] Writing date to client");

						printf(
							"[LOG] Sending date %s to client...\n",
							format_date(strarr[DATE])
						);
						break;
					}
					case 'e':{
						printf("[LOG] Ending connection...\n");
						close(new_client_socket);
						break;
					}
					case 0:
					case '\n':
						break;
					default:{
						printf(
							"[ERROR] Invalid character received: %c (%d)\n",
							incoming_char,
							incoming_char
						);
						return EXIT_FAILURE;
					}
				}
			}
		}
	}
	
	close(server_socket);

	return EXIT_SUCCESS;
}
