#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define DEBUG_STATEMENTS 1

int main(int argc, char **argv){
	if(argc != 1){
		fprintf(stderr, "[ERROR] Number of arguments is invalid!\n");
		exit(EXIT_FAILURE);
	}
	printf("Hello world!\n");

	return(EXIT_SUCCESS);
}
