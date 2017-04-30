//
// Created by Goksu Guvendiren on 24/04/2017.
//

#include <cmath>
#include <vector>
#include <random>
#include <iostream>
#include "PerlinNoise.hpp"
#include "glm/common.hpp"
#include "glm/geometric.hpp"

float PerlinNoise::weight(float t) const
{
    return float(-6  * std::pow(std::abs(t), 5) +
                15  * std::pow(std::abs(t), 4) +
                -10 * std::pow(std::abs(t), 3) + 1);
}

int PerlinNoise::phi(int i, const std::vector<int>& P) const
{
    return P[i % 16];
}

glm::vec3 PerlinNoise::e(int i, int j, int k) const
{
    static std::vector<glm::vec3> G = {{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
                                {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
                                {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1},
                                {1, 1, 0}, {-1, 1, 0}, {0, -1, 1}, {0, -1,  1}};

    i%=16;
    j%=16;
    k%=16;

    if (i < 0) i += 16;
    if (j < 0) j += 16;
    if (k < 0) k += 16;

    int index = P[(i + P[(j + P[k % 16]) % 16]) % 16];

    return G[index];
}

float PerlinNoise::omega(glm::vec3 uvw, int i, int j, int k) const
{
    return weight(uvw.x) * weight(uvw.y) * weight(uvw.z) * float(glm::dot(e(i, j, k), uvw));
}

float PerlinNoise::Sample(const glm::vec3& point_) const
{
    auto point = point_ * scalingFactor;

    auto floor = [](float val) { return int(val / int(1)); };

    int x = floor(point.x);
    int y = floor(point.y);
    int z = floor(point.z);

    float result = 0;
    for (int i = x; i <= x + 1; i++)
    {
        for (int j = y; j <= y + 1; j++)
        {
            for (int k = z; k <= z + 1; k++)
            {
                result += omega({point.x - i, point.y - j, point.z - k}, i, j, k);
            }
        }
    }

    if (app == Noise_Appeareance::Vein)
        return std::abs(result);

    return (result + 1.f) / 2.f;
}

PerlinNoise::PerlinNoise(float sf, Noise_Appeareance a) : scalingFactor(sf), app(a)
{
    P = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(P.begin(), P.end(), g);
}
