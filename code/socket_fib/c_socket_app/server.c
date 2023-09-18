#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
// Networking includes
#include <netinet/in.h>
#include <sys/socket.h>
// Macros
#define DEBUG_STATEMENTS 1
#define START_PORT_RANGE 4200
#define END_PORT_RANGE 4300
// Personal headers
#include "err_checking.h"
#include "fibonacci.h"
// comparison functions to pass into func_err_check
int equal_to(int a, int b) { return a == b; }
int less_than(int a, int b) { return a < b; }
int not_equal_to(int a, int b) { return a != b; }

int main(int argc, char **argv) {
  int port;
  // reading in command line args
  if (argc == 1) {
    port = START_PORT_RANGE;
  } else if (argc == 2) {
    port = atoi(argv[1]);
    if (port < START_PORT_RANGE || port > END_PORT_RANGE) {
      fprintf(stderr, "[ERROR] Port provided outside of the range %d - %d.\n",
              START_PORT_RANGE, END_PORT_RANGE);
      if (DEBUG_STATEMENTS)
        printf("[DEBUG] Port number provided: %d\n", port);
      exit(EXIT_FAILURE);
    }
  } else {
    fprintf(stderr, "[ERROR] Number of arguments is invalid!\n");
    if (DEBUG_STATEMENTS)
      printf("[DEBUG] Number of args passed: %d\n", argc);
    fprintf(stderr, "\nUsage: %s <port number>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  // declare rest of variables now that we know the program didn't die
  int err_code;
  unsigned long fib_index;
  int server_socket = 0, client_socket = 0;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  // creating socket
  func_err_check("Creating the server socket failed!",
                 (server_socket = socket(AF_INET, SOCK_STREAM, 0)), &less_than,
                 0);
  // zero out the server_addr struct
  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr =
      htonl(INADDR_ANY); // allows server to accept a client on any interface
  // binding the socket
  func_err_check("Binding the socket failed!",
                err_code = bind(
					server_socket, 
					(struct sockaddr *)&server_addr, 
					sizeof(server_addr)),
                &less_than, 0);
  // listening on the socket for a connection; blocking I think
  func_err_check(
      "Listening on the socket failed!",
      listen(server_socket, 1), // listen for only a single connection
      &less_than, 0
	);
  if (DEBUG_STATEMENTS)
    printf("Server listening on port %d\n", port);
  // accept incoming connection(s)
  func_err_check("Accepting the socket failed!",
                 client_socket =
                     accept(server_socket, (struct sockaddr *)&client_addr,
                            &client_addr_len),
                 &less_than, 0);
  // reading the first index to start at
  read(client_socket, &fib_index, sizeof(fib_index));
  fib_index = ntohl(fib_index);
  printf("[LOG] Starting at fibonacci index %lu\n", fib_index);

  unsigned long send_result, receive_result;

  for (int i = 0; i < 10; i++) {
   // send result of fibonacci calculation and received index
    unsigned long fib_res = fibonacci(fib_index);

    send_result = htonl(fib_res);
    printf("[LOG] Sending fibonacci(%lu) = %lu\n", fib_index, fib_res);
    send(client_socket, &send_result, sizeof(send_result), 0);
    // increment fibonacci index to keep up with server
    fib_index++;
    // receive result of next fibonacci and the index used
    read(client_socket, &receive_result, sizeof(receive_result));
    receive_result = ntohl(receive_result);
    printf("[LOG] Received fibonacci(%lu) = %lu\n", fib_index, receive_result);
	// increment fibonacci index to perform calculation
	fib_index++;
  }
  // close out my ports
  func_err_check("Closing the server socket failed!",
                 err_code = close(server_socket), &less_than, 0);

  func_err_check("Closing the client socket failed!",
                 err_code = close(client_socket), &less_than, 0);

  return (EXIT_SUCCESS);
}
