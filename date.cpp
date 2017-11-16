#include <iostream>
#include "date.h"
using namespace std;

void Date::setDate (int d, int m, int y)
{
  if ( d<0 || d>31 || m<0 || m>12 )
    cout << "Error: incorrect date entry" << endl;
  else {
    day = d;
    month = m;
    year = y;
  }
}

string Date::display ()
{
  string str = to_string(month) + "/" + to_string(day) + "/" + to_string(year);
  return str;
}
