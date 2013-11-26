#include <stdexcept>
#include <iostream>
#include <ios>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <complex>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <climits>

using namespace std;

////////////////////////////////////////////////////////////////////////
// Copy from the book                                                 //
////////////////////////////////////////////////////////////////////////

typedef vector <char> Line;
typedef Line::iterator LineIter;

class Text_iterator {
    list <Line>::iterator ln;
    LineIter pos;
public:
    typedef forward_iterator_tag iterator_category;
    typedef char value_type;
    typedef size_t difference_type;
    typedef char *pointer;
    typedef char &reference;

    Text_iterator(list <Line>::iterator ll, Line::iterator pp)
        :ln(ll), pos(pp) {}

    char &operator*() {
        return *pos;
    };

    Text_iterator &operator++();

    bool operator==(const Text_iterator &other) const {
        return ln == other.ln && pos == other.pos;
    }

    bool operator!=(const Text_iterator &other) const {
        return !(*this==other);
    }

    // For task 2
    Line &current_line() {
        return *ln;
    }

    // For task 2
    // Cast to line iter, for text operation
    operator LineIter(){
        return pos;
    }
};

struct Document {
    list <Line> line;

    Document() {
        line.push_back(Line());
    }

    Text_iterator begin() {
        return Text_iterator(line.begin(), (*line.begin()).begin());
    }

    Text_iterator end() {
        list <Line>::iterator last = line.end();
        --last;
        return Text_iterator(last, (*last).end());
    }
};

istream &operator>>(istream &is, Document &d);
void print(Document &d);
void erase_line(Document &d, int n);
bool match(Text_iterator first, Text_iterator last, const string &s);
Text_iterator find_txt(Text_iterator first, Text_iterator last, const string &s);

////////////////////////////////////////////////////////////////////////
// End of copy                                                        //
////////////////////////////////////////////////////////////////////////

// Task 1

template <class Iter>
Iter advance(Iter p, int n) {
    // Value of n will never cross 0
    // Forward
    while (n > 0) {
        ++p;
        --n;
    }
    // Backward
    while (n < 0) {
        --p;
        ++n;
    }

    return p;
}

// Task 2
// See the .cpp file

void replace_txt(Text_iterator first, Text_iterator last, const string &s, const string &ns);

// Task 3-5

class Counting_helper {
public:
    // Return the index of group of a character
    // Zero means whitespace
    virtual unsigned int get(char c) = 0;
};

unsigned int count(Document &d); // Task 3
unsigned int count(Document &d, Counting_helper &ch);
unsigned int count_unit(Document &d); // Task 4A
unsigned int count_name(Document &d); // Task 4B
unsigned int count_ws(Document &d, const string &ws); // Task 5
