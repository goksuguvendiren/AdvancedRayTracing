
#include "BRDF.hpp"
#include "ClassicMaterial.hpp"
#include "../HitInfo.h"
#include "../Lights/Light.h"
#include "../Texture.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>

glm::vec3 ClassicMaterial::ComputeReflectance(const HitInfo& hit, const Light& light) const
{
    auto direction = light.Direction(hit.Position());
    
    if (!brdf_material)
    {
        auto intensity = light.Intensity(direction);
    
        glm::vec3 color = diffuse;
        
        if(hit.GetTexture())
        {
            glm::vec2 texCoords = hit.GetUV();
            glm::vec3 tex_color = hit.GetTexture()->GetColor(texCoords);
            if (hit.GetTexture()->ReplaceAll())
            {
                return tex_color;
            }
            color = hit.GetTexture()->BlendColor(color, tex_color);
        }
    
        // Diffuse shading :
        auto theta = std::max(0.f, glm::dot(glm::normalize(hit.Normal()), glm::normalize(direction)));
        color += theta * color * intensity;
    
        // Specular shading :
        auto half = glm::normalize(glm::normalize(direction) + glm::normalize(hit.HitRay().Origin() - hit.Position()));
        color += intensity * specular *std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), phong_exp);
        
        return color;
    }
    
    auto refl = brdf_material->ComputeReflectance(hit, light, *this);
    auto intens = light.Intensity(direction);
    return refl * intens;
}

glm::vec3 ClassicMaterial::CalculateColor(const HitInfo& hit, int recdepth) const
{
    glm::vec3 color = ambient * scene.AmbientLight();
    
    for (auto& light : scene.Lights()){
        auto direction = light->Direction(hit.Position());
        Ray shadowRay(hit.Position() + (scene.ShadowRayEpsilon() * glm::normalize(direction)),
                        glm::normalize(direction));
    
        boost::optional<float> sh;
        if ((sh = scene.ShadowHit(shadowRay))){
            if (*sh < glm::length(direction))
                continue;
            }
    
        color += ComputeReflectance(hit, *light);
    }
    
    return color;

}
