//
// Created by Göksu Güvendiren on 27/02/2017.
//

#include <vector>
#include <sstream>
#include "Triangle.h"
#include "Material.h"
#include <sstream>

inline float determinant(const glm::vec3& col1,
                         const glm::vec3& col2,
                         const glm::vec3& col3)   // only for a 3x3 matrix !
{
    return col1.x * (col2.y * col3.z - col2.z * col3.y) -
           col2.x * (col1.y * col3.z - col1.z * col3.y) +
           col3.x * (col1.y * col2.z - col1.z * col2.y);

}

std::pair<bool, HitInfo> Triangle::Hit (const Ray &ray) const
{
    glm::vec3 col1(3);
    glm::vec3 col2(3);
    glm::vec3 col3(3);
    glm::vec3 col4(3);

    col1 = pointA - pointB;
    col2 = pointA - pointC;
    col3 = ray.Direction();
    col4 = pointA - ray.Origin();

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.00001 || gamma < -0.00001 || beta < -0.00001 || param < 0) return std::make_pair(false, HitInfo());

    auto point = ray.Origin() + param * ray.Direction();

    return std::make_pair(true, HitInfo(surfNormal, *material, param, ray));
}


bool Triangle::FastHit(const Ray &ray) const
{
    glm::vec3 col1(3);
    glm::vec3 col2(3);
    glm::vec3 col3(3);
    glm::vec3 col4(3);

    col1 = pointA - pointB;
    col2 = pointA - pointC;
    col3 = ray.Direction();
    col4 = pointA - ray.Origin();

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.00001 || gamma < -0.00001 || beta < -0.00001 || param < 0) return false;

    return true;
}

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int mid, int tid) : pointA(a),
                                                            pointB(b),
                                                            pointC(c),
                                                            material(&scene.GetMaterial(mid)),
                                                            id(tid)
{
    surfNormal = glm::normalize(glm::cross(pointB - pointA,
                                           pointC - pointA));
}

int Triangle::ID() const
{
    return id;
}

const Material* Triangle::Material() const
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

std::vector<Triangle> CreateTriangles(tinyxml2::XMLElement* elem)
{
    std::vector<Triangle> tris;

    for (auto child = elem->FirstChildElement("Triangle"); child != NULL; child = child->NextSiblingElement("Triangle")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int matID = child->FirstChildElement("Material")->IntText(0);

        std::vector<std::string> transformations;
        if(auto trns = child->FirstChildElement("Transformations")){
            std::istringstream ss {trns->GetText()};
            transformations = std::move(GetTransformations(ss));
        }

        std::istringstream stream {child->FirstChildElement("Indices")->GetText()};

        auto ind0 = scene.GetVertex(GetInt(stream));
        auto ind1 = scene.GetVertex(GetInt(stream));
        auto ind2 = scene.GetVertex(GetInt(stream));

        glm::mat4 matrix;
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        glm::vec4 v0(ind0, 1);
        glm::vec4 v1(ind1, 1);
        glm::vec4 v2(ind2, 1);

        ind0 = matrix * v0;
        ind1 = matrix * v1;
        ind2 = matrix * v2;

        tris.push_back({ind0, ind1, ind2, matID});
    }

    return tris;
}