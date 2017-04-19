//
// Created by Göksu Güvendiren on 24/02/2017.
//

#include <iostream>
#include <sstream>
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "Triangle.h"
#include "Lights/PointLight.h"

glm::vec3 Camera::CalculateReflectance(const HitInfo& hit, int recDepth) const
{
    // Ambient shading :
    auto ambient = hit.Material().Ambient() * scene.AmbientLight();

    if (hit.Material().IsMirror() && recDepth < scene.MaxRecursionDepth()) {
        auto reflectedColor = CalculateMirror(hit, recDepth);
        ambient += reflectedColor;
    }

    if (hit.Material().IsTransparent() && recDepth < scene.MaxRecursionDepth()) {
        auto trColor = CalculateTransparency(hit, recDepth);
        if (trColor != glm::vec3{0, 0, 0})
            ambient += trColor;
    }

    for (auto& light : scene.Lights()){
        auto direction = glm::normalize(light->Position() - hit.Position());
        Ray shadowRay(hit.Position() + (scene.ShadowRayEpsilon() * direction),
                      direction);

        boost::optional<HitInfo> sh;
        if ((sh = scene.Hit(shadowRay))){
            auto& s_hit = *sh;
            if (s_hit.Parameter() < glm::length(light->Position() - hit.Position()))
                continue;
        }

//        glm::vec3 pointToLight = light->Position() - hit.Position();
        auto intensity = light->Intensity(hit.Position());

//        pointToLight = glm::normalize(pointToLight);

        // Diffuse shading :
        auto theta = std::max(0.f, glm::dot(glm::normalize(hit.Normal()), direction));
        ambient += (theta * hit.Material().Diffuse() * intensity);

        // Specular shading :
        auto half = glm::normalize(direction + glm::normalize(hit.HitRay().Origin() - hit.Position()));
        ambient  += intensity *
                    hit.Material().Specular() *
                    std::pow(std::max(glm::dot(half, hit.Normal()), 0.0f), hit.Material().PhongExp());
    }

    return ambient;
}

glm::vec3 Camera::RenderPixel(const glm::vec3& pixelcenter) const
{
    glm::vec3 pixelColor = {0, 0, 0};

    int sampleCount = 1; // 16
    int divCount = 1;    // std::sqrt(sampleCount);

    float cellWidth  = imagePlane.PixelWidth()  / float(divCount);
    float cellHeight = imagePlane.PixelHeight() / float(divCount);

    auto oneRight = cellWidth * right;
    auto oneDown  = -cellHeight * up;

    // label the first pixel, by taking the center of the pixel to the top left
    glm::vec3 firstPixel = pixelcenter - ((imagePlane.PixelWidth() / 2.f) * right);
    firstPixel -= ((imagePlane.PixelHeight() / 2.f) * up);
    // then add half a cell size to locate the center of the cell.
    firstPixel += cellWidth / 2.f;
    firstPixel += cellHeight / 2.f;

    for (int i = 0; i < sampleCount; i++) {
        //calculate pixel location
        int indH = i / divCount;
        int indW = i % divCount;

        glm::vec3 pixelLocation = firstPixel + (float(indW) * oneRight + float(indH) * oneDown);
        glm::vec3 cameraLocation = position;

        auto ray = Ray(cameraLocation, glm::normalize(pixelLocation - cameraLocation));

        boost::optional<HitInfo> hit  = scene.Hit(ray);

        if (hit){
            pixelColor += CalculateReflectance(*hit, 0);
        }
        else {
            pixelColor += scene.BackgroundColor();
        }
    }

    return pixelColor / float(sampleCount);
}


