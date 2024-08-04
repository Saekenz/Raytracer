#ifndef MATERIAL_H
#define MATERIAL_H

class Material {

public:
    vec3 phong;
    double phongExponent;
    double reflectance;
    double transmittance;
    double refraction;

    Material() {}
    Material(const vec3& phong, double phongExponent, double reflectance, double transmittance, double refraction) :
        phong(phong), phongExponent(phongExponent), reflectance(reflectance), transmittance(transmittance), refraction(refraction) {}

    virtual std::ostream& print(std::ostream& os) = 0;

    friend std::ostream& operator<<(std::ostream& os, Material& material);

};

inline std::ostream& operator<<(std::ostream& os, Material& material) {
    material.print(os);
    os << "Phong = " << material.phong << std::endl;
    os << "Phong Exponent = " << material.phongExponent << std::endl;
    os << "Reflectence = " << material.reflectance << std::endl;
    os << "Transmittance = " << material.transmittance << std::endl;
    os << "Refraction = " << material.refraction << std::endl;
    return os;
}

#endif