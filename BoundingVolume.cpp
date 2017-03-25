//
// Created by Göksu Güvendiren on 24/03/2017.
//

#include "HitInfo.h"
#include "BoundingVolume.h"
#include "Shape.h"
#include "Box.h"
#include "glm/glm.hpp"
#include "Triangle.h"

float Compare(float compared, float val, bool min)
{
    float res;
    if (min){
        res = std::min(compared, val);
    }
    else
    {
        res = std::max(compared, val);
    }
    return res;
}

BoundingVolume::BoundingVolume(const std::vector<Shape*>& shapes, Axis axis, glm::vec3 mins, glm::vec3 maxs)
{
    left  = nullptr;
    right = nullptr;
    shape = nullptr;

    assert(shapes.size() > 0);

    if (shapes.size() == 1){
        shape = shapes[0];
        box = Box(shapes[0]->Min(), shapes[0]->Max());

        return;
    }

    std::vector<Shape*> leftshapes; leftshapes.reserve(shapes.size() / 2 + 1);
    std::vector<Shape*> rightshapes; rightshapes.reserve(shapes.size() / 2 + 1);

    box = Box(mins, maxs);

    Axis nextAxis;
    glm::vec3 middle = box.Middle();

    switch(axis){
        case Axis::X :
            for (auto& shape : shapes){
//                mins.x = Compare(shape->Min().x, mins.x, true);
//                maxs.x = Compare(shape->Max().x, maxs.x, false);

                if (shape->Middle().x < middle.x) leftshapes.push_back(shape);
                else rightshapes.push_back(shape);
            }
            nextAxis = Axis::Y;
            break;

        case Axis::Y :
            for (auto& shape : shapes){
//                mins.y = Compare(shape->Min().y, mins.y, true);
//                maxs.y = Compare(shape->Max().y, maxs.y, false);

                if (shape->Middle().y < middle.y) leftshapes.push_back(shape);
                else rightshapes.push_back(shape);
            }
            nextAxis = Axis::Z;
            break;

        case Axis::Z :
            for (auto& shape : shapes){
//                mins.z = Compare(shape->Min().z, mins.z, true);
//                maxs.z = Compare(shape->Max().z, maxs.z, false);

                if (shape->Middle().z < middle.z) leftshapes.push_back(shape);
                else rightshapes.push_back(shape);
            }
            nextAxis = Axis::X;
            break;
    }

    if (leftshapes.size() > 0){
        left = new BoundingVolume(leftshapes, nextAxis, mins, middle);
        box.Min(glm::min(box.Min(), left->box.Min()));
        box.Max(glm::max(box.Max(), left->box.Max()));
    }

    if (rightshapes.size() > 0){
        right = new BoundingVolume(rightshapes, nextAxis, middle, maxs);
        box.Min(glm::min(box.Min(), right->box.Min()));
        box.Max(glm::max(box.Max(), right->box.Max()));
    }

    return;
}

auto to_ptrs(const std::vector<Triangle> &triangles)
{
    std::vector<Shape*> shapes(triangles.size());

    for(int i = 0; i < triangles.size(); i++){
        const Shape* s = triangles.data() + i;
        shapes[i] = const_cast<Shape*>(s);
    }

    return shapes;
}

BoundingVolume::BoundingVolume(const std::vector<Triangle> &triangles, Axis axis, glm::vec3 mins, glm::vec3 maxs) :
    BoundingVolume(to_ptrs(triangles), axis, mins, maxs)
{}

boost::optional<HitInfo> BoundingVolume::Hit(const Ray &ray) const
{
    if (!box.Hit(ray)) return boost::none;
    if (!left && !right) return shape->Hit(ray);

    boost::optional<HitInfo> leftHitInfo  = left->Hit(ray);
    boost::optional<HitInfo> rightHitInfo = right->Hit(ray);

    if (!leftHitInfo && !rightHitInfo) return boost::none;

    HitInfo ultimate;

    if (leftHitInfo) {
        ultimate = *leftHitInfo;
    }
    if (rightHitInfo){
        if (rightHitInfo->Parameter() < ultimate.Parameter())
            ultimate = *rightHitInfo;
    }

    return ultimate;
}
