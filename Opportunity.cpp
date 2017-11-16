#include "Opportunity.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

Opportunity::Opportunity(istream& is) { //reads in opportunity info from textfile and assigns state variables to it
  string blankline;
  getline(is, blankline);
  getline(is, title);
  getline(is, description);
  getline(is, qualifications);
  getline(is, deadline);
  getline(is, link);
}

void Opportunity::display() {
  //displays info in GUI
  cout << title << endl;
  cout << description << endl;
  cout << qualifications << endl;
  cout << deadline << endl;
  cout << link << endl;
  cout << endl;
}

char* Opportunity::getTitle() { //returns title as a char array
  int len = title.length();
  char* newTitle = new char [len];
  for (int i=0; i<len+1; i++)
    newTitle[i] = title[i];
  return newTitle;
}

char* Opportunity::getDate() { //returns date as a char array
  int len = deadline.length();
  char* date = new char [len];
  for (int i=0; i<len+1; i++)
    date[i] = deadline[i];
  return date;
}

char* Opportunity::getDescription() { //returns description as a char array
  int len = description.length();
  char* newDescription = new char [len];
  for (int i=0; i<len+1; i++)
    newDescription[i] = description[i];
  return newDescription;
}

char* Opportunity::getQualifications() { //returns qualifications as a char array
  int len = qualifications.length();
  char* qual = new char [len];
  for (int i=0; i<len+1; i++)
    qual[i] = qualifications[i];
  return qual;
}

char* Opportunity::getLink() { //returns link as a char array
  int len = link.length();
  char* newLink = new char [len];
  for (int i=0; i<len+1; i++)
    newLink[i] = link[i];
  return newLink;
}


