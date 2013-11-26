#include "work_20_6.hpp"

////////////////////////////////////////////////////////////////////////
// Copy from the book                                                 //
////////////////////////////////////////////////////////////////////////

Text_iterator &Text_iterator::operator++() {
    ++pos;
    if (pos == (*ln).end()) {
        ++ln;
        pos = (*ln).begin();
    }
    return *this;
}

istream &operator>>(istream &is, Document &d) {
    char ch;
    while (is.get(ch)) {
        d.line.back().push_back(ch);
        if (ch == '\n') {
            d.line.push_back(Line());
        }
    }

    return is;
}

void print(Document &d) {
    for (Text_iterator p = d.begin(); p != d.end(); ++p) {
        cout << *p;
    }
}

void erase_line(Document &d, int n) {
    if (n < 0 || d.line.size() <= n) return;
    d.line.erase(advance(d.line.begin(), n));
}

bool match(Text_iterator first, Text_iterator last, const string &s) {
    string::const_iterator p;

    for (
        p = s.begin();
        p != s.end() && first != last && *p == *first;
        ++p, ++first
    ) {}

    return p == s.end();
}

Text_iterator find_txt(Text_iterator first, Text_iterator last, const string &s) {
    if (s.size() == 0) return last;
    char first_char = s[0];

    while (1) {
        Text_iterator p = find(first, last, first_char);
        if (p == last || match(p, last, s)) {
            return p;
        }
    }
}

////////////////////////////////////////////////////////////////////////
// End of copy                                                        //
////////////////////////////////////////////////////////////////////////

// Task 2

void replace_txt(Text_iterator first, Text_iterator last, const string &s, const string &ns) {
    // The same as find_txt() here
    if (s.size() == 0) return;
    char first_char = s[0];

    while (1) {
        Text_iterator p = find(first, last, first_char);
        if (p == last) {
            return;
        } else if (match(p, last, s)) {
            // Do replace
            // In one line, not support "\n"
            p.current_line().erase(p, advance <LineIter>(p, s.size()));
            p.current_line().insert(p, ns.begin(), ns.end());
            return;
        }
    }
}

// Task 3-5

unsigned int count(Document &d) {
    unsigned int result = 0;
    // Iterate the whole document
    for (Text_iterator p = d.begin(); p != d.end(); ++p, ++result) {}
    return result;
}

unsigned int count(Document &d, Counting_helper &ch) {
    unsigned int result = 0;
    unsigned int nowi = 0;
    // Iterate the whole document
    for (Text_iterator p = d.begin(); p != d.end(); ++p) {
        unsigned int newi = ch.get(*p);
        if (newi != nowi) {
            nowi = newi;
            // Check if it is new word
            if (nowi != 0) {
                ++result;
            }
        }
    }
    // Check last word
    if (nowi != 0) {
        ++result;
    }
    return result;
}

class CHWS: public Counting_helper {
private:
    string ws;
public:
    CHWS(string space): ws(space) {};

    unsigned int get(char c) {
        if (find(ws.begin(), ws.end(), c) != ws.end()) {
            // Whitespace
            return 0;
        } else {
            // Not whitespace
            return 1;
        }
    }
};

class CHN: public Counting_helper {
public:
    unsigned int get(char c) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            // Text
            return 1;
        } else if (c >= '0' && c <= '9') {
            // Number
            return 2;
        } else {
            // Unknown
            return 0;
        }
    }
};

unsigned int count_unit(Document &d) {
    CHWS c(" ");
    return count(d, c);
}

unsigned int count_name(Document &d) {
    CHN c;
    return count(d, c);
}

unsigned int count_ws(Document &d, const string &ws) {
    CHWS c(ws);
    return count(d, c);
}
