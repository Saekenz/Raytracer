#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "intersection.h"

using std::vector;

class Raytracer {

public:
    // needed to handle shadow acne/bias
    const double EPSILON = 0.00001;

    Raytracer() {}
    Raytracer(const Scene&);

    Color trace(const Ray&);
    Color illuminate(const Ray&, const Intersection&, Light*);
    Intersection findClosestIntersection(const vector<Intersection>&);
    bool castShadowray(const Intersection&, Light* light);
    void renderScene(const Scene&, std::ofstream&);
    
private:
    Scene scene;
};

#endif
