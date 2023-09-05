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
void func_err_check(const char* ERR_MSG, int ret_val, int (*compare)(int, int), int expected_val){
	if(compare(ret_val, expected_val)){
		fprintf(stderr, "[ERROR] %s\n", ERR_MSG);
		if(DEBUG_STATEMENTS) printf("[DEBUG] Error code: %d\n", ret_val);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv){
	int port;
	// reading in command line args
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
	// declare rest of variables now that we know the program didn't die
	int err_code, is_connected;
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len;
	// creating socket
	func_err_check(
		"Creating the socket failed!", 
		(server_socket = socket(AF_INET, SOCK_STREAM, 0)),
		&less_than,
		0
	);
	// zero out the server_addr struct
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//allows server to accept a client on any interface
	// binding the socket
	func_err_check(
		"Binding the socket failed!",
		err_code = bind(
			server_socket, 
			(struct sockaddr *) &server_addr, 
			sizeof(server_addr)
		),
		&less_than,
		0
	);
	// listening on the socket for a connection
	func_err_check(
		"Listening on the socket failed!",
		listen(server_socket, 1),	// listen for only a single connection
		&less_than,
		0
	);

	func_err_check(
		"Accepting the socket failed!",
		err_code = accept(
			server_socket,
			(struct sockaddr*)&client_addr,
			&client_addr_len
		),
		&less_than,
		0
	);
	
	// Doing the fibonacci stuff here

	func_err_check(
		"Closing the server socket failed!",
		err_code = close(server_socket),
		&less_than,
		0
	);

	func_err_check(
		"Closing the client socket failed!",
		err_code = close(client_socket),
		&less_than,
		0
	);

	return(EXIT_SUCCESS);
}
