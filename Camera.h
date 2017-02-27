//
// Created by Göksu Güvendiren on 24/02/2017.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include <iostream>
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "ImagePlane.h"

class Image;
class Camera
{
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 right;

    ImagePlane imagePlane;

    glm::vec3 planePosition;

public:
    Camera(const ImagePlane& plane) : imagePlane(plane), up({0, 1, 0}), front({0, 0, -1}), position({0, 0, 0})
    {
        right = glm::cross(front, up);
        std::cout << "Right is : " << right.x << ", " << right.y << ", " << right.z << '\n';
        planePosition = position + imagePlane.Left() * right + imagePlane.Top() * up - imagePlane.DistanceToCamera() * front;
    };

    Image Render() const;

    auto Position() const { return position; }
    auto Up() const { return up; }
    auto Front() const { return front; }
    auto Right() const { return right; }

    glm::vec3 PlanePosition() const { return planePosition; }
    glm::vec3 GetPixelLocation(int i, int j) const;
};


#endif //RAYTRACER_CAMERA_H
