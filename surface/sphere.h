#ifndef SPHERE_H
#define SPHERE_H

#include "surface.h"
#include "../vec3.h"
#include "../ray.h"

class Sphere : public Surface {

public:
    double radius;
    point3 position;

    Sphere() {}
    Sphere(Material* material, const Transform& transform, double radius, const point3& position) : 
        Surface(material, transform) {
        this->radius = radius;
        this->position = position;
    }

    std::ostream& print(std::ostream& os) {
        os << "Sphere: \nRadius = " << radius << std::endl;
        os << "Position = " << position << std::endl;
        os << "Material = " << *material << std::endl;
        os << "Transform = " << transform << std::endl;
        return os;
    }
    
    // Adapted from:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
    bool intersect(const Ray& ray, double& t) const {
        double t1, t2;
        vec3 L = ray.getOrigin() - position;
        double a = ray.getDirection().squaredLength();
        double b = 2 * dot(ray.getDirection(), L);
        double c = L.squaredLength() - pow(radius, 2);
        
        if(!solveQuadratic(a, b, c, t1, t2)) return false;
        
        if (t1 > t2) std::swap(t1, t2);

        if (t1 < 0) {
            t1 = t2; 
            if (t1 < 0) return false; 
        }

        t = t1;
        return true;
    }

    // Taken from:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
    bool solveQuadratic(const double &a, const double &b, const double &c, double &x0, double &x1) const{
        float discr = b * b - 4 * a * c;
        if (discr < 0) return false;
        else if (discr == 0) x0 = x1 = - 0.5 * b / a;
        else {
            float q = (b > 0) ?
                -0.5 * (b + sqrt(discr)) :
                -0.5 * (b - sqrt(discr));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1) std::swap(x0, x1);
    
        return true;
    }
};

#endif