//
// Created by Göksu Güvendiren on 27/02/2017.
//

#include <vector>
#include "Triangle.h"

inline double determinant(const std::vector<double>& col1,
                          const std::vector<double>& col2,
                          const std::vector<double>& col3)   // only for a 3x3 matrix !
{
    double result = 0;
    result += col1[0] * (col2[1] * col3[2] - col2[2] * col3[1]) -
              col2[0] * (col1[1] * col3[2] - col1[2] * col3[1]) +
              col3[0] * (col1[1] * col2[2] - col1[2] * col2[1]);

    return result;
}

bool Triangle::Hit(const Ray &ray)
{
    std::vector<double> col1(3);
    std::vector<double> col2(3);
    std::vector<double> col3(3);
    std::vector<double> col4(3);

    col1[0] = pointA.x - pointB.x; col2[0] = pointA.x - pointC.x; col3[0] = ray.Direction().x; col4[0] = pointA.x - ray.Origin().x;
    col1[1] = pointA.y - pointB.y; col2[1] = pointA.y - pointC.y; col3[1] = ray.Direction().y; col4[1] = pointA.y - ray.Origin().y;
    col1[2] = pointA.z - pointB.z; col2[2] = pointA.z - pointC.z; col3[2] = ray.Direction().z; col4[2] = pointA.z - ray.Origin().z;

    auto detA  = determinant(col1, col2, col3);
    auto beta  = determinant(col4, col2, col3) / detA;
    auto gamma = determinant(col1, col4, col3) / detA;
    auto alpha = 1 - beta - gamma;

    return (alpha >= 0 && gamma >= 0 && beta >= 0);

}