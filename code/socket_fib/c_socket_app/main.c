#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
// Networking includes
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
// Macros
#define DEBUG_STATEMENTS	0
#define START_PORT_RANGE	4200
#define END_PORT_RANGE		4300
#define DEFAULT_PORT		START_PORT_RANGE
#define MAX_MESSAGES		10
#define FIB_INDEX_MAX		30
#define EXIT_FAILURE		1
#define EXIT_SUCCESS		0
// Personal headers
#include "fibonacci.h"
#include "networking.h"
#include "err_checking.h"
// comparison functions to pass into func_err_check
int equal_to(int a, int b) { return a == b; }

static void * run(void * arg){
	int * socket = (int *) arg;
	int fib_index = (rand() % FIB_INDEX_MAX) + 1;

	printf("[LOG] Sending Fibonacci index: %d\n", fib_index);
	write_to_socket(socket, &fib_index, sizeof(fib_index));

	return 0;
}

int main(int argc, char** argv){
	if(argc > 4){
		printf("[ERROR] Invalid number of arguments!\n");
		if(DEBUG_STATEMENTS)
			printf("Usage: %s <host order> <port> <ip address>\n", argv[0]);
	}

	int hosting = (argc > 1 ? atoi(argv[1]) : 1);
	int port = (argc > 2 ? atoi(argv[2]) : 0);
	char * ip_addr = (argc > 3 ? argv[3] : NULL);

	int host_socket, client_socket;
	struct sockaddr_in host_addr, client_addr;
	socklen_t client_length;
	pthread_t thread;

	memset(&host_addr, 0, sizeof(host_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	srand(time(NULL));

	if(hosting){
		host_socket = socket_init();
		port = address_config(&host_addr, port, ip_addr);

		bind(host_socket, (struct sockaddr *)&host_addr, sizeof(host_addr));
		listen(host_socket, 1);

		printf("[LOG] Host is waiting for a connection on port %d\n", port);
		client_length = sizeof(client_addr);

		client_socket = accept(
			host_socket, 
			(struct sockaddr *)&client_addr, 
			&client_length
		);
	}
	else{
		client_socket = socket_init();

		port = address_config(&host_addr, port, ip_addr);

		connect(
			client_socket, 
			(struct sockaddr *)&host_addr, 
			sizeof(host_addr)
		);
	}

	bool reading_index = true;
	int read_int = 0;
	int fib_result = 0;
	for(int i = 0; i < MAX_MESSAGES; i++){
		printf("[LOG] Sending message %d of %d\n", i + 1, MAX_MESSAGES);
		if(hosting){
			pthread_create(&thread, NULL, &run, &client_socket);
			pthread_join(thread, NULL);
			reading_index = false;
		}
		read_from_socket(&client_socket, &read_int, sizeof(read_int));
		read_int = ntohl(read_int);
		if(reading_index){
			printf("[LOG] Receieved Fibonacci index: %d\n", read_int);
			fib_result = fibonacci(read_int);
			write_to_socket(&client_socket, &fib_result, sizeof(fib_result));
		}
		else{
			printf("[LOG] Receieved Fibonacci result: %d\n", read_int);
			reading_index = true;
		}
	}

	close(client_socket);
	close(host_socket);

	return EXIT_SUCCESS;
}
