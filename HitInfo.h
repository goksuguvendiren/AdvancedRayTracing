//
// Created by Göksu Güvendiren on 03/03/2017.
//

#pragma once

#include <cassert>
#include "glm/vec3.hpp"
#include "Ray.h"
#include "Material.h"
#include "Shapes/Shape.h"
#include "Texture.hpp"

class HitInfo
{
    float param;
    glm::vec3 normal;
    glm::vec3 position;
    Ray ray;

    const Material* mat;
    const Texture*  tex;

    glm::vec2 uv;

public:
    HitInfo(glm::vec3 n,
            const Shape* sh,
            const Material* m,
            const Texture* textr,
            glm::vec3 point,
            Ray r,
            glm::vec2 _uv = {0, 0},
            float t = std::numeric_limits<float>::infinity()) : normal(n), mat(m),
                                                                position(point), uv(_uv),
                                                                ray(r), param(t),
                                                                shape(sh), tex(textr)
    {}

    const Shape* shape;

    HitInfo() : param(std::numeric_limits<float>::infinity()) {}

    const Material& Material() const { return *mat; }
    const Texture* Texture() const { return tex; }
    const Shape* GetShape() const { return shape; }
    Ray HitRay() const { return ray; }

    float Parameter() const { return param; }

    glm::vec3 Normal() const { return normal; }
    glm::vec3 Position() const { return position; }

    glm::vec2 GetUV() const { return uv; }
};
