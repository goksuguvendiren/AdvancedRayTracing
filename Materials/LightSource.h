//
// Created by Göksu Güvendiren on 11/06/2017.
//

#pragma once

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "Materialx.hpp"
#include "../Image.h"


inline bool hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}


class LightSource : public Materialx
{
    int id;
    cv::Mat image;

public:
    LightSource(int i, std::string source) : id(i)
    {
        std::cerr << source << '\n';
        assert(hasEnding(source, ".hdr"));

        image = cv::imread(source, cv::IMREAD_COLOR | cv::IMREAD_ANYDEPTH);
        assert(image.data);
        assert(image.rows != 0 && image.cols != 0);
    }

    bool is_BRDF() const  { return false; }
    int ID() const { return id; }

    glm::vec3 ComputeReflectance(const HitInfo &hit, const Light &light) const;
    glm::vec3 CalculateColor(const HitInfo &hit, int recursionDepth) const;
};
