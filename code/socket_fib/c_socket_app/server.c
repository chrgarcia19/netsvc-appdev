#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DEBUG_STATEMENTS 0
#define START_PORT_RANGE 4200
#define END_PORT_RANGE	 4300
// comparison functions to pass into func_err_check
int equal_to    (int a, int b){ return a == b; }
int less_than   (int a, int b){ return a < b;  }
int not_equal_to(int a, int b){ return a != b; }
// takes in error message to print if failed and received and expected return values as well as comparison to perform on the two
void func_err_check(const char* ERR_MESSAGE, int ret_val, int (*compare)(int, int), int expected_val){
	if(compare(ret_val, expected_val)){
		fprintf(stderr, "[ERROR] %s\n", ERR_MESSAGE);
		if(DEBUG_STATEMENTS) printf("[DEBUG] Error code: %d\n", ret_val);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv){
	int port, err_codes, connected;
	// reading in cl args
	if(argc == 1){
		port = START_PORT_RANGE;
	}
	else if(argc == 2){
		port = atoi(argv[1]);
		if(port < START_PORT_RANGE || port > END_PORT_RANGE){
			fprintf(stderr, "[ERROR] Port provided outside of the range %d - %d.\n", START_PORT_RANGE, END_PORT_RANGE);
			if(DEBUG_STATEMENTS) printf("[DEBUG] Port number provided: %d\n", port);
			exit(EXIT_FAILURE);
		}
	}
	else{
		fprintf(stderr, "[ERROR] Number of arguments is invalid!\n");
		if(DEBUG_STATEMENTS) printf("[DEBUG] Number of args passed: %d\n", argc);
		fprintf(stderr, "\nUsage: %s <port number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// performing socket magic
	int sock_listen;
	struct sockaddr_in my_addr;
	func_err_check(
		"Creating the socket failed!", 
		(sock_listen = socket(AF_INET, SOCK_STREAM, 0)),
		&less_than,
		0
	);

	bzero(&my_addr, sizeof(my_addr));

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//allows server to accept a client on any interface

	func_err_check(
		"Binding the socket failed!",
		err_codes = bind(
			sock_listen, 
			(struct sockaddr *) &my_addr, 
			sizeof(my_addr)),
		&less_than,
		0
	);
	//waits for the client to connect
	connected = 0;
	err_codes = (sock_listen, 1);

	

	return(EXIT_SUCCESS);
}
