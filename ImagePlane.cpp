//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <array>
#include <sstream>
#include <iostream>
#include "ImagePlane.h"

ImagePlane ReadImagePlane()
{
    return ImagePlane(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 800, 800);
}


ImagePlane CreatePlane(tinyxml2::XMLElement* element, int focalDistance)
{
    float distance = element->FirstChildElement("NearDistance")->FloatText();
    auto planeInfo = element->FirstChildElement("NearPlane")->GetText();

    auto lrbt = ([&planeInfo]
    {
        std::array<float, 4> arr;
        std::istringstream ss {planeInfo};

        ss >> arr[0];
        ss >> arr[1];
        ss >> arr[2];
        ss >> arr[3];

        return arr;
    })();

    auto resolution = element->FirstChildElement("ImageResolution")->GetText();

    auto n = ([&resolution]
    {
        std::array<unsigned int, 2> arr;
        std::istringstream ss {resolution};

        ss >> arr[0];
        ss >> arr[1];

        return arr;
    })();

    return ImagePlane(distance * focalDistance,
            lrbt[0] * focalDistance,
            lrbt[1] * focalDistance,
            lrbt[2] * focalDistance,
            lrbt[3] * focalDistance,
            n[0], n[1]);
}