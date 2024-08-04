#ifndef TEXTUREDMATERIAL_H
#define TEXTUREDMATERIAL_H

#include "material.h"

#include <iostream>
#include <string>

class TexturedMaterial : public Material {

public:
    std::string name;

    TexturedMaterial() {}
    TexturedMaterial(const vec3& phong, double phongExponent, double reflectance, double transmittance, double refraction, const std::string& name) :
        Material(phong, phongExponent, reflectance, transmittance, refraction) {
            this->name = name;
    }

    std::ostream& print(std::ostream& os) {
        os << "Textured Material: \nName = " << name << std::endl;
        return os;
    }
};

#endif