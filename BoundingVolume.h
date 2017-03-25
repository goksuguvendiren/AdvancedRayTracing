//
// Created by Göksu Güvendiren on 24/03/2017.
//

#pragma once
#include <vector>
#include "glm/vec3.hpp"
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
    BoundingVolume(const std::vector<Shape*>& shapes, Axis axis, glm::vec3 mins, glm::vec3 maxs);
    BoundingVolume(const std::vector<Triangle>& triangles, Axis axis, glm::vec3 mins, glm::vec3 maxs);

    boost::optional<HitInfo> Hit (const Ray& ray) const;
};