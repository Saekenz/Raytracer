#include "raytracer.h"
#include "ray.h"
#include "color.h"
#include "light/ambientlight.h"
#include "light/parallellight.h"
#include "light/pointlight.h"
#include "material/solidmaterial.h"
#include "material/texturedmaterial.h"
#include "surface/mesh.h"

using std::pow;
using std::max;

Raytracer::Raytracer(const Scene& scene): scene(scene) {}

Color Raytracer::trace(const Ray& ray) {
    Color pixelColor = Color(0.0, 0.0, 0.0); // Set starting color to black
    vector<Intersection> intersections;

    // Check for intersections
    for(Surface* surface : scene.surfaces) {
        double t;
        const bool hasIntersection = surface->intersect(ray, t);
        if(hasIntersection) {
            if(Sphere* sphere = dynamic_cast<Sphere*>(surface)) {
                // Pass t to the ray to get intersection point
                point3 intPoint = ray.r(t);
                // Compute the vector from center of the sphere to intersection point
                vec3 intNormal = unitVector(intPoint - sphere->position);
                Intersection intersection(sphere, t, intPoint, intNormal);
                intersections.push_back(intersection);
            } 
            else if (Mesh* mesh = dynamic_cast<Mesh*>(surface)) {
                // TODO Lab3b
            }
        }
    }

    // Determine color of the pixel
    if(intersections.size() > 0) {
         Intersection closestIntersection = findClosestIntersection(intersections);
         for(Light* light : scene.lights) {
            // check for shadow intersection
            bool shadowIntersection = castShadowray(closestIntersection, light);
            if(!shadowIntersection)
                pixelColor += illuminate(ray, closestIntersection, light);
         }
    }

    return pixelColor;
}

/* ---------- Compute Illumination with Phong ----------------*/
Color Raytracer::illuminate(const Ray& ray, const Intersection& intersection, Light* light) {
    Color pixelColor;
    double ka, kd, ks, exponent;
    if(Sphere* sphere = dynamic_cast<Sphere*>(intersection.surface)) {
        if(SolidMaterial* solidMaterial = dynamic_cast<SolidMaterial*>(sphere->material)) {
            ka = solidMaterial->phong.getX();
            kd = solidMaterial->phong.getY();
            ks = solidMaterial->phong.getZ();
            exponent = solidMaterial->phongExponent;
            pixelColor = solidMaterial->color;
        } 
        else if (TexturedMaterial* texMaterial = dynamic_cast<TexturedMaterial*>(sphere->material)) {
            // TODO Lab3b
        }
    }
    else if (Mesh* mesh = dynamic_cast<Mesh*>(intersection.surface)) {
            // TODO Lab3b
    }

    vec3 objectColor = vec3(pixelColor.getR(), pixelColor.getG(), pixelColor.getB());
    vec3 lightColor = vec3(light->color.getR(), light->color.getG(), light->color.getB());
    vec3 resultColor;

    if(AmbientLight* ambientlight = dynamic_cast<AmbientLight*>(light)) {
        resultColor += lightColor*objectColor*ka;

    } else if (ParallelLight* parallelLight = dynamic_cast<ParallelLight*>(light)) {
        vec3 L = -unitVector(parallelLight->direction); // Light source vector
        vec3 N = intersection.normalIntersection; // Normal vector
        vec3 E = -unitVector(ray.getDirection()); // Eye (View) vector
        vec3 R = unitVector(2 * dot(L,N) * N - L); // Perfect reflection vector (07_illum p. 27)

        double diffuse = max(dot(L, N), 0.0);
        double specular = pow(max(dot(R, E), 0.0), exponent);

        resultColor += lightColor*objectColor*diffuse*kd + lightColor*specular*ks;
            
    } else if (PointLight* pointLight = dynamic_cast<PointLight*>(light)) {
        // TODO Lab 3b
    }

    return Color(resultColor.getX(), resultColor.getY(), resultColor.getZ());
}

/* ---------------------- Cast shadow ray and check if it hits something ------------------------*/
bool Raytracer::castShadowray(const Intersection& intersection, Light* light) {
    Ray ray;
    // Construct shadow ray depending on type of light
    if(AmbientLight* ambientlight = dynamic_cast<AmbientLight*>(light)){
        return false;
    } 
    else if (ParallelLight* parallelLight = dynamic_cast<ParallelLight*>(light)) {
        ray = Ray(intersection.pIntersection, -parallelLight->direction);      
    } 
    else if (PointLight* pointLight = dynamic_cast<PointLight*>(light)) {
        // TODO Lab 3b, important: limit shadow ray distance! (length from intersection point to light src)
    }
    
    // Check for shadow ray intersection
    for(Surface* surface : scene.surfaces) {
        double t;
        const bool intersection = surface->intersect(ray, t);
        // Check if t is not too close to 0 to prevent shadows acne/bias
        if(intersection && t > EPSILON) {
            return true;
        }
    }
    return false;
}

/* -------------- Determine closest intersection ------------------*/
Intersection Raytracer::findClosestIntersection(const vector<Intersection>& intersections) {
    const double closestT = 9999999.999999;
    Intersection closestIntersection;

    // Iterate through all intersections & find closest one
    vector<Intersection>::const_iterator it = intersections.begin();
    for(; it != intersections.end(); ++it) {
        if(it->distance < closestT) {
            closestIntersection = *it;
        }
    }

    return closestIntersection;
}

/* -------------- Takes a Scene object and renders its properties into a .ppm file --------------*/
void Raytracer::renderScene(const Scene& scene, std::ofstream &outputFile) {
    const int imageWidth = scene.camera.getWidth();
    const int imageHeight = scene.camera.getHeight();

    outputFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    if(scene.outputFile == "blackimage") {
        // Set color for each pixel to black
        Color pixelColor = Color(0.0, 0.0, 0.0);
        for (int y = imageHeight-1; y >= 0; --y) {
            for (int x = 0; x < imageWidth; ++x) {
                pixelColor.printColor(outputFile, pixelColor);
            }
        }
    } else {
        // Construct rays for each pixel & determine color
        for (int y = imageHeight-1; y >= 0; --y) {
            for (int x = 0; x < imageWidth; ++x) {
                Ray ray = scene.camera.getRayToPixel(x,y);
                Color pixelColor = trace(ray);
                pixelColor.printColor(outputFile, pixelColor);
            }
        }
    }
    
    std::cerr << "Done rendering." << std::endl;
}