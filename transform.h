#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"

#include <iostream>

class Transform {

public:
    vec3 translate; // Moves an object by the vector [x,y,z]. 
    vec3 scale; // Scales an object by x on the x-axis, y on the y-axis and z on the z-axis. 
    double rotateX; // Rotates an object by theta degrees around x-axis. 
    double rotateY; // Rotates an object by theta degrees around y-axis. 
    double rotateZ; // Rotates an object by theta degrees around z-axis. 

    Transform() :  
        translate(vec3(0,0,0)), scale(vec3(1,1,1)), rotateX(0.0), rotateY(0.0), rotateZ(0.0) {}
    Transform(const vec3& t, const vec3& s, double rx, double ry, double rz) : 
        translate(t), scale(s), rotateX(rx), rotateY(ry), rotateZ(rz) {}

};

inline std::ostream& operator<<(std::ostream &out, const Transform &t) {
    out << "Transformations: \nTranslation = " << t.translate << std::endl;
    out << "Scaling = " << t.scale << std::endl;
    out << "Rotate X = " << t.rotateX << std::endl;
    out << "Rotate Y = " << t.rotateY << std::endl;
    out << "Rotate Z = " << t.rotateZ << std::endl;
    
    return out;
}

#endif