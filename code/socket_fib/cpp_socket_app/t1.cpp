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
  int server_socket, client_socket, fib_input;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_len;
  
  srand((unsigned) time(NULL));

  server_socket = create_socket();

  port = socket_setup(&server_addr, ip, port);

  bind_socket(&server_socket, server_addr);
  listen_to_socket(&server_socket, 5);

  std::cout << "Server is listening on port " << port << std::endl;

  
  client_len = sizeof(client_addr);
  client_socket =
     accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
  //accept_socket(&server_socket, &client_addr, client_len);

  for (int i = 0; i < 10; i++) {
    // receiving the input of the fibonacci function from the client
    fib_input = read_data(&client_socket, fib_input);

    std::cout << "Client replied with: fibonacci(" << fib_input
              << ") = " << fibonacci(fib_input) << std::endl;

    fib_input = rand() % 40;
    // sending the input of the fibonacci function to the client
    write_data(&client_socket, fib_input);
  }
  
  close_two_sockets(&client_socket, &server_socket);

  return 0;

}

