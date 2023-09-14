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
  unsigned long fib_index_start = 5;
  int client_socket = 0;
  struct sockaddr_in server_addr;
  // creating socket
  func_err_check("Creating the client socket failed!",
                 (client_socket = socket(AF_INET, SOCK_STREAM, 0)), &less_than,
                 0);
  // zero out the server_addr struct
  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr =
      htonl(INADDR_ANY); // allows server to accept a client on any interface

  func_err_check(
      "Connecting to the server failed!",
      (err_code = connect(client_socket, (struct sockaddr *)&server_addr,
                          sizeof(server_addr))),
      &less_than, 0);

  // reading the first index to start at
  printf("[LOG] Sending starting fibonacci index %lu\n", fib_index_start);
  fib_index_start = htonl(fib_index_start);
  send(client_socket, &fib_index_start, sizeof(fib_index_start), 0);

  unsigned long send_index, send_result;
  unsigned long receive_index, receive_result;

  for (int i = 0; i < 10; i++) {
    // receive result of next fibonacci and the index used
    read(client_socket, &receive_result, sizeof(receive_result));
    receive_result = ntohl(receive_result);
    read(client_socket, &receive_index, sizeof(receive_index));
    receive_index = ntohl(receive_index);
    printf("[LOG] Received fibonacci(%lu) = %lu\n", receive_index,
           receive_result);
    // receive next index
    read(client_socket, &receive_index, sizeof(receive_index));
    receive_index = ntohl(receive_index);
    printf("[LOG] Received %lu as next fibonacci index to compute\n",
           receive_index);

    // send result of fibonacci calculation and received index
    unsigned long fib_res = fibonacci(receive_index);

    send_result = htonl(fib_res);
    send_index = htonl(receive_index);
    printf("[LOG] Sending fibonacci(%lu) = %lu\n", receive_index, fib_res);
    send(client_socket, &send_result, sizeof(send_result), 0);
    send(client_socket, &send_index, sizeof(send_index), 0);
    // send next index to compute
    receive_index++;
    send_index = htonl(receive_index);
    printf("[LOG] Sending %lu as next fibonacci index\n", receive_index);
    send(client_socket, &send_index, sizeof(send_index), 0);
  }
  // close out my ports
  func_err_check("Closing the client socket failed!",
                 err_code = close(client_socket), &less_than, 0);

  return (EXIT_SUCCESS);
}
