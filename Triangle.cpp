//
// Created by Göksu Güvendiren on 27/02/2017.
//

#include <vector>
#include "Triangle.h"

inline double determinant(const glm::vec3& col1,
                          const glm::vec3& col2,
                          const glm::vec3& col3)   // only for a 3x3 matrix !
{
    return col1.x * (col2.y * col3.z - col2.z * col3.y) -
           col2.x * (col1.y * col3.z - col1.z * col3.y) +
           col3.x * (col1.y * col2.z - col1.z * col2.y);

}

std::pair<bool, HitInfo> Triangle::Hit(const Ray &ray)
{
    glm::vec3 col1(3);
    glm::vec3 col2(3);
    glm::vec3 col3(3);
    glm::vec3 col4(3);

    col1.x = pointA.x - pointB.x; col2.x = pointA.x - pointC.x; col3.x = ray.Direction().x; col4.x = pointA.x - ray.Origin().x;
    col1.y = pointA.y - pointB.y; col2.y = pointA.y - pointC.y; col3.y = ray.Direction().y; col4.y = pointA.y - ray.Origin().y;
    col1.z = pointA.z - pointB.z; col2.z = pointA.z - pointC.z; col3.z = ray.Direction().z; col4.z = pointA.z - ray.Origin().z;

    auto detA  = determinant(col1, col2, col3);

    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto param = determinant(col1, col2, col4) / detA;
    auto alpha = 1 - beta - gamma;

    if (alpha < 0 || gamma < 0 || beta < 0 || param < 0) return std::make_pair(false, HitInfo());

    auto point = ray.Origin() + (float)param * ray.Direction();

    return std::make_pair(true, HitInfo(point, surfNormal, param, ray));

}
