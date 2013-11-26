#ifndef __SHAPE_3D__
#define __SHAPE_3D__

#include "shape.h"
#include "shape2d.h"

// Cylinder-like shape, shape with height and a cross section
template <typename value, typename sectype>
class CylinderLike: public Shape3D <value> {
protected:
    sectype section;
    value height;
public:
    // Init
    CylinderLike(sectype s, value h): section(s), height(h) {}

    // Name
    std::string name() {return "Unknown cylinder-like";}

    // Area
    value getArea() {return height * section.getPerimeter() + 2 * section.getArea();}

    // Volume
    value getVolume() {return height * section.getArea();}

    // Return the cross section, read only
    sectype getSection() {return section;}

    // Return the height, read only
    value getHeight() {return height;}

    // Change the cross section
    void setSection(sectype s) {section = s;}

    // Change the height
    void setHeight(value h) {height = h;}
};

// Cylinder, a cylinder-like shape with a circle as its cross section
template <typename value>
class Cylinder: public CylinderLike <value, Circle <value> > {
public:
    // Init with radius and height
    Cylinder(value r, value h): CylinderLike<value, Circle <value> > (Circle <value>(r), h) {}

    // Name
    std::string name() {return "Cylinder";}

    // Return the radius, read only
    value getRadius() {return this->section.getRadius();}

    // Change the radius
    void setRadius(value r) {this->section.setRadius();}
};

// Cube, a cylinder-like shape with a rectangle as its cross section
template <typename value>
class Cube: public CylinderLike <value, Rect <value> > {
public:
    // Init with 2D size and height
    Cube(value x, value y, value z): CylinderLike<value, Rect <value> > (Rect <value>(x, y), z) {}

    // Name
    std::string name() {return "Cube";}

    // Return the size x, read only
    value getSizeX() {return this->section.getSizeX();}

    // Return the size y, read only
    value getSizeY() {return this->section.getSizeY();}

    // Return the size z, read only
    value getSizeZ() {return this->getHeight();}

    // Change the size x
    void setSizeX(value x) {this->section.setSizeX(x);}

    // Change the size y
    void setSizeY(value y) {this->section.setSizeY(y);}

    // Change the size z
    void setSizeZ(value z) {this->setHeight(z);}
};

#endif
