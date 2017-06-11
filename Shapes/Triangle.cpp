//
// Created by Göksu Güvendiren on 27/02/2017.
//

#include <vector>
#include <sstream>
#include <boost/optional.hpp>
#include "Triangle.h"
#include "../Texture.hpp"
#include "../Materials/Materialx.hpp"

extern Scene scene;

inline float determinant(const glm::vec3& col1,
                         const glm::vec3& col2,
                         const glm::vec3& col3)
{
    return col1.x * (col2.y * col3.z - col2.z * col3.y) -
           col2.x * (col1.y * col3.z - col1.z * col3.y) +
           col3.x * (col1.y * col2.z - col1.z * col2.y);

}
//
//std::atomic<std::uint64_t> cnt;

boost::optional<HitInfo> Triangle::Hit(const Ray &ray) const
{
//    cnt++;
    glm::vec3 col1 = pointA.Data() - pointB.Data();
    glm::vec3 col2 = pointA.Data() - pointC.Data();
    glm::vec3 col3 = ray.Direction();
    glm::vec3 col4 = pointA.Data() - ray.Origin();

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.0001 || gamma < -0.0001 || beta < -0.0001 || param < -0.0001) return boost::none;

    auto point = ray.Origin() + param * ray.Direction();

    glm::vec3 normal = glm::normalize(alpha * pointA.Normal() + beta * pointB.Normal() + gamma * pointC.Normal());
    glm::vec2 uv = GetTexCoords(point, beta, gamma);
    
    if (texture && texture->IsBump())
    {
        std::pair<glm::vec3, glm::vec3> gradientVectors = this->GradientVectors(normal);
        normal = glm::normalize(texture->CalculateBumpNormal(gradientVectors.first, gradientVectors.second, normal, uv));
    }

    return HitInfo(normal, this, material, texture, point, ray, uv, param);
}

std::pair<glm::vec3, glm::vec3> Triangle::GradientVectors(glm::vec3 normal) const
{
    // u1 = pointA, u2 = pointB, u3 = pointC
    float u2_u1 = pointB.GetUV().x - pointA.GetUV().x;
    float v2_v1 = pointB.GetUV().y - pointA.GetUV().y;
    
    float u3_u1 = pointC.GetUV().x - pointA.GetUV().x;
    float v3_v1 = pointC.GetUV().y - pointA.GetUV().y;
    
    glm::vec3 p2_p1 = pointB.Data() - pointA.Data();
    glm::vec3 p3_p1 = pointC.Data() - pointA.Data();
    
    glm::vec3 dp_du = u3_u1 * p2_p1 + v3_v1 * p3_p1;
    glm::vec3 dp_dv = u2_u1 * p2_p1 + v2_v1 * p3_p1;
    
//    auto bumpNormal = glm::normalize(glm::cross(dp_du, dp_dv));
    
//    std::cerr << bumpNormal.x << ", " << bumpNormal.y << ", " << bumpNormal.z << '\n';
//    std::cerr << normal.x << ", " << normal.y << ", " << normal.z << '\n';
    
    return std::make_pair(dp_du, dp_dv);
}

boost::optional<float> Triangle::ShadowHit(const Ray& ray) const
{
    glm::vec3 col1 = pointA.Data() - pointB.Data();
    glm::vec3 col2 = pointA.Data() - pointC.Data();
    glm::vec3 col3 = ray.Direction();
    glm::vec3 col4 = pointA.Data() - ray.Origin();
    
    auto detA  = determinant(col1, col2, col3);
    
    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;
    
    if (alpha < -0.00001 || gamma < -0.00001 || beta < -0.00001 || param < 0) return boost::none;
    
    return param;
}

Triangle::Triangle(Vertex a, Vertex b, Vertex c,
                   const Materialx* matr, int tid, int trid, bool sm) : pointA(a),
                                                          pointB(b),
                                                          pointC(c),
                                                          material(matr),
                                                          texture(&scene.GetTexture(tid)),  // TODO : get the pointer as the argument, not its id
                                                          id(trid), smooth(sm)

{
    surfNormal = glm::normalize(glm::cross(pointB.Data() - pointA.Data(),
                                           pointC.Data() - pointA.Data()));

    pointA.Normal(surfNormal);
    pointB.Normal(surfNormal);
    pointC.Normal(surfNormal);

    bbox.Min(pointA.Data());
    bbox.Max(pointA.Data());
    bbox.Compare(pointA.Data());
    bbox.Compare(pointB.Data());
    bbox.Compare(pointC.Data());

    texture = nullptr;
    if (tid!=-1) {
        texture = &scene.GetTexture(tid);
        assert(texture->ID()==tid);
    }
//    classic_material = new ClassicMaterial(material->ID(), material->Ambient(), material->Diffuse(), material->Specular(),
//                                           material->RefractionIndex(), material->PhongExp(), material->BRDF_ID());

}

int Triangle::ID() const
{
    return id;
}

const Materialx* Triangle::Mat() const
{
    return material;
}

Triangle::~Triangle() {}

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

std::vector<Triangle> LoadTriangles(tinyxml2::XMLElement* elem)
{
    std::vector<Triangle> tris;

    for (auto child = elem->FirstChildElement("Triangle"); child != NULL; child = child->NextSiblingElement("Triangle")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int matID = child->FirstChildElement("Material")->IntText(0);

        int texID = -1;
        if (auto texelem = child->FirstChildElement("Texture"))
        {
            texID = texelem->IntText(-1);
        }

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations")){
            std::istringstream ss {trns->GetText()};
            transformations = GetTransformations(ss);
        }

        std::istringstream stream {child->FirstChildElement("Indices")->GetText()};

        auto id0 = GetInt(stream);
        auto id1 = GetInt(stream);
        auto id2 = GetInt(stream);

        auto ind0 = glm::vec4(scene.GetVertex(id0).Data(), 1);
        auto ind1 = glm::vec4(scene.GetVertex(id1).Data(), 1);
        auto ind2 = glm::vec4(scene.GetVertex(id2).Data(), 1);

        glm::mat4 matrix;
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        ind0 = matrix * ind0;
        ind1 = matrix * ind1;
        ind2 = matrix * ind2;

        if (texID != -1 && !scene.GetTexture(texID).IsPerlin())
        {
            tris.push_back({Vertex{id0, {ind0.x, ind0.y, ind0.z}, {0, 0, 0}, scene.Get_UV(id0)},
                            Vertex{id1, {ind1.x, ind1.y, ind1.z}, {0, 0, 0}, scene.Get_UV(id1)},
                            Vertex{id2, {ind2.x, ind2.y, ind2.z}, {0, 0, 0}, scene.Get_UV(id2)},
                            scene.GetMaterial(matID), texID, id});
        }
        else
        {
            tris.push_back({Vertex{id0, {ind0.x, ind0.y, ind0.z}},
                            Vertex{id1, {ind1.x, ind1.y, ind1.z}},
                            Vertex{id2, {ind2.x, ind2.y, ind2.z}},
                            scene.GetMaterial(matID), texID, id});
        }

    }

    return tris;
}

glm::vec3 Triangle::Min() const
{
    return bbox.Min();
}

glm::vec3 Triangle::Max() const
{
    return bbox.Max();
}

glm::vec3 Triangle::Middle() const
{
    return bbox.Middle();
}

glm::vec2 Triangle::GetTexCoords(glm::vec3 pos, float beta, float gamma) const
{
    auto a_uv = pointA.GetUV();
    auto b_uv = pointB.GetUV();
    auto c_uv = pointC.GetUV();
    return a_uv + beta * (b_uv - a_uv) + gamma * (c_uv - a_uv);
}
