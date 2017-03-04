//
// Created by Göksu Güvendiren on 24/02/2017.
//
#pragma once

#include <iostream>
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "ImagePlane.h"

class Image;
class Camera
{
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 gaze;
    glm::vec3 right;

    ImagePlane imagePlane;

    glm::vec3 planePosition;

public:
    Camera(const ImagePlane& plane, glm::vec3 p = {0, 0, 0},
                                    glm::vec3 g = {0, 1, 0},
                                    glm::vec3 u = {0, 0, 1}) : imagePlane(plane), position(p)
    {
        up    = glm::normalize(u);
        gaze  = glm::normalize(g);
        right = glm::normalize(glm::cross(gaze, up));

        planePosition = position + imagePlane.Left() * right + imagePlane.Top() * up + imagePlane.DistanceToCamera() * gaze;
    };

    Image Render() const;

    auto Position() const { return position; }
    auto Up() const { return up; }
    auto Gaze() const { return gaze; }
    auto Right() const { return right; }

    glm::vec3 PlanePosition() const { return planePosition; }
    glm::vec3 GetPixelLocation(int i, int j) const;
};

