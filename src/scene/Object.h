#ifndef INC_OBJECT_H
#define INC_OBJECT_H

#include <vector>
#include "Primitives.h"
#include "Texture.h"

class Camera {

public:

    Camera() {}
    ~Camera() {}

    double fieldOfView;
    Point position;
    Vector look;
    Vector up;

    Vector xBasis;
    Vector yBasis;
    Vector zBasis;

    void initBasis() {
        yBasis.x = up.x;
        yBasis.y = up.y;
        yBasis.z = up.z;
        yBasis.normalize();

        zBasis.x = -look.x;
        zBasis.y = -look.y;
        zBasis.z = -look.z;
        zBasis.normalize();

        xBasis = yBasis.crossProduct(zBasis);
        xBasis.normalize();
    }
};

class Light {

public:

    Light() {}
    ~Light() {}

    Point position;
    Color intensity;
};

class Object {

public:

    Object() {}
    virtual ~Object() {}

    virtual double getDistanceFromPoint(Point origin, Vector rayDirection) = 0;

    Color getColorAtPoint(std::vector<Light> & lights,
                          std::vector<Object *> & objects,
                          Color ambience,
                          Point point,
                          Vector incidentRay,
                          double distanceFromPoint);

    Vector getReflectedRayAtPoint(Point point, Vector incidentRay, double distanceFromPoint);

    Color ambient;
    Color diffuse;
    Color specular;

    int specularity;
    double reflectivity;

    Texture texture;

protected:

    Color getDiffuseColorAtPoint(std::vector<Light> & lights,
                                 std::vector<Object *> & objects,
                                 Point point,
                                 Vector normal);

    Color getSpecularColorAtPoint(std::vector<Light> & lights,
                                  std::vector<Object *> & objects,
                                  Point point,
                                  Vector incidentRay,
                                  Vector normal);

    bool pointIsInShadow(std::vector<Object *> & objects, Point point, Vector lightDirection);

    virtual Vector getNormal(Point point, Vector incidentRay, double distanceFromPoint) = 0;

    virtual Color getDiffuseTextureColorAtPoint(Point point) = 0;
};

#endif
