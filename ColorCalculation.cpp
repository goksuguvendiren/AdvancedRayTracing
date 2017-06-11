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
    
    
//    const float r = std::sqrt(sample1);
//    const float theta = 2 * glm::pi<float>() * sample2;
//    
//    const float x = r * std::cos(theta);
//    const float y = r * std::sin(theta);
//    
//    return glm::vec3(x, y, std::sqrt(std::max(0.0f, 1 - sample1)));
//    

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
//                auto& sth = *mc_hit;
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
