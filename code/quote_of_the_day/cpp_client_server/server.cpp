#include "host_functions.h"
#include <iostream>
#include <fstream>
#include <string>

#define QUOTES_FILE "../quotes.csv"
#define MAX_QUOTES 1662

int main(int argc, char* argv[]){
    int port;
    char *ip;
    string client_or_server;
    if (argc == 1){
        ip = NULL;
        port = 0;
        client_or_server = "client";
    } else if (argc == 2){
        ip = NULL;
        port = 0;
        client_or_server = argv[1];
    } else if (argc == 3){
        ip = argv[1];
        port = atoi(argv[2]);
        client_or_server = "client";
    } else if (argc == 4){
        ip = argv[1];
        port = atoi(argv[2]);
        client_or_server = argv[3];
    }

    srand((unsigned)time(NULL));
    //code for parsing a csv file came from:
    //https://java2blog.com/cpp-read-file-into-array/
	string line;   // To read each line from code
    int i=0;    // Variable to keep count of each line
    string arr[MAX_QUOTES];  // array to store each line
    ifstream mFile (QUOTES_FILE);   
    if(mFile.is_open()){
        while(!mFile.eof()){
            getline(mFile, line);
            arr[i]=line;
            i++;
        }
        mFile.close();
    }else{
        cout<<"Couldn't open the file\n";
    }
 
    Quote quotes[MAX_QUOTES];

    for (int i = 0; i < MAX_QUOTES; i++){
        Quote newQuote = Quote();
        newQuote = newQuote.stringToQuote(arr[i]);
        quotes[i] = newQuote;
    }
    
    if (client_or_server == "server"){
        cout << "I am the server" << endl;

    } else if (client_or_server == "client"){
        cout << "I am the client";
    }

    return 0;
}