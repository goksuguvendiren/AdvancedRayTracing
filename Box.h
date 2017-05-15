//
// Created by Göksu Güvendiren on 18/03/2017.
//

#pragma once

#include <boost/optional.hpp>
#include <glm/vec3.hpp>

class HitInfo;
class Ray;

class Box
{
    glm::vec3 minValues;
    glm::vec3 maxValues;

public:
    Box();
    Box(glm::vec3 minval, glm::vec3 maxval);

//    boost::optional<HitInfo> Hit(const Ray &ray) const;
    bool Hit(const Ray &ray) const;

    void Min(glm::vec3 val) { minValues = val; }
    void Max(glm::vec3 val) { maxValues = val; }

    const glm::vec3& Min() const { return minValues; }
    const glm::vec3& Max() const { return maxValues; }
    const glm::vec3 Middle() const { return (minValues + maxValues) / 2.0f; }

    void Compare(const glm::vec3& val);

};
