#ifndef SURFACE_H
#define SURFACE_H

#include <iostream>

#include "../material/material.h"
#include "../transform.h"

class Surface {

public:
    Material* material;
    Transform transform;

    Surface() {}
    Surface(Material* material, const Transform& transform) : material(material),
        transform(transform) {}

    virtual bool intersect(const Ray& ray, double& t) const = 0;    

    virtual std::ostream& print(std::ostream& os) = 0;

    friend std::ostream& operator<<(std::ostream& os, Surface& surface);

};

inline std::ostream& operator<<(std::ostream& os, Surface& surface) {
    return surface.print(os);
}

#endif