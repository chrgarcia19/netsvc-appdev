#include "host_functions.h"
#include <thread>

using namespace std;

void run(int socket) {
  int delay = (1 + rand() % 5) * 1000;
  cout << "[Timer Thread] Setting timer in " << delay / 1000 << " seconds."
       << endl;
  int fib_input = rand() % 30;
  cout << "[Timer Thread: SEND] Fibonacci input: " << fib_input << endl;
  write_data(&socket, fib_input);
}

int main(int argc, char *argv[]) {
  int port, host_order;
  char *ip;
  if (argc == 2) {
    ip = NULL;
    port = 0;
    host_order = atoi(argv[1]);
  } else if (argc == 3) {
    ip = NULL;
    port = atoi(argv[2]);
    host_order = atoi(argv[1]);
  } else if (argc == 4) {
    ip = argv[3];
    port = atoi(argv[2]);
    host_order = atoi(argv[1]);
  } else {
    cout << "Command line arguments should be as follows: " << endl;
    cout << "1 argument: ./host (1 or 2)" << endl;
    cout << "2 arguments: ./host (1 or 2) (port number)" << endl;
    cout << "3 arguments: ./host (1 or 2) (port number) (ip address)" << endl;
    exit(EXIT_FAILURE);
  }

  srand((unsigned)time(NULL));

  int socket, fib_numb;
  struct sockaddr_in sock_addr;
  bool read_index = true;

  if (host_order == 1) {
    socket = create_socket();

    port = socket_setup(&socket, &sock_addr, ip, port);

    bind_socket(&socket, sock_addr);
    listen_socket(&socket);

    cout << "Host 1 is waiting for a connection on port " << port << endl;

    accept_socket(&socket, sock_addr);
  } else if (host_order == 2) {
    socket = create_socket();

    socket_setup(&socket, &sock_addr, ip, port);

    connect_socket(&socket, sock_addr);
  }

  for (int i = 0; i < 10; i++) {
    cout << endl;
    cout << "Message: " << i + 1 << "/10" << endl;
    if (host_order == 1) {
      thread t1(run, socket);
      t1.join();
      read_index = false;
    }
    fib_numb = read_data(&socket, fib_numb);
    if (read_index) {
      cout << "[Main Thread: RECEIVE] Fibonacci index: " << fib_numb << endl;

      fib_numb = fibonacci(fib_numb);

      write_data(&socket, fib_numb);

    } else {
      cout << "[Timer Thread: RECEIVE] Fibonacci result: " << fib_numb << endl;
      read_index = true;
    }
  }

  close_socket(&socket);
  return 0;
}