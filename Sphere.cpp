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

std::pair<bool, HitInfo> Sphere::Hit(const Ray &ray) const
{
    auto eminc = ray.Origin() - center;

    auto A = glm::dot(ray.Direction(), ray.Direction());
    auto B = 2.0f * glm::dot(ray.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;

    auto delta = B * B - 4 * A * C;

    if (delta < scene.IntersectionTestEpsilon()) return std::make_pair(false, HitInfo());

    auto param = (- B - std::sqrt(delta)) / (2.0f * A);

    auto pointOfIntersection = ray.Origin() + param * ray.Direction();
    auto surfaceNormal = glm::normalize(pointOfIntersection - center);

    return std::make_pair(true, HitInfo(surfaceNormal, scene.GetMaterial(materialID), param, ray));
}

bool Sphere::FastHit(const Ray &ray) const
{
    auto eminc = ray.Origin() - center;

    auto A = glm::dot(ray.Direction(), ray.Direction());
    auto B = 2.0f * glm::dot(ray.Direction(), eminc);
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

std::vector<Sphere> LoadSpheres(tinyxml2::XMLElement *elem)
{
    std::vector<Sphere> spheres;

    for (auto child = elem->FirstChildElement("Sphere"); child != NULL; child = child->NextSiblingElement("Sphere")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int matID = child->FirstChildElement("Material")->IntText(0);
        int centerID = child->FirstChildElement("Center")->IntText(0);
        float radius = child->FirstChildElement("Radius")->FloatText(0);

        glm::vec3 center = scene.GetVertex(centerID);

        spheres.push_back({id, radius, center, matID});
    }

    return spheres;
}