#include <string>
#include <fstream>
#include <iostream>
#include "post.h"
using namespace std;

Post:: Post(){
  comment= " ";
}

Post::Post(istream& is) { //pulls post info from file and sets state varaibles to it
  string blankline;
  getline(is, blankline);
  getline(is, postDate);
  getline(is, name);
  getline(is, comment);
}

Post::Post(string a, string c, string d){ //sets postDate, name, comment
  postDate=a;
  name=c;
  comment=d;
}

void Post::display(ofstream & out) { //outputs info for post into textfile
  out << endl;
  out << postDate << endl;
  out << name << endl;
  out << comment << endl;
}
