/*Networking Libraries*/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

/*Standard Libraries*/
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <strings.h>
#include <unistd.h>

#define DEFAULT_PORT 4200

int create_socket() {
  int new_socket;
  /*AF_INET determines the type of IP address
    AF_INET is used for IPv4.
    SOCK_STREAM determines the type of connection.
    SOCK_STREAM is TCP*/
  if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("The socket was failed to be created!");
    exit(EXIT_FAILURE);
  }
  return new_socket;
}

int socket_setup(int *socket, struct sockaddr_in *socket_addr, char *ip,
                 int port) {
  const int opt = 1;
  if (setsockopt(*socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("An error occurred setting the socket operation!");
    exit(EXIT_FAILURE);
  } else {
    bzero(socket_addr, sizeof(socket_addr));
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
      socket_addr->sin_addr.s_addr = INADDR_ANY;
    }
    return port;
  }
}

void bind_socket(int *socket, struct sockaddr_in addr) {
  if (bind(*socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("The socket failed to bind!");
    exit(EXIT_FAILURE);
  }
}

void listen_socket(int *socket) {
  if (listen(*socket, 1) < 0) {
    perror("The socket failed to listen!");
    exit(EXIT_FAILURE);
  }
}

void connect_socket(int *socket, struct sockaddr_in ip_addr) {
  if (connect(*socket, (struct sockaddr *)&ip_addr, sizeof(ip_addr)) < 0) {
    perror("The socket failed to connect!");
    exit(EXIT_FAILURE);
  }
}

int accept_socket(int *socket, struct sockaddr_in sock_addr) {
  socklen_t sock_len = sizeof(sock_addr);
  if ((*socket = accept(*socket, (struct sockaddr *)&sock_addr, &sock_len)) <
      0) {
    perror("There was an error accepting a socket");
    exit(EXIT_FAILURE);
  }
  return *socket;
}

void write_data(int *socket, int data) {
  char buffer[sizeof(data)];
  memcpy(buffer, &data, sizeof(data));
  send(*socket, buffer, sizeof(buffer), 0);
  // int new_data = htonl(data);
  // write(*socket, &new_data, sizeof(new_data));
}

int read_data(int *socket, int data) {
  // read(*socket, &data, sizeof(data));
  // int new_data = ntohl(data);
  char buffer[sizeof(data)];
  recv(*socket, buffer, sizeof(buffer), 0);
  int new_data;
  memcpy(&new_data, buffer, sizeof(buffer));
  return new_data;
}

void close_socket(int *socket) { close(*socket); }

int fibonacci(int n) {
  if (n == 0) {
    return 0;
  } else if (n == 1 || n == 2) {
    return 1;
  } else {
    return fibonacci(n - 1) + fibonacci(n - 2);
  }
}