//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <cmath>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
#include "Sphere.h"
#include "Ray.h"
#include "HitInfo.h"
#include "Scene.h"

Sphere::Sphere(int sid, float rd, Vertex c, int mid) : id(sid), radius(rd), center(c), material(&scene.GetMaterial(mid))
{
    minval = center.Data() - rd;
    maxval = center.Data() + rd;
}


boost::optional<HitInfo> Sphere::Hit(const Ray &ray) const
{
    auto inverseRay = Ray{glm::vec3(inverseTrMatrix * glm::vec4(ray.Origin(), 1)),
                          glm::vec3(inverseTrMatrix * glm::vec4(ray.Direction(), 0))};

    auto eminc = inverseRay.Origin() - center.Data();

    auto A = glm::dot(inverseRay.Direction(), inverseRay.Direction());
    auto B = 2.0f * glm::dot(inverseRay.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;

    auto delta = B * B - 4 * A * C;

    if (delta < scene.IntersectionTestEpsilon()) return boost::none;

    auto param = (- B - std::sqrt(delta)) / (2.0f * A);

    auto modelPoint = inverseRay.Origin() + param * inverseRay.Direction();
    auto worldPoint = glm::vec3(transformationMatrix * glm::vec4(modelPoint, 1));

    auto surfaceNormal = glm::normalize(glm::vec3(inverseTranspose * glm::vec4(modelPoint - center.Data(), 0)));

    if (param < 0)
    {
        return boost::none;
    }

    return HitInfo(surfaceNormal, this, *material, worldPoint, ray, param);
}

bool Sphere::FastHit(const Ray &ray) const
{
    auto inverseRay = Ray{glm::vec3(inverseTrMatrix * glm::vec4(ray.Origin(), 1)),
                          glm::vec3(inverseTrMatrix * glm::vec4(ray.Direction(), 0))};

    auto eminc = inverseRay.Origin() - center.Data();

    auto A = glm::dot(inverseRay.Direction(), inverseRay.Direction());
    auto B = 2.0f * glm::dot(inverseRay.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;

    auto delta = B * B - 4 * A * C;

    if (delta < scene.IntersectionTestEpsilon()) return false;

    if (- B - std::sqrt(delta) < 0) return false;

    return true;
}

inline int GetInt(std::istringstream& stream)
{
    int val;
    stream >> val;

    return val;
}

inline auto GetTransformations(std::istringstream& stream)
{
    std::vector<std::string> result;

    while(stream.good()){
        std::string tr;
        stream >> tr;
        result.push_back(tr);
    }

    return result;
}

std::vector<Sphere> LoadSpheres(tinyxml2::XMLElement *elem)
{
    std::vector<Sphere> spheres;

    for (auto child = elem->FirstChildElement("Sphere"); child != NULL; child = child->NextSiblingElement("Sphere")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int matID = child->FirstChildElement("Material")->IntText(0);
        int centerID = child->FirstChildElement("Center")->IntText(0);
        float radius = child->FirstChildElement("Radius")->FloatText(0);

        Vertex center = scene.GetVertex(centerID);

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations")){
            std::istringstream ss {trns->GetText()};
            transformations = std::move(GetTransformations(ss));
        }

        glm::mat4 matrix;
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        Sphere sp {id, radius, center, matID};
        sp.TransformationMatrix(matrix);

        spheres.push_back(std::move(sp));
    }

    return spheres;
}