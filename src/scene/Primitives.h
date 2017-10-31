#ifndef INC_PRIMITIVES_H
#define INC_PRIMITIVES_H

#include <vector>
#include <math.h>

const double PI = 3.14159265358979323846;

class Color {

public:

    double red;
    double green;
    double blue;

    Color() {}
    Color(double red, double green, double blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
    ~Color() {}

    void normalize() {
        if (red > 1.0) red = 1.0;
        if (green > 1.0) green = 1.0;
        if (blue > 1.0) blue = 1.0;
    }

    void adjust(double coefficient) {
        red *= coefficient;
        green *= coefficient;
        blue *= coefficient;
    }

    void add(Color c) {
        red += c.red;
        green += c.green;
        blue += c.blue;
    }

    double difference(Color c) {
        double r = red - c.red;
        double g = green - c.green;
        double b = blue - c.blue;
        return r*r + g*g + b*b;
    }

    static Color averageColor(std::vector<Color> colors) {
        Color result(0, 0, 0);
        for (int i = 0; i < colors.size(); i++) {
            result.add(colors[i]);
        }
        result.red /= colors.size();
        result.green /= colors.size();
        result.blue /= colors.size();
        return result;
    }
};

class Point {

public:

    double x;
    double y;
    double z;

    Point() {}
    Point(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    ~Point() {}
};

class Vector {

public:

    double x;
    double y;
    double z;

    Vector() {}
    Vector(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    ~Vector() {}

    double magnitude() {
	    return sqrt(x*x + y*y + z*z);
    }

    void normalize() {
        double m = magnitude();
        x /= m;
        y /= m;
        z /= m;
    }

    double dotProduct(Vector v) {
	    return x*v.x + y*v.y + z*v.z;
    }

    Vector crossProduct(Vector v) {
        Vector result;
        result.x = (y * v.z) - (z * v.y);
        result.y = (z * v.x) - (x * v.z);
        result.z = (x * v.y) - (y * v.x);
        return result;
    }
};

#endif
