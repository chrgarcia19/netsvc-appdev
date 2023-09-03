#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TESTING		1

int main(int argc, char **argv){
	if(argc != 1){
		fprintf(stderr, "[ERROR] Number of arguments of invalid!\n");
		exit(EXIT_FAILURE);
	}
	printf("Hello world!\n");

	return(EXIT_SUCCESS);
}
