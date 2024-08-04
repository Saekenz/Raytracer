#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#include <fstream>

class Color {
    private:
        double r, b, g;

    public:
        double getR() const { return r; }
        double getG() const { return g; }
        double getB() const { return b; }

        Color() : r(0), g(0), b(0) {}
        Color(const vec3& color): r(color.getX()), g(color.getY()), b(color.getZ()) {}
        Color(double r, double g, double b) : r(r), g(g), b(b) {}

        // Adapted from Raytracing In One Weekend
        // https://raytracing.github.io/books/RayTracingInOneWeekend.html#thevec3class/colorutilityfunctions
        void printColor(std::ofstream &outputFile, Color pixelColor) {
            outputFile << static_cast<int>(255.999 * pixelColor.getR()) << ' '
                << static_cast<int>(255.999 * pixelColor.getG()) << ' '
                << static_cast<int>(255.999 * pixelColor.getB()) << '\n';
        }

        Color& operator+=(const Color & color) {
            r += color.getR();
            g += color.getG();
            b += color.getB();
            return *this;
        }
};

inline std::ostream& operator<<(std::ostream &out, const Color &c) {
    return out << "[" << c.getR() << ", " << c.getG() << ", " << c.getB() << "]";
}


#endif