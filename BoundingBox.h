//
// Created by Göksu Güvendiren on 18/03/2017.
//

#pragma once

#include <boost/optional.hpp>
#include "glm/vec3.hpp"
#include "HitInfo.h"

class BoundingBox
{
    glm::vec3 minValues;
    glm::vec3 maxValues;

public:
    boost::optional<HitInfo> Hit(const Ray &ray) const;

    void Min(glm::vec3 val) { minValues = val; }
    void Max(glm::vec3 val) { maxValues = val; }

    const glm::vec3& Min() { return minValues; }
    const glm::vec3& Max() { return maxValues; }

    void Compare(const glm::vec3& val);
};
