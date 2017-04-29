//
// Created by Göksu Güvendiren on 19/04/2017.
//

#pragma once

#include "Light.h"
#include <random>

extern std::mt19937 seed;

float generateRandomFloat();

class AreaLight : public Light
{
    glm::vec3 position;
    glm::vec3 edgeVec1;
    glm::vec3 edgeVec2;
    glm::vec3 intensity;

    int id;

public:
    AreaLight(glm::vec3 pos, glm::vec3 ev1, glm::vec3 ev2, glm::vec3 intens, int i) : position(pos),
                                                                                              edgeVec1(ev1),
                                                                                              edgeVec2(ev2),
                                                                                              intensity(intens),
                                                                                              id(i) {}
    glm::vec3 Position() const;
    glm::vec3 Intensity(const glm::vec3& lightPos_hitPos) const;
    glm::vec3 Direction(const glm::vec3& hitpos) const;
    
    int ID() const { return id; }
};
