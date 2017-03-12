//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <iostream>
#include <sstream>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "Triangle.h"
#include "Mesh.h"
#include "LightSource.h"

glm::vec3 Camera::CalculateReflectance(const HitInfo& hit) const
{
    // Ambient shading :
    auto ambient = hit.Material().Ambient() * scene.AmbientLight();

    for (auto& light : scene.Lights()){
        auto direction = light.Position() - hit.Position();
        Ray ray(hit.Position() + (scene.ShadowRayEpsilon() * direction),
                direction);

        if (std::any_of(scene.Spheres().begin(), scene.Spheres().end(), [&ray](const Sphere& sph) {
            return sph.BoolHit(ray);
        })) continue;

        if (std::any_of(scene.Triangles().begin(), scene.Triangles().end(), [&ray](const Triangle& tr) {
            return tr.BoolHit(ray);
        })) continue;

        if (std::any_of(scene.Meshes().begin(), scene.Meshes().end(), [&ray](const Mesh& ms) {
            return ms.BoolHit(ray);
        })) continue;

        glm::vec3 pointToLight = light.Position() - hit.Position();
        auto intensity = light.Intensity(pointToLight);

        pointToLight = glm::normalize(pointToLight);

        // Diffuse shading :
        auto theta = glm::dot(glm::normalize(hit.Normal()), pointToLight);
        ambient += (theta * hit.Material().Diffuse() * intensity);

        // Specular shading :
        auto half = glm::normalize(pointToLight + glm::normalize(Position() - hit.Position()));
        ambient  += intensity *
                    hit.Material().Specular() *
                    std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), hit.Material().PhongExp());
    }

    return ambient;
}

Image Camera::Render() const
{
    Image image(imagePlane.NX(), imagePlane.NY());

    auto oneRight = imagePlane.PixelWidth() * right;
    auto oneDown  = -imagePlane.PixelHeight() * up;

    auto pixLocation = PlanePosition();
    pixLocation -= oneRight * 0.5f;
    pixLocation -= oneDown * 0.5f;

    auto rowPixLocation = pixLocation;
    auto rowBeginning   = pixLocation;

    for (int i = 0; i < imagePlane.NY(); i++){          // ny = height
        rowBeginning += oneDown;
        rowPixLocation = rowBeginning;
        for (int j = 0; j < imagePlane.NX(); j++){      // nx = width
            rowPixLocation += oneRight;
//            rowPixLocation = GetPixelLocation(i, j);
            auto ray = Ray(position, rowPixLocation - position);

            HitInfo ultHit;
            bool hashit = false;

            auto proc_shape = [&](auto& shape)
            {
                std::pair<bool, HitInfo> hit;
                if ((hit = shape.Hit(ray)).first && hit.second.Parameter() < ultHit.Parameter()){
                    ultHit = hit.second;
                    hashit = true;
                }
            };

            for_each(scene.Spheres().begin(), scene.Spheres().end(), proc_shape);
            for_each(scene.Triangles().begin(), scene.Triangles().end(), proc_shape);
            for_each(scene.Meshes().begin(), scene.Meshes().end(), proc_shape);

            if (hashit)
                image.at(i, j) = CalculateReflectance(ultHit);
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