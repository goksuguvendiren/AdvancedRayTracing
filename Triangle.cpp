//
// Created by Göksu Güvendiren on 27/02/2017.
//

#include <vector>
#include <sstream>
#include "Triangle.h"

inline double determinant(const glm::vec3& col1,
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

    col1.x = A.Data().x - B.Data().x; col2.x = A.Data().x - C.Data().x; col3.x = ray.Direction().x; col4.x = A.Data().x - ray.Origin().x;
    col1.y = A.Data().y - B.Data().y; col2.y = A.Data().y - C.Data().y; col3.y = ray.Direction().y; col4.y = A.Data().y - ray.Origin().y;
    col1.z = A.Data().z - B.Data().z; col2.z = A.Data().z - C.Data().z; col3.z = ray.Direction().z; col4.z = A.Data().z - ray.Origin().z;

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < -0.00001 || gamma < -0.00001 || beta < -0.00001 || param < 0) return std::make_pair(false, HitInfo());

    auto point = ray.Origin() + (float)param * ray.Direction();

    return std::make_pair(true, HitInfo(surfNormal, scene.GetMaterial(materialID), param, ray));
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