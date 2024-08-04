#ifndef RAY_H
#define RAY_H

#include <iostream>

#include "vec3.h"

class Ray {
    point3 origin;
    vec3 direction;

public:
    Ray() {}
    Ray(const point3& origin, const vec3& direction) {
        this->origin = origin;
        this->direction = direction; 
    }

    point3 getOrigin() const { return origin; }
    vec3 getDirection() const { return direction; }

    // Needed to compute the intersection point
    point3 r(double t) const {
        return (origin + t * direction);
    }

};

inline std::ostream& operator<<(std::ostream& os, Ray& ray) {
    os << "Ray: " << std::endl;
    os << "Origin = " << ray.getOrigin() << std::endl;
    os << "Direction = " << ray.getDirection() << std::endl;
    return os;
}

#endif