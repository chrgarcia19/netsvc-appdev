#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "fibonacci.h"

int main(){
    printf("%d" , fibonacci(10));
    return 0;
}
