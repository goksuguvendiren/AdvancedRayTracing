//
// Created by Göksu Güvendiren on 27/02/2017.
//

#include <vector>
#include <sstream>
#include "Triangle.h"

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

    col1 = pointA.Data() - pointB.Data();
    col2 = pointA.Data() - pointC.Data();
    col3 = ray.Direction();
    col4 = pointA.Data() - ray.Origin();

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

    col1 = pointA.Data() - pointB.Data();
    col2 = pointA.Data() - pointC.Data();
    col3 = ray.Direction();
    col4 = pointA.Data() - ray.Origin();

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.00001 || gamma < -0.00001 || beta < -0.00001 || param < 0) return false;

    return true;
}

Triangle::Triangle(Vertex a, Vertex b, Vertex c, int mid, int tid) : pointA(a),
                                                                     pointB(b),
                                                                     pointC(c),
                                                                     material(&scene.GetMaterial(mid)),
                                                                     id(tid)
{
    surfNormal = glm::normalize(glm::cross(pointB.Data() - pointA.Data(),
                                           pointC.Data() - pointA.Data()));

    pointA.Normal(surfNormal);
    pointB.Normal(surfNormal);
    pointC.Normal(surfNormal);
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

std::vector<Triangle> LoadTriangles(tinyxml2::XMLElement* elem)
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

        auto ind0 = glm::vec4(scene.GetVertex(GetInt(stream)).Data(), 1);
        auto ind1 = glm::vec4(scene.GetVertex(GetInt(stream)).Data(), 1);
        auto ind2 = glm::vec4(scene.GetVertex(GetInt(stream)).Data(), 1);

        glm::mat4 matrix;
        for (auto& tr : transformations){
            auto m = scene.GetTransformation(tr);
            matrix = m * matrix;
        }

        ind0 = matrix * ind0;
        ind1 = matrix * ind1;
        ind2 = matrix * ind2;

//        return std::make_pair(true, Triangle{Vertex{{ind0.x, ind0.y, ind0.z}},
//                                             Vertex{{ind1.x, ind1.y, ind1.z}},
//                                             Vertex{{ind2.x, ind2.y, ind2.z}}});

        tris.push_back({Vertex{{ind0.x, ind0.y, ind0.z}},
                        Vertex{{ind1.x, ind1.y, ind1.z}},
                        Vertex{{ind2.x, ind2.y, ind2.z}}, matID});
    }

    return tris;
}