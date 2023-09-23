#include "host_functions.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTNUM 4200

int main() {
  int client_socket, send_fib_num, send_fib_input, received_num, fib_input;
  struct sockaddr_in server_addr;

  fib_input = 1;

  client_socket = create_socket();

  socket_setup(&server_addr);

  connect_socket(&client_socket, server_addr);

  for (int i = 0; i < 10; i++) {
    // htonl converts from byte order to network byte order
    send_fib_input = htonl(fib_input);
    // sending the input of the fibonacci function to the server
    write(client_socket, &send_fib_input, sizeof(send_fib_input));

    // receiving the input of the fibonacci function from the server
    read(client_socket, &send_fib_input, sizeof(send_fib_input));
    fib_input = ntohl(send_fib_input);


    std::cout << "Server replied with: fibonacci(" << fib_input
              << ") = " << fibonacci(fib_input) << std::endl;

    fib_input += 1;
  }

  close(client_socket);

  return 0;
}