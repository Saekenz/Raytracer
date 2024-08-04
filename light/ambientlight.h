#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include "light.h"

class AmbientLight : public Light {

public:
    AmbientLight(const Color& color) : Light(color) {}

    std::ostream& print(std::ostream& os) {
        os << "Ambient Light: Color = " << color << std::endl;
        return os;
    }
};

#endif