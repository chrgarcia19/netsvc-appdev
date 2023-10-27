#include "host_functions.h"
#include <fstream>

#define QUOTES_FILE "../quotes.csv"
#define MAX_QUOTES 1662
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

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

  int socket;
  struct sockaddr_in sock_addr;
  string read_quote;
  string data_ask, curr_quote, author_q, quote_q, date_q;
  bool done = false;

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

  bzero(&sock_addr, sizeof(sock_addr));

  if (client_or_server == "server") {
    socket = create_socket();

    port = socket_setup(&socket, &sock_addr, ip, port);

    bind_socket(&socket, sock_addr);
    listen_to_socket(&socket, MAX_CONNECTIONS);

    cout << "The server is waiting for a connection on port " << port << endl;
    cout << endl;

    socket = accept_socket(&socket, sock_addr, sizeof(sock_addr));
    cout << "The server has connected to a client" << endl;
  } else if (client_or_server == "client") {

    socket = create_socket();

    socket_setup(&socket, &sock_addr, ip, port);

    connect_socket(&socket, sock_addr);
    
    cout << "The client has connected to the server\n" << endl;
  }

  int random = rand() % MAX_QUOTES;

  while (!done){
    if (check_connection(&socket) == -1){
      close_one_socket(&socket);
      cout << "The connection has been lost!" << endl;
    } else {
      if (client_or_server == "server"){

        curr_quote = arr[random];
        Quote new_quote = Quote();
        new_quote = new_quote.stringToQuote(curr_quote);
        author_q = new_quote.getName();
        quote_q = new_quote.getQuote();
        date_q = new_quote.getDate();

        data_ask = read_data(&socket, BUFFER_SIZE);

        cout << data_ask << endl;

        if (data_ask == "q"){
          write_data(&socket, quote_q);
          cout << "\nWaiting to send a quote...\n" << endl;
        } else if (data_ask == "d"){
          write_data(&socket, date_q);
          cout << "\nWaiting to send a date...\n" << endl;
        } else if (data_ask == "a"){
          write_data(&socket, author_q);
          cout << "\nWaiting to send an author...\n" << endl;
        } else if (data_ask == "r"){
          write_data(&socket, curr_quote);
          cout << "\nWaiting to send all available data...\n" << endl;
        } else if (data_ask == "n"){
          cout << "\nSelecting a new quote\n" << endl;
          random = rand() % MAX_QUOTES;
          write_data(&socket, "A new quote was selected!");
        } else if (data_ask == "e"){
          write_data(&socket, "e");
          done = true;
          cout << "\nEnding program...\n" << endl;
        } else {
          cout << "\nIncorrect input! Please try again!\n" << endl;
        }

      } else if (client_or_server == "client"){
        data_ask = ask_for_data();

        if (data_ask == "q"){
          write_data(&socket, "q");
          cout << "\nWaiting to receive a quote...\n" << endl;
        } else if (data_ask == "d"){
          write_data(&socket, "d");
          cout << "\nWaiting to receive a date...\n" << endl;
        } else if (data_ask == "a"){
          write_data(&socket, "a");
          cout << "\nWaiting to receive an author...\n" << endl;
        } else if (data_ask == "r"){
          write_data(&socket, "r");
          cout << "\nWaiting to receive all available data...\n" << endl;
        } else if (data_ask == "n"){
          write_data(&socket, "n");
          cout << "\nWaiting for a new quote to be selected...\n" << endl;
        } else if (data_ask == "e"){
          write_data(&socket, "e");
          done = true;
          cout << "\nEnding program...\n" << endl;
        } else {
          cout << "\nIncorrect input! Please try again!\n" << endl;
        }


        read_quote = read_data(&socket, BUFFER_SIZE);
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

  cout << "The connection has been closed!" << endl;

  close_one_socket(&socket);

  return 0;
}