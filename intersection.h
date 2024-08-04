#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "surface/sphere.h"

class Intersection {
    
public:
    Surface* surface;
    point3 pIntersection;
    vec3 normalIntersection;
    double distance;

    Intersection() {}
    Intersection(Surface* surface, double distance, point3 pInt, vec3 nInt):
        surface(surface), distance(distance), pIntersection(pInt), normalIntersection(nInt) {}

};

#endif