Image Camera::Render() const
{
    Image image(imagePlane.NX(), imagePlane.NY());

    auto oneRight = imagePlane.PixelWidth() * right;
    auto oneDown  = -imagePlane.PixelHeight() * up;

    auto pixLocation = PlanePosition();
    pixLocation -= oneRight * 0.5f;
    pixLocation -= oneDown * 0.5f;

    auto rowPixLocation = pixLocation;
    auto rowBeginning   = pixLocation;

    for (int i = 0; i < imagePlane.NY(); i++){          // ny = height
        rowBeginning += oneDown;
        rowPixLocation = rowBeginning;
        for (int j = 0; j < imagePlane.NX(); j++){      // nx = width
            rowPixLocation += oneRight;
            image.at(i, j) = RenderPixel(rowPixLocation);
        }

        auto progress = i / (float)imagePlane.NY();

        auto UpdateProgress = [](float progress)
        {
            int barWidth = 70;

            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
        };

        UpdateProgress(progress);
    }
    std::cout << std::endl;

    return image;
}

glm::vec3 Camera::GetPixelLocation(int i, int j) const
{
    return PlanePosition() - (i + 0.5f) * imagePlane.PixelHeight() * up +
           (j + 0.5f) * imagePlane.PixelWidth()  * right;
}

static glm::vec3 GetElem(tinyxml2::XMLElement* element)
{
    glm::vec3 color;

    std::istringstream ss {element->GetText()};
    ss >> color.r;
    ss >> color.g;
    ss >> color.b;

    return color;
}

Camera LoadCamera(tinyxml2::XMLElement *element)
{
    int id;
    if (element->QueryIntAttribute("id", &id) == tinyxml2::XML_NO_ATTRIBUTE){
        std::cerr << "No such attribute as id" << '\n';
        std::abort();
    }

    ImagePlane plane = CreatePlane(element);

    glm::vec3 position = GetElem(element->FirstChildElement("Position"));
    glm::vec3 gaze = GetElem(element->FirstChildElement("Gaze"));
    glm::vec3 up = GetElem(element->FirstChildElement("Up"));

    int sampleCount = element->FirstChildElement("NumSamples")->IntText(1);

    std::string name = element->FirstChildElement("ImageName")->GetText();

    return Camera(plane, id, position, gaze, up, name, sampleCount);
}


glm::vec3 Camera::CalculateMirror(const HitInfo& hit, int recDepth) const
{
    auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
    float costheta = glm::dot(v, hit.Normal());
    auto dir = glm::normalize(2.f * costheta * hit.Normal() - v);

    Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * dir),
                      dir);

    boost::optional<HitInfo> refHit = scene.BoundingBox().Hit(reflectionRay);

    glm::vec3 reflectedColor = {0, 0, 0};
    if (refHit)
        reflectedColor += hit.Material().Mirror() * CalculateReflectance(*refHit, recDepth + 1);

    return reflectedColor;
}

