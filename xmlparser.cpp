#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "xmlparser.h"
#include "light/parallellight.h"
#include "light/pointlight.h"
#include "light/ambientlight.h"
#include "surface/sphere.h"
#include "surface/mesh.h"

using namespace rapidxml;

Scene XmlParser::parseScene(const string& filepath) {
    Scene scene;
    
    std::ifstream file(filepath);
    if(!file.is_open()) {
        std::cerr << "Error while trying to open file " << filepath << std::endl;
    }

    xml_document<> doc;
    xml_node<>* rootNode = NULL;

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), 
                                std::istreambuf_iterator<char>());

    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    rootNode = doc.first_node("scene");

    // Parse scene ouput file
    xml_attribute<>* outputFileAttrib = rootNode->first_attribute("output_file"); 
    scene.outputFile = convertToPPM(outputFileAttrib->value());

    // Parse scene background color
    xml_node<>* bgColorNode = rootNode->first_node("background_color");
    scene.backgroundColor = Color(std::stod(bgColorNode->first_attribute("r")->value()),
        std::stod(bgColorNode->first_attribute("g")->value()),
        std::stod(bgColorNode->first_attribute("b")->value()));

    // Parse scene camera location
    xml_node<>* cameraNode = rootNode->first_node("camera");
    vec3 cameraPosition = vec3(std::stod(cameraNode->first_node("position")->first_attribute("x")->value()),
        std::stod(cameraNode->first_node("position")->first_attribute("y")->value()),
        std::stod(cameraNode->first_node("position")->first_attribute("z")->value()));

    // Parse scene camera lookat
    vec3 cameralookAt;
    double lookatX = std::stod(cameraNode->first_node("lookat")->first_attribute("x")->value());
    double lookatY = std::stod(cameraNode->first_node("lookat")->first_attribute("y")->value());
    double lookatZ = std::stod(cameraNode->first_node("lookat")->first_attribute("z")->value());
    cameralookAt = vec3(lookatX, lookatY, lookatZ);

    // Parse scene camera up
    vec3 cameraUp;
    double upX = std::stod(cameraNode->first_node("up")->first_attribute("x")->value());
    double upY = std::stod(cameraNode->first_node("up")->first_attribute("y")->value());  
    double upZ = std::stod(cameraNode->first_node("up")->first_attribute("z")->value());    
    cameraUp = vec3(upX, upY, upZ);

    // Parse scene camera fov
    int cameraHorizontalFov = std::stoi(cameraNode->first_node("horizontal_fov")->first_attribute("angle")->value());

    // Parse scene camera resolution
    int resHorizontal = std::stoi(cameraNode->first_node("resolution")->first_attribute("horizontal")->value());
    int resVertical = std::stoi(cameraNode->first_node("resolution")->first_attribute("vertical")->value());

    // Parse scene camera max bounces
    int cameraMaxBounces = std::stoi(cameraNode->first_node("max_bounces")->first_attribute("n")->value());

    // Add camera to scene
    scene.camera = Camera(cameraPosition, cameralookAt, cameraUp, cameraHorizontalFov, resHorizontal,
        resVertical, cameraMaxBounces);

    // Parse ambient light
    xml_node<>* alightNode = rootNode->first_node("lights")->first_node("ambient_light");
    
    // Parse ambient light color
    Color color = parseColor(alightNode);

    // Add ambient light to scene lights
    AmbientLight ambientlight = AmbientLight(color); 
    scene.lights.push_back(&ambientlight);

    // Parse parallel light
    xml_node<>* parallelLightNode = rootNode->first_node("lights")->first_node("parallel_light");
    vec3 parallelLightColor, parallelLightDirection;
    if(parallelLightNode) {
        // Parse parallel light color
        Color color = parseColor(parallelLightNode);

        // Parse parallel light direction
        double pLightDirX = std::stod(parallelLightNode->first_node("direction")->first_attribute("x")->value());
        double pLightDirY = std::stod(parallelLightNode->first_node("direction")->first_attribute("y")->value());
        double pLightDirZ = std::stod(parallelLightNode->first_node("direction")->first_attribute("z")->value());
        parallelLightDirection = vec3(pLightDirX, pLightDirY, pLightDirZ);

        // Add parallel light to scene lights
        ParallelLight* parallellight = new ParallelLight(color, parallelLightDirection);
        scene.lights.push_back(parallellight);
    }

    // Parse point lights
    xml_node<>* lightsNode = rootNode->first_node("lights");
    xml_node<>* pointLightNode = rootNode->first_node("lights")->first_node("point_light");
    if(pointLightNode) {
        for(pointLightNode; pointLightNode; pointLightNode = pointLightNode->next_sibling()){
            // Parse point light color
            Color color = parseColor(pointLightNode);
 
            // Parse point light position
            double plPosX = std::stod(pointLightNode->first_node("position")->first_attribute("x")->value());
            double plPosY = std::stod(pointLightNode->first_node("position")->first_attribute("y")->value());
            double plPosZ = std::stod(pointLightNode->first_node("position")->first_attribute("z")->value());

            PointLight* pointlight = new PointLight(color, vec3(plPosX, plPosY, plPosZ));
            // NEW => memory has to be released later on to avoid memory leaks!
            scene.lights.push_back(pointlight);
        }
    }

    // Parse spot lights TODO Lab3b

    // Parse surfaces
    xml_node<>* surfacesNode = rootNode->first_node("surfaces");
    for(xml_node<>* node = surfacesNode->first_node(); node; node = node->next_sibling()) {
        std::string nodeName = node->name();
        if (nodeName == "sphere") {
            Sphere sphere;

            // Parse sphere radius
            sphere.radius = std::stod(node->first_attribute("radius")->value());
            
            // Parse sphere position
            sphere.position = vec3(std::stod(node->first_node("position")->first_attribute("x")->value()),
                std::stod(node->first_node("position")->first_attribute("y")->value()),
                std::stod(node->first_node("position")->first_attribute("z")->value())
            );
   
            // Parse sphere material
            xml_node<>* materialNode = node->first_node("material_solid");
            if(materialNode) {
                SolidMaterial* solidMaterial = new SolidMaterial(parseSolidMaterial(materialNode));
                sphere.material = solidMaterial;
            }
            else{
                materialNode = node->first_node("material_textured");
                if(materialNode) {
                    TexturedMaterial* texturedMaterial = new TexturedMaterial(parseTexturedMaterial(materialNode));
                    sphere.material = texturedMaterial;
                }
            }

            // Parse sphere transformation
            xml_node<>* transformNode = node->first_node("transform");
            if(transformNode) {
                Transform transform = parseTransform(transformNode);
                sphere.transform = transform;
            }

            // Add sphere to scene surfaces
            Sphere* parsedSphere = new Sphere(sphere);
            scene.surfaces.push_back(parsedSphere);
        }
        else if (nodeName == "mesh") {
            Mesh mesh;
            
            // Parse mesh name
            mesh.name = node->first_attribute("name")->value();

            // Parse mesh material
            xml_node<>* materialNode = node->first_node("material_solid");
            if(materialNode) {
                SolidMaterial* solidMaterial = new SolidMaterial(parseSolidMaterial(materialNode));
                mesh.material = solidMaterial;
            }
            else{
                materialNode = node->first_node("material_textured");
                if(materialNode) {
                    TexturedMaterial* texturedMaterial = new TexturedMaterial(parseTexturedMaterial(materialNode));
                    mesh.material = texturedMaterial;
                }
            }

            // Parse mesh transformations
            xml_node<>* transformNode = node->first_node("transform");
            if(transformNode) {
                Transform transform = parseTransform(transformNode);
                mesh.transform = transform;
            }

            // Add mesh to scene surfaces
            Mesh* parsedMesh = new Mesh(mesh);
            scene.surfaces.push_back(parsedMesh);
        }
    }

    return scene;
}

