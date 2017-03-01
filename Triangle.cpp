//
// Created by Göksu Güvendiren on 27/02/2017.
//

#include <vector>
#include "Triangle.h"

inline double determinant(const std::vector<double>& row1,
                          const std::vector<double>& row2,
                          const std::vector<double>& row3)   // assume the matrix is 3x3
{
    double result = 0;
    result += row1[0] * (row2[1] * row3[2] - row2[2] * row3[1]) -
              row2[0] * (row1[1] * row3[2] - row1[2] * row3[1]) +
              row3[0] * (row1[1] * row2[2] - row1[2] * row2[1]);

    return result;
}

bool Triangle::Hit(const Ray &ray)
{
    std::vector<double> row1(3);
    std::vector<double> row2(3);
    std::vector<double> row3(3);
    std::vector<double> row4(3);

    row1[0] = pointA.x - pointB.x; row2[0] = pointA.x - pointC.x; row3[0] = ray.Direction().x; row4[0] = pointA.x - ray.Origin().x;
    row1[1] = pointA.y - pointB.y; row2[1] = pointA.y - pointC.y; row3[1] = ray.Direction().y; row4[1] = pointA.y - ray.Origin().y;
    row1[2] = pointA.z - pointB.z; row2[2] = pointA.z - pointC.z; row3[2] = ray.Direction().z; row4[2] = pointA.z - ray.Origin().z;

    auto detA  = determinant(row1, row2, row3);
    auto beta  = determinant(row4, row2, row3) / detA;
    auto gamma = determinant(row1, row4, row3) / detA;
    auto alpha = 1 - beta - gamma;

    return (alpha >= 0 && gamma >= 0 && beta >= 0);

}