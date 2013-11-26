#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>

inline void error(const std::string& s)
{
    throw std::runtime_error(s);
}

namespace Chrono {
using namespace std;

enum Month {
    jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};

enum Day {
    sunday=0, monday, tuesday, wednesday, thursday, friday, saturday
};

class Date {
public:
    Date(int y, Month m, int d);       // check for valid date and initialize
    Date();                            // default constructor
    // the default copy operations are fine

    // non-modifying operations:
    int   day()   const { return d; }
    Month month() const { return m; }
    int   year()  const { return y; }
    void write();

    // modifying operations:
    void add_day(int n);        
    void add_month(int n);
    void add_year(int n);
private:
    int   y;
    Month m;
    int   d;
};

class Date_2 {
public:
    Date_2(int y, Month m, int d);       // check for valid date and initialize
    Date_2(int l);                       // construct using linear date
    Date_2();                            // default constructor
    // the default copy operations are fine

    // non-modifying operations:
    int   day();
    Month month();
    int   year();
    void write();

    // modifying operations:
    void add_day(int n);        
    void add_month(int n);
    void add_year(int n);

    // I'm lazy..XD // cast Date_2 to Date in other function
    operator Date();
private:
    int linear;
};

bool is_date(int y, Month m, int d); // true for valid date

bool leapyear(int y);                  // true if y is a leap year

bool operator==(const Date& a, const Date& b);
bool operator!=(const Date& a, const Date& b);

ostream& operator<<(ostream& os, const Date& d);
istream& operator>>(istream& is, Date& dd);

const Date& default_date();

} // Chrono
