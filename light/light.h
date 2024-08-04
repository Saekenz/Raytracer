#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>

#include "../color.h"

class Light {

public:
    Color color;

    Light() {}
    Light(const Color& color) : color(color) {}

    virtual std::ostream& print(std::ostream& os) = 0;

    friend std::ostream& operator<<(std::ostream& os, Light& light);

};

inline std::ostream& operator<<(std::ostream& os, Light& light) {
    return light.print(os);
}

#endif