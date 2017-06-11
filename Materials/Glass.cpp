//
// Created by Göksu Güvendiren on 11/06/2017.
//

#include <glm/glm.hpp>

#include "Glass.hpp"
#include "../HitInfo.h"
#include "../Scene.h"
#include "../ColorCalculation.h"

extern Scene scene;

glm::vec3 Glass::ComputeReflectance(const HitInfo &hit, const Light &light) const
{
    return {};
}

glm::vec3 Glass::CalculateColor(const HitInfo &hit, int recursionDepth) const
{
    bool entering = glm::dot(hit.Normal(), hit.HitRay().Direction()) < 0;
    glm::vec3 t;
    
    glm::vec3 normal;
    float ratio;
    glm::vec3 refractedColor = {0, 0, 0};
    
    if (entering) {
        normal = hit.Normal();
        auto n1 = 1.0f;
        auto n2 = refraction_index;
        
        ratio = n1 / n2;
        float cos_t = glm::dot(-hit.HitRay().Direction(), normal);
        float delta = 1 - std::pow(ratio, 2.0f) * (1 - cos_t * cos_t);
        
        float c2 = std::sqrt(delta);
        float c1 = cos_t;
        
        t = ratio * hit.HitRay().Direction() + (ratio * c1 - c2) * normal;
        t = glm::normalize(t);
        
        //
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float costheta = glm::dot(v, normal);
        auto reflDir = glm::normalize(2.f * costheta * normal - v);
        
        Ray reflectingRay(hit.Position() + (scene.ShadowRayEpsilon() * reflDir), reflDir);
        
        boost::optional<HitInfo> reflectionHit = scene.Hit(reflectingRay);
        
        glm::vec3 reflectioncolor = {0, 0, 0};
        if (reflectionHit)
            reflectioncolor = CalculateMaterialReflectances(*reflectionHit, recursionDepth + 1);
        
        float r0 = std::pow(((n1 - n2) / (n1 + n2)), 2);
        float rSchlick = r0 + ((1.0f - r0) * std::pow(1 - costheta, 5.0f));
        
        reflectioncolor *= rSchlick;
        
        Ray refractionRay(hit.Position() + (scene.ShadowRayEpsilon() * t), t);
        boost::optional<HitInfo> refHit = scene.Hit(refractionRay);
        
        if (refHit) {
            auto calcColor =  CalculateMaterialReflectances(*refHit, recursionDepth + 1);
            float r = calcColor.r * (std::exp(std::log(transparency.r) * refHit->Parameter()));
            float g = calcColor.g * (std::exp(std::log(transparency.g) * refHit->Parameter()));
            float b = calcColor.b * (std::exp(std::log(transparency.b) * refHit->Parameter()));
            
            refractedColor += (1 - rSchlick) * glm::vec3{r, g, b};
        }
        
        refractedColor += reflectioncolor;
    }
    else
    { //exiting
        normal = -hit.Normal();
        
        float n1 = refraction_index;
        float n2 = 1.0f;
        ratio = n1 / n2;
        
        float costheta = glm::dot(-hit.HitRay().Direction(), normal);
        float delta = 1 - std::pow(ratio, 2.0f) * (1 - costheta * costheta);
        
        if (delta < 0)
        {
            auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
            float costheta = glm::dot(v, normal);
            auto dir = glm::normalize(2.f * costheta * normal - v);
            
            Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * dir),
                              dir);
            
            boost::optional<HitInfo> refHit = scene.Hit(reflectionRay);
            
            glm::vec3 reflectedColor = {0, 0, 0};
            if (refHit)
            {
                reflectedColor += CalculateMaterialReflectances(*refHit, recursionDepth + 1);
                
                float r = reflectedColor.r * (std::exp(std::log(transparency.r) * refHit->Parameter()));
                float g = reflectedColor.g * (std::exp(std::log(transparency.g) * refHit->Parameter()));
                float b = reflectedColor.b * (std::exp(std::log(transparency.b) * refHit->Parameter()));
                
                reflectedColor = {r, g, b};
                
            }
            
            return reflectedColor;
        }
        
        float c2 = std::sqrt(delta);
        float c1 = costheta;
        
        t = ratio * hit.HitRay().Direction() + (ratio * c1 - c2) * normal;
        t = glm::normalize(t);
        
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float cost = glm::dot(v, normal);
        auto reflDir = glm::normalize(2.f * cost * normal - v);
        
        Ray reflectingRay(hit.Position() + (scene.ShadowRayEpsilon() * reflDir), reflDir);
        
        boost::optional<HitInfo> reflectionHit = scene.Hit(reflectingRay);
        
        glm::vec3 reflectioncolor = {0, 0, 0};
        if (reflectionHit)
            reflectioncolor = CalculateMaterialReflectances(*reflectionHit, recursionDepth + 1);
        
        float r0 = std::pow(((n1 - n2) / (n1 + n2)), 2);
        float rSchlick = r0 + ((1.0f - r0) * std::pow(1 - cost, 5.0f));
        
        reflectioncolor *= rSchlick;
        
        
        Ray refractionRay(hit.Position() + (scene.ShadowRayEpsilon() * t), t);
        boost::optional<HitInfo> refHit = scene.Hit(refractionRay);
        
        if (refHit) {
            auto calcColor = CalculateMaterialReflectances(*refHit, recursionDepth + 1);
            
            refractedColor += (1 - rSchlick) * calcColor;
        }
        
        refractedColor += reflectioncolor;
    }
    
    return refractedColor;

}
