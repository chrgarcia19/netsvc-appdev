#include "host_functions.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int port;
  char *ip;
  if (argc == 1){
    ip = NULL;
    port = 0;
  }else if (argc == 3){
    ip = argv[1];
    port = atoi(argv[2]);
  }
  int client_socket, fib_input;
  struct sockaddr_in server_addr;

  srand((unsigned) time(NULL));

  fib_input = 1;

  client_socket = create_socket();

  socket_setup(&server_addr, ip, port);

  connect_socket(&client_socket, server_addr);

  for (int i = 0; i < 10; i++) {
    // sending the input of the fibonacci function to the server
    write_data(&client_socket, fib_input);

    // receiving the input of the fibonacci function from the server
    fib_input = read_data(&client_socket, fib_input);

    std::cout << "Server replied with: fibonacci(" << fib_input
              << ") = " << fibonacci(fib_input) << std::endl;

    fib_input = rand() % 40;
  }

  close_one_socket(&client_socket);

  return 0;
}