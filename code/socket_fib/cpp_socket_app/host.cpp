#include "host_functions.h"
#include <thread>

using namespace std;


void run1(int socket){
    int delay = (1 + rand() % 5) * 1000;
    cout << "[Timer Thread 1] Setting timer in " << delay/1000 << " seconds." << endl;
    int fib_input = rand() % 30;
    cout << "[Timer Thread 1: SEND] Fibonacci input: " << fib_input << endl;
    write_data(&socket, fib_input);
    int fib_out = read_data(&socket, fib_out);
    cout << "[Timer Thread 1: RECEIVE] Fibonacci result: " << fibonacci(fib_out) << endl;
}

void run2(int socket){
    int fib_out = read_data(&socket, fib_out);
    cout << "[Timer Thread 2: RECEIVE] Fibonacci result: " << fibonacci(fib_out) << endl;
    int fib_input = rand() % 30;
    cout << "[Timer Thread 2: SEND] Fibonacci input: " << fib_input << endl;
    write_data(&socket, fib_input);
}



int main(int argc, char* argv[]){
    int port, host_order;
    char *ip;
    if (argc == 1) {
        ip = NULL;
        port = 0;
        host_order = 1;
    } else if (argc == 2) {
        ip = NULL;
        port = 0;
        host_order = atoi(argv[1]);
    } else if (argc == 3) {
        ip = argv[1];
        port = atoi(argv[2]);
        host_order = 1;
    } else if (argc == 4) {
        ip = argv[1];
        port = atoi(argv[2]);
        host_order = atoi(argv[3]);
    }

    srand((unsigned)time(NULL));

    int socket;
    struct sockaddr_in sock_addr;
    socklen_t sock_len;

    if (host_order == 1){
        socket = create_socket();

        port = socket_setup(&sock_addr, ip, port);

        bind_socket(&socket, sock_addr);
        listen_to_socket(&socket, 1);

        cout << "Host 1 is waiting for a connection on port " << port << endl;
        cout << endl;

        sock_len = sizeof(sock_addr);
        socket = accept(socket, (struct sockaddr *)&sock_addr, &sock_len);
    } else if (host_order == 2){
        socket = create_socket();

        socket_setup(&sock_addr, ip, port);

        connect_socket(&socket, sock_addr);
    }

    for (int i = 0; i < 10; i++){
        if (host_order == 1){
            cout << "Message: " << i+1 << "/10" << endl;
            thread t1(run1, socket);
            t1.join();
            cout << endl;
        } else if (host_order == 2){
            cout << "Message: " << i+1 << "/10" << endl;
            thread t2(run2, socket);
            t2.join();
            cout << endl;
        }
    }
    
    close_one_socket(&socket);
    close_two_sockets(&socket, &socket);
    return 0;
}