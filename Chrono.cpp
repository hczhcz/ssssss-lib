#include "Chrono.h"

namespace Chrono {

// info and forward declaration

int days_in_month(int y, Month m);
int days_in_year(int y);
int week_of_year(const Date& d);
Day day_of_week(const Date& d);
int linear_day(Date a);
Date date_from_linear(int n);

const Date first_date(1600,jan,1);    // don't try dates anywhere near this far back in time
                                            // the calendar calculation code is brittle the first date
                                            // must be the first day of a year divisible by 400
const Day first_day = monday;               //Januaray 1, 1600 would have been a Monday (had they had the Georgian calendar)

const Date first_date2(1970,jan,1);   // first date under UNIX, for Date_2 unit
const int linear_first = linear_day(first_date2);


const Date& default_date()
{
    static const Date dd(2001,jan,1); // start of 21st century
    return dd;
}

// member function definitions:

// Date

Date::Date(int yy, Month mm, int dd)
    : y(yy), m(mm), d(dd)
{
    if (!is_date(yy,mm,dd))
        error("Date(): invalid date");
}

Date::Date()
    :y(default_date().year()),
     m(default_date().month()),
     d(default_date().day())
{
}

void Date::add_day(int n)
{
    // use linear_day() below or things get really messy
    if (n<0) error("add_day(): can't handle negative n");   // not yet
    while (days_in_month(y,m)<n) {  // move one month at a time
        add_month(1);
        n -= days_in_month(y,m);
    }
    d += n;
}


void Date::add_month(int n)
{
    if (n<0) error("add_month(): can't handle negative n");    // not yet
    while (12<n) {  // move one month at a time
        add_year(1);
        n -= 12;
    }
    for (; n>0; --n,++m);
}


void Date::add_year(int n)
{
    if (m==feb && d==29 && !leapyear(y+n)) { // beware of leap years!
            // makes sense for both positive and negative n (n==0 should be impossible here)
            m = mar;        // use March 1 instead of February 29
            d = 1;
    }
    y+=n;
}

void Date::write()   // debug output function
{
    Date& d = *this;
    cout << d << ": " << day_of_week(d) << "; linear: " << linear_day(d) << "; of week #" << week_of_year(d) << '\n';
}

// Date_2

Date_2::Date_2(int y, Month m, int d)
{
    linear = linear_day(Date(y,m,d)) - linear_first;
    if (linear < 0) error("Date_2(): can't handle days before (1970,jan,1)");
}

Date_2::Date_2(): linear(0) {}

int Date_2::day()
{
    return date_from_linear(linear).day();
}

Month Date_2::month()
{
    return date_from_linear(linear).month();
}

int Date_2::year()
{
    return date_from_linear(linear).year();
}

void Date_2::write()
{
    date_from_linear(linear).write();
}

void Date_2::add_day(int n)
{
    if (n<0) error("add_day(): can't handle negative n");
    linear += n;
}
        
void Date_2::add_month(int n)
{
    Date d1 = date_from_linear(linear);
    d1.add_month(n);
    linear = linear_day(d1);
}

void Date_2::add_year(int n)
{
    Date d1 = date_from_linear(linear);
    d1.add_year(n);
    linear = linear_day(d1);
}

Date_2::operator Date()
{
    return date_from_linear(linear);
}

//------------------------------------------------------------------------------

// helper functions, etc.:

int days_in_month(int y, Month m)
{
    switch (m) {
    case feb:                        // the length of February varies
        return (leapyear(y))?29:28;
    case apr: case jun: case sep: case nov:
        return 30;
    default:
        return 31;
    }
}

int days_in_year(int y)
{
    if (leapyear(y)) {
        return 366;
    }
    else {
        return 365;
    }
}

bool is_date(int y, Month  m, int d)
{
    // assume that y is valid

    if (d<=0) return false;            // d must be positive
    if (days_in_month(y,m)<d) return false;

    return true;
} 


bool leapyear(int y)
{
    return ((y%4 == 0) && (y%100 != 0)) || (y%400 == 0);
}


bool operator==(const Date& a, const Date& b)
{
    return a.year()==b.year()
        && a.month()==b.month()
        && a.day()==b.day();
}


bool operator!=(const Date& a, const Date& b)
{
    return !(a==b);
}


ostream& operator<<(ostream& os, const Date& d)
{
    return os << '(' << d.year()
              << ',' << d.month()
              << ',' << d.day() 
              << ')';
}


istream& operator>>(istream& is, Date& dd)
{
    int y, m, d;
    char ch1, ch2, ch3, ch4;
    is >> ch1 >> y >> ch2 >> m >> ch3 >> d >> ch4;
    if (!is) return is;
    if (ch1!='(' || ch2!=',' || ch3!=',' || ch4!=')') { // oops: format error
        is.clear(ios_base::failbit);                    // set the fail bit
        return is;
    }
    dd = Date(y,Month(m),d);     // update dd
    return is;
}

ostream& operator<<(ostream& os, Day d)
    // sloppy: I should have used a table
{
    switch (d) {
    case sunday:
        os << "Sunday";
        break;
    case monday:
        os << "Monday";
        break;
    case tuesday:
        os << "Tuesday";
        break;
    case wednesday:
        os << "Wednesday";
        break;
    case thursday:
        os << "Thursday";
        break;
    case friday:
        os << "Friday";
        break;
    case saturday:
        os << "Saturday";
        break;
    }
    return os;
}

int nmonth(int m)
    // number of days before the first day of month #m (january is #1) ignoring leap days
{
    switch(m)
    {
    case jan: return 0;
    case feb: return 31;
    case mar: return 31+28;
    case apr: return 31+28+31;
    case may: return 31+28+31+30;
    case jun: return 31+28+31+30+31;
    case jul: return 31+28+31+30+31+30;
    case aug: return 31+28+31+30+31+30+31;
    case sep: return 31+28+31+30+31+30+31+31;
    case oct: return 31+28+31+30+31+30+31+31+30;
    case nov: return 31+28+31+30+31+30+31+31+30+31;
    case dec: return 31+28+31+30+31+30+31+31+30+31+30;
    default: error("nmonth(): bad month"); return 0;
    }
}


int day_in_year(Date a)
    // e.g. Jan 1 is day #1, Jan 2 is day #1 and feb 3 is day #34
{
    int m = nmonth(a.month());
    if (feb<a.month() && leapyear(a.year())) ++m; // adjust if we passed a dat added for a leapyear
    return m+a.day();
}


int nleaps(int y)
    // number of leapyears between Jan 1, y and first_year
    // first_year must be divisible by 400
{
    const int yy = y-first_date.year();
    return yy/4 - yy/100 + yy/400; // number of leapyears
}

int linear_day(Date a)
    // days since default date
{
    int y = a.year()-first_date.year();
    if (y<0) error("linear_day(): can't handle days before (1601,jan,1)");
    int m = a.month()-first_date.month();
    int d = a.day()-first_date.day();
    /*
        If it wasn't for leap years and different lengths of month the answer would be
            365*y+31*m+d
        However, ther real world (the real physical world + conventions) is not that simple.
    */
    if (y==0 && m==0) return d; // same month
    int days_in_years = 365*y+nleaps(a.year());
    return days_in_years+day_in_year(a)-1;
}

Date date_from_linear(int n)
    // compose the Date (2001,jan,1)+n
{
    return Date(first_date.year(),first_date.month(),first_date.day()+n);   // rather limited implementatiuon :-)
}

int operator-(Date a, Date b)
    // how many days are there between a and b?
    // if b is earlier than a the answer will be negative
{
    int aval = linear_day(a);
    int bval = linear_day(b);
    return aval-bval;
}


Date operator+(const Date& d, int dd)
    // dd days beyond d
{
    Date x = d;
    x.add_day(dd);
    return x;
}

/*
    Note the difference between + and - for Date.
    It makes sense to subtract two dates, but not to add two dates
*/


Day day_of_week(const Date& d)
    // ``just count the days since the start of (our) dates''
{
    int x = first_day + linear_day(d);
    return Day(x%7);                // every week is 7 days
}


Date next_Sunday(const Date& d)
{
    Day dd = day_of_week(d);
    Date ns = d;
    ns.add_day(7-dd);
    return ns;
}


Date next_workday(const Date& d)
    // assume that Saturday and Sunday are not weekdays
{
    Day dd = day_of_week(d);
    Date n = d;
    switch(dd) {
    case friday:
        n.add_day(3);
        return n;
    case saturday:
        n.add_day(2);
        return n;
    default:
        n.add_day(1);
        return n;
    }
}

int week_of_year(const Date& d)
    // the number of a week in a year is defined by ISO 8601:
    //  week #1 is the week with the year's first Thursday in it
    //  Monday is the first day of the week

    // 0 means that the date is in the last week ofthe previous year
{
    int diy = day_in_year(d);   // jan 1 is day #1
    Day jan1 = day_of_week(Date(d.year(),jan,1));
    int week1 = 0;  // Jan 1 is in the last week of the previous year
    int delta;
    switch(jan1) {
        // Jan 1 is in the first week of the year
    case monday:
        delta = 0;
        break;
    case tuesday:
        delta = 1;
        break;
    case wednesday:
        delta = 2;
        break;
    case thursday: 
        delta = 3;
        break;
        // Jan 1 is in the last week of the previous year
    case friday:
        delta = -3;
        break;
    case saturday:
        delta = -2;
        break;
    case sunday:
        delta = -1;
        break;
    }

    return (diy+delta)/7;
}


//------------------------------------------------------------------------------

} // Chrono
