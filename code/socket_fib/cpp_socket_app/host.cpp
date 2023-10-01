#include "host_functions.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

static void *start_host(void *arg){
    int *socket = (int*) arg;
    int timer = (2 + rand() % 9) * 1000;
    int fib_input = rand() % 40;
    cout << "[SEND] Fibonacci index: " << fib_input << endl;
    write_data(socket, fib_input);
    fib_input = read_data(socket, fib_input);
    cout << "[RECEIVE] Fibonacci(" << fib_input << ") = " << fibonacci(fib_input) << endl;
    cout << "Resetting timer in: " << timer/1000 << " seconds.\n" << endl;
    return 0;
}

static void *receive_host(void *arg){
    int *socket = (int*) arg;
    int fib_input = read_data(socket, fib_input);
    cout << "[SEND] Fibonacci index: " << fib_input << endl;
    cout << "[RECEIVE] Fibonacci(" << fib_input << ") = " << fibonacci(fib_input) << endl;
    write_data(socket, fibonacci(fib_input));
    return 0;
}





int main(int argc, char* argv[]){
    int port, host_order;
    char *ip;
    if (argc == 1){
        ip = NULL;
        port = 0;
        host_order = 1;
    }else if (argc == 2){
        ip = NULL;
        port = 0; 
        host_order = atoi(argv[1]);
    }else if (argc == 3){
        ip = argv[1];
        port = atoi(argv[2]);
        host_order = 1;
    }else if (argc == 4){
        ip = argv[1];
        port = atoi(argv[2]);
        host_order = atoi(argv[3]);
    }

    int socket_h1, socket_h2, fib_input, i1;
    struct sockaddr_in h1_addr;
    struct sockaddr_in h2_addr;
    socklen_t h2_len;
    pthread_t start_thread;

    srand((unsigned) time(NULL));

    
    if (host_order == 1){
        socket_h1 = create_socket();

        port = socket_setup(&h1_addr, ip, port);

        bind_socket(&socket_h1, h1_addr);
        listen_to_socket(&socket_h1, 1);

        cout << "Host 1 is waiting for a connection on port " << port << endl;
        cout << endl;

        h2_len = sizeof(h2_addr);
        socket_h2 = accept(socket_h1, (struct sockaddr*)&h2_addr, &h2_len);

    }else if (host_order == 2){
        socket_h2 = create_socket();

        socket_setup(&h1_addr, ip, port); 

        connect_socket(&socket_h2, h1_addr);
    }

    
    for (int i = 0; i < 10; i++){
        cout << "Sending Message: " << i << "/10" << endl;
        i1 = pthread_create(&start_thread, NULL, &start_host, &socket_h2);
        pthread_join(start_thread, NULL);

        fib_input = read_data(&socket_h2, fib_input);
        cout << "[SEND] Fibonacci index: " << fib_input << endl;
        cout << "[RECEIVE] Fibonacci(" << fib_input << ") = " << fibonacci(fib_input) << endl;
        write_data(&socket_h2, fibonacci(fib_input));
        //i2 = pthread_create(&receive_thread, NULL, &receive_host, &socket_h2);
        //pthread_join(receive_thread, NULL);
    }

    close_two_sockets(&socket_h2, &socket_h1);


    
    return 0;
}