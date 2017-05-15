//
// Created by Göksu Güvendiren on 14/03/2017.
//

#pragma once

#include <boost/optional.hpp>
#include <iostream>
#include <glm/vec3.hpp>

class HitInfo;
class Ray;

class Shape
{
public:
    virtual boost::optional<HitInfo> Hit(const Ray &ray) const = 0;
    virtual boost::optional<float>   ShadowHit(const Ray& ray) const = 0;
    virtual int ID() const = 0;
    virtual ~Shape() = default;
    virtual glm::vec3 Min() const = 0;
    virtual glm::vec3 Max() const = 0;
    virtual glm::vec3 Middle() const = 0;
};
