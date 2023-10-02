#include "host_functions.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

static void *run(void *arg) {
  int *socket = (int *)arg;
  int timer = (2 + rand() % 9) * 1000;
  int fib_input = rand() % 30;
  cout << "[SEND] Fibonacci index: " << fib_input << endl;
  write_data(socket, fib_input);
  fib_input = read_data(socket, fib_input);
  cout << "[RECEIVE] Fibonacci(" << fib_input << ") = " << fibonacci(fib_input)
       << endl;
  cout << "Resetting timer in: " << timer / 1000 << " seconds.\n" << endl;
  return 0;
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

  int socket_h1, socket_h2;
  struct sockaddr_in h1_addr;
  struct sockaddr_in h2_addr;
  socklen_t h2_len;
  pthread_t thread;

  srand((unsigned)time(NULL));

  if (host_order == 1) {
    socket_h1 = create_socket();

    port = socket_setup(&h1_addr, ip, port);

    bind_socket(&socket_h1, h1_addr);
    listen_to_socket(&socket_h1, 1);

    cout << "Host 1 is waiting for a connection on port " << port << endl;
    cout << endl;

    h2_len = sizeof(h2_addr);
    socket_h2 = accept(socket_h1, (struct sockaddr *)&h2_addr, &h2_len);

  } else if (host_order == 2) {
    socket_h2 = create_socket();

    socket_setup(&h1_addr, ip, port);

    connect_socket(&socket_h2, h1_addr);
  }

  for (int i = 0; i < 10; i++) {
    cout << "Sending Message: " << i + 1 << "/10" << endl;
    pthread_create(&thread, NULL, &run, &socket_h2);
    pthread_join(thread, NULL);
  }
  cout << "The thread has ended!" << endl;

  close_two_sockets(&socket_h2, &socket_h1);

  cout << "The sockets have been closed!" << endl;

  return 0;
}
