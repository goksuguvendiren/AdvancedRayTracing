//
// Created by Göksu Güvendiren on 18/03/2017.
//

#include "Box.h"

bool BoundingBox::Hit(const Ray &ray) const
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

void BoundingBox::Compare(const glm::vec3 &val)
{
    minValues.x = std::min(minValues.x, val.x);
    minValues.y = std::min(minValues.y, val.y);
    minValues.z = std::min(minValues.z, val.z);

    maxValues.x = std::max(maxValues.x, val.x);
    maxValues.y = std::max(maxValues.y, val.y);
    maxValues.z = std::max(maxValues.z, val.z);
}