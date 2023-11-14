#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
// Networking includes
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
// Macros
#define EXIT_SUCCESS		0
#define EXIT_FAILURE		1
#define DEBUG_STATEMENTS	1
#define START_PORT_RANGE	4200
#define END_PORT_RANGE		4300
#define DEFAULT_PORT		START_PORT_RANGE
// Personal headers
#include "networking.h"

void error(char * msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

int looping = 1;
uint32_t last_user = 0, last_user_low = 0, last_sys = 0, last_idle = 0;

double get_cpu_percentage(void){
	double percent;
	FILE* stat;
	uint32_t total_user, total_user_low, total_sys, total_idle, total;
	uint32_t user_diff, user_low_diff, sys_diff, idle_diff, total_diff;

	stat = fopen("/proc/stat", "r");
	fscanf(
		stat, 
		"cpu %llu %llu %llu %llu", 
		&total_user,
		&total_user_low,
		&total_sys, 
		&total_idle
	);
	fclose(stat);

	user_low_diff = (total_user_low - last_user_low);
	user_diff     = (total_user     - last_user);
	sys_diff      = (total_sys      - last_sys);
	idle_diff     = (total_idle     - last_idle);

	total_diff = user_diff + user_low_diff + sys_diff + idle_diff;

	percent = (user_diff + user_low_diff + sys_diff) / (total_diff);

	last_user_low = total_user_low;
	last_user     = total_user;
	last_sys      = total_sys;
	last_idle     = total_idle;

	return (percent * 100);
}

void nice_close(int sig){
	looping = 0;
	signal(sig, SIG_IGN);
}

int main(int argc, char** argv){
	signal(SIGINT, nice_close);
	if(DEBUG_STATEMENTS){
		printf("[DEBUG] Number of args: %d\n", argc);
	}
	
	if(argc != 3){
		printf("[ERROR] Invalid number of arguments!\n");
		printf("Usage: %s [PORT] [IP ADDRESS]\n", argv[0]);
		return EXIT_FAILURE;
	}

	int port = atoi(argv[1]);
	char * ip_addr = argv[2];

	int client_socket, errval;
	struct sockaddr_in server_addr, client_addr;
	char * msg = (char *)malloc(MAX_MSG_SIZE);
	socklen_t client_len;

	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));
	memset(msg, 0, MAX_MSG_SIZE);

	client_len = sizeof(client_addr);

	srand(time(NULL));

	client_socket = udp_socket_init();
	socket_config(&client_socket);
	port = address_config(&server_addr, port, ip_addr);

	errval = connect(
		client_socket, 
		(struct sockaddr *)&server_addr, 
		sizeof(server_addr)
	);

	if(errval < 0)
		error("[ERROR] Connecting to the server");


	while(looping){
		double percent = get_cpu_percentage();
		snprintf(msg, MAX_MSG_SIZE, "%lf", percent);
		printf("[LOG] Current CPU usage is %.2lf%%\n", percent);

		sendto(
			client_socket,
			msg,
			strlen(msg),
			MSG_CONFIRM,
			&client_addr,
			client_len
		);

		memset(msg, 0, MAX_MSG_SIZE);
		sleep(1);
	}

	close(client_socket);

	return EXIT_SUCCESS;
}
