#include "host_functions.h"
#include <sys/types.h>
#include <fstream>

#define QUOTES_FILE "../quotes.csv"
#define MAX_QUOTES 1662
#define MAX_CONNECTIONS 10

string handle_data_request(int socket, string data_ask){
  data_ask = ask_for_data();

  if (data_ask == "q"){
    write_data(&socket, "q");
    cout << "\n[CLIENT] Waiting to receive a quote...\n" << endl;
  } else if (data_ask == "d"){
    write_data(&socket, "d");
    cout << "\n[CLIENT] Waiting to receive a date...\n" << endl;
  } else if (data_ask == "a"){
    write_data(&socket, "a");
    cout << "\n[CLIENT] Waiting to receive an author...\n" << endl;
  } else if (data_ask == "r"){
    write_data(&socket, "r");
    cout << "\n[CLIENT] Waiting to receive all available data...\n" << endl;
  } else if (data_ask == "n"){
    write_data(&socket, "n");
    cout << "\n[CLIENT] Waiting for a new quote to be selected...\n" << endl;
  } else if (data_ask == "e"){
    write_data(&socket, "e");
    cout << "\n[CLIENT] Ending program...\n" << endl;
    close_one_socket(&socket);
  } else {
    cout << "\n[CLIENT] Incorrect input! Please try again!\n" << endl;
    data_ask = handle_data_request(socket, data_ask);
  }
  return data_ask;
}

int main(int argc, char *argv[]) {
  int port;
  char *ip;
  string client_or_server;

  if (argc == 1) {
    ip = NULL;
    port = 0;
    client_or_server = "client";
  } else if (argc == 2) {
    ip = NULL;
    port = 0;
    client_or_server = argv[1];
  } else if (argc == 3) {
    ip = NULL;
    port = atoi(argv[2]);
    client_or_server = argv[1];
  } else if (argc == 4) {
    ip = argv[2];
    port = atoi(argv[3]);
    client_or_server = argv[1];
  }


  srand((unsigned)time(NULL));
  // code for parsing a csv file came from:
  // https://java2blog.com/cpp-read-file-into-array/
  string line;            // To read each line from code
  int i = 0;              // Variable to keep count of each line
  string arr[MAX_QUOTES]; // array to store each line
  ifstream mFile(QUOTES_FILE);
  if (mFile.is_open()) {
    while (!mFile.eof()) {
      getline(mFile, line);
      arr[i] = line;
      i++;
    }
    mFile.close();
  } else {
    cout << "Couldn't open the file\n";
  }

  /*Client and Server variables*/
  int server_socket, client_socket, new_client;
  struct sockaddr_in server_addr; //used in both
  struct sockaddr_in new_addr;
  socklen_t addr_size;
  pid_t client_pid;

  /*Quote of the day variables*/
  string data_ask, read_quote, curr_quote,
    author_q, quote_q, date_q;
  int random;

  //Setting up client and server
  if (client_or_server == "server"){
    server_socket = create_socket();
    cout << "[SERVER] A server socket has been created!" << endl;
    memset(&server_addr, '\0', sizeof(server_addr));
    port = socket_setup(&server_socket, &server_addr, ip, port);
    bind_socket(&server_socket, server_addr);
    listen_to_socket(&server_socket, MAX_CONNECTIONS);
    cout << "[SERVER] Waiting for a connection on port " << port << "..." << endl;
  } else if (client_or_server == "client"){
    client_socket = create_socket();
    memset(&server_addr, '\0', sizeof(server_addr));
    port = socket_setup(&client_socket, &server_addr, ip, port);
    connect_socket(&client_socket, server_addr);
  }

  random = rand() % MAX_QUOTES;

  while(1){
    if (client_or_server == "server"){
        new_client = accept(server_socket, (struct sockaddr*)&new_addr, &addr_size);
        if (new_client < 0){
            perror("The server failed to accept a client!");
            exit(EXIT_FAILURE);
        }
        cout << "[SERVER] Connection accepted from " << inet_ntoa(new_addr.sin_addr) 
        << ": " << ntohs(new_addr.sin_port) << endl;

        if ((client_pid = fork()) == 0){
            close_one_socket(&server_socket);

            data_ask = "";
            while (data_ask != "e"){
                curr_quote = arr[random];
                Quote new_quote = Quote();
                new_quote = new_quote.stringToQuote(curr_quote);
                author_q = new_quote.getName();
                quote_q = new_quote.getQuote();
                date_q = new_quote.getDate();

                data_ask = read_data(&new_client);

                if (data_ask == "q"){
                    write_data(&new_client, quote_q);
                    cout << "\n[SERVER] Waiting to send a quote...\n" << endl;
                } else if (data_ask == "d"){
                    write_data(&new_client, date_q);
                    cout << "\n[SERVER] Waiting to send a date...\n" << endl;
                } else if (data_ask == "a"){
                    write_data(&new_client, author_q);
                    cout << "\n[SERVER] Waiting to send an author...\n" << endl;
                } else if (data_ask == "r"){
                    write_data(&new_client, curr_quote);
                    cout << "\n[SERVER] Waiting to send all available data...\n" << endl;
                } else if (data_ask == "n"){
                    cout << "\n[SERVER] Selecting a new quote\n" << endl;
                    random = rand() % MAX_QUOTES;
                    write_data(&new_client, "[SERVER] A new quote was selected!");
                } else if (data_ask == "e"){
                    write_data(&new_client, "e");
                    cout << "\n[SERVER] Ending program...\n" << endl;
                } else {
                    cout << "\n[SERVER] Incorrect input! Waiting to send data!\n" << endl;
                }
            }
        }
    } else if (client_or_server == "client"){
        data_ask = "";
        while (data_ask != "e"){
            data_ask = handle_data_request(client_socket, data_ask);

            read_quote = read_data(&client_socket);
            Quote quote_recv = Quote();
            if (data_ask == "q"){
                quote_recv.setQuote(read_quote);
                quote_recv.printQuote(); 
            } else if (data_ask == "d"){
                quote_recv.setDate(read_quote);
                quote_recv.printDate(); 
            } else if (data_ask == "a"){
                quote_recv.setName(read_quote);
                quote_recv.printName(); 
            } else if (data_ask == "r"){
                quote_recv = quote_recv.stringToQuote(read_quote);
                quote_recv.toString();
            } else if (data_ask == "n"){
                cout << read_quote << endl;
            }
        }
    }
  }
}