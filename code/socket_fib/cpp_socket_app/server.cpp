#include "fibonacci.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTNUM 4204

int main() {
  int socket_s, socket_c, fib_num, send_fib_num, send_num, received_num;
  sockaddr_in server_addr, client_addr;
  socklen_t client_len;

  fib_num = 5;
  send_num, send_fib_num, received_num = 0;

  socket_s = socket(AF_INET, SOCK_STREAM, 0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORTNUM);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  bind(socket_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
  listen(socket_s, 5);

  std::cout << "Server is listening on port " << PORTNUM << std::endl;

  client_len = sizeof(client_addr);
  socket_c = accept(socket_s, (struct sockaddr *)&client_addr, &client_len);

  //sending the input of the fibonacci function to the client
  send_fib_num = htonl(fib_num);
  write(socket_c, &send_fib_num, sizeof(send_fib_num));

  //sending the result of the fibonacci function to the client
  send_num = htonl(fibonacci(fib_num));
  write(socket_c, &send_num, sizeof(send_num));

  //receiving the result of the fibonacci function from the client
  read(socket_c, &received_num, sizeof(received_num));
  received_num = ntohl(received_num);

  std::cout << "Client replied with fibonacci result: " << received_num
            << std::endl;

  close(socket_c);
  close(socket_s);

  return 0;
}