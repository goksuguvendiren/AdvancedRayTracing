//
// Created by Goksu Guvendiren on 21/04/2017.
//

#include "Texture.hpp"
#include "Scene.h"

extern Scene scene;

Interpolation GetInterpolation(const std::string& text)
{
    switch(text[0])
    {
    case 'b':
        return Interpolation::Bilinear;
    case 'n':
        return Interpolation::Nearest;
    default:
        return Interpolation::None;
    }
}

DecalMode GetDecalMode(const std::string& text)
{
    if (text == "replace_kd") return DecalMode::Replace_KD;
    if (text == "blend_kd")   return DecalMode::Blend_KD;
    else return DecalMode::Replace_All;
}

Appearance GetAppearance(const std::string& text)
{
    switch(text[0])
    {
    case 'r':
        return Appearance::Repeat;
    case 'v':
        return Appearance::Vein;
    case 'p':
        return Appearance::Patch;
    default:
        return Appearance::None;
    }
}

std::vector<Texture> LoadTextures(tinyxml2::XMLElement *elem)
{
    std::vector<Texture> texs;
    for (auto child = elem->FirstChildElement("Texture"); child != NULL; child = child->NextSiblingElement())
    {
        int id;
        child->QueryIntAttribute("id", &id);
        bool isBump = false;
        if (child->QueryBoolAttribute("bumpmap", &isBump));

        std::string name = std::string(child->FirstChildElement("ImageName")->GetText());

        auto type = Type::Texture;
        if (name == "perlin") type = Type::Perlin;

        DecalMode dm = DecalMode::Replace_KD;
        if (auto elem = child->FirstChildElement("DecalMode"))
        {
            dm = GetDecalMode(std::string(elem->GetText()));
        }

        Interpolation interp = Interpolation::Nearest;
        if (auto elem = child->FirstChildElement("Interpolation"))
        {
            interp = GetInterpolation(std::string(elem->GetText()));
        }

        Appearance appr = Appearance::None;
        if (auto elem = child->FirstChildElement("Appearance"))
        {
            appr = GetAppearance(std::string(elem->GetText()));
        }

        float scale = 255;
        if (type == Type::Perlin)
        {
            if (auto elem = child->FirstChildElement("ScalingFactor"))
            {
                scale = elem->FloatText(1.f);
            }
        }

        if (auto elem = child->FirstChildElement("Normalizer"))
        {
            scale = elem->FloatText(255);
        }

        texs.push_back(Texture(scene.GetPath() + name, interp, dm, appr, type, scale, id, isBump));
    }

    return texs;
}

glm::vec3 Texture::GetColor(glm::vec2 texCoords) const
{
    texCoords.x = texCoords.x - int(std::floor(texCoords.x));
    texCoords.y = texCoords.y - int(std::floor(texCoords.y));

    float i = texCoords.y * image.rows;
    float j = texCoords.x * image.cols;

    cv::Vec3b final_color;
    float diff_a, diff_b;
    int a, b;

    switch (interpolation)
    {
    case Interpolation::Bilinear:
        a = int(std::floor(i));
        b = int(std::floor(j));

        diff_a = i - a;
        diff_b = j - b;

        final_color = (1 - diff_a) * (1 - diff_b) * image.at<cv::Vec3b>(a, b) +
                      (1 - diff_a) * diff_b       * image.at<cv::Vec3b>(a, b + 1) +
                      diff_a       * (1 - diff_b) * image.at<cv::Vec3b>(a + 1, b) +
                      diff_a       * diff_b       * image.at<cv::Vec3b>(a + 1, b + 1);

        return {final_color[2] / 255.f,
                final_color[1] / 255.f,
                final_color[0] / 255.f};

    case Interpolation::Nearest:
        return {image.at<cv::Vec3b>(i, j)[2] / 255.f,
                image.at<cv::Vec3b>(i, j)[1] / 255.f,
                image.at<cv::Vec3b>(i, j)[0] / 255.f};
    case Interpolation::None:
            assert(false);
    }
}

glm::vec3 Texture::BlendColor(glm::vec3 diffuse, glm::vec3 texcolor) const
{
    switch (mode)
    {
    case DecalMode::Replace_KD:
        return texcolor;

    case DecalMode::Blend_KD:
        return (texcolor + diffuse) / 2.f;

    case DecalMode::Replace_All:
        assert(false);
        return {0, 0, 0};

    default:
        return {0, 0, 0};
    }
}

glm::vec2 Texture::GetImageGradients(const glm::vec2& uv) const
{
    auto uvx = uv.x - int(std::floor(uv.x));
    auto uvy = uv.y - int(std::floor(uv.y));

    float i = uvy * image.rows;
    float j = uvx * image.cols;
    
    auto accPixelChannels = [](const cv::Vec<unsigned char, 3>& pixel)
    {
        return (pixel[0] + pixel[1] + pixel[2]);
    };
    
    float dd_du = accPixelChannels(image.at<cv::Vec3b>(i + 1, j) - image.at<cv::Vec3b>(i, j));
    float dd_dv = accPixelChannels(image.at<cv::Vec3b>(i, j + 1) - image.at<cv::Vec3b>(i, j));
    
    return glm::vec2{dd_du, dd_dv};
}

glm::vec3 Texture::CalculateBumpNormal(const glm::vec3& dp_du, const glm::vec3& dp_dv, const glm::vec3& surfaceNormal, const glm::vec2& uv) const
{
    glm::vec2 image_gradients = GetImageGradients(uv);
    
    glm::vec3 dq_du = dp_du + image_gradients.x * surfaceNormal;
    glm::vec3 dq_dv = dp_dv + image_gradients.y * surfaceNormal;
    
    return glm::cross(dq_du, dq_dv);
}