glm::vec3 Camera::CalculateTransparency(const HitInfo &hit, int recDepth) const
{
    bool entering = (glm::dot(hit.Normal(), hit.HitRay().Direction()) < 0);
    glm::vec3 t;

    glm::vec3 normal;
    float ratio;
    glm::vec3 refractedColor = {0, 0, 0};

    if (entering) {
        normal = hit.Normal();
        auto n1 = 1.0f;
        auto n2 = hit.Material().RefractionIndex();

        ratio = n1 / n2;
        float cost = glm::dot(-hit.HitRay().Direction(), normal);
        float delta = 1 - std::pow(ratio, 2.0f) * (1 - cost * cost);

        float c2 = std::sqrt(delta);
        float c1 = cost;

        t = ratio * hit.HitRay().Direction() + (ratio * c1 - c2) * normal;
        t = glm::normalize(t);

        //
        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float costheta = glm::dot(v, normal);
        auto reflDir = glm::normalize(2.f * costheta * normal - v);

        Ray reflectingRay(hit.Position() + (scene.ShadowRayEpsilon() * reflDir), reflDir);

        boost::optional<HitInfo> reflectionHit = scene.Hit(reflectingRay);

        glm::vec3 reflectioncolor = {0, 0, 0};
        if (reflectionHit)
            reflectioncolor = CalculateReflectance(*reflectionHit, recDepth + 1);

        float r0 = std::pow(((n1 - n2) / (n1 + n2)), 2);
        float rSchlick = r0 + ((1.0f - r0) * std::pow(1 - costheta, 5.0f));

        reflectioncolor *= rSchlick;

        Ray refractionRay(hit.Position() + (scene.ShadowRayEpsilon() * t), t);
        boost::optional<HitInfo> refHit = scene.Hit(refractionRay);

        if (refHit) {
            auto calcColor =  CalculateReflectance(*refHit, recDepth + 1);
            float r = calcColor.r * (std::exp(std::log(hit.Material().Transparency().r) * refHit->Parameter()));
            float g = calcColor.g * (std::exp(std::log(hit.Material().Transparency().g) * refHit->Parameter()));
            float b = calcColor.b * (std::exp(std::log(hit.Material().Transparency().b) * refHit->Parameter()));

            refractedColor += (1 - rSchlick) * glm::vec3{r, g, b};
        }

        refractedColor += reflectioncolor;
    }
    else
    { //exiting
        normal = -hit.Normal();

        float n1 = hit.Material().RefractionIndex();
        float n2 = 1.0f;
        ratio = n1 / n2;

        float costheta = glm::dot(-hit.HitRay().Direction(), normal);
        float delta = 1 - std::pow(ratio, 2.0f) * (1 - costheta * costheta);

        if (delta < 0)
        {
            auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
            float costheta = glm::dot(v, normal);
            auto dir = glm::normalize(2.f * costheta * normal - v);

            Ray reflectionRay(hit.Position() + (scene.ShadowRayEpsilon() * dir),
                              dir);

            boost::optional<HitInfo> refHit = scene.Hit(reflectionRay);

            glm::vec3 reflectedColor = {0, 0, 0};
            if (refHit)
            {
                reflectedColor += CalculateReflectance(*refHit, recDepth + 1);

                float r = reflectedColor.r * (std::exp(std::log(hit.Material().Transparency().r) * refHit->Parameter()));
                float g = reflectedColor.g * (std::exp(std::log(hit.Material().Transparency().g) * refHit->Parameter()));
                float b = reflectedColor.b * (std::exp(std::log(hit.Material().Transparency().b) * refHit->Parameter()));

                reflectedColor = {r, g, b};

            }

            return reflectedColor;
        }

        float c2 = std::sqrt(delta);
        float c1 = costheta;

        t = ratio * hit.HitRay().Direction() + (ratio * c1 - c2) * normal;
        t = glm::normalize(t);


        auto v = glm::normalize(hit.HitRay().Origin() - hit.Position());
        float cost = glm::dot(v, normal);
        auto reflDir = glm::normalize(2.f * cost * normal - v);

        Ray reflectingRay(hit.Position() + (scene.ShadowRayEpsilon() * reflDir), reflDir);

        boost::optional<HitInfo> reflectionHit = scene.Hit(reflectingRay);

        glm::vec3 reflectioncolor = {0, 0, 0};
        if (reflectionHit)
            reflectioncolor = CalculateReflectance(*reflectionHit, recDepth + 1);

        float r0 = std::pow(((n1 - n2) / (n1 + n2)), 2);
        float rSchlick = r0 + ((1.0f - r0) * std::pow(1 - cost, 5.0f));

        reflectioncolor *= rSchlick;


        Ray refractionRay(hit.Position() + (scene.ShadowRayEpsilon() * t), t);
        boost::optional<HitInfo> refHit = scene.Hit(refractionRay);

        if (refHit) {
            auto calcColor = CalculateReflectance(*refHit, recDepth + 1);

            refractedColor += (1 - rSchlick) * calcColor;
        }

        refractedColor += reflectioncolor;
    }

    return refractedColor;
}