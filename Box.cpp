//
// Created by Göksu Güvendiren on 18/03/2017.
//

#include "Box.h"
#include "Shape.h"
#include "Ray.h"

bool Box::Hit(const Ray &ray) const
{
    auto mins = (minValues - ray.Origin()) * ray.InvDirection();
    auto maxs = (maxValues - ray.Origin()) * ray.InvDirection();

    float tx1 = mins.x;
    float tx2 = maxs.x;

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = mins.y;
    float ty2 = maxs.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = mins.z;
    float tz2 = maxs.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= std::max(0.f, tmin);
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