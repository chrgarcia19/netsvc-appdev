#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

#define TESTING				false
#define DEBUG_STATEMENTS 	false
#define PIPE_NAME 			"FIBOPIPE"
#define MAX_STR_LEN			512

int num_threads = 0;

long fibonacci(long num){
	if(num == 1 || num == 2)
		return 1;
	else
		return fibonacci(num - 1) + fibonacci(num - 2);
}

void* fib_thread(void* num){
	if(sizeof(num) != sizeof(long)){
		fprintf(stderr, "[ERROR] Incompatible variable passed to fib_thread!\n");
		pthread_exit(0);
	}
	long fib_num = *(long*)num;
	long fib_res = fibonacci(fib_num);
	fprintf(stdout, "\n[INFO] The %ld fibonacci number is %ld.\n", fib_num, fib_res);

	num_threads--;
	pthread_exit(0);
}

int main(void){
	int pipe;		//the pipe to read from
	long fib_ret;
	int child_pid;	//the child is the interface
	int err_check;

	//unlink the FIFO if it's still around at the start
	unlink(PIPE_NAME);

	//creates the FIFO pipe
	if(DEBUG_STATEMENTS)
		printf("[DEBUG] Making the FIFO pipe...\n");

	if(mkfifo(PIPE_NAME, 0777) == -1){
		perror("[ERROR] Making the FIFO pipe");
		exit(EXIT_FAILURE);
	}

	//fork the process
	if(DEBUG_STATEMENTS){
		printf("[DEBUG] FIFO pipe created!\n");
		printf("[DEBUG] Forking process...\n");
	}

	child_pid = fork();

	if(DEBUG_STATEMENTS)
		printf("[DEBUG] Forked!\n");
	
	if(child_pid == 0){	//if the child_pid is 0, we're in the child
		execlp("./interface", PIPE_NAME, NULL);
		exit(EXIT_SUCCESS);
	}
	else{				//we're in the parent
		//open the pipe to read; read only, sync up
		pipe = open(PIPE_NAME, O_RDONLY);
		if(pipe == -1){
			perror("[ERROR] Opening pipe to read");
			exit(EXIT_FAILURE);
		}
		//keep reading until the return is 0, which indicates an end
		err_check = read(pipe, &fib_ret, sizeof(long));
		while(fib_ret != 0){
			if(err_check == -1){
				perror("[ERROR] Attempting to read from pipe failed");
				exit(EXIT_FAILURE);
			}
			printf("\n[INFO] Received from FIFO: %ld\n", fib_ret);
			pthread_t thread_id;

			pthread_create(
				&thread_id, 
				NULL, 
				fib_thread, 
				&fib_ret
			);
			num_threads++;
			pthread_detach(thread_id);
			read(pipe, &fib_ret, sizeof(long));
		}
	}
	while(num_threads != 0){
		printf("[INFO] Waiting for %d threads to finish...\n", num_threads);
		sleep(1);
	}

	//report the CPU usage of the program
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	printf(
		"[INFO] Fibonacci has used %ld s %ld ms\n",
		usage.ru_utime.tv_sec + usage.ru_stime.tv_sec,
		usage.ru_utime.tv_usec + usage.ru_stime.tv_usec
	);

	//close the pipe
	close(pipe);
	//unlink the FIFO
	unlink(PIPE_NAME);
	//sweet victory
	exit(EXIT_SUCCESS);
}
