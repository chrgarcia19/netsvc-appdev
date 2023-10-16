#include "host_functions.h"
#include <fstream>
#include <iostream>
#include <string>

#define QUOTES_FILE "../quotes.csv"
#define MAX_QUOTES 1662
#define MAX_CONNECTIONS 10

int main(int argc, char *argv[]) {
  int port;
  char *ip;
  string client_or_server;

  if (argc == 1) {
    ip = NULL;
    port = 0;
    client_or_server = "client";
  } else if (argc == 2) {
    ip = NULL;
    port = 0;
    client_or_server = argv[1];
  } else if (argc == 3) {
    ip = argv[1];
    port = atoi(argv[2]);
    client_or_server = "client";
  } else if (argc == 4) {
    ip = argv[1];
    port = atoi(argv[2]);
    client_or_server = argv[3];
  }

  int socket;
  struct sockaddr_in sock_addr;
  socklen_t sock_len;
  string read_quote;
  size_t size = 10000;

  srand((unsigned)time(NULL));
  // code for parsing a csv file came from:
  // https://java2blog.com/cpp-read-file-into-array/
  string line;            // To read each line from code
  int i = 0;              // Variable to keep count of each line
  string arr[MAX_QUOTES]; // array to store each line
  ifstream mFile(QUOTES_FILE);
  if (mFile.is_open()) {
    while (!mFile.eof()) {
      getline(mFile, line);
      arr[i] = line;
      i++;
    }
    mFile.close();
  } else {
    cout << "Couldn't open the file\n";
  }

  if (client_or_server == "server") {
    socket = create_socket();

    port = socket_setup(&sock_addr, ip, port);

    bind_socket(&socket, sock_addr);
    listen_to_socket(&socket, MAX_CONNECTIONS);

    cout << "The server is waiting for a connection on port " << port << endl;
    cout << endl;

    sock_len = sizeof(sock_addr);
    socket = accept(socket, (struct sockaddr *)&sock_addr, &sock_len);

  } else if (client_or_server == "client") {

    socket = create_socket();

    socket_setup(&sock_addr, ip, port);

    connect_socket(&socket, sock_addr);

    cout << "The client has connected to the server\n" << endl;
  }

  for (int i = 0; i < 5; i++) {
    int random = rand() % MAX_QUOTES;

    if (client_or_server == "server") {
      size = write_data(&socket, &arr[random]);

      cout << "The server sent out a quote to the client!\n" << endl;
    } else if (client_or_server == "client") {
      read_quote = read_data(&socket, size);

      Quote quote = Quote();
      quote = quote.stringToQuote(read_quote);
      quote.toString();
      cout << endl;
    }
  }

  close_one_socket(&socket);

  return 0;
}