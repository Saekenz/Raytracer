#include<iostream>

#include "camera.h"

Camera::Camera() {
    this->position = point3(0,0,0);
    this->lookAt = vec3(0,0,-2.5);
    this->up = vec3(0,1,0);
    this->horizontalFov = 45;
    this->width = 512;
    this->height = 512;
    this->maxBounces = 8;
}

Camera::Camera(const point3& pos, const vec3& lookAt, const vec3& up, int fov, 
               int camWidth, int camHeight, int bounces) {
    this->position = pos;
    this->lookAt = lookAt;
    this->up = up;
    this->horizontalFov = fov;
    this->width = camWidth;
    this->height = camHeight;
    this->maxBounces = bounces;
}

Ray Camera::getRayToPixel(const int x, const int y) const {
    // convert fov from deg -> rad
    const double pi = 3.141592653589793238462643383279502884197;
    double theta = horizontalFov * pi/180;
    
    // set fov
    double fovx = theta;
    double fovy = fovx * height/width;

    // normalize coordinates
    double xn = (double(x)+0.5) / width;
    double yn = (double(y)+0.5) / height;

    // adjust coordinates according to fov    
    double xi = (2*xn-1) * tan(fovx);
    double yi = (2*yn-1) * tan(fovy);

    // create vector that describes direction of the ray
    vec3 dir = unitVector(vec3(xi, yi, -1));
    
    return Ray(position, dir);
}