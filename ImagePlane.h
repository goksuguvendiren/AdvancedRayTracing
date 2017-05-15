//
// Created by Göksu Güvendiren on 24/02/2017.
//

#pragma once

#include <vector>
#include "Image.h"
#include <glm/vec3.hpp>
#include "tinyxml/tinyxml2.h"

// ImagePlane does not contain any information about the values of pixels.

class ImagePlane
{
    float distance_to_camera;
    float right;
    float left;
    float top;
    float bottom;

    unsigned int n_x;
    unsigned int n_y;

    float pixel_width;
    float pixel_height;


public:
    ImagePlane() {}
    ImagePlane(float dist, float l, float r, float b, float t, unsigned int nx, unsigned int ny) : distance_to_camera(dist),
                                                                                                   right(r), left(l), 
                                                                                                   top(t), bottom(b),
                                                                                                   n_x(nx), n_y(ny)
    {
        pixel_width  = (right - left) / n_x;
        pixel_height = (top - bottom) / n_y;
    }

    float DistanceToCamera() const { return distance_to_camera; }
    void  DistanceToCamera(float distance) { distance_to_camera = distance; }

    float Right() const { return right; }
    void  Right(float r) { right = r; }
    float Left() const { return left; }
    void  Left(float l) { left = l; }
    float Top() const { return top; }
    void  Top(float t) { top = t; }
    float Bottom() const { return bottom; }
    void  Bottom(float b) { bottom = b; }

    unsigned int NX() const { return n_x; }
    void NX(unsigned int x) { n_x = x; }
    unsigned int NY() const { return n_y; }
    void NY(unsigned int y) { n_y = y; }

    float PixelWidth() const { return pixel_width; }
    float PixelHeight() const { return pixel_height; }
};

ImagePlane ReadImagePlane();

ImagePlane CreatePlane(tinyxml2::XMLElement* element, int focalDistance);
