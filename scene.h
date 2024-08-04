#ifndef SCENE_H
#define SCENE_H

#include "color.h"
#include "camera.h"
#include "light/light.h"
#include "surface/surface.h"

#include <string>
#include <vector>
#include <iostream>

class Scene {

public:
    std::string outputFile;
    Color backgroundColor;
    Camera camera;
    std::vector<Light*> lights; //stores objects derived from Light
    std::vector<Surface*> surfaces; //stores objects derived from Surface

    Scene() {}
    Scene(const std::string&, Color, Camera, 
          const std::vector<Light*>&, const std::vector<Surface*>&);

};

std::ostream& operator<<(std::ostream&, const Scene&);

#endif