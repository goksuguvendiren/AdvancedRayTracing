//
// Created by Göksu Güvendiren on 24/02/2017.
//
#pragma once

#include <iostream>
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "ImagePlane.h"
#include "tinyxml/tinyxml2.h"

class Image;
class HitInfo;

class Camera
{
    int id;
    std::string imageName;

    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 gaze;
    glm::vec3 right;

    ImagePlane imagePlane;

    glm::vec3 planePosition;

public:
    Camera(const ImagePlane& plane = {} , int i = 0, glm::vec3 p = {0, 0, 0},
                                                             glm::vec3 g = {0, 1, 0},
                                                             glm::vec3 u = {0, 0, 1},
                                                             std::string name = "") : imagePlane(plane),
                                                                                      position(p), id(i),
                                                                                      imageName(name)
    {
        up    = glm::normalize(u);
        gaze  = glm::normalize(g);
        right = glm::normalize(glm::cross(gaze, up));
        up    = glm::normalize(glm::cross(right, gaze));

        planePosition = position + imagePlane.Left() * right + imagePlane.Top() * up + imagePlane.DistanceToCamera() * gaze;
    };

    Image Render() const;

    auto Position() const { return position; }
    auto Up() const { return up; }
    auto Gaze() const { return gaze; }
    auto Right() const { return right; }

    glm::vec3 PlanePosition() const { return planePosition; }
    glm::vec3 GetPixelLocation(int i, int j) const;


    glm::vec3 CalculateReflectance(const HitInfo& hit, int recDepth) const;
    glm::vec3 CalculateMirror(const HitInfo& hit, int recDepth) const;
    glm::vec3 CalculateTransparency(const HitInfo& hit, int recDepth) const;
};

Camera LoadCamera(tinyxml2::XMLElement *element);

