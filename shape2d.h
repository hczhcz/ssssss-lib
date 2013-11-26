#ifndef __SHAPE_2D__
#define __SHAPE_2D__

#include "shape.h"

// Circle, a shape with radius
template <typename value>
class Circle: public Shape2D <value> {
private:
    value radius;
public:
    // Init
    Circle(value r): radius(r) {}

    // Name
    std::string name() {return "Circle";}

    // Perimeter
    value getPerimeter() {return 2 * M_PI * radius;}

    // Area
    value getArea() {return M_PI * radius * radius;}

    // Return the radius, read only
    value getRadius() {return radius;}

    // Change the radius
    void setRadius(value r) {radius = r;}
};

// Rect, a shape with 2D size (width and height)
template <typename value>
class Rect: public Shape2D <value> {
private:
    value sizeX;
    value sizeY;
public:
    // Init
    Rect(value x, value y): sizeX(x), sizeY(y) {}

    // Name
    std::string name() {return "Rectangle";}

    // Perimeter
    value getPerimeter() {return 2 * sizeX + 2 * sizeY;}

    // Area
    value getArea() {return sizeX * sizeY;}

    // Return the size x, read only
    value getSizeX() {return sizeX;}

    // Return the size y, read only
    value getSizeY() {return sizeY;}

    // Change the size x
    void setSizeX(value x) {sizeX = x;}

    // Change the size y
    void setSizeY(value y) {sizeY = y;}
};

// Square, a shape like rect but use one size (width equal to height)
template <typename value>
class Square: public Shape2D <value> {
private:
    value size;
public:
    // Init
    Square(value s): size(s) {}

    // Name
    std::string name() {return "Square";}

    // Perimeter
    value getPerimeter() {return 4 * size;}

    // Area
    value getArea() {return size * size;}

    // Return the size, read only
    value getSize() {return size;}

    // Change the size
    void setSize(value s) {size = s;}

    // Cast to rectangle
    operator Rect <value>() {return Rect <value>(size, size);}
};

#endif
