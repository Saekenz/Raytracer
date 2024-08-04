#include "color.h"
#include "vec3.h"
#include "scene.h"
#include "xmlparser.h"
#include "light/ambientlight.h"
#include "light/parallellight.h"
#include "light/pointlight.h"
#include "material/solidmaterial.h"
#include "material/texturedmaterial.h"
#include "ray.h"
#include "surface/sphere.h"
#include "intersection.h"
#include "raytracer.h"

#include <iostream>
#include <fstream>

#include <cmath>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./main <input_file_path>" << std::endl;
        return 1;
    }
    
    // read user input file path
    std::string inputFile = argv[1];
    Scene scene;

    if(inputFile == "blackimage") {
        scene = XmlParser::blackImage(); // T1
    } else if (inputFile == "spheresimage") {
        scene = XmlParser::spheresImage(); // T2
    } else {
        scene = XmlParser::parseScene(inputFile);
    }

    std::ofstream outputFile(scene.outputFile);

    if(!outputFile.is_open()) {
        std::cerr << "Error opening file " << scene.outputFile << std::endl;
        return 1;
    }

    // start rendering the scene
    Raytracer raytracer = Raytracer(scene);
    std::cerr << "Rendering scene: " << inputFile << std::endl;
    raytracer.renderScene(scene, outputFile);
    
    return 0;
}