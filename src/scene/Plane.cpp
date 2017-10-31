#include "Plane.h"

#include <float.h>
#include <math.h>
#include <iostream>
#include <vector>

double Plane::getDistanceFromPoint(Point point, Vector rayDirection) {

    double vd = this->a * rayDirection.x
        + this->b * rayDirection.y
        + this->c * rayDirection.z;

    // Ray is parallel to the plane
    if (vd < FLT_EPSILON && vd > -FLT_EPSILON) {
        return DBL_MAX;
    }

    double v0 = - (this->a * point.x + this->b * point.y +
        this->c * point.z + this->d);

    double dist = v0 / vd;

    // Intersection is behind the origin
    if (dist < FLT_EPSILON) {
        return DBL_MAX;
    }

    return dist;
}

Vector Plane::getNormal(Point point, Vector incidentRay, double distanceFromPoint) {
    Vector normal(this->a, this->b, this->c);
    return normal;
}

Color Plane::getDiffuseTextureColorAtPoint(Point point) {

    int x, y;

    // Get plane basis
    Vector normal(this->a, this->b, this->c);
    normal.normalize();
    Vector a(this->p2.x - this->p1.x,
        this->p2.y - this->p1.y,
        this->p2.z - this->p1.z);
    a.normalize();
    Vector b = a.crossProduct(normal);
    b.normalize();

    // Transform to plane coordinates
    double xt = point.x - this->p1.x;
    double yt = point.y - this->p1.y;
    double zt = point.z - this->p1.z;
    double u = (xt * a.x) + (yt * a.y) + (zt * a.z);
    double v = (xt * b.x) + (yt * b.y) + (zt * b.z);

    Color *texturePixels = texture.getPixels();

    // Normalize to texture coordinates [0,1]
    x = ((x = (int) u % texture.getWidth()) < 0) ? x + texture.getWidth() : x;
    y = ((y = (int) v % texture.getHeight()) < 0) ? y + texture.getHeight() : y;
    int index = y * texture.getWidth() + x;

    Color result = texturePixels[index];
    
    return result;
}
