#include "host_functions.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

void run(int socket) {
  int timer = (2 + rand() % 9) * 1000;
  int fib_input = rand() % 30;
  cout << "[SEND] Fibonacci index: " << fib_input << endl;
  write_data(&socket, fib_input);
  fib_input = read_data(&socket, fib_input);
  cout << "[RECEIVE] Fibonacci(" << fib_input << ") = " << fibonacci(fib_input)
       << endl;
  cout << "Resetting timer in: " << timer / 1000 << " seconds.\n" << endl;
}

int main(int argc, char *argv[]) {
  int port, host_order;
  char *ip;
  if (argc == 1) {
    ip = NULL;
    port = 0;
    host_order = 1;
  } else if (argc == 2) {
    ip = NULL;
    port = 0;
    host_order = atoi(argv[1]);
  } else if (argc == 3) {
    ip = argv[1];
    port = atoi(argv[2]);
    host_order = 1;
  } else if (argc == 4) {
    ip = argv[1];
    port = atoi(argv[2]);
    host_order = atoi(argv[3]);
  }

  int socket;
  struct sockaddr_in sock_addr;
  socklen_t sock_len;

  srand((unsigned)time(NULL));

  if (host_order == 1) {
    socket = create_socket();

    port = socket_setup(&sock_addr, ip, port);

    bind_socket(&socket, sock_addr);
    listen_to_socket(&socket, 1);

    cout << "Host 1 is waiting for a connection on port " << port << endl;
    cout << endl;

    sock_len = sizeof(sock_addr);
    socket = accept(socket, (struct sockaddr *)&sock_addr, &sock_len);

  } else if (host_order == 2) {
    socket = create_socket();

    socket_setup(&sock_addr, ip, port);

    connect_socket(&socket, sock_addr);
  }

  for (int i = 0; i < 10; i++) {
    cout << "Sending Message: " << i + 1 << "/10" << endl;
    thread t1(run, socket);
    t1.join();
  }
  
  cout << "The thread has ended!" << endl;

  close_two_sockets(&socket, &socket);

  cout << "The sockets have been closed!" << endl;

  return 0;
}
