#include "fibonacci.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTNUM 4200

int main(){
    int client_socket, server_socket, send_fib_num, 
    send_fib_input, received_num, fib_input;
    sockaddr_in server_addr;

    fib_input = 1;
    send_fib_num, send_fib_input, received_num = 0;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORTNUM);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    //The line below is if we need to link to a specific ip addr
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    for (int i = 0; i < 10; i++){
        //htonl converts from byte order to network byte order
        send_fib_input = htonl(fib_input);
        //sending the input of the fibonacci function to the server
        write(client_socket, &send_fib_input, sizeof(send_fib_input));

        //sending the result of the fibonacci function to the server
        send_fib_num = htonl(fibonacci(fib_input));
        write(client_socket, &send_fib_num, sizeof(send_fib_num));

        //receiving the input of the fibonacci function from the server
        read(client_socket, &send_fib_input, sizeof(send_fib_input));
        fib_input = ntohl(send_fib_input);
        //receiving the result of the fibonacci function from the server
        read(client_socket, &send_fib_num, sizeof(send_fib_num));
        received_num = ntohl(send_fib_num);

        std::cout << "Server replied with: fibonacci(" << fib_input <<
        ") = " << received_num << std::endl;

        fib_input += 1;
    }

    close(client_socket);

    return 0;
}