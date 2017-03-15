//
// Created by Göksu Güvendiren on 14/03/2017.
//

#pragma once

#include <iostream>

class HitInfo;
class Ray;

class Shape
{
public:
    virtual std::pair<bool, HitInfo> Hit(const Ray &ray) const = 0;
    virtual bool FastHit(const Ray &ray) const = 0;
    virtual int ID() const = 0;
};
