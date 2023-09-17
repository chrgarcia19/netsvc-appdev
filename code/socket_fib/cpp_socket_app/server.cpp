#include "fibonacci.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORTNUM 4200

int main(){
    int server_socket, client_socket, fib_input,
    send_fib_num, send_fib_input, received_num;
    sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    send_fib_num, send_fib_input, received_num = 0; 

    //AF_INET determines the type of IP address
    //AF_INET is used for IPv4
    //SOCK_STREAM determines the type of connection. 
    //SOCK_STREAM is TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    //htons converts a byte order into a network byte order
    server_addr.sin_port = htons(PORTNUM);
    //INADDR_ANY binds to any interface (IP or localhost)
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    //The line below is if we need to link to a specific ip addr
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    std::cout << "Server is listening on port " << PORTNUM << std::endl;

    client_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    
    for (int i = 0; i < 10; i++){
        //receiving the input of the fibonacci function from the client
        read(client_socket, &send_fib_input, sizeof(send_fib_input));
        fib_input = ntohl(send_fib_input);

        //receiving the result of the fibonacci function from the client
        read(client_socket, &send_fib_num, sizeof(send_fib_num));
        received_num = ntohl(send_fib_num);

        std::cout << "Client replied with: fibonacci(" << fib_input <<
        ") = " << received_num << std::endl;
        
        fib_input += 1; 
        //sending the input of the fibonacci function to the client
        send_fib_input = htonl(fib_input);
        write(client_socket, &send_fib_input, sizeof(send_fib_input));

        //sending the result of the fibonacci function to the client
        send_fib_num = htonl(fibonacci(fib_input));
        write(client_socket, &send_fib_num, sizeof(send_fib_num));
    }

    close(client_socket);
    close(server_socket);



    return 0;
}