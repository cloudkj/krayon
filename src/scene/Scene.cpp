#include "Object.h"
#include "Scene.h"

#include <vector>
#include <iostream>
#include <float.h>
#include <math.h>

void debug(Vector v) {
    std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
}

std::vector<std::vector<Color> >
Scene::trace(int width, int height) {

    std::vector<std::vector<Color> > result;
    camera.initBasis();

    // get z-coord of picture plane in camera basis
    double z = -((double) height) / tan(camera.fieldOfView/180*PI);

    // optimize change of basis calculations
    double xR = (z * camera.zBasis.x) + camera.position.x;
    double yR = (z * camera.zBasis.y) + camera.position.y;
    double zR = (z * camera.zBasis.z) + camera.position.z;

    int progress = 0;

    for (int y = -(height/2); y < height/2; y++) {

        std::vector<Color> row;
        result.push_back(row);

        for (int x = -(width/2); x < width/2; x++) {

            Color c;

            if (antiAliasing) {
                c = adaptiveSample(x, y, height, z, xR, yR, zR, camera.position, 1.0);
            } else {
                Vector rayDirection = getViewRay(x, y, height, z, xR, yR, zR);
                c = traceRay(camera.position, rayDirection, 0);
            }

            result[y+height/2].push_back(c);

            // Output progress          
            if (++progress % ((width * height) / 20) == 0) {
                std::cout << "[Trace progress: "
                    << ((100 * progress) / (width * height))
                    << "%]" << std::endl;
            }
        }
    }

    return result;
}

Vector
Scene::getViewRay(double x, double y, int height,
                  double z, double xR, double yR, double zR) {

    Point p;
    p.x = (x * camera.xBasis.x) + (y * camera.yBasis.x) + xR;
    p.y = (x * camera.xBasis.y) + (y * camera.yBasis.y) + yR;
    p.z = (x * camera.xBasis.z) + (y * camera.yBasis.z) + zR;

    Vector rayDirection(p.x - camera.position.x,
        p.y - camera.position.y,
        p.z - camera.position.z);

    rayDirection.normalize();
    return rayDirection;
}

Color
Scene::traceRay(Point point, Vector rayDirection, int reflectionDepth) {

    // Trace through this ray and get the closest object
    Object *closestObject = NULL;
    double distanceToClosestObject = DBL_MAX;

    for (int i = 0; i < objects.size(); i++) {
        Object *obj = objects[i];
        double distanceToObject = obj->getDistanceFromPoint(point, rayDirection);

        // if the ray actually intersects the object and has    
        // a closer distance, save the object
        if (distanceToObject < distanceToClosestObject) {

            distanceToClosestObject = distanceToObject;
            closestObject = obj;
        }
    }

    // if the ray intersects no objects, or has reflected more than
    // the maximum reflection depth, return the background color
    if (closestObject == NULL || reflectionDepth > maxReflectionDepth) {

        return background;

    } else {

        Color colorAtPoint = closestObject->getColorAtPoint(lights,
            objects, ambience, point, rayDirection, distanceToClosestObject);

        Color reflectedColorAtPoint = traceReflectedRay(closestObject,
            distanceToClosestObject, point, rayDirection, reflectionDepth);

        colorAtPoint.adjust(1.0 - closestObject->reflectivity);
        reflectedColorAtPoint.adjust(closestObject->reflectivity);

        colorAtPoint.add(reflectedColorAtPoint);
        colorAtPoint.normalize();

        return colorAtPoint;
    }
}

Color
Scene::traceReflectedRay(Object *obj, double distanceFromPoint, Point point,
                         Vector rayDirection, int reflectionDepth) {

    Vector reflectedRay = obj->getReflectedRayAtPoint(point, rayDirection, distanceFromPoint);

    Point intersect(point.x + rayDirection.x * distanceFromPoint,
        point.y + rayDirection.y * distanceFromPoint,
        point.z + rayDirection.z * distanceFromPoint);

    return traceRay(intersect, reflectedRay, reflectionDepth+1);
}

Color
Scene::adaptiveSample(double x, double y, int height, double z, double xR, double yR,
                      double zR, Point point, double pixelScale) {

    Vector rayDirection;
    std::vector<Color> colors;

    rayDirection = getViewRay(x + (pixelScale/4.0), y + (pixelScale/4.0), height, z, xR, yR, zR);
    Color c1 = traceRay(point, rayDirection, 0);
    rayDirection = getViewRay(x + (pixelScale/4.0), y - (pixelScale/4.0), height, z, xR, yR, zR);
    Color c2 = traceRay(point, rayDirection, 0);
    rayDirection = getViewRay(x - (pixelScale/4.0), y + (pixelScale/4.0), height, z, xR, yR, zR);
    Color c3 = traceRay(point, rayDirection, 0);
    rayDirection = getViewRay(x - (pixelScale/4.0), y - (pixelScale/4.0), height, z, xR, yR, zR);
    Color c4 = traceRay(point, rayDirection, 0);

    colors.push_back(c1);
    colors.push_back(c2);
    colors.push_back(c3);
    colors.push_back(c4);

    Color averageColor = Color::averageColor(colors);

    if (pixelScale <= subdivisionLimit) {
        return averageColor;
    }

    if (averageColor.difference(c1) > colorDifferentialEpsilon) {
        c1 = adaptiveSample(x + (pixelScale/4.0), y + (pixelScale/4.0), height, z, xR, yR, zR, camera.position, pixelScale/2.0);
    }
    
    if (averageColor.difference(c2) > colorDifferentialEpsilon) {
        c2 = adaptiveSample(x + (pixelScale/4.0), y - (pixelScale/4.0), height, z, xR, yR, zR, camera.position, pixelScale/2.0);
    }
    
    if (averageColor.difference(c3) > colorDifferentialEpsilon) {
        c3 = adaptiveSample(x - (pixelScale/4.0), y + (pixelScale/4.0), height, z, xR, yR, zR, camera.position, pixelScale/2.0);
    }
    
    if (averageColor.difference(c4) > colorDifferentialEpsilon) {
        c4 = adaptiveSample(x - (pixelScale/4.0), y - (pixelScale/4.0), height, z, xR, yR, zR, camera.position, pixelScale/2.0);
    }

    return Color::averageColor(colors);
}
