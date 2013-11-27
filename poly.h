#ifndef __POLY_BASE__
#define __POLY_BASE__
#include <vector>
#include <iostream>

namespace poly {
    using namespace std;

    template <class T = double, class Alloc = allocator<T> >
    class vectorpoly: public vector <T, Alloc> {
    private:
        // Type of vector
        typedef vector <T, Alloc> VctType;
        // Type of itself
        typedef vectorpoly <T, Alloc> SelfType;
    public:
        // Construction
        vectorpoly(): VctType(1, 1) {}
        vectorpoly(const size_t s): VctType(s) {}
        vectorpoly(const size_t s, const T &x): VctType(s, x) {}
        vectorpoly(const VctType &x): VctType(x) {}
        vectorpoly(const SelfType &x): VctType(x) {}
        // Literal construction, c++11 only
#if __cplusplus >= 201103L
        vectorpoly(std::initializer_list <T> x): VctType(x) {}
#endif

        // Destruction
        ~vectorpoly() {/* Nothing to do */}

        // Assignation
        SelfType &operator=(const VctType &x) {return VecType(*this) = x;}
        SelfType &operator=(const SelfType &x) {return VecType(*this) = x;}
        // R-value ref assignation, c++11 only
#if __cplusplus >= 201103L
        SelfType &operator=(SelfType &&x) {return VecType(*this) = x;}
#endif

        // Add
        SelfType operator+(const SelfType &x) {
            SelfType result(0);

            // Iterate both vector
            typename VctType::const_iterator iter1 = this->begin();
            typename VctType::const_iterator iter2 = x.begin();
            for (;iter1 != this->end() || iter2 != x.end();) {
                T newdata = 0;
                // Do add
                if (iter1 != this->end()) {newdata += *iter1; ++iter1;}
                if (iter2 != x.end()) {newdata += *iter2; ++iter2;}

                result.push_back(newdata);
            }

            return result;
        }

        // Sub
        SelfType operator-(const SelfType &x) {
            SelfType result(0);

            // Iterate both vector
            typename VctType::const_iterator iter1 = this->begin();
            typename VctType::const_iterator iter2 = x.begin();
            for (;iter1 != this->end() || iter2 != x.end();) {
                T newdata = 0;
                // Do sub
                if (iter1 != this->end()) {newdata += *iter1; ++iter1;}
                if (iter2 != x.end()) {newdata -= *iter2; ++iter2;}

                result.push_back(newdata);
            }

            return result;
        }

        // Mul
        SelfType operator*(const SelfType &x) {
            SelfType result(this->size() + x.size() - 1);

            // Nested iteration
            for (typename VctType::const_iterator iter1 = this->begin(); iter1 != this->end(); ++iter1)
                for (typename VctType::const_iterator iter2 = x.begin(); iter2 != x.end(); ++iter2) {
                    // Do mul
                    result[(iter1 - this->begin()) + (iter2 - x.begin())] +=
                        (*iter1) * (*iter2);
                }

            return result;
        }

        // Pos
        SelfType operator+() {
            // Just return self
            return *this;
        }

        // Neg
        SelfType operator-() {
            SelfType result(0);

            // Iterate self
            for (typename VctType::const_iterator iter1 = this->begin(); iter1 != this->end(); ++iter1) {
                result.push_back(-*iter1);
            }

            return result;
        }

        bool operator==(const SelfType &x) {
            // Iterate both vector
            typename VctType::const_iterator iter1 = this->begin();
            typename VctType::const_iterator iter2 = x.begin();
            for (;iter1 != this->end() || iter2 != x.end();) {
                T val1 = 0;
                T val2 = 0;

                // Get value
                if (iter1 != this->end()) {val1 = *iter1; ++iter1;}
                if (iter2 != x.end()) {val2 *iter2; ++iter2;}

                if (val1 != val2) return 0;
            }

            return 1;
        }

        bool operator!=(const SelfType &x) {
            return !(*this == x);
        }

    };

    // Input
    template <class T, class Alloc = allocator<T> >
    istream &operator<<(istream &is, vectorpoly <T, Alloc> x) {
        // Input size
        size_t s;
        is >> s;
        x.resize(s);

        // Input data
        for (typename vector <T, Alloc>::iterator iter = x.begin(); iter != x.end(); ++iter) {
            is >> *iter;
        }

        return is;
    }

    // Output
    template <class T = double, class Alloc = allocator<T>, char Spe = ' ', bool Friendly = 1>
    ostream &operator<<(ostream &os, const vectorpoly <T, Alloc> &x) {
        if (Friendly) {
            // Call print()
            print(x, os);
        } else {
            // Output size
            os << x.size();

            // Output data
            for (typename vector <T, Alloc>::const_iterator iter = x.begin(); iter != x.end(); ++iter) {
                os << Spe << *iter;
            }
        }

        return os;
    }

    // Formatted output
    template <class T = double, class Alloc = allocator<T> >
    void print(const vectorpoly <T, Alloc> &x, ostream &os = cout) {
        bool showsign = 0;
        for (typename vector <T, Alloc>::const_iterator iter = x.begin(); iter != x.end(); ++iter) {
            if (*iter != 0) {
                const size_t iterpos = iter - x.begin();
                if (showsign && *iter > 0) os << '+';
                if (showsign && *iter == -1) os << '-';
                showsign = 1;
                if (iterpos == 0 || (*iter != 1 && *iter != -1)) os << *iter;
                if (iterpos > 0) os << 'x';
                if (iterpos > 1) os << '^' << iterpos;
            }
        }
    }
}

#endif
