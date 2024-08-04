#include "scene.h"

#include <iostream>
#include <string>
#include <vector>

Scene::Scene(const std::string& outputFile, Color backgroundColor, Camera camera, 
             const std::vector<Light*>& lights, const std::vector<Surface*>& surfaces) {
    this->outputFile = outputFile;
    this->backgroundColor = backgroundColor;
    this->camera = camera;
    this->lights = lights;
    this->surfaces = surfaces;
}

std::ostream& operator<<(std::ostream& os, const Scene& scene) {
    os << "Scene ouput file: " << scene.outputFile << std::endl;
    os << "Scene background color: " << scene.backgroundColor << std::endl;
    os << scene.camera << std::endl;

    os << "================= LIGHTS =================" << std::endl;
    for (Light* light : scene.lights) {
        os << *light;
    }

    os << "\n================= SURFACES =================" << std::endl;
    for (Surface* surface : scene.surfaces) {
        os << *surface;
    }

    return os;
}