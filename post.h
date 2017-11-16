#include <string>
#include <fstream>
#include <iostream>
using namespace std;


class Post {
private:
  string comment; //what user says
  string postDate; //date user posts in forum
  string name; //name of user
public:
  Post();
  Post(istream& is); //pulls post info from file and sets state varaibles to it
  Post(string a, string c, string d); //sets postDate, name, comment
  void display(ofstream& out); //outputs info for post into textfile
};


struct Forum {
  int numPosts;
  Post ** posts;
};
