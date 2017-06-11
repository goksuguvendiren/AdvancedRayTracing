//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <cmath>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "../Ray.h"
#include "../HitInfo.h"
#include "../Texture.hpp"
#include "../Scene.h"
#include "../Materials/ClassicMaterial.hpp"

Sphere::Sphere(int sid, float rd, Vertex c, const Materialx* matr, int tid) : id(sid), radius(rd), center(c), material(matr)
{
    // TODO: Clean up this code !!!
//    classic_material = new ClassicMaterial(material->ID(), material->Ambient(), material->Diffuse(), material->Specular(),
//                                           material->RefractionIndex(), material->PhongExp(), material->BRDF_ID());
    minval = center.Data()-rd;
    maxval = center.Data()+rd;

    texture = nullptr;
    if (tid!=-1) {
        texture = &scene.GetTexture(tid);
        assert(texture->ID()==tid);
    }
}


boost::optional<float> Sphere::ShadowHit(const Ray& ray) const
{
    auto inverseRay = Ray{glm::vec3(inverseTrMatrix * glm::vec4(ray.Origin(), 1)),
                      glm::vec3(inverseTrMatrix * glm::vec4(ray.Direction(), 0))};
    
    auto eminc = inverseRay.Origin() - center.Data();
    
    auto A = glm::dot(inverseRay.Direction(), inverseRay.Direction());
    auto B = 2.0f * glm::dot(inverseRay.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;
    
    auto delta = B * B - 4 * A * C;
    
    if (delta < scene.IntersectionTestEpsilon()) return boost::none;
    
    auto param = (- B - std::sqrt(delta)) / (2.0f * A);
    
    if (param < 0)
    {
        param = (- B + std::sqrt(delta)) / (2.0f * A);
        
        if (param < 0)
            return boost::none;
    }
    
    return param;
}

boost::optional<HitInfo> Sphere::Hit(const Ray &ray) const
{
    auto inverseRay = Ray{glm::vec3(inverseTrMatrix * glm::vec4(ray.Origin(), 1)),
                          glm::vec3(inverseTrMatrix * glm::vec4(ray.Direction(), 0))};

    auto eminc = inverseRay.Origin() - center.Data();

    auto A = glm::dot(inverseRay.Direction(), inverseRay.Direction());
    auto B = 2.0f * glm::dot(inverseRay.Direction(), eminc);
    auto C = glm::dot(eminc, eminc) - radius * radius;

    auto delta = B * B - 4 * A * C;

    if (delta < scene.IntersectionTestEpsilon()) return boost::none;

    auto param = (- B - std::sqrt(delta)) / (2.0f * A);

    if (param < 0)
    {
        param = (- B + std::sqrt(delta)) / (2.0f * A);

        if (param < 0)
            return boost::none;
    }

    auto modelPoint    = inverseRay.Origin() + param * inverseRay.Direction();
    auto surfaceNormal = glm::normalize(glm::vec3(inverseTranspose * glm::vec4(modelPoint - center.Data(), 0)));
    auto worldPoint    = glm::vec3(transformationMatrix * glm::vec4(modelPoint, 1));
    
    auto uv = GetTexCoords(worldPoint);
    
    if (texture && texture->IsBump())
    {
        std::pair<glm::vec3, glm::vec3> gradientVectors = this->GradientVectors(uv, modelPoint - center.Data());
        surfaceNormal = glm::normalize(texture->CalculateBumpNormal(gradientVectors.first, gradientVectors.second, surfaceNormal, uv));
    }

    if (std::isinf(param) || std::isnan(param))
    {
        return boost::none;
    }
    
    return HitInfo(surfaceNormal, this, material, texture, worldPoint, ray, uv, param);
}

std::pair<glm::vec3, glm::vec3> Sphere::GradientVectors(const glm::vec2& uv, const glm::vec3& localCoord) const
{
    float theta = M_PI * uv.y;
    float phi   = M_PI * 2 * (1 - uv.x);
    
    glm::vec3 dp_du = {-localCoord.z * M_PI * 2, 0, localCoord.x * 2 * M_PI};
    glm::vec3 dp_dv = { localCoord.y * M_PI * cos(phi), -radius * M_PI * sin(theta) , localCoord.y * M_PI * sin(phi)};
    
    return std::make_pair(dp_du, dp_dv);
}

inline int GetInt(std::istringstream& stream)
{
    int val;
    stream >> val;

    return val;
}

inline auto GetTransformations(std::istringstream& stream)
{
    std::vector<std::string> result;

    while(stream.good()){
        std::string tr;
        stream >> tr;
        result.push_back(tr);
    }

    return result;
}

std::vector<Sphere> LoadSpheres(tinyxml2::XMLElement *elem)
{
    std::vector<Sphere> spheres;

    for (auto child = elem->FirstChildElement("Sphere"); child != NULL; child = child->NextSiblingElement("Sphere")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int matID = child->FirstChildElement("Material")->IntText(0);
        int centerID = child->FirstChildElement("Center")->IntText(0);
        float radius = child->FirstChildElement("Radius")->FloatText(0);

        Vertex center = scene.GetVertex(centerID);

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations"))
        {
            std::istringstream ss {trns->GetText()};
            transformations = GetTransformations(ss);
        }

        glm::mat4 matrix;
        for (auto& tr : transformations)
        {
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        int tid = -1;
        if (auto tex = child->FirstChildElement("Texture"))
        {
            tid = tex->IntText(-1);
        }

        Sphere sp {id, radius, center, scene.GetMaterial(matID), tid};
        sp.TransformationMatrix(matrix);

        spheres.push_back(std::move(sp));
    }

    return spheres;
}

glm::vec2 Sphere::GetTexCoords(glm::vec3 pos) const
{
    //TODO : You can get rid of this matrix multiplication!
    
    pos = glm::vec3(inverseTrMatrix * glm::vec4(pos, 1.f));
    pos = pos - center.Data();

    auto theta = acos(pos.y / radius);   // You get v from theta
    auto phi   = atan2(pos.z, pos.x);    // and u from phi

    // u, v coordinates
    return {(2 * M_PI - phi) / (2 * M_PI), float(theta) / (M_PI)};
}
