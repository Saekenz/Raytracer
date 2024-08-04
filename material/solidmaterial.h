#ifndef SOLIDMATERIAL_H
#define SOLIDMATERIAL_H

#include "../color.h"
#include "material.h"

#include <iostream>

class SolidMaterial : public Material {

public:
    Color color;

    SolidMaterial() {}
    SolidMaterial(const vec3& phong, double phongExponent, double reflectance, double transmittance, double refraction, const Color& color) :
        Material(phong, phongExponent, reflectance, transmittance, refraction) {
            this->color = color;
    }

    std::ostream& print(std::ostream& os) {
        os << "Solid Material: \nColor = " << color << std::endl;
        return os;
    }

};

#endif