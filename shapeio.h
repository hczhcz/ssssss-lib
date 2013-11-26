#ifndef __SHAPE_IO__
#define __SHAPE_IO__

#include <iostream>
#include "shape.h"
#include "shape2d.h"

// Limit range of number
template <typename value>
value lim(value x, value low, value hi) {
    if (x < low) return low;
    if (x > hi) return hi;
    return x;
}

// Interface of char-drawable object
class Drawable {
public:
    // Draw the object to a string
    virtual std::string draw() = 0;
};

// Output a drawable object to an output stream
std::ostream &operator<<(std::ostream &o, Drawable &d) {
    o << d.draw();
    return o;
}

// Drawable rect
// Print example:
// *****
// *   *
// *****
template <char sign, char fill>
class DrawableRect: public Rect <int>, public Drawable {
public:
    // Init, check the size
    DrawableRect(int x, int y): Rect(lim(x, 1, 19), lim(y, 1, 19)) {
    }

    // Draw
    std::string draw() {
        // Line 1
        std::string result = "";
        for (int i = 0; i < getSizeX(); ++i) {
            result += sign;
        }
        result += '\n';

        // Middle
        if (getSizeY() > 2) for (int i = 0; i < getSizeY() - 2; ++i) {
            // Column 0
            result += sign;

            // Middle
            if (getSizeX() > 2) for (int j = 0; j < getSizeX() - 2; ++j) {
                result += fill;
            }

            // Column n
            if (getSizeX() > 1) result += sign;
            result += '\n';
        }

        // Line n
        if (getSizeY() > 1) for (int i = 0; i < getSizeX(); ++i) {
            result += sign;
        }
        result += '\n';

        return result;
    }
};

#endif
