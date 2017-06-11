//
// Created by Göksu Güvendiren on 08/06/2017.
//

#pragma once

#include <glm/vec3.hpp>
#include "HitInfo.h"

glm::vec3 CalculateMaterialReflectances(const HitInfo& hit, int recDepth);