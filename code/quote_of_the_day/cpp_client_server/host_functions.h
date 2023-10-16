#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "quote_class.h"

#define DEFAULT_PORT 4200

/*NETWORKING METHODS*/
int create_socket() {
  /*AF_INET determines the type of IP address
  AF_INET is used for IPv4.
  SOCK_STREAM determines the type of connection.
  SOCK_STREAM is TCP*/
  int new_socket = socket(AF_INET, SOCK_STREAM, 0);
  return new_socket;
}

int socket_setup(struct sockaddr_in *socket_addr, char *ip, int port) {
  socket_addr->sin_family = AF_INET;
  if (port == 0) { // sets to default port num
    // htons converts a byte order into a network byte order
    socket_addr->sin_port = htons(DEFAULT_PORT);
    port = DEFAULT_PORT;
  } else { // sets to specified port num
    socket_addr->sin_port = htons(port);
  }
  if (ip != NULL) { // connects to a specific IP address
    socket_addr->sin_addr.s_addr = inet_addr(ip);
  } else { // connects to any IP address
    // INADDR_ANY binds to any interface (IP or localhost)
    socket_addr->sin_addr.s_addr = htonl(INADDR_ANY);
  }
  return port;
}

void bind_socket(int *socket, struct sockaddr_in ip_addr) {
  bind(*socket, (struct sockaddr *)&ip_addr, sizeof(ip_addr));
}

void listen_to_socket(int *socket, int num_connections) {
  listen(*socket, num_connections);
}

void connect_socket(int *socket, struct sockaddr_in ip_addr) {
  connect(*socket, (struct sockaddr *)&ip_addr, sizeof(ip_addr));
}

Quote read_data(int *socket, Quote data) {
  read(*socket, &data, sizeof(data));
  Quote new_data = data;
  return new_data;
}

void write_data(int *socket, Quote data) {
  write(*socket, &data, sizeof(data));
}

void close_one_socket(int *socket) { close(*socket); }