SolidMaterial XmlParser::parseSolidMaterial(xml_node<>* materialNode) {
    SolidMaterial solidMaterial;
    
    // Parse material color
    Color materialColor = parseColor(materialNode);
    solidMaterial.color = materialColor;

    // Parse material phong
    vec3 phong = vec3(
        std::stod(materialNode->first_node("phong")->first_attribute("ka")->value()),
        std::stod(materialNode->first_node("phong")->first_attribute("kd")->value()),
        std::stod(materialNode->first_node("phong")->first_attribute("ks")->value())
    );
    solidMaterial.phongExponent = std::stod(materialNode->first_node("phong")->first_attribute("exponent")->value());
    solidMaterial.phong = phong;

    // Parse material reflectance
    solidMaterial.reflectance = std::stod(materialNode->first_node("reflectance")->first_attribute("r")->value());

    // Parse material transmitance
    solidMaterial.transmittance = std::stod(materialNode->first_node("transmittance")->first_attribute("t")->value());

    // Parse material refraction
    solidMaterial.refraction = std::stod(materialNode->first_node("refraction")->first_attribute("iof")->value());
                
    return solidMaterial;
}



TexturedMaterial XmlParser::parseTexturedMaterial(xml_node<>* materialNode) {
    TexturedMaterial texturedMaterial;
    texturedMaterial.name = materialNode->first_node("texture")->first_attribute("name")->value();

    // Parse material phong
    vec3 phong = vec3(
        std::stod(materialNode->first_node("phong")->first_attribute("ka")->value()),
        std::stod(materialNode->first_node("phong")->first_attribute("kd")->value()),
        std::stod(materialNode->first_node("phong")->first_attribute("ks")->value())
    );
    texturedMaterial.phongExponent = std::stod(materialNode->first_node("phong")->first_attribute("exponent")->value());
    texturedMaterial.phong = phong;

    // Parse material reflectance
    texturedMaterial.reflectance = std::stod(materialNode->first_node("reflectance")->first_attribute("r")->value());

    // Parse material transmitance
    texturedMaterial.transmittance = std::stod(materialNode->first_node("transmittance")->first_attribute("t")->value());

    // Parse material refraction
    texturedMaterial.refraction = std::stod(materialNode->first_node("refraction")->first_attribute("iof")->value());

    return texturedMaterial;
}

