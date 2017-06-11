//
// Created by Göksu Güvendiren on 03/03/2017.
//

#pragma once

#include <cassert>
#include <glm/vec3.hpp>
#include "Ray.h"
#include "Materials/Materialx.hpp"
#include "Shapes/Shape.h"
#include "Texture.hpp"

class ClassicMaterial;

class HitInfo
{
    float param;
    glm::vec3 normal;
    glm::vec3 position;
    Ray ray;

    const Materialx* mat;
//    const ClassicMaterial* classic_material;
    const Texture*  tex;

    glm::vec2 uv;
    const Shape* shape;

public:
    HitInfo(glm::vec3 n,
            const Shape* sh,
            const Materialx* m,
//            const ClassicMaterial* cm,
            const Texture* textr,
            glm::vec3 point,
            Ray r,
            glm::vec2 _uv = {0, 0},
            float t = std::numeric_limits<float>::infinity()) : param(t), normal(n), position(point), ray(r),
                                                                mat(m), tex(textr),
                                                                uv(_uv), shape(sh)
    {}


    HitInfo() : param(std::numeric_limits<float>::infinity()) {}

    const Materialx* GetMaterial() const { return mat; }
//    const ClassicMaterial& GetClassicMaterial() const { return *classic_material; }
    const Texture* GetTexture() const { return tex; }
    const Shape* GetShape() const { return shape; }
    void SetShape(const Shape* sh) { shape = sh; }
    Ray HitRay() const { return ray; }

    float Parameter() const { return param; }

    glm::vec3 Normal() const { return normal; }
    glm::vec3 Position() const { return position; }

    glm::vec2 GetUV() const { return uv; }
};
