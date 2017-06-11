//
// Created by Göksu Güvendiren on 08/06/2017.
//

#include <random>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ColorCalculation.h"
#include "Lights/DirectionalLight.hpp"
#include "Scene.h"

std::mt19937 hemisphere_seed;

glm::vec3 sample_hemisphere(const glm::vec3& normal)
{
    static std::uniform_real_distribution<float> asd(0, 1);
    static std::uniform_real_distribution<float> asd1(0, 1);
    float sample1 = asd(hemisphere_seed);
    float sample2 = asd1(hemisphere_seed);

    auto dir = glm::vec3{cos(2 * glm::pi<float>() * sample2) * glm::sqrt(1 - glm::pow(sample1, 2.f)),
                         sample1,
                         sin(2 * glm::pi<float>() * sample2) * glm::sqrt(1 - glm::pow(sample2, 2.f))};

    auto c = glm::cross(glm::vec3{0.f, 1.f, 0.f}, normal);
    auto angle = glm::acos(glm::dot(glm::vec3{0, 1, 0}, normal));
    auto res = glm::angleAxis(angle, c) * glm::normalize(dir);

    if (normal == glm::vec3{0, -1, 0})
    {
        return -res;
    }
    return res;
}

extern Scene scene;


//std::cerr << mc_color.r << " - " << mc_color.g << " - " << mc_color.b << '\n';

glm::vec3 CalculateMaterialReflectances(const HitInfo& hit, int recDepth)
{
//    if (!hit.GetMaterial()->is_BRDF()) return CalculateReflectance(hit, recDepth);
    if (recDepth < scene.MaxRecursionDepth())  // mirror calls this function over and over
    {
        auto color = hit.GetMaterial()->CalculateColor(hit, recDepth);
        // TODO : It may be necessary to call Compute Reflectance for the material because right now, after mirror, no color is computed using light data etc.
        // Orda bi gariplikler soz konusu but not that bad.
        
        
        // Monte Carlo Integration
        if (recDepth < scene.MaxRecursionDepth())
        {
            auto direction = sample_hemisphere(hit.Normal()); //hit.Normal(); // sample_hemisphere(hit.Normal());
            Ray monte_carlo_ray (hit.Position() + (scene.ShadowRayEpsilon() * direction), direction);

            boost::optional<HitInfo> mc_hit = scene.Hit(monte_carlo_ray);

            if (mc_hit)
            {
                auto& sth = *mc_hit;
                
//                assert(hit.GetShape()->ID() != mc_hit->GetShape()->ID());
                assert(!std::isinf(mc_hit->Parameter()));
                assert(!std::isnan(mc_hit->Parameter()));
                
                auto mc_color = CalculateMaterialReflectances(*mc_hit, recDepth + 1);
                
                DirectionalLight dl(-direction, mc_color, 0);
                auto some_color = hit.GetMaterial()->ComputeReflectance(hit, dl);
//              assert(some_color.r >= 0 && some_color.g >= 0 && some_color.b >= 0);

                color += some_color;
            }
        }

        glm::clamp(color, 0.f, 1000.f);
        return color;
    }
    
    else
        return {};
}

//glm::vec3 Camera::CalculateReflectance(const HitInfo& hit, int recDepth) const
//{
//    if (hit.GetTexture() && hit.GetTexture()->GetDecalMode() == DecalMode::Replace_All)
//    {
//        glm::vec2 texCoords = hit.GetUV();
//        return hit.GetTexture()->GetColor(texCoords) * 255.f;
//    }
//
//    if (hit.GetMaterial().IsLight())
//    {
//        return hit.GetMaterial().GetLight();
//    }
//
//    // Ambient shading :
//    auto ambient = hit.GetMaterial().Ambient() * scene.AmbientLight();
//
//    if (hit.GetMaterial().IsMirror() && recDepth < scene.MaxRecursionDepth()) {
//        auto reflectedColor = CalculateMirror(hit, recDepth);
//        ambient += reflectedColor;
//    }
//
//    if (hit.GetMaterial().IsTransparent() && recDepth < scene.MaxRecursionDepth()) {
//        auto trColor = CalculateTransparency(hit, recDepth);
//        if (trColor != glm::vec3{0, 0, 0})
//            ambient += trColor;
//    }
//
//    for (auto& light : scene.Lights()){
//        auto direction = light->Direction(hit.Position());
//        Ray shadowRay(hit.Position() + (scene.ShadowRayEpsilon() * glm::normalize(direction)),
//                      glm::normalize(direction));
//
//        boost::optional<float> sh;
//        if ((sh = scene.ShadowHit(shadowRay))){
//            if (*sh < glm::length(direction))
//                continue;
//        }
//
//        auto intensity = light->Intensity(direction);
//
//        auto diffuse_color = hit.GetMaterial().Diffuse();
//        if(hit.GetTexture())
//        {
//            if (hit.GetTexture()->IsPerlin()) {
//                auto noise = hit.GetTexture()->Perlin().Sample(hit.Position());
//                diffuse_color = hit.GetTexture()->BlendColor(diffuse_color, {noise, noise, noise});
//            }
//            else {
//                glm::vec2 texCoords = hit.GetUV();
//                glm::vec3 tex_color = hit.GetTexture()->GetColor(texCoords);
//                diffuse_color = hit.GetTexture()->BlendColor(diffuse_color, tex_color);
//            }
//        }
//
//        // Diffuse shading :
//        auto theta = std::max(0.f, glm::dot(glm::normalize(hit.Normal()), glm::normalize(direction)));
//        ambient += (theta * diffuse_color * intensity);
//
//        // Specular shading :
//        auto half = glm::normalize(glm::normalize(direction) + glm::normalize(hit.HitRay().Origin() - hit.Position()));
//        ambient  += intensity *
//                    hit.GetMaterial().Specular() *
//                    std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), hit.GetMaterial().PhongExp());
//    }
//
//    return ambient;
//}
