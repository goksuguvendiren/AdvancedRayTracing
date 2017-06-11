//
// Created by Göksu Güvendiren on 11/06/2017.
//
#include <cmath>

#include "LightSource.h"
#include "../HitInfo.h"
#include "../Shapes/Sphere.h"

glm::vec3 LightSource::CalculateColor(const HitInfo &hit, int recursionDepth) const
{
    const Sphere* sph = static_cast<const Sphere*>(hit.GetShape());
    
    glm::vec3 dir = glm::normalize(hit.Position() - sph->Center())		;
    auto rad = ((std::acos(dir.z) / M_PI) / std::sqrt(dir.x * dir.x + dir.y * dir.y));
    
    assert(!std::isnan(rad));
    
    glm::vec2 texCoords = {dir.x * rad, dir.y * rad};
    
    texCoords += glm::vec2{1, 1};
    texCoords /= 2;
    
    texCoords.x = texCoords.x - int(std::floor(texCoords.x));
    texCoords.y = texCoords.y - int(std::floor(texCoords.y));
    
    texCoords.y*=-1;
    texCoords += 1;
    
    float i = texCoords.y * image.rows;
    float j = texCoords.x * image.cols;
    
    i = std::floor(i);
    j = std::floor(j);
    
    return {image.at<cv::Vec3f>(i, j)[2] * 400.f,
            image.at<cv::Vec3f>(i, j)[1] * 400.f,
            image.at<cv::Vec3f>(i, j)[0] * 400.f};
}

glm::vec3 LightSource::ComputeReflectance(const HitInfo &hit, const Light &light) const
{
    return {};
}
