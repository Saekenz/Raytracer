#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "util/rapidxml.hpp"
#include "scene.h"
#include "material/solidmaterial.h"
#include "material/texturedmaterial.h"
#include "transform.h"
#include "vec3.h"
#include "color.h"

using std::string;
using namespace rapidxml;

class XmlParser {
    xml_document<> doc;
    xml_node<> * root_node;
    
    public:
        static SolidMaterial parseSolidMaterial(xml_node<>*);
        static TexturedMaterial parseTexturedMaterial(xml_node<>*);
        static Transform parseTransform(xml_node<>*);
        static Scene parseScene(const string&);
        static Scene blackImage();
        static Scene spheresImage();
        static string convertToPPM(const string&);
        static Color parseColor(xml_node<>*);
};

#endif