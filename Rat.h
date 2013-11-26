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

namespace Rat {
    using namespace std;

    template <class BaseT>
    struct rational {
    protected:
        // nume / deno
        BaseT nume;
        BaseT deno; // Always positive

    public:
        rational(BaseT a, BaseT b) {
            set_value(a, b);
        }

        void set_value(BaseT a, BaseT b) {
            if (b == 0) {
                // N / 0 or 0 / 0
                throw runtime_error("zero div");
            } else if (a == 0) {
                // 0 / N
                nume = 0;
                deno = 1;
            } else {
                // N1 / N2
                // = (sgn(N2) * N1 / gcd(N1, N2)) / (abs(N2) / gcd(N1, N2))
                if (b < 0) {
                    a = -a;
                    b = -b;
                }
                nume = a;
                deno = b;
                // Calculate GCD
                while (1) {
                    a = a % b;
                    if (a == 0) {
                        a = b;
                        break;
                    }
                    b = b % a;
                    if (b == 0) {
                        b = a;
                        break;
                    }
                }
                nume = nume / a;
                deno = deno / b;
            };
        }

        BaseT get_nume() const {return nume;}

        BaseT get_deno() const {return deno;}

        operator double() {
            return (double)nume / (double)deno;
        }

        operator int() {
            if (deno == 1) {
                return nume;
            } else {
                throw runtime_error("not int");
            }
        }
    };

    template <class BaseT>
    rational <BaseT> operator+(const rational <BaseT> &a, const rational <BaseT> &b) {
        return rational <BaseT>(a.get_nume() * b.get_deno() + a.get_deno() * b.get_nume(), a.get_deno() * b.get_deno());
    }

    template <class BaseT>
    rational <BaseT> operator-(const rational <BaseT> &a, const rational <BaseT> &b) {
        return rational <BaseT>(a.get_nume() * b.get_deno() - a.get_deno() * b.get_nume(), a.get_deno() * b.get_deno());
    }

    template <class BaseT>
    rational <BaseT> operator*(const rational <BaseT> &a, const rational <BaseT> &b) {
        return rational <BaseT>(a.get_nume() * b.get_nume(), a.get_deno() * b.get_deno());
    }

    template <class BaseT>
    rational <BaseT> operator/(const rational <BaseT> &a, const rational <BaseT> &b) {
        return rational <BaseT>(a.get_nume() * b.get_deno(), a.get_deno() * b.get_nume());
    }

    template <class BaseT>
    bool operator==(const rational <BaseT> &a, const rational <BaseT> &b) {
        // Already simplified
        return a.get_nume() == b.get_nume() && a.get_deno() == b.get_deno();
    }

    template <class BaseT>
    bool operator!=(const rational <BaseT> &a, const rational <BaseT> &b) {
        return !(a == b);
    }

    template <class BaseT>
    bool operator<(const rational <BaseT> &a, const rational <BaseT> &b) {
        return a.get_nume() * b.get_deno() < b.get_nume() * a.get_deno();
    }

    template <class BaseT>
    bool operator>(const rational <BaseT> &a, const rational <BaseT> &b) {
        return a.get_nume() * b.get_deno() > b.get_nume() * a.get_deno();
    }

    template <class BaseT>
    bool operator<=(const rational <BaseT> &a, const rational <BaseT> &b) {
        return a.get_nume() * b.get_deno() <= b.get_nume() * a.get_deno();
    }

    template <class BaseT>
    bool operator>=(const rational <BaseT> &a, const rational <BaseT> &b) {
        return a.get_nume() * b.get_deno() >= b.get_nume() * a.get_deno();
    }

    template <class BaseT>
    rational <BaseT> operator+(const rational <BaseT> &a) {
        return a;
    }

    template <class BaseT>
    rational <BaseT> operator-(const rational <BaseT> &a) {
        return rational <BaseT>(-a.get_nume(), a.get_deno());
    }

    template <class BaseT>
    ostream &operator<<(ostream &os, const rational <BaseT> &a) {
        return os << a.get_nume() << " / " << a.get_deno();
    }
}
