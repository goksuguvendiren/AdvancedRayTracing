//
// Created by Goksu Guvendiren on 24/04/2017.
//

#pragma once

#include "glm/vec3.hpp"

enum class Noise_Appeareance
{
    Patch,
    Vein
};

class PerlinNoise
{
    float scalingFactor;
    Noise_Appeareance app;

    std::vector<int> P;

    glm::vec3 e(int i, int j, int k) const;
    int phi(int i, const std::vector<int>& P) const;
    float weight(float t) const;
    float omega(glm::vec3 uvw, int i, int j, int k) const;

public:
    PerlinNoise(float sf, Noise_Appeareance app = Noise_Appeareance::Patch);
    float Sample(const glm::vec3& point) const;

    auto Type() const { return app; }
};
