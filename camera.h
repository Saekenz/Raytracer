#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "vec3.h"
#include "ray.h"

class Camera {
    point3 position;
    vec3 lookAt;
    vec3 up;
    int horizontalFov;
    int width, height;
    int maxBounces;

public:
    Camera();
    Camera(const point3&, const vec3&, const vec3&, int, int, int, int);

    vec3 getPos() const { return position; }
    vec3 getLookAt() const { return lookAt; }
    vec3 getUp() const { return up; }
    int getFov() const { return horizontalFov; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getBounces() const { return maxBounces; }

    Ray getRayToPixel(const int, const int) const;

};

inline std::ostream& operator<<(std::ostream &out, const Camera& camera) {
        out << "================= CAMERA =================" << std::endl;
        out << "Position: " << camera.getPos() << std::endl;
        out << "LookAt: " << camera.getLookAt() << std::endl;
        out << "Up: " << camera.getUp() << std::endl;
        out << "Horizontal FOV: " << camera.getFov() << std::endl;
        out << "Resolution: " << camera.getWidth() << "x" << camera.getHeight() << std::endl;
        out << "Max Bounces: " << camera.getBounces() << std::endl;
        return out;
}

#endif