#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

using std::sqrt;
using std::pow;

// Inspired by:
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#thevec3class
class vec3 {
    private:
        double x, y, z;

    public:
        vec3() : x(0.0), y(0.0), z(0.0) {}
        vec3(double x, double y, double z) : x(x), y(y), z(z) {}

        double getX() const { return x; }
        double getY() const { return y; }
        double getZ() const { return z; }

        vec3 operator-() const { return vec3(-x, -y, -z); }
       
        vec3 &operator+=(const vec3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        vec3 &operator*=(const double s) {
            x *= s;
            y *= s;
            z *= s;
            return *this;
        }

        vec3& operator/=(const double s) {
            return *this *= 1/s;
        }

        double squaredLength() const {
            return pow(x, 2) + pow(y, 2) + pow(z, 2);
        }

        double length() const {
            return sqrt(squaredLength());
        }
};

// to differentiate between points and directions more easily
using point3 = vec3;

inline double dot(const vec3 &u, const vec3 &v) {
    return u.getX() * v.getX() + u.getY() * v.getY() + u.getZ() * v.getZ();
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.getY() * v.getZ() - u.getZ() * v.getY(),
                u.getZ() * v.getX() - u.getX() * v.getZ(),
                u.getX() * v.getY() - u.getY() * v.getX());
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.getX() * v.getX(), u.getY() * v.getY(), u.getZ() * v.getZ());
}

inline vec3 operator*(const double s, const vec3 &v) {
    return vec3(s * v.getX(), s * v.getY(), s * v.getZ());
}

inline vec3 operator*(const vec3 &v, double s) {
    return s * v;
}

inline vec3 operator/(vec3 v, const double s) {
    return (1/s) * v;
}

inline vec3 unitVector(vec3 v) {
    return v / v.length();
}

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << "[" << v.getX() << ", " << v.getY() << ", " << v.getZ() << "]";
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.getX() - v.getX(), u.getY() - v.getY(), u.getZ() - v.getZ());
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.getX() + v.getX(), u.getY() + v.getY(), u.getZ() + v.getZ());
}

#endif