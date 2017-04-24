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
#include "PerlinNoise.hpp"

enum class Interpolation
{
    Bilinear,
    Nearest,
    None
};

enum class DecalMode
{
    Replace_KD,
    Blend_KD,
    Replace_All,
    None
};

enum class Appearance
{
    Repeat,
    Patch,
    Vein,
    None
};

enum class Type
{
    Perlin,
    Texture
};

class Texture
{
    std::string imageSource;

    Interpolation interpolation;
    DecalMode mode;
    Appearance appr;
    Type type;
    PerlinNoise pn;

    cv::Mat image;

    float scale;
    int id;

public:
    Texture(const std::string& source, Interpolation inter, DecalMode dm, Appearance app,
            Type typ, int nor, int i) : imageSource(source),
                                        interpolation(inter),
                                        mode(dm), appr(app),
                                        type(typ), scale(nor),
                                        id(i), pn(nor, app == Appearance::Patch ? Noise_Appeareance::Patch : Noise_Appeareance::Vein)
    {
        if (type!=Type::Perlin) {
            image = cv::imread(imageSource, CV_LOAD_IMAGE_COLOR);
            assert(image.data);
            assert(image.rows!=0 && image.cols!=0);
        }
    }

    auto DecalMode() const { return mode; }
    bool IsPerlin() const { return type == Type::Perlin; }
    const PerlinNoise& Perlin() const { return pn; }
    float Scale() const { return scale; }
    int ID() const { return id; }
    glm::vec3 GetColor(glm::vec2 texCoords) const;
    glm::vec3 BlendColor(glm::vec3 diffuse, glm::vec3 texcolor) const;
};

std::vector<Texture> LoadTextures(tinyxml2::XMLElement *elem);