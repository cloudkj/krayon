#include <iostream>
#include "SceneParser.h"
#include "scene/Sphere.h"
#include "scene/Plane.h"

SceneParser::SceneParser(std::string filename) {
    xmlReader = irr::io::createIrrXMLReader(filename.c_str());

    nodeFunctionMap["Scene"] = &SceneParser::parseScene;
    nodeFunctionMap["Camera"] = &SceneParser::parseCamera;
    nodeFunctionMap["Background"] = &SceneParser::parseBackground;
    nodeFunctionMap["Ambience"] = &SceneParser::parseAmbience;
    nodeFunctionMap["Light"] = &SceneParser::parseLight;
    nodeFunctionMap["Sphere"] = &SceneParser::parseSphere;
    nodeFunctionMap["Plane"] = &SceneParser::parsePlane;
}

SceneParser::~SceneParser() {
    delete xmlReader;
}

void
SceneParser::createScene() {
    while (xmlReader->read()) {

        if (xmlReader->getNodeType() == irr::io::EXN_ELEMENT) {
            std::string nodeNameString(xmlReader->getNodeName());
            if (nodeFunctionMap.count(nodeNameString) > 0) {
                void (SceneParser::*nodeParsingFunction)() =
                    nodeFunctionMap[nodeNameString];

                (this->*nodeParsingFunction)();
            }

        }
    }
}

void
SceneParser::parseScene() {
    scene.maxReflectionDepth = xmlReader->getAttributeValueAsInt("maxReflectionDepth");

    // anti-aliasing attributes
    scene.antiAliasing = (strcmp(xmlReader->getAttributeValue("antiAliasing"), "true") == 0) ? true : false;
    scene.subdivisionLimit = 1 / pow(2.0, (double) xmlReader->getAttributeValueAsInt("subdivisionLimit"));
    scene.colorDifferentialEpsilon = xmlReader->getAttributeValueAsFloat("colorDifferentialEpsilon");
}

void
SceneParser::parseCamera() {
    scene.camera.fieldOfView = xmlReader->getAttributeValueAsFloat("fieldOfView");

    traverseToNode("Position");
    setPointValues(scene.camera.position);

    traverseToNode("Look");
    setVectorValues(scene.camera.look);

    traverseToNode("Up");
    setVectorValues(scene.camera.up);
}

void
SceneParser::parseBackground() {
    setColorValues(scene.background);
}

void
SceneParser::parseAmbience() {
    setColorValues(scene.ambience);
}

void
SceneParser::parseLight() {
    Light light;

    traverseToNode("Position");
    setPointValues(light.position);

    traverseToNode("Intensity");
    setColorValues(light.intensity);

    scene.lights.push_back(light);
}

void SceneParser::parseSphere() {
    Sphere *sphere = new Sphere;

    sphere->radius = xmlReader->getAttributeValueAsFloat("radius");
    sphere->specularity = xmlReader->getAttributeValueAsInt("specularity");
    sphere->reflectivity = xmlReader->getAttributeValueAsFloat("reflectivity");
    
    if (xmlReader->getAttributeValue("texture")) {
        sphere->texture.load(xmlReader->getAttributeValue("texture"));
    }

    traverseToNode("Center");
    setPointValues(sphere->center);

    traverseToNode("Ambient");
    setColorValues(sphere->ambient);

    traverseToNode("Diffuse");
    setColorValues(sphere->diffuse);

    traverseToNode("Specular");
    setColorValues(sphere->specular);

    scene.objects.push_back(sphere);
}

void SceneParser::parsePlane() {
    Plane *plane = new Plane;

    plane->a = xmlReader->getAttributeValueAsFloat("a");
    plane->b = xmlReader->getAttributeValueAsFloat("b");
    plane->c = xmlReader->getAttributeValueAsFloat("c");
    plane->d = xmlReader->getAttributeValueAsFloat("d");

    plane->specularity = xmlReader->getAttributeValueAsInt("specularity");
    plane->reflectivity = xmlReader->getAttributeValueAsFloat("reflectivity");

    if (xmlReader->getAttributeValue("texture")) {
        plane->texture.load(xmlReader->getAttributeValue("texture"));
    }

    traverseToNode("Point1");
    setPointValues(plane->p1);

    traverseToNode("Point2");
    setPointValues(plane->p2);

    traverseToNode("Ambient");
    setColorValues(plane->ambient);

    traverseToNode("Diffuse");
    setColorValues(plane->diffuse);

    traverseToNode("Specular");
    setColorValues(plane->specular);

    scene.objects.push_back(plane);
}

void SceneParser::traverseToNode(std::string nodeName) {
    while (xmlReader->getNodeType() != irr::io::EXN_ELEMENT ||
        nodeName.compare(xmlReader->getNodeName()) != 0) {
            xmlReader->read();
    }
}

void SceneParser::setPointValues(Point & p) {
    p.x = xmlReader->getAttributeValueAsFloat("x");
    p.y = xmlReader->getAttributeValueAsFloat("y");
    p.z = xmlReader->getAttributeValueAsFloat("z");
}

void SceneParser::setVectorValues(Vector & v) {
    v.x = xmlReader->getAttributeValueAsFloat("x");
    v.y = xmlReader->getAttributeValueAsFloat("y");
    v.z = xmlReader->getAttributeValueAsFloat("z");
}

void SceneParser::setColorValues(Color & c) {
    c.red = xmlReader->getAttributeValueAsFloat("red");
    c.green = xmlReader->getAttributeValueAsFloat("green");
    c.blue = xmlReader->getAttributeValueAsFloat("blue");
}
