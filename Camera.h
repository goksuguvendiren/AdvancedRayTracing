//
// Created by Göksu Güvendiren on 24/02/2017.
//
#pragma once

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "ImagePlane.h"
#include "tinyxml/tinyxml2.h"

class Image;
class HitInfo;

extern std::vector<int> samples;

class Camera
{
    ImagePlane imagePlane;
 
    int id;
    std::string imageName;

    glm::vec3 position;
    glm::vec3 planePosition;
    int sampleCount;
    int divCount;

    int focalDistance;
    float apertureSize;
    
    glm::vec3 up;
    glm::vec3 gaze;
    glm::vec3 right;
    
    glm::vec3 GetCameraPosition() const;

public:
    Camera(const ImagePlane& plane = {} , int i = 0, glm::vec3 p = {0, 0, 0},
           glm::vec3 g = {0, 1, 0},
           glm::vec3 u = {0, 0, 1},
           std::string name = "",
           int numSamp = 1,
           int fd = 1, float as = 0) : imagePlane(plane),
                                       id(i), imageName(name),
                                       position(p), sampleCount(numSamp),
                                       divCount(std::floor(std::sqrt(numSamp))),
                                       focalDistance(fd), apertureSize(as)
    {
        up    = glm::normalize(u);
        gaze  = glm::normalize(g);
        right = glm::normalize(glm::cross(gaze, up));
        up    = glm::normalize(glm::cross(right, gaze));

        samples.resize(sampleCount, 0);
        for (int i = 0; i < sampleCount; i++){
            samples[i] = i;
        }

        planePosition = position + imagePlane.Left() * right + imagePlane.Top() * up + imagePlane.DistanceToCamera() * gaze;
    };

    Image Render() const;
    glm::vec3 RenderPixel(const glm::vec3& pixelcenter) const;

    auto Position() const { return position; }
    auto Up() const { return up; }
    auto Gaze() const { return gaze; }
    auto Right() const { return right; }

    glm::vec3 PlanePosition() const { return planePosition; }
    glm::vec3 GetPixelLocation(int i, int j) const;
//    glm::vec3 CalculateMaterialReflectances(const HitInfo& hit, int recDepth) const;
//
//    glm::vec3 CalculateReflectance(const HitInfo& hit, int recDepth) const;
//    glm::vec3 CalculateMirror(const HitInfo& hit, int recDepth) const;
//    glm::vec3 CalculateTransparency(const HitInfo& hit, int recDepth) const;
};

Camera LoadCamera(tinyxml2::XMLElement *element);

