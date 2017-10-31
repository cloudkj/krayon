#ifndef INC_PLANE_H
#define INC_PLANE_H

#include "Object.h"

class Plane : public Object {

public:

    Plane() {}
    ~Plane() {}

    double getDistanceFromPoint(Point origin, Vector rayDirection);

    Color getColorAtPoint(std::vector<Light> & lights,
                          std::vector<Object *> & objects,
                          Color ambience,
                          Point point,
                          Vector incidentRay,
                          double distanceFromPoint);

    Vector getReflectedRayAtPoint(Point point, Vector incidentRay, double distanceFromPoint);

    Point p1;
    Point p2;
    double a;
    double b;
    double c;
    double d;

protected:

    Vector getNormal(Point point, Vector incidentRay, double distanceFromPoint);

    Color getDiffuseTextureColorAtPoint(Point point);
};

#endif
