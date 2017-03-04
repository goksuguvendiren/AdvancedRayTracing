//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <cmath>
#include <iostream>
#include "glm/glm.hpp"
#include "Sphere.h"
#include "Ray.h"
#include "HitInfo.h"

std::pair<bool, HitInfo> Sphere::Hit(const Ray &ray)
{
    auto eminc = ray.Origin() - center;

    auto A = glm::dot(ray.Direction(), ray.Direction());
    auto B = 2.0f * glm::dot(ray.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;

    auto delta = B * B - 4 * A * C;

    if (delta < 0) return std::make_pair(false, HitInfo());

    auto param = (- B - std::sqrt(delta)) / (2.0f * A);

    auto pointOfIntersection = ray.Origin() + param * ray.Direction();
    auto surfaceNormal = pointOfIntersection - center;

    return std::make_pair(true, HitInfo(pointOfIntersection, surfaceNormal, param, ray));
}
