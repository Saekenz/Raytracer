#ifndef MESH_H
#define MESH_H

#include "surface.h"
#include "../ray.h"

#include <string>

class Mesh : public Surface {

public:
    std::string name;

    Mesh() {}
    Mesh(Material* material, const Transform& transform, const std::string& name) : 
        Surface(material, transform) {
        this->name = name;
    }

    std::ostream& print(std::ostream& os) {
        os << "Mesh: " << std::endl;
        os << "Name = " << name << std::endl;
        os << "Material = " << *material << std::endl;
        os << "Transform = " << transform << std::endl;
        return os;
    }

    bool intersect(const Ray& ray, double& t) const{
        return false;
        // TODO Lab3b
    }

};

#endif