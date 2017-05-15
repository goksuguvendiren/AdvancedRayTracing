//
// Created by Göksu Güvendiren on 24/03/2017.
//

#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "boost/optional.hpp"
#include "Box.h"

class Shape;
class Triangle;
class Ray;
class HitInfo;

enum class Axis
{
    X,
    Y,
    Z
};

class BoundingVolume
{
    Box box;
    BoundingVolume* left;
    BoundingVolume* right;

    Shape* shape;

public:
    BoundingVolume() : left(nullptr), right(nullptr), shape(nullptr) {};
    const Box& BBox() const { return box; }
    BoundingVolume(const std::vector<Shape*>& shapes, Axis axis);
    BoundingVolume(const std::vector<Triangle>& triangles, Axis axis);

    boost::optional<HitInfo> Hit (const Ray& ray) const;
    boost::optional<float>   ShadowHit(const Ray &ray) const;
};
