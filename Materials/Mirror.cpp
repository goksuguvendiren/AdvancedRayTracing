//
// Created by Göksu Güvendiren on 08/06/2017.
//

#include <random>
#include <glm/glm.hpp>

#include "../ColorCalculation.h"
#include "../HitInfo.h"
#include "../Scene.h"
#include "Mirror.hpp"


extern Scene scene;

glm::vec3 Mirror::ComputeReflectance(const HitInfo &hit, const Light &light) const
{
    return {};
}


auto getT(const glm::vec3& v)
{
    auto vec = glm::abs(v);
    if (vec.x < vec.y && vec.x < vec.z){
        return glm::normalize(glm::vec3{1, v.y, v.z});
    }
    if (vec.y < vec.x && vec.y < vec.z){
        return glm::normalize(glm::vec3{v.x, 1, v.z});
    }

    return glm::normalize(glm::vec3{v.x, v.y, 1});
}

std::mt19937 mirrorseed;
glm::vec3 Mirror::CalculateColor(const HitInfo &hit, int recursionDepth) const
{
    if (roughness < 0.0001)
    {
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float costheta = glm::dot(v, hit.Normal());
        auto dir = glm::normalize(2.f * costheta * hit.Normal() - v);

        Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * dir),
                          dir);

        boost::optional<HitInfo> refHit = scene.Hit(reflectionRay);

        glm::vec3 reflectedColor = {0, 0, 0};
        if (refHit){
//            assert(hit.GetShape()->ID() != refHit->GetShape()->ID());
            reflectedColor += mirror * CalculateMaterialReflectances(*refHit, recursionDepth + 1);
        }

        return reflectedColor;
    }
    else
    {
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float costheta = glm::dot(v, hit.Normal());
        auto dir = glm::normalize(2.f * costheta * hit.Normal() - v);

        glm::vec3 t = glm::normalize(getT(dir));
        auto m = glm::cross(dir, t);   // should already be normalized
        auto u = glm::cross(m, dir);

        auto generateRandomFloat = []()
        {
            std::uniform_real_distribution<float> asd(0, 1);
            return asd(mirrorseed) - 0.5f;
        };

        m = roughness * generateRandomFloat() * m;
        u = roughness * generateRandomFloat() * u;

        auto newdir = glm::normalize(dir + m + u);

        Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * newdir), newdir);

        boost::optional<HitInfo> refHit = scene.Hit(reflectionRay);

        glm::vec3 reflectedColor = {0, 0, 0};
        if (refHit)
            reflectedColor += mirror * CalculateMaterialReflectances(*refHit, recursionDepth + 1);

        return reflectedColor;
    }
}
