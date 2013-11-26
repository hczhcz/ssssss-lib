#ifndef __SHAPE_BASE__
#define __SHAPE_BASE__
#define _USE_MATH_DEFINES

#include <cmath>
#include <string>

template <typename value> class Shape;
template <typename value> class Shape2D;
template <typename value> class Shape3D;

// Base class of shape
template <typename value>
class Shape {
public:
    // Return the name of the shape
    virtual std::string name() {return "Unknown";}
};

// Shape in 2D area
template <typename value>
class Shape2D: public Shape <value> {
public:
    // Name
    std::string name() {return "Unknown 2D";}

    // Perimeter of the shape
    virtual value getPerimeter() = 0;

    // Area of the shape
    virtual value getArea() = 0;
};

// Shape in 3D area
template <typename value>
class Shape3D: public Shape <value> {
public:
    // Name
    std::string name() {return "Unknown 3D";}

    // Surface area of the shape
    virtual value getArea() = 0;

    // Volume of the shape
    virtual value getVolume() = 0;
};

#endif
