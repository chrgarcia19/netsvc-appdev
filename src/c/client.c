#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

#define TESTING 	false
#define PIPE_NAME 	"FIBOPIPE"

int main(int argc, char **argv)
	{
	if(argc != 1){
		fprintf(stderr, "[ERROR] Number of arguments of invalid!\n");
		exit(EXIT_FAILURE);
	}

	///open pipe for writing 
	int pipe = open(argv[0], O_WRONLY);
	
	if(pipe == -1){
		perror("[ERROR] opening pipe");
		exit(EXIT_FAILURE);
	}

	long fib = 1;	//input fib number from the user

	///prompt and read integer from input
	while(fib != 0){
		printf("[QUERY] Enter a fibonacci number index: ");
		if(fscanf(stdin, "%ld", &fib) == EOF){
			perror("\n[ERROR] Attempting to fscanf failed");
			exit(EXIT_FAILURE);
		}
		
		///write int into FIFO while its not 0
		if(write(pipe, &fib, sizeof(long)) == -1){
			perror("[ERROR] Attempting to write to pipe failed");
			exit(EXIT_FAILURE);
		}
	}

	///close FIFO
	close(pipe);

	return(EXIT_SUCCESS);
}
