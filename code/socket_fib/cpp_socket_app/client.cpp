#include "fibonacci.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTNUM 4204

int main() {
  int socket_c, socket_s, received_num, received_fib_num;
  sockaddr_in server_addr;

  socket_c = socket(AF_INET, SOCK_STREAM, 0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORTNUM);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  connect(socket_c, (struct sockaddr *)&server_addr, sizeof(server_addr));

  read(socket_c, &received_fib_num, sizeof(received_fib_num));

  read(socket_c, &received_num, sizeof(received_num));

  std::cout << "Server replied with fibonacci result: " << ntohl(received_num)
            << std::endl;

  int send_num = htonl(fibonacci(ntohl(received_fib_num) + 1));

  write(socket_c, &send_num, sizeof(send_num));
  close(socket_c);

  return 0;
}