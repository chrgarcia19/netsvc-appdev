#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

#define DEFAULT_PORT 4200
#define BUFFER_SIZE 1024
#define DEFAULT_IP "127.0.0.1" //localhost

using namespace std;

/*NETWORKING METHODS*/
int create_socket(){
  int new_socket;
  /*AF_INET determines the type of IP address
    AF_INET is used for IPv4.
    SOCK_STREAM determines the type of connection.
    SOCK_DGRAM is UDP*/
  if ((new_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Socket creation failed!");
    exit(EXIT_FAILURE);
  }
  return new_socket;
}

void bind_socket(int socket, struct sockaddr_in addr){
  if (bind(socket, (const struct sockaddr*)&addr, sizeof(addr)) < 0){
    perror("Socket bind failed!");
    exit(EXIT_FAILURE);
  }
}

string read_data(int socket, struct sockaddr_in addr, unsigned int len){
  char buf[BUFFER_SIZE];
  string new_mess;
  int res;
  if ((res = recvfrom(socket, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &len)) > 0){
    new_mess.append(buf, buf + res);
  } else {
    cout << "Error reading data" << endl;
  }
  return new_mess;
}

void write_data(int socket, struct sockaddr_in addr, unsigned int len, string message){
  int res;
  res = sendto(socket, message.data(), message.size(), 0, (struct sockaddr*)&addr, len);
  if (res < 0){
      cout << "Message sending error" << endl;
  } else {
    cout << "A message was sent!" << endl;
  }  
}