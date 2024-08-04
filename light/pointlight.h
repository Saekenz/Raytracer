#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"

class PointLight : public Light {

public:
    point3 position;

    PointLight(const Color& color, const point3& position) : Light(color) {
        this->position = position;
    }

    std::ostream& print(std::ostream& os) {
        os << "Point Light: Color = " << color << ", Position = " << position << std::endl;
        return os;
    }

};

#endif