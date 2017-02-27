//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <cmath>
#include <iostream>
#include "glm/glm.hpp"
#include "Sphere.h"
#include "Ray.h"

bool Sphere::Hit(const Ray &ray)
{
    auto eminc = ray.Origin() - center;

    auto A = glm::dot(ray.Direction(), ray.Direction());
    auto B = 2.0f * glm::dot(ray.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;

    auto delta = B * B - 4 * A * C;

    std::cout << delta << '\n';
    if (delta < 0) return false;

    return true;
}