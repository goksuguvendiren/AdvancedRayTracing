//
// Created by Göksu Güvendiren on 24/03/2017.
//

#include "HitInfo.h"
#include "BoundingVolume.h"
#include "Shape.h"
#include "Box.h"
#include "glm/glm.hpp"
#include "Triangle.h"

BoundingVolume::BoundingVolume(const std::vector<Shape*>& shapes, Axis axis)
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

    Axis nextAxis;
    glm::vec3 middle;

    std::vector<Shape*>::const_iterator beginning;
    std::vector<Shape*>::const_iterator middling;
    std::vector<Shape*>::const_iterator ending;

    auto sortedShapes = shapes;

    assert(axis == Axis::X || axis == Axis::Y || axis == Axis::Z);

    switch(axis){
        case Axis::X :
            std::sort(sortedShapes.begin(), sortedShapes.end(), [](auto& sh1, auto& sh2){
                return sh1->Middle().x < sh2->Middle().x;
            });

            beginning = sortedShapes.begin();
            middling  = sortedShapes.begin() + (sortedShapes.size() / 2);
            ending    = sortedShapes.end();

            leftshapes  = std::vector<Shape*>(beginning, middling);
            rightshapes = std::vector<Shape*>(middling, ending);

            nextAxis = Axis::Y;
            break;

        case Axis::Y :
            std::sort(sortedShapes.begin(), sortedShapes.end(), [](auto& sh1, auto& sh2){
                return sh1->Middle().y < sh2->Middle().y;
            });

            beginning = sortedShapes.begin();
            middling  = sortedShapes.begin() + (sortedShapes.size() / 2);
            ending    = sortedShapes.end();

            leftshapes  = std::vector<Shape*>(beginning, middling);
            rightshapes = std::vector<Shape*>(middling, ending);

            nextAxis = Axis::Z;
            break;

        case Axis::Z :
            std::sort(sortedShapes.begin(), sortedShapes.end(), [](auto& sh1, auto& sh2){
                return sh1->Middle().z < sh2->Middle().z;
            });

            beginning = sortedShapes.begin();
            middling  = sortedShapes.begin() + (sortedShapes.size() / 2);
            ending    = sortedShapes.end();

            leftshapes  = std::vector<Shape*>(beginning, middling);
            rightshapes = std::vector<Shape*>(middling, ending);

            nextAxis = Axis::X;
            break;
    }

    assert(shapes.size() == (leftshapes.size() + rightshapes.size()));

    left = new BoundingVolume(leftshapes, nextAxis);
    right = new BoundingVolume(rightshapes, nextAxis);

    box.Min(glm::min(left->box.Min(), right->box.Min()));
    box.Max(glm::max(left->box.Max(), right->box.Max()));
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
