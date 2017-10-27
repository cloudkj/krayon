#ifndef INC_SCENEPARSER_H
#define INC_SCENEPARSER_H

#include <map>
#include <string>
#include "irrXML.h"
#include "scene/Primitives.h"
#include "scene/Scene.h"

class SceneParser {

public:

    SceneParser(std::string filename);
    ~SceneParser();

    void createScene();
    Scene scene;

private:

    /* XML reader */
    irr::io::IrrXMLReader *xmlReader;

    /* Mappings from a XML node to a parsing function */
    std::map<std::string, void (SceneParser::*)()> nodeFunctionMap;

    void parseScene();
    void parseCamera();
    void parseBackground();
    void parseAmbience();
    void parseLight();
    void parseSphere();
    void parsePlane();

    /* Helper methods */
    void traverseToNode(std::string nodeName);
    void setColorValues(Color & c);
    void setPointValues(Point & p);
    void setVectorValues(Vector & v);
};

#endif
