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

    auto A = scene.GetVertex(pointA);
    auto B = scene.GetVertex(pointB);
    auto C = scene.GetVertex(pointC);

    col1 = A.Data() - B.Data();
    col2 = A.Data() - C.Data();
    col3 = ray.Direction();
    col4 = A.Data() - ray.Origin();

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.00001 || gamma < -0.00001 || beta < -0.00001 || param < 0) return std::make_pair(false, HitInfo());

    auto point = ray.Origin() + param * ray.Direction();

    return std::make_pair(true, HitInfo(surfNormal, scene.GetMaterial(materialID), param, ray));
}


bool Triangle::BoolHit (const Ray &ray) const
{
    glm::vec3 col1(3);
    glm::vec3 col2(3);
    glm::vec3 col3(3);
    glm::vec3 col4(3);

    auto A = scene.GetVertex(pointA);
    auto B = scene.GetVertex(pointB);
    auto C = scene.GetVertex(pointC);

    col1 = A.Data() - B.Data();
    col2 = A.Data() - C.Data();
    col3 = ray.Direction();
    col4 = A.Data() - ray.Origin();

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.00001 || gamma < -0.00001 || beta < -0.00001 || param < 0) return false;

    return true;
}

Triangle::Triangle(int a, int b, int c, int mid, int tid) : pointA(a),
                                                            pointB(b),
                                                            pointC(c),
                                                            materialID(mid),
                                                            ID(tid)
{
    auto vertA = scene.GetVertex(pointA);
    auto vertB = scene.GetVertex(pointB);
    auto vertC = scene.GetVertex(pointC);
    surfNormal = glm::normalize(glm::cross(vertB.Data() - vertA.Data(),
                                           vertC.Data() - vertA.Data()));
}

Triangle::~Triangle() {}

inline glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

inline int GetInt(std::istringstream& stream)
{
    int val;

    stream >> val;

    return val;
}

std::vector<Triangle> CreateTriangles(tinyxml2::XMLElement* elem)
{
    std::vector<Triangle> tris;

    for (auto child = elem->FirstChildElement("Triangle"); child != NULL; child = child->NextSiblingElement("Triangle")) {
        int id;
        child->QueryIntAttribute("id", &id);
        int matID = child->FirstChildElement("Material")->IntText(0);

        std::istringstream stream {child->FirstChildElement("Indices")->GetText()};

        auto ind0 = GetInt(stream);
        auto ind1 = GetInt(stream);
        auto ind2 = GetInt(stream);

        tris.push_back({ind0, ind1, ind2, matID});
    }

    return tris;
}