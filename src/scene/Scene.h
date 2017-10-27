#ifndef INC_SCENE_H
#define INC_SCENE_H

#include <vector>
#include "Primitives.h"
#include "Object.h"

class Scene {

public:

    Scene() {}
    ~Scene() {
        for (int i = 0; i < objects.size(); i++) {
            delete objects[i];
        }
    }

    // Scene attributes
    int maxReflectionDepth;

    // Anti-aliasing attributes
    bool antiAliasing;
    double subdivisionLimit;
    double colorDifferentialEpsilon;

    // Scene structures
    Camera camera;
    Color background;
    Color ambience;
    std::vector<Light> lights;
    std::vector<Object *> objects;

    // Tracing methods
    std::vector<std::vector<Color> > trace(int width, int height);

private:

    Vector getViewRay(double x, double y, int height,
        double z, double xR, double yR, double zR);

    Color traceRay(Point point, Vector rayDirection, int reflectionDepth);

    Color traceReflectedRay(Object *obj, double distanceFromPoint,
        Point point, Vector rayDirection, int reflectionDepth);

    Color adaptiveSample(double x, double y, int height, double z, double xR,
        double yR, double zR, Point point, double pixelScale);
};

#endif
