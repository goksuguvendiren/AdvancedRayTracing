//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <iostream>
#include <sstream>
#include "Camera.h"
#include "glm/vec3.hpp"
#include "Ray.h"
#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"
#include "HitInfo.h"
#include "tinyxml/tinyxml2.h"
#include "Mesh.h"
#include "LightSource.h"

glm::vec3 CalculateReflectance(const HitInfo& hit)
{
    auto ambient = hit.Material().Ambient(); // * scene.AmbientLight();

    for (auto& light : scene.Lights()){
        glm::vec3 pointToLight = glm::normalize(light.Position() - hit.Ray().Origin());
        auto theta = glm::dot(hit.Normal(), pointToLight);
        ambient += (theta * hit.Material().Diffuse() * light.Intensity());
    }

    return ambient;
}

Image Camera::Render() const
{
    Image image(imagePlane.NX(), imagePlane.NY());

    for (int i = 0; i < imagePlane.NY(); i++){          // nx = width
        for (int j = 0; j < imagePlane.NX(); j++){      // ny = height
            auto pixLocation = GetPixelLocation(i, j);
            auto ray = Ray(position, pixLocation - position);

            HitInfo ultHit;
            bool hashit = false;

            for (auto& sphere : scene.Spheres()) {
                std::pair<bool, HitInfo> hit;
                if ((hit = sphere.Hit(ray)).first && hit.second.Parameter() < ultHit.Parameter()){
                    ultHit = hit.second;
                    hashit = true;
                }
            }

            for (auto& triangle : scene.Triangles()){
                std::pair<bool, HitInfo> hit;
                if ((hit = triangle.Hit(ray)).first && hit.second.Parameter() < ultHit.Parameter()){
                    ultHit = hit.second;
                    hashit = true;
                }
            }

            for (auto& mesh : scene.Meshes()){
                std::pair<bool, HitInfo> hit;
                if ((hit = mesh.Hit(ray)).first && hit.second.Parameter() < ultHit.Parameter()){
                    ultHit = hit.second;
                    hashit = true;
                }
            }

            if (hashit)
                image.at(i, j) = (ultHit.Normal() + glm::vec3(1, 1, 1)) / 2.0f; //NCalculateReflectance(ultHit);
            else
                image.at(i, j) = scene.BackgroundColor();
        }
    }

    return image;
}

glm::vec3 Camera::GetPixelLocation(int i, int j) const
{
        return PlanePosition() - (i + 0.5f) * imagePlane.PixelHeight() * up +
                                 (j + 0.5f) * imagePlane.PixelWidth()  * right;
}

inline glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

Camera CreateCamera(tinyxml2::XMLElement* element)
{
    int id;
    if (element->QueryIntAttribute("id", &id) == tinyxml2::XML_NO_ATTRIBUTE){
        std::cerr << "No such attribute as id" << '\n';
        std::abort();
    }

    ImagePlane plane = CreatePlane(element);

    glm::vec3 position = GetElem(element->FirstChildElement("Position"));
    glm::vec3 gaze = GetElem(element->FirstChildElement("Gaze"));
    glm::vec3 up = GetElem(element->FirstChildElement("Up"));

    std::string name = element->FirstChildElement("ImageName")->GetText();

    return Camera(plane, id, position, gaze, up, name);
}