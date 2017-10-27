#include "Sphere.h"

#include <float.h>
#include <math.h>
#include <iostream>
#include <vector>

double
Sphere::getDistanceFromPoint(Point point, Vector rayDirection) {

    Vector cameraToSphere(center.x - point.x,
        center.y - point.y,
        center.z - point.z);

    double closestApproach = rayDirection.dotProduct(cameraToSphere);

    if (closestApproach < FLT_EPSILON) {
        return DBL_MAX;
    }

    double halfCordSquared = (radius * radius) -
        cameraToSphere.dotProduct(cameraToSphere) + 
        (closestApproach * closestApproach);

    if (halfCordSquared < FLT_EPSILON) {
        return DBL_MAX;
    }

    return closestApproach - sqrt(halfCordSquared);
}

Vector
Sphere::getNormal(Point point, Vector incidentRay, double distanceFromPoint) {

    Point intersect(point.x + incidentRay.x * distanceFromPoint,
        point.y + incidentRay.y * distanceFromPoint,
        point.z + incidentRay.z * distanceFromPoint);

    Vector normal((intersect.x - center.x) / radius,
        (intersect.y - center.y) / radius,
        (intersect.z - center.z) / radius);

    return normal;
}

Color
Sphere::getDiffuseTextureColorAtPoint(Point point) {

    double u;
    double v = acos((point.y - center.y) / radius) / PI;

    if (point.x - center.x >= 0) {
        u = acos((point.z - center.z) / (radius * sin(v * PI)))
            / (2 * PI);
    } else {
        u = (PI + acos((center.z - point.z) / (radius * sin(v * PI))))
            / (2 * PI);
    }

    // Read the texture from bottom up
    v = 1 - v;

    Color *texturePixels = texture.getPixels();

    int x = (int) (u * (texture.getWidth() - 1));
    int y = (int) (v * (texture.getHeight() - 1));
    int index = y * texture.getWidth() + x;

    Color result = texturePixels[index];
    
    return result;
}
