#include "fibonacci.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTNUM 4204

int main() {
  int socket_c, socket_s, send_num, received_num, fib_num, send_fib_num;
  sockaddr_in server_addr;

  send_num, send_fib_num, received_num = 0;

  socket_c = socket(AF_INET, SOCK_STREAM, 0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORTNUM);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  connect(socket_c, (struct sockaddr *)&server_addr, sizeof(server_addr));

  for (int i = 0; i < 10; i++){
    //receiving the input of the fibonacci function from the server
    read(socket_c, &fib_num, sizeof(fib_num));
    fib_num = ntohl(fib_num);
    fib_num += 1;
    //receiving the result of the fibonacci function from the server
    read(socket_c, &received_num, sizeof(received_num));
    received_num = ntohl(received_num);

    std::cout << "Server replied with fibonacci result: " << received_num
              << std::endl;

    //send the inout of the fibonacci function to the server
    send_fib_num = htonl(fib_num);
    write(socket_c, &send_fib_num, sizeof(send_fib_num));
    
    //sending the result of the fibonacci function to the server
    send_num = htonl(fibonacci(fib_num));
    write(socket_c, &send_num, sizeof(send_num));
  }

  close(socket_c);

  return 0;
}