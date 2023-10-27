#include <string>
#include <iostream>

using namespace std;

class Quote {
private:
  string name, quote, date;

public:
  Quote() {
    this->name = "";
    this->quote = "";
    this->date = "";
  }

  Quote(string q, string d) {
    this->name = "";
    this->quote = q;
    this->date = d;
  }

  Quote(string n, string q, string d) {
    this->name = n;
    this->quote = q;
    this->date = d;
  }

  Quote stringToQuote(string data) {
    string quote_split;
    int position = data.find(",");
    string name = data.substr(0, position);
    string after = data.substr(position + 1);
    position = after.find(",");
    while (after.substr(position + 1, 1) == " ") {
      quote_split = after.substr(position + 1);
      position += quote_split.find(",") + 1;
    }
    string quote = after.substr(0, position);
    after = after.substr(position + 1, 10);
    Quote newQuote = Quote(name, quote, after);
    return newQuote;
  }
  
  void printName(){
    cout << "Author: " << this->name << endl;
  }

  void printQuote(){
    cout << "Quote: " << this->quote << endl;
  }

  void printDate(){
    cout << "Date: " << this->date << endl;
  }

  void toString() {
    cout << "Author: " << this->name << "\nQuote: " << this->quote
    << "\nDate: " << this->date << endl;
  }

  void setName(string n) { this->name = n; }
  void setQuote(string q) { this->quote = q; }
  void setDate(string d) { this->date = d; }

  string getName() { return this->name; }
  string getQuote() { return this->quote; }
  string getDate() { return this->date; }
};