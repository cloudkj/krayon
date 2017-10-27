#ifndef INC_SPHERE_H
#define INC_SPHERE_H

#include "Object.h"

class Sphere : public Object {

public:

    Sphere() {}
    ~Sphere() {}
    
    double getDistanceFromPoint(Point origin, Vector rayDirection);

    Color getColorAtPoint(std::vector<Light> & lights,
        std::vector<Object *> & objects,
        Color ambience, Point point,
        Vector incidentRay, double distanceFromPoint);

    Vector getReflectedRayAtPoint(Point point, Vector incidentRay,
        double distanceFromPoint);

    Point center;
    double radius;

protected:

    Vector getNormal(Point point, Vector incidentRay,
        double distanceFromPoint);

    Color getDiffuseTextureColorAtPoint(Point point);
};

#endif
