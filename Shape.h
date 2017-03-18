//
// Created by Göksu Güvendiren on 14/03/2017.
//

#pragma once

#include <boost/optional.hpp>
#include <iostream>

class HitInfo;
class Ray;

class Shape
{
public:
    virtual boost::optional<HitInfo> Hit(const Ray &ray) const = 0;
    virtual bool FastHit(const Ray &ray) const = 0;
    virtual int ID() const = 0;
    virtual ~Shape() = default;
};
