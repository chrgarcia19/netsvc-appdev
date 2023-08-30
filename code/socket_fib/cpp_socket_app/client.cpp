#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "fibonacci.h"
#include <cstring>

#define PORTNUM 4203

int main(){
    int socket_c, socket_s;
    sockaddr_in server_addr;

    socket_c = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORTNUM);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    connect(socket_c, (struct sockaddr *)&server_addr, sizeof(server_addr));

    char message[1024] = "Hello, Server!";
    send(socket_c, message, strlen(message), 0);

    char buffer[1024] = {0};
    read(socket_c, buffer, sizeof(buffer));
    std::cout << "Server replied: " << buffer << std::endl;

    close(socket_c);

    return 0;
}