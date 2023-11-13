#include <time.h>
#include "host_functions.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define LOCAL_IP "10.1.135.93"

using namespace std; 


//Thank you Stack Overflow
//https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

double getCurrentValue(){
  double percent;
  FILE* file;
  unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

  file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
      &totalSys, &totalIdle);
  fclose(file);

  if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
      totalSys < lastTotalSys || totalIdle < lastTotalIdle){
      //Overflow detection. Just skip this value.
      percent = -1.0;
  }
  else{
      total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
          (totalSys - lastTotalSys);
      percent = total;
      total += (totalIdle - lastTotalIdle);
      percent /= total;
      percent *= 100;
  }

  lastTotalUser = totalUser;
  lastTotalUserLow = totalUserLow;
  lastTotalSys = totalSys;
  lastTotalIdle = totalIdle;

  return percent;
}

int main(int argc, char *argv[]){
  int port;
  char *ip;
  string client_or_server;

  if (argc == 2) {
      ip = "127.0.0.1";
      port = DEFAULT_PORT;
      client_or_server = argv[1];
  } else if (argc == 3) {
      ip = "127.0.0.1";
      port = atoi(argv[2]);
      client_or_server = argv[1];
  } else if (argc == 4) {
      ip = argv[2];
      port = atoi(argv[3]);
      client_or_server = argv[1];
  } else {
    cout << "Command line arguments should be as follows: " << endl;
    cout << "1 argument: ./broadcast (client or server)" << endl;
    cout << "2 arguments: ./broadcast (client or server) (port number)" << endl;
    cout << "3 arguments: ./broadcast (client or server) (ip address) (port number)" << endl;
    exit(EXIT_FAILURE);
  }

  int socket_send, socket_recv; 
  struct sockaddr_in send_addr, recv_addr;
  struct ip_mreq recv_mreq;
  unsigned int send_len, recv_len;
  //pid_t send_pid;

  if (client_or_server == "server"){
    socket_send = create_socket();
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(port);
    send_addr.sin_addr.s_addr = inet_addr(ip);
    send_len = sizeof(send_addr);
  } else if (client_or_server == "client"){
    socket_recv = create_socket();
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(port);
    recv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    recv_len = sizeof(recv_addr);

    bind_socket(socket_recv, recv_addr);
  }

  string buffer;
  double cpu_usage;
  string usage_str;
  
  while(true){
    cpu_usage = getCurrentValue();
    usage_str = to_string(cpu_usage);
    if (client_or_server == "server"){
      write_data(socket_send, send_addr, send_len, usage_str);
      sleep(5);
    } else if (client_or_server == "client"){
      buffer = read_data(socket_recv, recv_addr, recv_len);
      cout << "The CPU usage is: " << buffer << "%" << endl;
      sleep(5);
    }
  }
  close(socket_send);
  close(socket_recv);
  return 0;
}