#ifndef PARALLELLIGHT_H
#define PARALLELLIGHT_H

#include "light.h"

class ParallelLight : public Light {

public:
    vec3 direction;

    ParallelLight(const Color& color, const vec3& direction) : Light(color) {
        this->direction = direction;
    }

    std::ostream& print(std::ostream& os) {
        os << "Parallel Light: Color = " << color << ", Direction = " << direction << std::endl;
        return os;
    }

};

#endif