#include "host_functions.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int server_socket, client_socket, fib_input, send_fib_num, send_fib_input,
      received_num;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_len;
  int port;

  server_socket = create_socket();

  port = socket_setup(&server_addr);

  bind_socket(&server_socket, server_addr);
  listen_to_socket(&server_socket, 5);

  std::cout << "Server is listening on port " << port << std::endl;

  
  client_len = sizeof(client_addr);
  client_socket =
      accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

  for (int i = 0; i < 10; i++) {
    // receiving the input of the fibonacci function from the client
    read(client_socket, &send_fib_input, sizeof(send_fib_input));
    fib_input = ntohl(send_fib_input);

    std::cout << "Client replied with: fibonacci(" << fib_input
              << ") = " << fibonacci(fib_input) << std::endl;

    fib_input += 1;
    // sending the input of the fibonacci function to the client
    send_fib_input = htonl(fib_input);
    write(client_socket, &send_fib_input, sizeof(send_fib_input));
  }
  
  close(client_socket);
  close(server_socket);

  return 0;

}

