//
// Created by Goksu Guvendiren on 21/04/2017.
//

#pragma once

#include <vector>
#include <string>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "tinyxml/tinyxml2.h"

enum class Interpolation
{
    Bilinear,
    Nearest
};

enum class DecalMode
{
    Replace_KD,
    Blend_KD,
    Replace_All
};

enum class Appearance
{
    Repeat,
    Patch,
    Vein
};

class Texture
{
    std::string imageSource;

    Interpolation interpolation;
    DecalMode mode;
    Appearance appr;

    cv::Mat image;

    int normalizer;
    int id;
public:
    Texture(const std::string& source, Interpolation inter, DecalMode dm, Appearance app, int nor, int i) : imageSource(source),
                                                                                                            interpolation(inter),
                                                                                                            mode(dm), appr(app),
                                                                                                            normalizer(nor), id(i)
    {
        image = cv::imread(imageSource, CV_LOAD_IMAGE_COLOR);

//        cv::imshow("asd", image);
//        cv::waitKey(0);
        assert(image.data);
        assert(image.rows != 0 && image.cols != 0);
    }

    auto DecalMode() const { return mode; }

    int ID() const { return id; }
    glm::vec3 GetColor(glm::vec2 texCoords) const;
    glm::vec3 BlendColor(glm::vec3 diffuse, glm::vec3 texcolor) const;
};

std::vector<Texture> LoadTextures(tinyxml2::XMLElement *elem);