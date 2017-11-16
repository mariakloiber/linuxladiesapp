#include<iostream>
#include<fstream>
#include<string>
using namespace std;


enum elementType {Internship, Scholarship}; //determines whether opportunity is an internship or scholarship

class Opportunity {
 protected:
  string title; //title of opportunity
  string description; //description of opportunity
  string qualifications; //any qualifications for opportunity
  string link; //link to site of opportunity
  string deadline; //when opportunity's application is due
 public:
  Opportunity(istream& is);
  void display(); //displays opportunity in a terminal to test it
  char* getTitle(); //returns title
  char* getDate(); //returns date
  char* getDescription(); //returns description
  char* getQualifications(); //returns qualifications
  char* getLink(); //returns link
};

struct Data { //struct for database
  Opportunity ** opp; //array of pointers pointing to opportunities
  int numElements; //how many opportunities are in the array
  elementType currentType; //what type of opportunities are in the array
};
