#include "host_functions.h"
#include <iostream>
#include <fstream>
#include <string>

#define QUOTES_FILE "../quotes.csv"
#define MAX_QUOTES 1662

int main(){
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
    
    quotes[1].toString();

    return 0;
}