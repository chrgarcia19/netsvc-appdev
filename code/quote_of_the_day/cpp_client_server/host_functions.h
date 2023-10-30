#include "quote_class.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_PORT 4200
#define BUFFER_SIZE 1024

/*NETWORKING METHODS*/
int create_socket(){
  int new_socket;
  /*AF_INET determines the type of IP address
    AF_INET is used for IPv4.
    SOCK_STREAM determines the type of connection.
    SOCK_STREAM is TCP*/
  if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("The socket was failed to be created!");
    exit(EXIT_FAILURE);
  }
  return new_socket;
}

int socket_setup(int *socket, struct sockaddr_in *socket_addr, char* ip, int port){
  const int opt = 1;
  if (setsockopt(*socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
    perror("An error occurred setting the socket operation!");
    exit(EXIT_FAILURE);
  } else {
    bzero(socket_addr, sizeof(socket_addr));
    setsockopt(*socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
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

void bind_socket(int *socket, struct sockaddr_in addr){
  if (bind(*socket, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    perror("The socket failed to bind!");
    exit(EXIT_FAILURE);
  }
}

void listen_to_socket(int *socket, int num_connections){
  if (listen(*socket, num_connections) < 0){
    perror("The socket failed to listen!");
    exit(EXIT_FAILURE);
  } 
}

void connect_socket(int *socket, struct sockaddr_in ip_addr) {
  if (connect(*socket, (struct sockaddr *)&ip_addr, sizeof(ip_addr)) < 0){
    perror("The socket failed to connect!");
    exit(EXIT_FAILURE);
  }
}

int accept_socket(int *socket, struct sockaddr_in sock_addr, socklen_t sock_len){
  if ((*socket = accept(*socket, (struct sockaddr *)&sock_addr, &sock_len)) < 0){
    perror("There was an error accepting a socket");
    exit(EXIT_FAILURE);
  }
  return *socket;
}

void close_one_socket(int *socket) { close(*socket); }

string read_data(int *socket){
  char buffer[BUFFER_SIZE];
  string new_data; 
  int n;
  if ((n = recv(*socket, buffer, BUFFER_SIZE, 0)) > 0){
    new_data.append(buffer, buffer + n);
  } else {
    cout << "There was an error reading the data from the buffer!" << endl;
  }
  return new_data;
}

void write_data(int *socket, string data_q){
  int n;
  if ((n = send(*socket, data_q.data(), data_q.size(), 0)) < 0){
    cout << "There was an error writing the data!" << endl;
  }
}

int check_connection(int *socket){
  int error_code;
  socklen_t error_size = sizeof(error_code);
  return getsockopt(*socket, SOL_SOCKET, SO_ERROR, &error_code, &error_size);
}

string ask_for_data(){
  string ask;
  cout << "\n--------------------------------------" << endl;
  cout << "Quote of the Day Client Server in C++" << endl;
  cout << "--------------------------------------" << endl;
  cout << "Request the data you want..." << endl;
  cout << "a) Author" << endl;
  cout << "q) Quote" << endl;
  cout << "d) Date" << endl;
  cout << "r) All data" << endl;
  cout << "n) New quote" << endl;
  cout << "e) Exit" << endl;
  cout << "--------------------------------------\n" << endl;
  cin >> ask;
  return ask;
}

