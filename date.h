// struct for Date object

#include <iostream>
using namespace std;

class Date {
protected:
  int day;
  int month;
  int year;
public:
  void setDate (int d, int m, int y);
  string display ();
};

