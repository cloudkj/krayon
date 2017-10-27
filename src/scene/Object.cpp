#include <vector>
#include "Object.h"
#include "Primitives.h"

Color
Object::getColorAtPoint(std::vector<Light> & lights,
                        std::vector<Object *> & objects,
                        Color ambience,
                        Point point,
                        Vector incidentRay,
                        double distanceFromPoint) {

    Point intersect(point.x + incidentRay.x * distanceFromPoint,
        point.y + incidentRay.y * distanceFromPoint,
        point.z + incidentRay.z * distanceFromPoint);

    Vector normal = getNormal(point, incidentRay, distanceFromPoint);

    // ambient
    Color ambientColorAtPoint(ambient.red * ambience.red,
        ambient.green * ambience.green,
        ambient.blue * ambience.blue);

    // diffuse
    Color diffuseColorAtPoint = getDiffuseColorAtPoint(lights,
        objects, intersect, normal);

    // specular
    Color specularColorAtPoint = getSpecularColorAtPoint(lights,
        objects, intersect, incidentRay, normal);

    Color result(ambientColorAtPoint.red + diffuseColorAtPoint.red + specularColorAtPoint.red,
        ambientColorAtPoint.green + diffuseColorAtPoint.green + specularColorAtPoint.green,
        ambientColorAtPoint.blue + diffuseColorAtPoint.blue + specularColorAtPoint.blue);

    result.normalize();
    return result;
}

Vector
Object::getReflectedRayAtPoint(Point point,
                               Vector incidentRay,
                               double distanceFromPoint) {

    Point intersect(point.x + incidentRay.x * distanceFromPoint,
        point.y + incidentRay.y * distanceFromPoint,
        point.z + incidentRay.z * distanceFromPoint);

    Vector normal = getNormal(point, incidentRay, distanceFromPoint);

    double dp = normal.dotProduct(incidentRay);

    Vector reflectedRay(incidentRay.x - 2 * dp * normal.x, 
        incidentRay.y - 2 * dp * normal.y, 
        incidentRay.z - 2 * dp * normal.z);
    reflectedRay.normalize();

    return reflectedRay;
}

Color
Object::getDiffuseColorAtPoint(std::vector<Light> & lights,
                               std::vector<Object *> & objects,
                               Point point,
                               Vector normal) {

    Color lightingCoeff(0, 0, 0);

    for (int i = 0; i < lights.size(); i++) {
        Vector lightDirection(lights[i].position.x - point.x,
            lights[i].position.y - point.y,
            lights[i].position.z - point.z);

        if (!pointIsInShadow(objects, point, lightDirection)) {

            lightDirection.normalize();
            double tempCoeff;

            if ((tempCoeff = lightDirection.dotProduct(normal)) >= 0) {

                lightingCoeff.red += tempCoeff * lights[i].intensity.red;
                lightingCoeff.green += tempCoeff * lights[i].intensity.green;
                lightingCoeff.blue += tempCoeff * lights[i].intensity.blue;
            }
        }
    }

    lightingCoeff.normalize();

    // get diffuse color from material property or texture
    Color diffuse;

    if (texture.isLoaded) {
        diffuse = getDiffuseTextureColorAtPoint(point);
    } else {
        diffuse = this->diffuse;
    }

    Color result(lightingCoeff.red * diffuse.red,
        lightingCoeff.green * diffuse.green,
        lightingCoeff.blue * diffuse.blue);

    return result;
}

Color
Object::getSpecularColorAtPoint(std::vector<Light> & lights,
                                std::vector<Object *> & objects,
                                Point point,
                                Vector incidentRay,
                                Vector normal) {

    Color lightingCoeff(0, 0, 0);

    for (int i = 0; i < lights.size(); i++) {
        Vector lightDirection(lights[i].position.x - point.x,
            lights[i].position.y - point.y,
            lights[i].position.z - point.z);

        if (!pointIsInShadow(objects, point, lightDirection)) {

            lightDirection.normalize();

            double dp = normal.dotProduct(lightDirection);
            Vector reflectedLight(lightDirection.x - 2 * dp * normal.x, 
                lightDirection.y - 2 * dp * normal.y, 
                lightDirection.z - 2 * dp * normal.z);
            reflectedLight.normalize();

            double tempCoeff;

            if ((tempCoeff = incidentRay.dotProduct(reflectedLight)) >= 0) {

                tempCoeff = pow(tempCoeff, specularity);
                lightingCoeff.red += tempCoeff * lights[i].intensity.red;
                lightingCoeff.green += tempCoeff * lights[i].intensity.green;
                lightingCoeff.blue += tempCoeff * lights[i].intensity.blue;
            }

        }
    }

    lightingCoeff.normalize();

    Color result(lightingCoeff.red * specular.red,
        lightingCoeff.green * specular.green,
        lightingCoeff.blue * specular.blue);

    return result;
}

bool
Object::pointIsInShadow(std::vector<Object *> & objects,
                        Point point,
                        Vector lightDirection) {

    double distanceToLight = lightDirection.magnitude();
    lightDirection.normalize();

    for (int i = 0; i < objects.size(); i++) {
        Object *obj = objects[i];
        if (obj == this) {
            continue;
        }

        double distanceToObject = obj->getDistanceFromPoint(point, lightDirection);
        if (distanceToObject < distanceToLight) {
            return true;
        }
    }

    return false;
}
