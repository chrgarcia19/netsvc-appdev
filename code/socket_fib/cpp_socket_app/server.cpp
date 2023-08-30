#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "fibonacci.h"
#include <cstring>

#define PORTNUM 4203

int main(){
    int socket_s, socket_c;
    sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    socket_s = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORTNUM);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(socket_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(socket_s, 5);

    std::cout << "Server is listening on port 4203" << std::endl;

    client_len = sizeof(client_addr);
    socket_c = accept(socket_s, (struct sockaddr*)&client_addr, &client_len);

    char buffer[1024] = {0};
    read(socket_c, buffer, sizeof(buffer));

    std::cout << "Received: " << buffer << std::endl;

    send(socket_c, buffer, strlen(buffer), 0);

    close(socket_c);
    close(socket_s);

    return 0;
}