Transform XmlParser::parseTransform(xml_node<>* transformNode) {
    // set default values for transform object in case <transform> tag is missing
    Transform transform = Transform(vec3(0,0,0), vec3(1,1,1), 0.0, 0.0, 0.0);
    if(transformNode) {
        // Parse translation
        xml_node<>* translateNode = transformNode->first_node("translate");
        if(translateNode) {
            transform.translate = vec3(std::stod(translateNode->first_attribute("x")->value()),
                std::stod(translateNode->first_attribute("y")->value()),
                std::stod(translateNode->first_attribute("z")->value()));
        }
        
        // Parse scaling
        xml_node<>* scaleNode = transformNode->first_node("scale");
        if(scaleNode) {
            transform.scale = vec3(std::stod(scaleNode->first_attribute("x")->value()),
            std::stod(scaleNode->first_attribute("y")->value()),
            std::stod(scaleNode->first_attribute("z")->value()));
        }
        
        // Parse rotation about x
        xml_node<>* rxNode = transformNode->first_node("rotateX");
        if(rxNode) {
            transform.rotateX = std::stod(rxNode->first_attribute("theta")->value());
        }
        
        // Parse rotation about y
        xml_node<>* ryNode = transformNode->first_node("rotateY");
        if(ryNode) {
            transform.rotateY = std::stod(ryNode->first_attribute("theta")->value());
        }        
        
        // Parse rotation about z
        xml_node<>* rzNode = transformNode->first_node("rotateZ");
        if(rzNode) {
            transform.rotateZ = std::stod(rzNode->first_attribute("theta")->value());
        }
    }

    return transform;
}

std::string XmlParser::convertToPPM(const std::string& filename) {
    std::string convertedFilename = filename;

    if (convertedFilename.substr(convertedFilename.size() - 4) == ".png") {
        convertedFilename.replace(convertedFilename.size() - 4, 4, ".ppm");
    }

    return convertedFilename;
}

Color XmlParser::parseColor(xml_node<>* colorParentNode) {
    double colorR = std::stod(colorParentNode->first_node("color")->first_attribute("r")->value());
    double colorG = std::stod(colorParentNode->first_node("color")->first_attribute("g")->value());
    double colorB = std::stod(colorParentNode->first_node("color")->first_attribute("b")->value());
    return Color(colorR, colorG, colorB);
}

// Sets up a black image
Scene XmlParser::blackImage() {
    Scene scene;
    scene.outputFile = "blackimage.ppm";
    return scene;
}

// Sets up a scene containing 3 spheres
Scene XmlParser::spheresImage() {
    Scene scene;
    
    // Set up camera
    Camera camera = Camera(vec3(0,0,1), vec3(0,0,-2.5), vec3(0,1,0), 45, 512, 512, 8);
    scene.camera = camera;
    
    // Set output file name
    scene.outputFile = "spheresimage.ppm";

    // Set background color
    scene.backgroundColor = Color(0.0, 0.0, 0.0);

    // Set ambient light
    AmbientLight* ambientlight = new AmbientLight(Color(1.0, 1.0, 1.0));
    scene.lights.push_back(ambientlight);

    // Set up solid materials    
    SolidMaterial* solidmaterial1 = new SolidMaterial(vec3(0.2, 0.1, 0.35), 100.0, 
    0.0, 0.0, 2.3, Color(0.15, 1.0, 0.2));
    SolidMaterial* solidmaterial2 = new SolidMaterial(vec3(0.4, 0.15, 0.1), 200.0, 
    0.0, 0.0, 2.3, Color(1.0, 0.1, 1.0));;
    SolidMaterial* solidmaterial3 = new SolidMaterial(vec3(0.4, 0.15, 0.1), 200.0, 
    0.0, 0.0, 2.3, Color(0.5, 0.5, 0.1));;    

    // Set up surfaces
    Transform transform;
    Sphere* sphere1 = new Sphere(solidmaterial1, transform, 1.0, vec3(2.4, 1.5, -3.0));
    Sphere* sphere2 = new Sphere(solidmaterial3, transform, 2.0, vec3(-1.0, 1.0, -3.0));
    Sphere* sphere3 = new Sphere(solidmaterial2, transform, 1.0, vec3(1.5, -2.0, -3.0));

    scene.surfaces.push_back(sphere1);
    scene.surfaces.push_back(sphere2);
    scene.surfaces.push_back(sphere3);

    return scene;
}