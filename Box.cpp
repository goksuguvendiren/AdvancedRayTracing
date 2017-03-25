//
// Created by Göksu Güvendiren on 18/03/2017.
//

#include "Box.h"
#include "Shape.h"
#include "Ray.h"
#include "glm/vec3.hpp"

bool Box::Hit(const Ray &ray) const
{
    double tx1 = (minValues.x - ray.Origin().x) / ray.Direction().x;
    double tx2 = (maxValues.x - ray.Origin().x) / ray.Direction().x;

    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (minValues.y - ray.Origin().y) / ray.Direction().y;
    double ty2 = (maxValues.y - ray.Origin().y) / ray.Direction().y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (minValues.z - ray.Origin().z) / ray.Direction().z;
    double tz2 = (maxValues.z - ray.Origin().z) / ray.Direction().z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin;
}

void Box::Compare(const glm::vec3 &val)
{
    minValues = glm::min(minValues, val);
    maxValues = glm::max(maxValues, val);
}

Box::Box()
{}

Box::Box(glm::vec3 minval, glm::vec3 maxval)
{
    minValues = minval;
    maxValues = maxval;
}